//
// Created by zj on 8/6/19.
//

#include "message.h"
#include "person/m_person.h"

namespace zj {

const std::map<unsigned int, std::function<SPMessage()> >& Message::s_instance_map = {
        {M_Person::s_type, get_instance<M_Person>},
};

} //namespace zj
