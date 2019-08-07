//
// Created by zj on 2/24/19.
//

#include "gtest/gtest.h"
#include "package.h"
#include "person/m_person.h"
#include <fstream>

using namespace std;

bool write() {
    zj::M_Person person_test(4, "张建", "1043326391@qq.com");
    person_test.addPhone(zj::Phone::MOBILE, "18811307371");
    person_test.addPhone(zj::Phone::HOME, "0395-444444");

    zj::SPPackage mes = person_test.serialize();
    if (mes == nullptr)
        return false;
    ofstream output("heartbeat.db", ios::out|ios::trunc|ios::binary);
    output.write(mes->mes.get(), mes->len);
    output.close();
    return true;
}

TEST(protocol, test) {
    ASSERT_TRUE(write());
    //rpc::Person person;
    ifstream input("heartbeat.db", ios::in|ios::binary);

    input.seekg (0, input.end);
    int length = input.tellg();
    input.seekg (0, input.beg);

    zj::SPPackage mes = zj::Package::make(length);
    input.read(mes->mes.get(), mes->len);

    unsigned int bodyLen, type;
    zj::Message::read_hdr(mes->mes.get(), bodyLen, type);

    bool ret = zj::Message::begin_parse(mes, type);
    ASSERT_TRUE(ret);
}