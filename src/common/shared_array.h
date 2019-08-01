//
// Created by zj on 6/23/19.
//

#ifndef RPC_CHAR_H
#define RPC_CHAR_H

#include <memory>

template <typename T>
std::shared_ptr<T> make_shared_array(size_t size) {
    //default_delete是STL中的默认删除器
    return std::shared_ptr<T>(new T[size], std::default_delete<T[]>());
}

typedef std::shared_ptr<char> SPChar;
inline SPChar make_char_array(size_t size) {
    return make_shared_array<char>(size);
}

#endif //RPC_CHAR_H
