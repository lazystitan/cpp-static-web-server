//
// Created by Riton on 2019/9/27.
//

#ifndef SERVER_THREAD_POOL_H
#define SERVER_THREAD_POOL_H

#include <thread>
#include <vector>

#include "channel.h"
#include "request.h"

struct Work {
    Request *_request;
    void (*_function)(Request&);
    Work() {
        _function = nullptr;
        _request = nullptr;
    }
    void operator()() {
        _function(*_request);
    }
};

class Worker {
public:
    void operator()(int id, Channel<Work> &channel) {
        Work work;
        while (channel >> work) {
            work();
        }
    }
};

class ThreadPool {
private:
    std::vector<std::thread> *_workers;
    Channel<Work> &_channel;
    int _size;
public:
    ThreadPool(int size, Channel<Work> &channel) : _channel(channel), _size(size) {
        _workers = new std::vector<std::thread>(_size);
        for (int i = 0; i < _size; ++i) {
            (*_workers)[i] = std::thread(Worker{}, i, std::ref(channel));
        }
    }

    ~ThreadPool() {
        for (auto &t : *_workers) {
            t.join();
        }
        delete [] _workers;
    }

    void close() {
        _channel.close();
    }
};

#endif //SERVER_THREAD_POOL_H
