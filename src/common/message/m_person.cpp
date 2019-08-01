//
// Created by zj on 6/23/19.
//

#include "test.pb.h"
#include "m_person.h"
#include "logs.h"

namespace zj {

SPPackage M_Person::serialize() {
    rpc::Person person;
    person.set_id(m_id);
    person.set_name(m_name);
    person.set_email(m_email);
    for(const Phone& one_phone: m_phones ) {
        rpc::Person_PhoneNumber* n= person.add_phones();
        n->set_number(one_phone.m_number);
        n->set_type(rpc::Person_PhoneType(one_phone.m_type));
    }

    return Message::serialize_help(person);
}

bool M_Person::parse(SPPackage message) {
    rpc::Person person;

    if (!Message::parse_help(person, message)) {
        return false;
    }

    m_name = person.name();
    m_id = person.id();
    m_email = person.email();
    for (int i=0; i<person.phones_size(); ++i) {
        Phone::Type t = static_cast<Phone::Type>(person.phones(i).type());
        addPhone(t, person.phones(i).number());
    }

    LOG_DEBUG("Read person Done");
    log();

    return true;
}

void M_Person::log() {
    LOG_DEBUG(m_name);
    LOG_DEBUG(m_id);
    LOG_DEBUG(m_email);
    for (Phone& p : m_phones) {
        LOG_DEBUG("Type:{},Number:{}", p.m_type, p.m_number);
    }
}

} //namespace zj
