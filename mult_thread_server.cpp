//
// Created by Riton on 2019/9/28.
//

#include <iostream>
#include <cstring>

#include "mult_thread_server.h"
#include "request.h"
#include "server.h"
#include "thread_pool.h"

#define BUFFER_LEN 4096

using namespace std;

void handle_request(Request &request) {
    char buffer[BUFFER_LEN];
    bzero(buffer, BUFFER_LEN);
    request.read(buffer, BUFFER_LEN - 1);
    cout << buffer << endl;
    char content[] = "Hello, this is a multiple thread web server.";
    request.send(content, strlen(content), 0);
    request.close();
}

int main() {
    TCPServer tcpServer(60000);
    tcpServer.bind();
    tcpServer.listen(5);
    Channel<Work> channel;
    ThreadPool threadPool(5, channel);

    for (int i = 0; i < 5; ++i) {
        Request request(tcpServer.accept());
        Work work;
        work._request = &request;
        work._function = handle_request;
        channel << work;
    }

    threadPool.close();
    return 0;
}