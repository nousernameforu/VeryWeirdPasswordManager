#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error connecting to the server." << std::endl;
        return 1;
    }

    std::cout << "Connected to the server." << std::endl;

    while (true) {
        std::string message;
        std::cout << "Enter message (type 'Exit' to quit): ";
        std::getline(std::cin, message);

        send(clientSocket, message.c_str(), message.size(), 0);

        if (message == "Exit") {
            std::cout << "Exiting client." << std::endl;
            break;
        }

        char buffer[1024];
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            std::cerr << "Error reading from server." << std::endl;
            break;
        }

        buffer[bytesRead] = '\0'; // Null-terminate the received data
        std::cout << "Received from server: " << buffer << std::endl;
    }

    close(clientSocket);
    return 0;
}