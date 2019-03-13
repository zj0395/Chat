//
// Created by zj on 3/13/19.
//

#include "connector.h"
#include "logs.h"
#include "test.pb.h"

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

void Connector::fd_send() {
    chat::Person person_test;
    person_test.set_name("张建");
    person_test.set_id(4);
    person_test.set_email("1043326391@qq.com");
    {
        chat::Person_PhoneNumber* n1= person_test.add_phones();
        n1->set_number("18811307371");
        n1->set_type(chat::Person_PhoneType(0));
        chat::Person_PhoneNumber* n2= person_test.add_phones();
        n2->set_number("0395-444444");
        n2->set_type(chat::Person_PhoneType(1));
    }
    int size = person_test.ByteSize();
    char data[521];
    bool ret = person_test.SerializeToArray(data, size);
    LOG_INFO("Serialize to fd {}, {}, {}", m_fd, ret, size);
    write(m_fd, data, 521);
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
}

ConnectManager::~ConnectManager() {
    close(m_epoll_fd);
}

int ConnectManager::add(int fd, const std::string &desc) {
    m_manager.insert(std::make_pair(fd, std::make_shared<Connector>(fd, desc)));
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    if ( epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, fd, &event) != 0 ) {
        LOG_ERROR("epoll_ctl error.errno:{}, fd:{}, m_epoll_fd:{}, reason:{}", errno, fd, m_epoll_fd, strerror(errno));
    }
    return 0;
}

void ConnectManager::read_begin() {
    LOG_INFO("read_begin");
    while (m_run_flag) {
        int n = epoll_wait(m_epoll_fd, m_events, EPOLL_MAX_FD, -1);
        LOG_DEBUG("Message number:{}", n);
        for (int i=0; i<n; ++i) {
            int fd = m_events[i].data.fd;
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
    }
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
} //namespace zj
