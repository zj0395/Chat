//
// Created by zj on 12/2/18.
//

#ifndef CHAT_SINGLETON_H
#define CHAT_SINGLETON_H

#include <memory>
#include <iostream>

namespace zj {
    template <typename T>
    class Singleton {
    public:
        static T* getInstance() {
            return m_instance;
        }

    protected:
        Singleton() { m_instance = instance.get(); };
        ~Singleton() = default;
        Singleton(Singleton&) = delete;
        Singleton(Singleton&&) = delete;
        static void destoryInstance( T *t) {
            delete t;
        }
        static std::shared_ptr<T> instance;
        static T *m_instance;
    };

    template <typename T>
    std::shared_ptr<T> Singleton<T>::instance(new T, Singleton<T>::destoryInstance);
    template <typename T>
    T* Singleton<T>::m_instance = instance.get();
}

#endif //CHAT_SINGLETON_H
