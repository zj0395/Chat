
#include "client.h"
#include "logs.h"
#include "message/m_person.h"

#include <signal.h>
#include <fstream>

#define SERVER_NAME "127.0.0.1"
#define SERVER_PORT 11111

using namespace std;

void signal_handle(int signal_id) {
    LOG_INFO("Signal {} Trigger", signal_id);
}

int main() {
    zj::Logger::init("client", "client.log");
    zj::Client &client = zj::Client::get_instance();
    if (client.connect_to(SERVER_NAME, SERVER_PORT)) {
        zj::M_Person person(4, "张建", "1043326391@qq.com" );
        person.addPhone(zj::Phone::MOBILE, "18811307371");
        for(int i=1; i<150; ++i) {
            person.addPhone(zj::Phone::HOME, fmt::format("011-{}", i));
        }
        client.get_first()->fd_send(person.serialize());
    } else {
        LOG_ERROR("connect to server fail.");
    }
    return 0;
}
