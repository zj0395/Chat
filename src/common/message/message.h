//
// Created by zj on 6/23/19.
//

#ifndef CHAT_MESSAGE_H
#define CHAT_MESSAGE_H

#include "package.h"
#include <string>
#include <unistd.h>
#include <google/protobuf/io/coded_stream.h>

namespace zj {

const size_t MAX_LEN = 1024;

// base class of all message
class Message {
public:
    Message() = default;
    void add_hdr(google::protobuf::io::CodedOutputStream& coded_output, int size) {
        coded_output.WriteVarint32(size);
    }
    virtual SPPackage serialize() = 0;
    virtual bool parse(SPPackage ) = 0;
    virtual void log() {}
};

} //namespace zj



#endif //CHAT_MESSAGE_H
