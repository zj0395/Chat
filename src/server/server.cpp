//
// Created by zj on 12/2/18.
//

#include "logs.h"
#include "server.h"
#include "heads.h"
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <cstring>

#define QLEN 10
using namespace std;

namespace {
int init_server(int type, const struct sockaddr *addr, socklen_t alen, int qlen) {
    char ff[100];
    LOG_DEBUG("Server ip:{}, port:{}", inet_ntop(AF_INET, &((struct sockaddr_in *) addr)->sin_addr, ff, 100),
            ntohs(((struct sockaddr_in *) addr)->sin_port));
    int fd = socket(addr->sa_family, type, 0);
    LOG_INFO("Server fd:{}", fd);
    if (fd < 0) {
        LOG_ERROR("Fail to get socket");
        return false;
    }

    if (bind(fd, addr, alen) < 0) {
        LOG_ERROR("Fail to bind");
        return false;
    }

    if (listen(fd, qlen) < 0) {
        LOG_ERROR("Fail to listen");
        return false;
    }
    return fd;
}
}

namespace zj {
Server::Server() : m_pool(8), m_manager(m_pool) {
    m_sockfd = 0;
    m_init_flag = false;
    m_run_flag = true;
}

Server::~Server() {
    LOG_INFO("Server destructor");
}

bool Server::init(const char *port) {
    if (m_init_flag) {
        return true;
    }

    char host[128];
    int err;

    if ((err = gethostname(host, 128)) < 0) {
        LOG_ERROR("Fail to gethostname");
        return false;
    }

    struct addrinfo *ailist, *aip;
    struct addrinfo hint;
    memset(&hint, 0, sizeof(hint));
    hint.ai_flags = AI_CANONNAME;
    hint.ai_socktype = SOCK_STREAM;
    if ((err = getaddrinfo(host, port, &hint, &ailist)) < 0) {
        LOG_ERROR("Fail to getaddrinfo");
        return false;
    }

    int sockfd = 0;
    for (aip = ailist; aip != NULL; aip = aip->ai_next) {
        if ((sockfd = init_server(SOCK_STREAM, aip->ai_addr, aip->ai_addrlen, QLEN)) > 0) {
            m_port = port;
            m_sockfd = sockfd;
            m_init_flag = true;
            LOG_INFO("Server init success");
            return true;
        }
    }

    LOG_ERROR("Server init fail");
    return false;
}

void Server::wait_for_connect() {
    int clfd = 0;
    while (m_run_flag) {
        if ((clfd = accept(m_sockfd, NULL, NULL)) < 0) {
            if (m_run_flag) {
                LOG_ERROR("Error when accept");
            }
            return;
        }
        LOG_INFO("New connect fd:{}", clfd);
        m_manager.add(clfd, TYPE_TO_CLIENT);
        usleep(10000);
    }
}

void Server::stop() {
    LOG_INFO("Server stop");
    m_run_flag = false;
    m_manager.read_stop();
    close(m_sockfd);
}
}
