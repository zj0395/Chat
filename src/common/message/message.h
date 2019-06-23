//
// Created by zj on 6/23/19.
//

#ifndef CHAT_MESSAGE_H
#define CHAT_MESSAGE_H

#include "shared_array.h"
#include "connector.h"
#include <string>
#include <unistd.h>

namespace zj {

const size_t MAX_LEN = 1024;
// base class of all message
class Message {
public:
    Message() = default;
    virtual void send(SPConnector conn) const = 0;
};

} //namespace zj



#endif //CHAT_MESSAGE_H
