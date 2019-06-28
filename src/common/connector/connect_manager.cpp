//
// Created by zj on 6/24/19.
//

#include "connect_manager.h"
#include "logs.h"

#include <sys/epoll.h>
#include <unistd.h>

namespace zj {

ConnectManager::ConnectManager(ThreadPool& pool) : m_pool(pool) {
    m_epoll_fd = epoll_create(EPOLL_MAX_FD);
    if (m_epoll_fd == -1) {
        LOG_ERROR("epoll_create error");
        m_events = nullptr;
        m_run_flag = false;
    } else {
        m_events = new epoll_event[EPOLL_MAX_FD];
        m_run_flag = true;
    }
    init_wakeup_pipe();
    std::thread tmp(&ConnectManager::read_function, this);
    m_epoll_thread.swap(tmp);
}

void ConnectManager::init_wakeup_pipe() {
    pipe(m_pipefds);
    int read_pipe = m_pipefds[0];
    int write_pipe = m_pipefds[1];
    LOG_INFO("Init wakeup pipes. fd:{},{}", read_pipe, write_pipe);

    // make read-end non-blocking
    int flags = fcntl(read_pipe, F_GETFL, 0);
    fcntl(read_pipe, F_SETFL, flags|O_NONBLOCK);

    // add the read end to the epoll
    epoll_event event = {};
    event.data.fd = read_pipe;
    event.events = EPOLLIN;
    epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, read_pipe, &event);
}

void ConnectManager::wakeup_epoll() {
    char ch = 'x';
    write(m_pipefds[1], &ch, 1);
}

ConnectManager::~ConnectManager() {
    read_stop();
}

// only called by main thread
int ConnectManager::add(int fd, const std::string &desc) {
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_all_connector.insert(std::make_pair(fd, std::make_shared<Connector>(fd, desc, *this)));
    }
    struct epoll_event event = {};
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    if ( epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, fd, &event) != 0 ) {
        LOG_ERROR("EPOLL_CTL_ADD error.errno:{}, fd:{}, m_epoll_fd:{}, reason:{}", errno, fd, m_epoll_fd, strerror(errno));
    } else {
        LOG_INFO("EPOLL_CTL_ADD success, fd:{}, m_epoll_fd:{}", fd, m_epoll_fd);
    }
    return 0;
}

// can be called from other thread
void ConnectManager::remove(int fd) {
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_all_connector.erase(fd);
    }
    LOG_INFO("Delete fd:{} from manager.", fd);
}

void ConnectManager::read_function() {
    LOG_INFO("Begin read_function");
    while (m_run_flag) {
        LOG_INFO("read_function one loop begin");
        int n = epoll_wait(m_epoll_fd, m_events, EPOLL_MAX_FD, -1);
        LOG_INFO("Message number:{}", n);
        for (int i=0; i<n; ++i) {
            int fd = m_events[i].data.fd;

            // wakeup by read_stop
            if (fd == m_pipefds[0]) {
                LOG_INFO("Wakeup by pipe, fd{}", fd);
                char ch;
                int result = 1;
                while (result > 0) {
                    result = read(fd, &ch, 1);
                }
                continue;
            }

            if ((m_events[i].events & EPOLLERR) ||
                (m_events[i].events & EPOLLHUP) ||
                (!(m_events[i].events & EPOLLIN))) {
                /* An error has occured on this fd, or the socket is not
                   ready for reading (why were we notified then?) */
                LOG_ERROR("epoll error");
                remove(fd);
                continue;
            } else {
                read_from(fd);
            }
        }
        LOG_INFO("read_function loop end");
    }
    LOG_INFO("End read_function");
}

SPConnector ConnectManager::find(int fd) {
    auto iter = m_all_connector.find(fd);
    if (iter == m_all_connector.end())
        return nullptr;
    return (iter->second);
}

SPConnector ConnectManager::find(const std::string &desc) {
    auto iter = m_all_connector.begin();
    while (iter != m_all_connector.end()) {
        if (iter->second->get_desc() == desc) {
            return (iter->second);
        }
    }
    return nullptr;
}

void ConnectManager::read_stop() {
    m_run_flag = false;
    close(m_epoll_fd);
    if (m_epoll_thread.joinable()) {
        LOG_INFO("BEGIN Join");
        wakeup_epoll();
        m_epoll_thread.join();
        LOG_INFO("END Join");
    }
}

SPConnector ConnectManager::get_first() {
    auto iter = m_all_connector.begin();
    if (iter == m_all_connector.end()) {
        return nullptr;
    }
    return (iter->second);
}

void ConnectManager::read_from(int fd) {
    SPConnector conn = find(fd);
    if (nullptr == conn) {
        LOG_ERROR("Read Error. Can not find fd:{}", fd);
        return;
    }
    m_pool.enqueue(
            [conn](){
                conn->fd_read();
            }
    );
}

} //namespace zj
