#include <iostream>
#include "heads.h"
#include "server.h"

using namespace std;

#define SERVER_PORT "11111"

int main() {
    zj::Server &t = zj::Server::getInstance();
    t.init(SERVER_PORT);
    cout << "Server Ready" << endl;
    t.waitForConnect();
    return 0;
}