//
// Created by zj on 12/2/18.
//

#include "client.h"
#include "logs.h"
#include "heads.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <cstring>

using namespace std;

namespace zj {

bool Client::connect_to(const char *ip, int port) {
    LOG_INFO("Try to connect to {}:{}", ip, port);
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        LOG_ERROR("Fail to get socket");
        return false;
    }

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &server.sin_addr) < 0) {
        LOG_ERROR("Fail to inet_pton");
        return false;
    }

    if (connect(fd, (struct sockaddr *) &server, sizeof(server)) < 0) {
        LOG_ERROR("Fail to connect:{}", strerror(errno));
        return false;
    }

    LOG_INFO("Connect success");
    m_manager.add(fd, TYPE_TO_SERVER);
    return true;
}
}
