//
// Created by zj on 12/2/18.
//

#ifndef RPC_CLIENT_H
#define RPC_CLIENT_H

#include "singleton.h"
#include "connect_manager.h"
#include <string>

namespace zj {
class Client : public Singleton<Client> {
    friend class Singleton<Client>;

private:
    Client() : m_pool(4), m_manager(m_pool) {}

    std::string m_serverIp;
    int m_serverPort;
    ThreadPool m_pool;
    ConnectManager m_manager; // often only have one fd, but allow many

public:
    SPConnector get_first() { return m_manager.get_first(); }
    bool connect_to(const char *ip, int port);
};
}

#endif //RPC_CLIENT_H
