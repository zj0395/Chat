//
// Created by zj on 2/24/19.
//

#include <fstream>
#include "test.pb.h"
#include "gtest/gtest.h"

using namespace std;

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
}

TEST(protocol, test) {
    write();
    chat::Person person;
    ifstream output("heartbeat.db", ios::in|ios::binary);
    bool ret = person.ParseFromIstream(&output);
    ASSERT_TRUE(ret);
    ASSERT_EQ(person.name(), "张建");
    ASSERT_EQ(person.id(), 4);
    ASSERT_EQ(person.email(), "1043326391@qq.com");
    ASSERT_EQ(person.phones_size(), 2);
    ASSERT_EQ(person.phones(0).number(), "18811307371");
    ASSERT_EQ(person.phones(0).type(), chat::Person_PhoneType(0));
    ASSERT_EQ(person.phones(1).number(), "0395-444444");
    ASSERT_EQ(person.phones(1).type(), chat::Person_PhoneType(1));
}