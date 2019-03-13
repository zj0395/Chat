#include "client.h"

#include <fstream>

#include "logs.h"

#include <signal.h>

#define SERVER_NAME "127.0.0.1"
#define SERVER_PORT 11111

using namespace std;

void signal_handle(int signal_id) {
    LOG_INFO("Signal {} Trigger", signal_id);
}

int main() {
    zj::Logger::init("client");
    zj::Client &client = zj::Client::get_instance();
    client.connect_to(SERVER_NAME, SERVER_PORT);

    return 0;
}
