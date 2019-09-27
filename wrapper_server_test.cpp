//
// Created by Riton on 2019/9/27.
//

#include <iostream>
#include <cstring>

#include "server.h"
#include "request.h"

using namespace std;

int main() {
    char buffer[4096];
    int port = 60000;
    TCPServer server(port);
    server.bind();
    server.listen(5);
    cout << "start listen" << endl;
//    auto p = server.accept();
//    struct sockaddr_in client = p.first;
//    int client_fd = p.second;

    Request request(server.accept());

    cout << "got connection" << endl;
    char content[] = "Hello,world!\n";
//    TCPServer::send(client_fd, content, 13, 0);
    request.send(content, 255, 0);
    bzero(buffer, 256);
//    int read_size = TCPServer::read(client_fd, buffer, 255);
    int read_size = request.read(buffer, 255);
    if (read_size < 0) {
        cout << "error" << endl;
    }
    cout << buffer << endl;
    server.close();
    request.close();

    return 0;
}