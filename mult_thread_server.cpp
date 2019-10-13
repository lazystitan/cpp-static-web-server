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

/*
 * 根据文件名获取html，加入buffer中，n是缓冲区大小
 * 假如没有找到对应的html，将404.html的内容加入buffer
 */
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

/*
 * 处理请求
 */
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
    //创建TCP socket
    TCPServer tcpServer(60000);
    tcpServer.bind();
    tcpServer.listen(5);

    //创建channel
    Channel<Work> channel;
    //创建线程池
    ThreadPool threadPool(5, channel);

    //接受请求
    for (int i = 0; i < MAX_REQUEST_TIMES; ++i) {
        Request request(tcpServer.accept());
        Work work;
        work._request = &request;
        work._function = handle_request;
        //将work通过channel传入线程池
        channel << work;
    }

    //关闭线程池
    threadPool.close();
    return 0;
}