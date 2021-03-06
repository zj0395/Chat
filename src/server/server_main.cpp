#include "type_heads.h"
#include "server.h"
#include "logs.h"
#include "fmt/format.h"
#include <signal.h>
using namespace std;

#define SERVER_PORT "11111"

void signal_handle(int signal_id) {
    LOG_INFO("Signal {} Trigger", signal_id);
    zj::Server::get_instance().stop();
}

int main() {
    zj::Logger::init("server", "server.log");
    signal(SIGINT, signal_handle);
    fmt::print("Hello, {}!\n", "world");
    zj::Server &server = zj::Server::get_instance();
    server.init(SERVER_PORT);
    server.wait_for_connect();
    return 0;
}
