//
// Created by zj on 12/2/18.
//

#include "client.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <cstring>
#include <iostream>

using namespace std;

namespace zj {
    Client::Client()  {

    }
    Client::~Client() {

    }

    bool Client::connect_to(const char *ip, const char *port) {
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        if( fd < 0 ) {
            cerr << "socket fail" <<endl;
            return false;
        }

        struct sockaddr_in server;
        memset(&server, 0, sizeof(server));
        server.sin_family = AF_INET;
        server.sin_port = atoi(port);
        if (inet_pton(AF_INET, ip, &server.sin_addr) < 0) {
            cerr << "inet_pton fail" <<endl;
            return false;
        }

        if (connect(fd, (struct sockaddr*)&server, sizeof(server)) < 0) {
            cerr << "connect fail:" << strerror(errno) <<endl;
            return false;
        }

        return false;
    }
}
