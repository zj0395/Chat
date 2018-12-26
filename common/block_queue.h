//
// Created by zj on 12/25/18.
//

#ifndef CHAT_BLOCK_QUEUE_H
#define CHAT_BLOCK_QUEUE_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

// size(), empty() 没有必要做到线程安全
template <typename T>
class BlockQueue {
public:
    std::size_t size() {
        return _queue.size();
    }
    bool empty() {
        return _queue.empty();
    }

    // 线程安全的函数
    void push(const T& node) {
        std::lock_guard<std::mutex> guard(_mutex);
        _queue.push(node);
        if (_queue.size() == 1) {
            _condition.notify_one();
        }
    }
    T pop() {
        std::unique_lock<std::mutex> guard(_mutex);
        _condition.wait(guard, [&]{return !_queue.empty();});
        T data = _queue.front();
        _queue.pop();
        return data;
    }
    void clear() {
        std::lock_guard<std::mutex> guard(_mutex);
        std::queue<T>().swap(_queue);
    }
    // 放置一个空包，以退出阻塞中的pop
    void quit() {
        push(T());
    }

private:
    std::queue<T> _queue;
    std::mutex _mutex;
    std::condition_variable _condition;
};
#endif
