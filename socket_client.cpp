//
// Created by Riton on 2019/9/18.
//
#include <iostream>
#include <cstring>

#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

using namespace std;

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr{};
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
        cout << "usage " << argv[0] << "hostname port" << endl;
        return 1;
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        cout << "ERROR opening socket" << endl;
        return 1;
    }
    server = gethostbyname(argv[1]);
    if (server == nullptr) {
        cout << "ERROR, no such host" << endl;
        return 1;
    }
//    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        cout << "ERROR connecting" << endl;
        return 1;
    }
    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) {
        cout << "ERROR writing to socket" << endl;
        return 1;
    }
    bzero(buffer,256);
    n = read(sockfd, buffer, 255);
    if (n < 0) {
        cout << "ERROR reading from socket" << endl;
        return 1;
    }
    cout << buffer << endl;
    close(sockfd);
    return 0;
}