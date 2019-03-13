//
// Created by zj on 3/13/19.
//

#ifndef CHAT_CONNECTOR_H
#define CHAT_CONNECTOR_H

#include <sys/epoll.h>
#include <string>
#include <map>
#include <memory>

namespace zj {

class Connector {
public:
    Connector(int fd, const std::string& desc) : m_fd(fd), m_desc(desc) {}
    ~Connector();
    void fd_read();
    void fd_send();
    const std::string get_desc() { return m_desc; }
private:
    int m_fd;
    std::string m_desc; // description
};

typedef std::shared_ptr<Connector> SPConnector;

class ConnectManager{
public:
    ConnectManager();
    ~ConnectManager();
    int add(int fd, const std::string& desc);
    void read_begin();
    void read_stop() { m_run_flag = false; }
    SPConnector find(int fd);
    SPConnector find(const std::string& desc);

private:
    std::map<int, SPConnector> m_manager;
    int m_epoll_fd;
    bool m_run_flag;
    const int EPOLL_MAX_FD = 500;
    struct epoll_event* m_events;
};

} //namespace zj

#endif //CHAT_CONNECTOR_H
