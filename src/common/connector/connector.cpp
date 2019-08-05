//
// Created by zj on 3/13/19.
//

#include "connector.h"
#include "connect_manager.h"
#include "logs.h"
#include "message.h"

#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <unistd.h>
#include <sys/socket.h>

namespace zj {


void Connector::fd_read() {
    LOG_DEBUG("Begin read From fd:{}", m_fd);

    // the first 4 bytes is the length of message
    char buffer[8] = {0};
    int byteCount = 0;
    if ((byteCount = recv(m_fd, buffer, 8, MSG_PEEK))== -1){
        LOG_ERROR("Recv error from fd:{}, errno:{}", m_fd, errno);
    }

    if (byteCount == 0) {
        LOG_INFO("Can not recv hdr info. Should be close message. Begin close connect.");
        m_manager.remove(m_fd);
        return;
    }

    unsigned int bodyLen, type;
    read_hdr(buffer, bodyLen, type);
    LOG_INFO("Read hdr size:{}, bodyLen:{}", byteCount, bodyLen);
    read_body(bodyLen, type);
}

void Connector::fd_send(SPPackage message) {
    int byteCount = 0;
    if( (byteCount=send(m_fd, message->mes.get(), message->len, 0))== -1 ) {
        LOG_ERROR("Send error, fd:{}, errno:{}", m_fd, errno);
    }
    usleep(1);
}

bool Connector::read_hdr(char* buf, unsigned int & size, unsigned int & type) {
    google::protobuf::io::ArrayInputStream ais(buf, 8);
    google::protobuf::io::CodedInputStream coded_input(&ais);
    coded_input.ReadVarint32(&size);//Decode the HDR and get the size
    coded_input.ReadVarint32(&type);//Decode the HDR and get the size
    return true;
}

void Connector::read_body(unsigned int bodyLen, unsigned int type) {
    LOG_INFO("Will read message from fd:{}, total length:{}", m_fd, bodyLen + 8);

    SPPackage message = Package::make(bodyLen + 8);
    char* buf = message->mes.get();
    int count = recv(m_fd, buf, bodyLen + 8, MSG_WAITALL);

    if (-1 == count) {
        LOG_ERROR("Recv error from fd:{}, errno:{}", m_fd, errno);
        return;
    }
    if (0 == count) {
        LOG_ERROR("Recv 0 byte when read_body, should not be this.");
        return;
    }


    LOG_INFO("fd {} read message len:{}", m_fd, count);
    bool ret = Message::begin_parse(message, type);
    if (ret) {
        LOG_INFO("Parse person from fd:{} success.", m_fd);
    }
}

Connector::~Connector() {
    LOG_INFO("Close Connect, fd:{}", m_fd);
    close(m_fd);
}


} //namespace zj
