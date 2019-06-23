#include "client.h"

#include <fstream>

#include "logs.h"
#include "message/m_person.h"

#include <signal.h>

#define SERVER_NAME "127.0.0.1"
#define SERVER_PORT 11111

using namespace std;

void signal_handle(int signal_id) {
    LOG_INFO("Signal {} Trigger", signal_id);
}

int main() {
    zj::Logger::init("client", "client.log");
    zj::Client &client = zj::Client::get_instance();
    client.connect_to(SERVER_NAME, SERVER_PORT);

    zj::M_Person person(4, "张建", "1043326391@qq.com" );
    person.addPhone(zj::Phone::MOBILE, "18811307371");
    person.addPhone(zj::Phone::HOME, "011-1111111");
    person.send(client.get_first());

    return 0;
}
