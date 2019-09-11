//
// Created by Riton on 2019/9/11.
//

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>

#define SERV_PORT 9980
#define MAXLINE 4096

using namespace std;

int main() {
    int socket_fd, connect_fd;
    struct sockaddr_in servaddr{};
    char buf[MAXLINE], sendbuf[MAXLINE];
    int len;

    if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        cout << "error 0" <<endl;
        return 1;
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htonl(SERV_PORT);

    if (bind(socket_fd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
        cout << "error 1" <<endl;
        return 1;
    }

    if (listen(socket_fd, 10) < 0) {
        cout << "error 2" <<endl;
        return 1;
    }

    cout << "waiting for client's connection......" <<endl;

    if ((connect_fd = accept(socket_fd, (struct sockaddr*) nullptr, nullptr)) < 0) {
        cout << "error 3" <<endl;
        return 1;
    }

    while ((len = recv(connect_fd, buf, MAXLINE, 0)) > 0) {
        buf[len] = '\0';
        cout << "receive from : " <<buf << endl;
        cout << "send" <<endl;
        fgets(sendbuf, MAXLINE, stdin);
        if (send(connect_fd, sendbuf, strlen(sendbuf), 0) < 0) {
            cout << "error 4" <<endl;
            return 1;
        }
    }

    close(connect_fd);
    close(socket_fd);

    return 0;
}