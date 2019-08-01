//
// Created by zj on 12/2/18.
//

#ifndef RPC_MUTEX_H
#define RPC_MUTEX_H

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

#endif //RPC_MUTEX_H
