//
// Created by zj on 3/13/19.
//

#ifndef CHAT_CONNECTOR_H
#define CHAT_CONNECTOR_H

#include <string>
#include <map>

namespace zj {

class Connector {
public:
    Connector(int fd, const std::string& desc) : _fd(fd), _desc(desc) {}
private:
    int _fd;
    std::string _desc; // description
};

class ConnectManager{
public:
    ConnectManager() {

    }
    void add(int fd) {

    }
    Connector& find(int fd);
    Connector& find(const std::string& desc);
private:
    
};

} //namespace zj

#endif //CHAT_CONNECTOR_H
