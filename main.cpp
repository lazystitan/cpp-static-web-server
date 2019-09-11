//
// Created by Riton on 2019/9/10.
//

#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <vector>
#include <condition_variable>
#include <chrono>
#include "channel.h"

using namespace std;
using namespace chrono_literals;

struct Worker {
    void operator() (Channel<int> channel) {
        int n;
        cout << "thread id:" << this_thread::get_id() <<endl;
        while (channel >> n) {
            cout << n << endl;
            this_thread::sleep_for(100ms);
        }
    }
};



int main(int arg, char *argv[]) {
    cout << "start server at port " << argv[1] << endl;

    Channel<int> channel(20);

    mutex mtx;
    condition_variable cv;
    thread threads[5];
    for (auto & i : threads) {
        i = thread(Worker{}, channel);
    }

    for (int j = 0; j < 10; ++j) {
        channel << j;
//        cout << j <<endl;
    }

    for(auto &t : threads)
        t.join();

    return 0;
}

