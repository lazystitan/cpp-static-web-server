//
// Created by Riton on 2019/9/27.
//

#ifndef SERVER_CLIENT_H
#define SERVER_CLIENT_H

#include <netinet/in.h>
#include <cstdlib>
#include <netdb.h>
#include <cstring>
#include <unistd.h>

class Client {
private:
    int _client_socket_fd;
    struct sockaddr_in _server_address;
    struct hostent *_server;

public:
    Client(char ip[], int port) : _server_address{} {
        _client_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (_client_socket_fd < 0) {
            exit(1);
        }
        _server = gethostbyname(ip);
        if (_server == nullptr)
            exit(1);
        _server_address.sin_family = AF_INET;
        bcopy((char *)_server->h_addr,(char *)&_server_address.sin_addr.s_addr,_server->h_length);
        _server_address.sin_port = htons(port);
    }

    void connect() {
        if (::connect(_client_socket_fd, (struct sockaddr *)&_server_address, sizeof(_server_address)) < 0) {
            exit(1);
        }
    }

    int write(char buffer[], int len) {
        return ::write(_client_socket_fd, buffer, len);
    }

    int read(char buffer[], int len) {
        return ::read(_client_socket_fd, buffer, len);
    }

    int close() {
        return ::close(_client_socket_fd);
    }

};

#endif //SERVER_CLIENT_H
