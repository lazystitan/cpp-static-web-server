//
// Created by Riton on 2019/9/27.
//

#include <iostream>
#include "request.h"
#include "channel.h"
#include "thread_pool.h"

using namespace std;

void do_something(Request &request) {
    cout << "hello" << endl;
}

int main() {
    Channel<Work> channel(5);

    ThreadPool threadPool(5, channel);

    vector<Request> v(5);
    for (int j = 0; j < 5; ++j) {
        v[j] = Request::test();
    }

    for (int i = 0; i < 10; ++i) {
        Work work;
        work._function = do_something;
        work._request = &v[i];
        channel << work;
    }


    return 0;
}