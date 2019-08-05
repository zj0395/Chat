//
// Created by zj on 6/23/19.
//

#ifndef RPC_MESSAGE_H
#define RPC_MESSAGE_H

#include "logs.h"
#include "package.h"
#include "defines.h"

#include <string>
#include <unistd.h>
#include <map>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

namespace zj {

const size_t MAX_LEN = 1024;

class Message;
typedef std::shared_ptr<Message> SPMessage;

// base class of all message
class Message {
public:
    Message() = default;

    template <typename T>
    static SPMessage get_instance() {
        return std::make_shared<T>();
    }

    virtual SPPackage serialize() = 0;
    virtual bool parse(SPPackage) = 0;
    virtual bool exec() = 0;
    virtual void log() {}

    static bool begin_parse(SPPackage message, unsigned int type) {
        auto iter = s_instance_map.find(type);
        if (iter == s_instance_map.end())
            return false;
        SPMessage mes = (iter->second)();
        mes->parse(message);
        mes->exec();
        return true;
    }

    template<typename T>
    SPPackage serialize_help(T* ig, typename T::Protobuf_Obj& obj) {
        int size = obj.ByteSize();
        LOG_INFO("Message size:{}", size);
        SPPackage message = Package::make(size + 8);

        google::protobuf::io::ArrayOutputStream aos(message->mes.get(), message->len);
        google::protobuf::io::CodedOutputStream coded_output(&aos);

        coded_output.WriteVarint32(size);
        coded_output.WriteVarint32(T::s_type);

        if (obj.SerializeToCodedStream(&coded_output)) {
            LOG_INFO("Serialize success. Total len:{}", message->len);
        } else {
            LOG_ERROR("Serialize fail. Need more info to trace this.");
            return nullptr;
        }
        //message->len = coded_output.ByteCount();
        return message;
    }

    template<typename T>
    bool parse_help(T& info, SPPackage message){
        google::protobuf::io::ArrayInputStream ais(message->mes.get(), message->len);
        google::protobuf::io::CodedInputStream coded_input(&ais);
        //Read an unsigned integer with Varint encoding, truncating to 32 bits.

        unsigned int t_size;
        coded_input.ReadVarint32(&t_size);
        unsigned int t_type;
        coded_input.ReadVarint32(&t_type);

        //After the message's length is read, PushLimit() is used to prevent the CodedInputStream
        //from reading beyond that length.Limits are used when parsing length-delimited
        //embedded messages
        google::protobuf::io::CodedInputStream::Limit msgLimit = coded_input.PushLimit(message->len);
        //De-Serialize
        bool ret = info.ParseFromCodedStream(&coded_input);
        //Once the embedded message has been parsed, PopLimit() is called to undo the limit
        coded_input.PopLimit(msgLimit);

        LOG_INFO("Parse result:{}, size:{}, message->len:{}", ret, info.ByteSize(), message->len);
        if (!ret) {
            LOG_WARN("Parse person fail");
            return false;
        }
        return true;
    }

private:
    const static std::map<unsigned int, std::function<SPMessage()> >& s_instance_map;
};

} //namespace zj



#endif //RPC_MESSAGE_H
