//
// Created by zj on 3/13/19.
//

#ifndef CHAT_CONNECTOR_H
#define CHAT_CONNECTOR_H

#include <sys/epoll.h>
#include <string>
#include <map>
#include <memory>
#include <thread>
#include <mutex>

namespace zj {

class ConnectManager;
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

typedef std::shared_ptr<Connector> SPConnector;

class ConnectManager{
public:
    ConnectManager();
    ~ConnectManager();
    int add(int fd, const std::string& desc);
    void remove(int fd);
    void read_function();
    void read_stop();
    SPConnector get_first();
    SPConnector find(int fd);
    SPConnector find(const std::string& desc);

private:
    std::map<int, SPConnector> m_all_connector;
    std::mutex m_mutex; // mutex for 'm_all_connector'
    int m_epoll_fd;
    bool m_run_flag;
    const int EPOLL_MAX_FD = 500;
    struct epoll_event* m_events;
    int m_pipefds[2]; // a pipe to wakeup epoll_wait
    std::thread m_epoll_thread;

private:
    void init_wakeup_pipe();
    void wakeup_epoll(); // wakeup epoll_wait
};

} //namespace zj

#endif //CHAT_CONNECTOR_H
