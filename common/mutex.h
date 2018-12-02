//
// Created by zj on 12/2/18.
//

#ifndef CHAT_MUTEX_H
#define CHAT_MUTEX_H

#include <pthread.h>
namespace zj {
    class Locker {
    public:
        Locker(pthread_mutex_t *mutex);
        ~Locker();
    private:
        pthread_mutex_t *m_t;
    };
}

#endif //CHAT_MUTEX_H
