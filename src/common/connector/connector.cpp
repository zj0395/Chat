//
// Created by zj on 3/13/19.
//

#include "connector.h"
#include "connect_manager.h"
#include "logs.h"
#include "test.pb.h"

#include <unistd.h>

namespace zj {

void Connector::fd_read() {
    LOG_DEBUG("Read From fd:{}", m_fd);
    chat::Person person;
    char buf[1024];
    int count = read (m_fd, buf, sizeof(buf));
    if (0 == count) {
        LOG_INFO("fd {} receive empty message", m_fd);
        m_manager.remove(m_fd);
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

} //namespace zj
