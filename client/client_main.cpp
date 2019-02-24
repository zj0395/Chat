#include "client.h"

#include <fstream>

#include "logs.h"
#include "test.pb.h"

#include <signal.h>

#define SERVER_NAME "127.0.0.1"
#define SERVER_PORT 11111

using namespace std;

void signal_handle(int signal_id) {
    LOG_INFO("Signal {} Trigger", signal_id);
}

void write() {
    chat::Person person_test;
    person_test.set_name("张建");
    person_test.set_id(4);
    person_test.set_email("1043326391@qq.com");
    {
        chat::Person_PhoneNumber* n1= person_test.add_phones();
        n1->set_number("18811307371");
        n1->set_type(chat::Person_PhoneType(0));
        chat::Person_PhoneNumber* n2= person_test.add_phones();
        n2->set_number("0395-444444");
        n2->set_type(chat::Person_PhoneType(1));
    }

    ofstream output("heartbeat.db",ios::out|ios::trunc|ios::binary);

    bool ret = person_test.SerializeToOstream(&output);
    LOG_INFO("Serialize {}", ret);
}

void print(chat::Person & person) {
    fmt::print("name:{}\n", person.name());
    fmt::print("id:{}\n", person.id());
    fmt::print("email:{}\n", person.email());
    for (int i = 0; i < person.phones_size(); ++i) {
        const chat::Person_PhoneNumber& phone = person.phones(i);
        fmt::print("phone{}:\ntype:{}, number:{}\n", i+1, phone.type(), phone.number());
    }
}

void read() {
    chat::Person person_test;
    ifstream output("heartbeat.db", ios::in|ios::binary);
    bool ret = person_test.ParseFromIstream(&output);
    LOG_INFO("Unerialize {}", ret);
    print(person_test);
}

int main() {
    zj::Client &client = zj::Client::get_instance();
    client.connect_to(SERVER_NAME, SERVER_PORT);

    write();
    read();

    return 0;
}
