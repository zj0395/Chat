//
// Created by zj on 6/23/19.
//

#include "test.pb.h"
#include "m_person.h"
#include "logs.h"

#include <google/protobuf/io/zero_copy_stream_impl.h>

namespace zj {

SPPackage M_Person::serialize() {
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
    LOG_INFO("Message size:{}", size);

    SPPackage message = Package::make(size + 4);

    google::protobuf::io::ArrayOutputStream aos(message->mes.get(), message->len);
    google::protobuf::io::CodedOutputStream coded_output(&aos);

    add_hdr(coded_output, size);

    if (person.SerializeToCodedStream(&coded_output)) {
        LOG_INFO("Serialize success. Total len:{}", message->len);
    } else {
        LOG_ERROR("Serialize fail. Need more info to trace this.");
        return nullptr;
    }

    return message;
}

bool M_Person::parse(SPPackage message) {
    chat::Person person;

    google::protobuf::io::ArrayInputStream ais(message->mes.get(), message->len);
    google::protobuf::io::CodedInputStream coded_input(&ais);
    //Read an unsigned integer with Varint encoding, truncating to 32 bits.
    unsigned int t_size;
    coded_input.ReadVarint32(&t_size);
    //After the message's length is read, PushLimit() is used to prevent the CodedInputStream
    //from reading beyond that length.Limits are used when parsing length-delimited
    //embedded messages
    google::protobuf::io::CodedInputStream::Limit msgLimit = coded_input.PushLimit(message->len - 4);
    //De-Serialize
    bool ret = person.ParseFromCodedStream(&coded_input);
    //Once the embedded message has been parsed, PopLimit() is called to undo the limit
    coded_input.PopLimit(msgLimit);

    LOG_INFO("Parse result:{}, size:{}" ,ret, person.ByteSize());
    if (!ret) {
        LOG_WARN("Parse person fail");
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
