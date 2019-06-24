//
// Created by zj on 3/13/19.
//

#ifndef CHAT_CONNECTOR_H
#define CHAT_CONNECTOR_H

#include "ThreadPool/ThreadPool.h"
#include <sys/epoll.h>
#include <string>
#include <map>
#include <memory>
#include <thread>
#include <mutex>

namespace zj {

class ConnectManager;
class Connector;
typedef std::shared_ptr<Connector> SPConnector;
class Connector {
public:
    Connector(int fd, const std::string& desc, ConnectManager& manager) : m_fd(fd), m_desc(desc), m_manager(manager) {}
    ~Connector();
    int get_fd() { return m_fd; }
    void fd_read();
    const std::string get_desc() { return m_desc; }
private:
    int m_fd;
    std::string m_desc; // description
    ConnectManager& m_manager;
};

} //namespace zj

#endif //CHAT_CONNECTOR_H
