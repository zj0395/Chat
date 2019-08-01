//
// Created by zj on 6/24/19.
//

#ifndef RPC_CONNECT_MANAGER_H
#define RPC_CONNECT_MANAGER_H


#include "connector.h"
#include "ThreadPool/ThreadPool.h"
#include <string>
#include <map>
#include <memory>
#include <thread>
#include <mutex>

namespace zj {

class ConnectManager{
public:
    ConnectManager(ThreadPool& pool);
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
    ThreadPool& m_pool;

private:
    void init_wakeup_pipe();
    void wakeup_epoll(); // wakeup epoll_wait
    void read_from(int fd);
};

} //namespace zj


#endif //RPC_CONNECT_MANAGER_H
