//
// Created by zj on 12/2/18.
//

#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

#include "singleton.h"
#include "connector.h"
#include <string>

namespace zj {
class Client : public Singleton<Client> {
    friend class Singleton<Client>;

private:
    Client() = default;

    std::string m_serverIp;
    int m_serverPort;
    ConnectManager m_manager;
public:
    bool connect_to(const char *ip, int port);
};
}

#endif //CHAT_CLIENT_H
