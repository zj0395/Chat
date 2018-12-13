//
// Created by zj on 12/2/18.
//

#ifndef CHAT_SINGLETON_H
#define CHAT_SINGLETON_H

namespace zj {
template<typename T>
class Singleton {
public:
    static T &getInstance() {
        return s_instance;
    }

protected:
    Singleton() = default;

    ~Singleton() = default;

    Singleton(Singleton &) = delete;

    Singleton(Singleton &&) = delete;

    static T s_instance;
};

template<typename T>
T Singleton<T>::s_instance;
};

#endif //CHAT_SINGLETON_H
