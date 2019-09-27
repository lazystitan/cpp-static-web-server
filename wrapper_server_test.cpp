//
// Created by Riton on 2019/9/27.
//

#include <iostream>
#include <cstring>

#include "server.h"

using namespace std;

int main() {
    char buffer[4096];

    int port = 60000;
    Server server(port);
    server.bind();
    server.listen(5);
    auto p = server.accept();
    struct sockaddr_in client = p.first;
    int client_fd = p.second;
    cout << "got connection" << endl;
    char content[] = "Hello,world!\n";
    Server::send(client_fd, content, 13, 0);
    bzero(buffer, 256);
    int read_size = Server::read(client_fd, buffer, 255);
    if (read_size < 0) {
        cout << "error" << endl;
    }
    cout << buffer << endl;
    server.close();
    close(client_fd);

    return 0;
}