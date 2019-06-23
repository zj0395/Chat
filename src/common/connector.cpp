//
// Created by zj on 3/13/19.
//

#include "connector.h"
#include "logs.h"
#include "test.pb.h"

#include <unistd.h>

namespace zj {

void Connector::fd_read() {
    LOG_DEBUG("Read From fd:{}", m_fd);
    chat::Person person;
    char buf[1024];
    int count = read (m_fd, buf, sizeof(buf));
    if (count <= 0) {
        LOG_INFO("fd {} receive empty message", m_fd);
        return;
    }
    bool ret = person.ParseFromArray(buf, count);
    LOG_INFO("{}, {}, {}", count ,ret, person.ByteSize());
    if (!ret) {
        LOG_WARN("Parse From fd Fail", m_fd);
        //return;
    }
    LOG_DEBUG(person.name());
    LOG_DEBUG(person.id());
    LOG_DEBUG(person.email());
    for (int i=0; i<person.phones_size(); ++i) {
        LOG_DEBUG("{},Type:{},Number:{}", i+1, person.phones(i).type(), person.phones(i).number());
    }
    LOG_DEBUG("Read Done From fd:{}", m_fd);
}

Connector::~Connector() {
    LOG_INFO("Close Connect, fd:{}", m_fd);
    close(m_fd);
}

ConnectManager::ConnectManager() {
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

void ConnectManager::init_wakeup_pipe()
{
    pipe(m_pipefds);
    int read_pipe = m_pipefds[0];
    int write_pipe = m_pipefds[1];
    LOG_INFO("Init wakeup pipes. {}-{}", read_pipe, write_pipe);

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

int ConnectManager::add(int fd, const std::string &desc) {
    m_manager.insert(std::make_pair(fd, std::make_shared<Connector>(fd, desc)));
    struct epoll_event event = {};
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    if ( epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, fd, &event) != 0 ) {
        LOG_ERROR("epoll_ctl error.errno:{}, fd:{}, m_epoll_fd:{}, reason:{}", errno, fd, m_epoll_fd, strerror(errno));
    }
    return 0;
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
                LOG_INFO("Wakeup by pipe, {}", fd);
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
                m_manager.erase(fd);
                continue;
            } else {
                find(fd)->fd_read();
            }
        }
        LOG_INFO("read_function loop end");
    }
    LOG_INFO("End read_function");
}

SPConnector ConnectManager::find(int fd) {
    auto iter = m_manager.find(fd);
    if (iter == m_manager.end())
        return nullptr;
    return (iter->second);
}

SPConnector ConnectManager::find(const std::string &desc) {
    auto iter = m_manager.begin();
    while (iter != m_manager.end()) {
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
    auto iter = m_manager.begin();
    if (iter == m_manager.end()) {
        return nullptr;
    }
    return (iter->second);
}
} //namespace zj
