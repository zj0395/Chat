//
// Created by zj on 3/13/19.
//

#ifndef RPC_CONNECTOR_H
#define RPC_CONNECTOR_H

#include "message/package.h"
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
    const std::string get_desc() { return m_desc; }
    void fd_read();
    void fd_send(SPPackage message);

private:
    int m_fd;
    std::string m_desc; // description
    ConnectManager& m_manager;

private:
    void read_body(unsigned int bodyLen);
    unsigned int read_hdr(char* buf);
};

} //namespace zj

#endif //RPC_CONNECTOR_H
