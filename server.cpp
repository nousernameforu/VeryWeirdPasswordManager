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

        // Read data from the client
        char buffer[1024];
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            std::cerr << "Error reading from client." << std::endl;
            close(clientSocket);
            continue;
        }

        // Print the received data
        buffer[bytesRead] = '\0'; // Null-terminate the received data
        std::cout << "Received from client: " << buffer << std::endl;

        send(clientSocket, "Goodbye, client!\n", 16, 0); //send goodbye

        close(clientSocket);

    }

    close(serverSocket);
    return 0;
}

