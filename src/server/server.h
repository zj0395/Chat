//
// Created by zj on 12/2/18.
//

#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include "singleton.h"
#include "connect_manager.h"
#include <string>

namespace zj {
class Server : public Singleton<Server> {
    friend class Singleton<Server>;

public:
    bool init(const char *port);

    void stop();

    void wait_for_connect();

private:
    Server();

    ~Server();

private:
    std::string m_port;
    bool m_init_flag;
    int m_sockfd;
    bool m_run_flag;
    ThreadPool m_pool;
    ConnectManager m_manager;
};
} //namespace zj

#endif //CHAT_SERVER_H
