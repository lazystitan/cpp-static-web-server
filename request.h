//
// Created by Riton on 2019/9/27.
//

#ifndef SERVER_REQUEST_H
#define SERVER_REQUEST_H

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

class Request {
private:
    struct sockaddr_in _request;
    int _request_fd;
public:
    Request(struct sockaddr_in request, int request_fd) : _request{} {
        _request = request;
        _request_fd = request_fd;
    }

    explicit Request(std::pair<sockaddr_in, int> request) : _request{} {
        _request = request.first;
        _request_fd = request.second;
    }

    int read(char buffer[], int len) {
        return ::read(_request_fd, buffer, len);
    }

    int send(char buffer[], int len, int flag) {
        return ::send(_request_fd, buffer, len, flag);
    }

    int close() {
        return ::close(_request_fd);
    }
};

#endif //SERVER_REQUEST_H
