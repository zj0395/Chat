//
// Created by zj on 6/28/19.
//

#ifndef CHAT_PACKAGE_H
#define CHAT_PACKAGE_H

#include "shared_array.h"

#include <memory>

namespace zj {

struct Package;
typedef std::shared_ptr<Package> SPPackage;

struct Package {
public:
    Package(int length) : mes(make_char_array(length)), len(length)  {}

    SPChar mes;
    int len;

    static SPPackage make(int length) {
        return SPPackage(new Package(length));
    }
};


} // namespace zj

#endif //CHAT_PACKAGE_H
