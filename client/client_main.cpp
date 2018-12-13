#include <iostream>
#include "client.h"

#define SERVER_NAME "127.0.0.1"
#define SERVER_PORT "26411"

int main() {
    zj::Client &client = zj::Client::getInstance();
    client.connect_to(SERVER_NAME, SERVER_PORT);
    return 0;
}