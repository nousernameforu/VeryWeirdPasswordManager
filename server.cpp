// server.cpp
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080);

    bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    listen(serverSocket, 5);

    std::cout << "Server listening on port 8080..." << std::endl;

    while (true) {
        int clientSocket = accept(serverSocket, NULL, NULL);
        send(clientSocket, "Hello, client!\n", 16, 0);
        close(clientSocket);
    }

    close(serverSocket);
    return 0;
}

