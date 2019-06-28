//
// Created by zj on 6/23/19.
//

#ifndef CHAT_M_PERSON_H
#define CHAT_M_PERSON_H

#include "message.h"
#include <vector>

namespace zj {

struct Phone {
    enum Type {
        MOBILE = 0, // should same with the defines in .proto file
        HOME,
        WORK
    };
    Phone(Phone::Type type, const std::string& number) : m_type(type), m_number(number) {}
    Type m_type;
    std::string m_number;
};

class M_Person : public Message{
public:
    M_Person() = default;
    M_Person(int id, std::string name, std::string email) : m_id(id), m_name(name), m_email(email) {}
    void addPhone(Phone::Type type, std::string number) {
        m_phones.emplace_back(type, number);
    }
    SPPackage serialize() override;
    bool parse(SPPackage mes) override;
    void log() override;

private:
    int m_id;
    std::string m_name;
    std::string m_email;
    std::vector<Phone> m_phones;
};

} //namespace zj

#endif //CHAT_M_PERSON_H
