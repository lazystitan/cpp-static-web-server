//
// Created by Riton on 2019/9/11.
//
/* The port number is passed as an argument */
#include <iostream>
#include <cstring>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, port_number;
    socklen_t client_length;
    char buffer[256];
    struct sockaddr_in server_address{}, client_address{};
    int read_size;

    if (argc < 2) {
        cout << "ERROR, no port provided" << endl;
        return 1;
    }
    // create a socket
    // socket(int domain, int type, int protocol)
    sockfd =  socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        cout << "ERROR opening socket" << endl;
        return 1;
    }

    port_number = atoi(argv[1]);

    /* setup the host_addr structure for use in bind call */
    // server byte order
    server_address.sin_family = AF_INET;

    // automatically be filled with current host's IP address
    server_address.sin_addr.s_addr = INADDR_ANY;

    // convert short integer value for port must be converted into network byte order
    server_address.sin_port = htons(port_number);

    // bind(int fd, struct sockaddr *local_addr, socklen_t addr_length)
    // bind() passes file descriptor, the address structure,
    // and the length of the address structure
    // This bind() call will bind  the socket to the current IP address on port, port_number
    if (bind(sockfd, (struct sockaddr *) &server_address,sizeof(server_address)) < 0) {
        cout << "ERROR on binding" << endl;
        return 1;
    }

    // This listen() call tells the socket to listen to the incoming connections.
    // The listen() function places all incoming connection into a backlog queue
    // until accept() call accepts the connection.
    // Here, we set the maximum size for the backlog queue to 5.
    listen(sockfd,5);

    // The accept() call actually accepts an incoming connection
    client_length = sizeof(client_address);

    // This accept() function will write the connecting client's address info
    // into the the address structure and the size of that structure is client_length.
    // The accept() returns a new socket file descriptor for the accepted connection.
    // So, the original socket file descriptor can continue to be used
    // for accepting new connections while the new socker file descriptor is used for
    // communicating with the connected client.
    newsockfd = accept(sockfd, (struct sockaddr *) &client_address, &client_length);
    if (newsockfd < 0) {
        cout << "ERROR on accept" << endl;
        return 1;
    }

    cout << "server: got connection from " << inet_ntoa(client_address.sin_addr) <<"port " <<ntohs(client_address.sin_port) << endl;


    // This send() function sends the 13 bytes of the string to the new socket
    send(newsockfd, "Hello, world!\n", 13, 0);

    bzero(buffer,256);

    read_size = read(newsockfd,buffer,255);
    if (read_size < 0) {
        cout << "ERROR reading from socket" << endl;
    }
    cout << "Here is the message: " << buffer << endl;

    close(newsockfd);
    close(sockfd);
    return 0;
}