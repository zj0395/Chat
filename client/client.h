//
// Created by zj on 12/2/18.
//

#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

#include "singleton.h"
#include <string>

namespace zj {
class Client : public Singleton<Client> {
    friend class Singleton<Client>;

private:
    Client();

    ~Client();

    std::string m_serverIp;
    std::string m_serverPort;
public:
    bool connect_to(const char *ip, const char *port);
};
}

#endif //CHAT_CLIENT_H
