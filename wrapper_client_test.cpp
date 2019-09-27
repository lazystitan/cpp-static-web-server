//
// Created by Riton on 2019/9/27.
//

#include <iostream>

#include "client.h"

using namespace std;

int main() {
    char buffer[4096];
    char ip[] = "127.0.0.1";
    TCPClient client(ip, 60000);
    client.connect();
    bzero(buffer, 256);
    cout << "please enter the message:";
    cin >> buffer;
    int n = client.write(buffer, strlen(buffer));
    if (n < 0) {
        cout << "error" << endl;
    }
    bzero(buffer, 256);
    n = client.read(buffer, 255);
    if (n < 0) {
        cout << "error" << endl;
    }
    cout << buffer << endl;
    client.close();

    return 0;
}