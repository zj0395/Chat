//
// Created by zj on 12/2/18.
//

#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include <string>
#include "singleton.h"

namespace zj {
class Server : public Singleton<Server> {
    friend class Singleton<Server>;

public:
    bool init(const char *port);

    void waitForConnect();

private:
    Server();

    ~Server();

private:
    std::string m_port;
    bool m_initFlag;
    int m_sockfd;
    bool m_runFlag;
};
}

#endif //CHAT_SERVER_H
