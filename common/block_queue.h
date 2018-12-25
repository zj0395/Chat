//
// Created by zj on 12/25/18.
//

#ifndef _BLOCK_QUEUE_H
#define _BLOCK_QUEUE_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

template <typename T>
class BlockQueue {
public:
    // 线程安全的函数
    void push(const T& node) {
        std::lock_guard<std::mutex> guard(_mutex);
        queues.push(node);
        if (queues.size() == 1) {
            _condition.notify_one();
        }
    }
    T pop() {
        std::unique_lock<std::mutex> guard(_mutex);
        _condition.wait(guard, [&]{return !queues.empty();});
        T data = queues.front();
        queues.pop();
        return data;
    }
    void clear() {
        std::lock_guard<std::mutex> guard(_mutex);
        std::queue<T>().swap(queues);
    }
    // 放置一个空包，以退出阻塞中的pop
    void quit() {
        push(T());
    }

private:
    std::queue<T> queues;
    std::mutex _mutex;
    std::condition_variable _condition;
};
#endif
