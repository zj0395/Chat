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
    Package(int length) : mes(make_char_array(length)), len(length)  {}

    SPChar mes;
    int len;

    static SPPackage make(int length) {
        return SPPackage(new Package(length));
    }
};


} // namespace zj

#endif //RPC_PACKAGE_H
