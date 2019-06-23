//
// Created by zj on 6/23/19.
//

#include "test.pb.h"
#include "m_person.h"
#include "logs.h"

namespace zj {

void M_Person::send(SPConnector conn) const {
    if (nullptr == conn) {
        LOG_INFO("Send Message to nullptr");
        return;
    }
    chat::Person person;
    person.set_id(m_id);
    person.set_name(m_name);
    person.set_email(m_email);
    for(const Phone& one_phone: m_phones ) {
        chat::Person_PhoneNumber* n= person.add_phones();
        n->set_number(one_phone.m_number);
        n->set_type(chat::Person_PhoneType(one_phone.m_type));
    }
    int size = person.ByteSize();
    char data[MAX_LEN];
    while (size > 0) {
        int len = size < MAX_LEN ? size : MAX_LEN;
        int ret = person.SerializeToArray(data, len);
        LOG_INFO("Serialize to fd {}, {}, {}", conn->get_fd(), ret, len);
        if ( ret ) {
            write(conn->get_fd(), data, len);
            usleep(len * 5); // 200KB = 1s, 1KB = 5ms, 1B = 5us
        }
        size -= len;
    }
}

} //namespace zj
