//
// Created by Riton on 2019/9/28.
//

#include <iostream>
#include <fstream>
#include <cstring>
#include <regex>

#include "request.h"
#include "server.h"
#include "thread_pool.h"

#define BUFFER_LEN 4096
#define MAX_REQUEST_TIMES 40

using namespace std;

void get_html(const char filename[], char buffer[], int n) {
    ifstream fin(filename);

    if (fin.is_open()) {
        char head[] = "HTTP/1.1 200 OK \r\n"
                      "\r\n";
        for (int j = 0; j < strlen(head); ++j) {
            buffer[j] = head[j];
        }
        char ch;
        for (int i = strlen(head); (i < n) && (fin.get(ch)); ++i) {
            buffer[i] = ch;
        }
    } else {
        char head[] = "HTTP/1.1 404 Not Found \r\n"
                      "\r\n";
        for (int j = 0; j < strlen(head); ++j) {
            buffer[j] = head[j];
        }
        ifstream fin_not_found("../404.html");
        char ch;
        for (int i = strlen(head); (i < n) && (fin_not_found.get(ch)); ++i) {
            buffer[i] = ch;
        }
        fin_not_found.close();
    }
    fin.close();
}

void handle_request(Request &request) {
    char buffer[BUFFER_LEN];
    bzero(buffer, BUFFER_LEN);
    request.read(buffer, BUFFER_LEN - 1);
    cout << buffer << endl;
    char content[4096]{'\0'};

    char filename[20] = "..";
    int start = 4, index = 2;
    for (char ch = buffer[start]; ch != ' '; ch = buffer[++start]) {
        filename[index++] = ch;
    }
    filename[index] = '\0';

    get_html(filename, content, 4095);
    request.send(content, strlen(content), 0);
    request.close();
}

int main() {
    TCPServer tcpServer(60000);
    tcpServer.bind();
    tcpServer.listen(5);
    Channel<Work> channel;
    ThreadPool threadPool(5, channel);


    for (int i = 0; i < MAX_REQUEST_TIMES; ++i) {
        Request request(tcpServer.accept());
        Work work;
        work._request = &request;
        work._function = handle_request;
        channel << work;
    }

    threadPool.close();
    return 0;
}