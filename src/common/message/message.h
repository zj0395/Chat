//
// Created by zj on 6/23/19.
//

#ifndef RPC_MESSAGE_H
#define RPC_MESSAGE_H

#include "logs.h"
#include "package.h"
#include <string>
#include <unistd.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

namespace zj {

const size_t MAX_LEN = 1024;

// base class of all message
class Message {
public:
    Message() = default;

    virtual SPPackage serialize() = 0;
    virtual bool parse(SPPackage ) = 0;
    virtual void log() {}

    template<typename T>
    SPPackage serialize_help(T& info) {
        int size = info.ByteSize();
        LOG_INFO("Message size:{}", size);
        SPPackage message = Package::make(size + 4);

        google::protobuf::io::ArrayOutputStream aos(message->mes.get(), message->len);
        google::protobuf::io::CodedOutputStream coded_output(&aos);

        coded_output.WriteVarint32(size);

        if (info.SerializeToCodedStream(&coded_output)) {
            LOG_INFO("Serialize success. Total len:{}", message->len);
        } else {
            LOG_ERROR("Serialize fail. Need more info to trace this.");
            return nullptr;
        }
        return message;
    }

    template<typename T>
    bool parse_help(T& info, SPPackage message) {
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
        bool ret = info.ParseFromCodedStream(&coded_input);
        //Once the embedded message has been parsed, PopLimit() is called to undo the limit
        coded_input.PopLimit(msgLimit);

        LOG_INFO("Parse result:{}, size:{}, t_size:{}, message->len:{}", ret, info.ByteSize(), t_size, message->len);
        if (!ret) {
            LOG_WARN("Parse person fail");
            return false;
        }
        return true;
    }
};

} //namespace zj



#endif //RPC_MESSAGE_H
