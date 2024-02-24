#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

//this is needed for command mappings in tcpserver run 
#include <unordered_map>
#include <functional>

//authentication library connection
#include "UserAuthentication.h"

//file handeling lib connection
#include "FileHandeling.h"

void dumpBufferToLog(const char* buffer, ssize_t size) {
    std::ofstream logFile("commands_log.txt", std::ios::app); // Open log file in append mode

    if (logFile.is_open()) {
        logFile.write(buffer, size);
        logFile << std::endl;
        logFile.close();
        std::cout << "Buffer dumped to commands_log.txt" << std::endl;
    } else {
        std::cerr << "Error opening log file." << std::endl;
    }
}

//just the server

class TCPServer {
private:
    int serverSocket;
    bool isAuthenticated = false;
    FileHandeling fileTransfer;

public:
    TCPServer(int port) {
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in serverAddress;
        memset(&serverAddress, 0, sizeof(serverAddress));
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = INADDR_ANY;
        serverAddress.sin_port = htons(port);

        bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
        listen(serverSocket, 5);

        std::cout << "Server listening on port " << port << "..." << std::endl;
    }

    ~TCPServer() {
        close(serverSocket);
    }

    //All of the commands will be here:

    //authenticate the user

    void authCommand(int clientSocket, char* buffer) {
        char* token = strtok(buffer + 5, " ");
        std::string username = token ? token : "";
        token = strtok(nullptr, " ");
        std::string password = token ? token : "";
        UserAuthentication Auth;
        if (Auth.authenticateUser(username, password)) {
            send(clientSocket, "Authentication successful\n", 27, 0);
            isAuthenticated = true;
        } else {
            send(clientSocket, "Authentication failed\n", 23, 0);
        }
    }

    //register the user

    void regCommand(int clientSocket, char* buffer) {
        char* token = strtok(buffer + 4, " ");
        std::string username = token ? token : "";
        token = strtok(nullptr, " ");
        std::string password = token ? token : "";
        UserAuthentication Auth;
        if (Auth.registerUser(username, password)) {
            send(clientSocket, "User registration successful\n", 30, 0);
        } else {
            send(clientSocket, "User registration failed\n", 26, 0);
        }
    }

    //print hello world to user

    void helloCommand(int clientSocket) {
        send(clientSocket, "Hello, client!\n", 16, 0);
    }

    //quit command

    void quitCommand(int clientSocket) {
        send(clientSocket, "Quitting the program\n", 22, 0);
        close(clientSocket); 
    }

    //sending of files

    void sendFileCommand(int clientSocket, char* buffer) {
        char* token = strtok(buffer + 10, " ");
        std::string filePath = token ? token : "";
        if (fileTransfer.sendFile(clientSocket, filePath)) {
            send(clientSocket, "File sent successfully\n", 23, 0);
        } else {
            send(clientSocket, "Failed to send file\n", 20, 0);
        }
    }

    //receiving of files

    void receiveFileCommand(int clientSocket, char* buffer) {
        char* token = strtok(buffer + 13, " ");
        std::string filePath = token ? token : "";
        if (fileTransfer.receiveFile(clientSocket, filePath)) {
            send(clientSocket, "File received successfully\n", 28, 0);
        } else {
            send(clientSocket, "Failed to receive file\n", 24, 0);
        }
    }

    //for unknown commands

    void unknownCommand(int clientSocket) {
        send(clientSocket, "Unknown command\n", 17, 0);
    }


    void run() {
        int clientSocket = accept(serverSocket, NULL, NULL);
        while (true) {
            char buffer[1024];
            ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (bytesRead <= 0) {
                std::cerr << "Error reading from client." << std::endl;
                break;
            }

            buffer[bytesRead] = '\0';

            for (ssize_t i = 0; i < bytesRead; ++i) {
                if (buffer[i] == '\r' || buffer[i] == '\n') {
                    buffer[i] = '\0';
                }
            }

            std::cout << "Received from client: " << buffer << std::endl;
            dumpBufferToLog(buffer, bytesRead);

            if (!isAuthenticated) {
                if (strncmp(buffer, "AUTH", 4) == 0) {
                    authCommand(clientSocket,buffer);
                } else if (strncmp(buffer, "REG", 3) == 0) {
                    regCommand(clientSocket,buffer);
                } else {
                    send(clientSocket, "Authentication needed to proceed\n", 34, 0);
                }
            } else {
                if (strcmp(buffer, "HELLO") == 0) {
                    helloCommand(clientSocket);
                } else if (strcmp(buffer, "QUIT") == 0) {
                    quitCommand(clientSocket);
                    break;
                } else if (strncmp(buffer, "SEND_FILE", 9) == 0) {
                    sendFileCommand(clientSocket,buffer);
                } else if (strncmp(buffer, "RECEIVE_FILE", 12) == 0) {
                    receiveFileCommand(clientSocket,buffer);
                } else {
                    unknownCommand(clientSocket);
                }
            }
        }
    }
};

int main() {
    TCPServer server(8080);
    server.run();

    return 0;
}
