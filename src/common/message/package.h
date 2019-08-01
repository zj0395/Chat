//
// Created by zj on 6/28/19.
//

#ifndef RPC_PACKAGE_H
#define RPC_PACKAGE_H

#include "shared_array.h"

#include <memory>

namespace zj {

struct Package;
typedef std::shared_ptr<Package> SPPackage;

struct Package {
public:
    SPChar mes;
    int len; // length of message

    static SPPackage make(int len) {
        return SPPackage(new Package(len));
    }

private:
    Package(int length) : mes(make_char_array(length)), len(length)  {}
};


} // namespace zj

#endif //RPC_PACKAGE_H
