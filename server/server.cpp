//
// Created by zj on 12/2/18.
//

#include "server.h"
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <cstring>
#include <iostream>

#define QLEN 10
using namespace std;

namespace {
int initServer(int type, const struct sockaddr *addr, socklen_t alen, int qlen) {
    char ff[100];
    addr->sa_data;
//        cout << inet_ntop(AF_INET, &((struct sockaddr_in *)addr)->sin_addr, ff, 100) <<endl;
//        cout << ntohs( ((struct sockaddr_in *)addr)->sin_port )<< endl;
    int fd = socket(addr->sa_family, type, 0);
    if (fd < 0) {
        cerr << "socket fail" << endl;
        return false;
    }

    if (bind(fd, addr, alen) < 0) {
        cerr << "bind fail" << endl;
        return false;
    }

    if (listen(fd, qlen) < 0) {
        cerr << "listern fail" << endl;
        return false;
    }
    return fd;
}
}

namespace zj {
Server::Server() {
    m_sockfd = 0;
    m_initFlag = false;
    m_runFlag = true;
}

Server::~Server() {
    cout << "DESTROCUOR" << endl;
}

bool Server::init(const char *port) {
    if (m_initFlag) {
        return true;
    }

    char host[128];
    int err;

    if ((err = gethostname(host, 128)) < 0) {
        cerr << "gethostname fail" << endl;
        return false;
    }

    struct addrinfo *ailist, *aip;
    struct addrinfo hint;
    memset(&hint, 0, sizeof(hint));
    hint.ai_flags = AI_CANONNAME;
    hint.ai_socktype = SOCK_STREAM;
    if ((err = getaddrinfo(host, port, &hint, &ailist)) < 0) {
        cerr << "getaddrinfo fail" << endl;
        return false;
    }

    int sockfd = 0;
    for (aip = ailist; aip != NULL; aip = aip->ai_next) {
        if ((sockfd = initServer(SOCK_STREAM, aip->ai_addr, aip->ai_addrlen, QLEN)) > 0) {
            m_port = port;
            m_sockfd = sockfd;
            m_initFlag = true;
            return true;
        }
    }

    return false;
}

void Server::waitForConnect() {
    int clfd;
    while (m_runFlag) {
        if ((clfd = accept(m_sockfd, NULL, NULL)) < 0) {
            cerr << "Error When Accept" << endl;
            return;
        }
        cout << "New Connect. Id: " << clfd << endl;
        usleep(10000);
    }
}
}