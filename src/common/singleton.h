//
// Created by zj on 12/2/18.
// Singleton template.
// use get_instance() function to control the order of instance initialize.
//

#ifndef RPC_SINGLETON_H
#define RPC_SINGLETON_H

namespace zj {
template<typename T>
class Singleton {
public:
    inline static T &get_instance() {
        static T instance;
        return instance;
    }

protected:
    Singleton() = default;

    ~Singleton() = default;

    Singleton(Singleton &) = delete;

    Singleton(Singleton &&) = delete;
};
} //namespace zj


#endif //RPC_SINGLETON_H
