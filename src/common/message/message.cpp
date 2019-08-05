//
// Created by zj on 8/6/19.
//

#include "message.h"
#include "m_person.h"

namespace zj {

const std::map<unsigned int, std::function<SPMessage()> >& Message::s_instance_map = {
        {T_Person, get_instance<M_Person>},
};

} //namespace zj
