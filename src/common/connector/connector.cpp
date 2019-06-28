//
// Created by zj on 3/13/19.
//

#include "connector.h"
#include "message/m_person.h"
#include "connect_manager.h"
#include "logs.h"
#include "test.pb.h"

#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <unistd.h>
#include <sys/socket.h>

namespace zj {


void Connector::fd_read() {
    LOG_DEBUG("Begin read From fd:{}", m_fd);

    // the first 4 bytes is the length of message
    char buffer[4] = {0};
    int byteCount = 0;
    if ((byteCount = recv(m_fd, buffer, 4, MSG_PEEK))== -1){
        LOG_ERROR("Recv error from fd:{}, errno:{}", m_fd, errno);
    }

    if (byteCount == 0) {
        LOG_INFO("Can not recv hdr info. Should be close message. Begin close connect.");
        m_manager.remove(m_fd);
        return;
    }

    unsigned int bodyLen = read_hdr(buffer);
    LOG_INFO("Read hdr size:{}, bodyLen:{}", byteCount, bodyLen);
    read_body(bodyLen);
}

void Connector::fd_send(SPPackage message) {
    int byteCount = 0;
    if( (byteCount=send(m_fd, message->mes.get(), message->len, 0))== -1 ) {
        LOG_ERROR("Send error, fd:{}, errno:{}", m_fd, errno);
    }
    usleep(1);
}

unsigned int Connector::read_hdr(char* buf) {
    google::protobuf::uint32 size;
    google::protobuf::io::ArrayInputStream ais(buf, 4);
    google::protobuf::io::CodedInputStream coded_input(&ais);
    coded_input.ReadVarint32(&size);//Decode the HDR and get the size
    return size;
}

void Connector::read_body(unsigned int bodyLen) {
    LOG_INFO("Will read message from fd:{}, total length:{}", m_fd, bodyLen + 4);

    SPPackage message = Package::make(bodyLen + 4);
    char* buf = message->mes.get();
    int count = recv(m_fd, buf, bodyLen + 4, MSG_WAITALL);

    if (-1 == count) {
        LOG_ERROR("Recv error from fd:{}, errno:{}", m_fd, errno);
        return;
    }
    if (0 == count) {
        LOG_ERROR("Recv 0 byte when read_body, should not be this.");
        return;
    }


    LOG_INFO("fd {} read message len:{}", m_fd, count);
    M_Person person;
    bool ret = person.parse(message);
    if (ret) {
        LOG_INFO("Parse person from fd:{} success.", m_fd);
    }
}

Connector::~Connector() {
    LOG_INFO("Close Connect, fd:{}", m_fd);
    close(m_fd);
}


} //namespace zj
