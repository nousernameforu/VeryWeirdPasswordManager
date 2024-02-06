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

        // Null-terminate the received data
        buffer[bytesRead] = '\0';

        // Process the received message and perform actions based on it
        if (strcmp(buffer, "HELLO") == 0) {
            send(clientSocket, "Hello, client!\n", 15, 0);
        } else if (strcmp(buffer, "TIME") == 0) {
            // Implement time-related functionality
            // For example, send the current time to the client
            // Replace the following line with your time-related logic
            send(clientSocket, "Current time: 12:00:00\n", 23, 0);
        } else {
            send(clientSocket, "Unknown command\n", 16, 0);
        }

        close(clientSocket);
    }

    close(serverSocket);
    return 0;
}
