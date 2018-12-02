//
// Created by zj on 12/2/18.
//

#include "mutex.h"
namespace zj {
    Locker::Locker(pthread_mutex_t *mutex) {
        m_t = mutex;
        if (m_t != NULL)
            pthread_mutex_lock(mutex);
    }

    Locker::~Locker() {
        if (m_t != NULL)
            pthread_mutex_unlock(m_t);
    }
}