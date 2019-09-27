//
// Created by Riton on 2019/9/18.
//

#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include <iostream>
#include <netinet/in.h>
#include <cstdlib>
#include <unistd.h>

class TCPServer {
private:
    sockaddr_in _server_address;
    int _socket_fd;
public:
    explicit TCPServer(int port) : _server_address{} {
        //create a socket, using fd represent it
        _socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (_socket_fd < 0)
            exit(1);

        _server_address.sin_family = AF_INET;
        _server_address.sin_addr.s_addr = INADDR_ANY;
        _server_address.sin_port = htons(port);
    }
//    ~TCPServer();
    void bind() {
        if (::bind(_socket_fd, (sockaddr *) &_server_address, sizeof(_server_address)) < 0) {
            exit(1);
        }
    }
    void listen(int n) {
        ::listen(_socket_fd, n);
    }

    std::pair<sockaddr_in, int> accept() {
        sockaddr_in client{};
        socklen_t client_len = sizeof(client);
        int client_socket_fd = ::accept(_socket_fd, (struct sockaddr*)&client, &client_len);
        if (client_socket_fd < 0) {
            exit(1);
        }
        return std::make_pair(client, client_socket_fd);
    }

    static int send(int client_socket_fd, char content[], int len, int flag) {
        return ::send(client_socket_fd, content, len, flag);
    }

    static int read(int client_socket_fd, char buffer[], int len) {
        return ::read(client_socket_fd, buffer, len);
    }

    int close() {
        return ::close(_socket_fd);
    }
};

#endif //SERVER_SERVER_H
