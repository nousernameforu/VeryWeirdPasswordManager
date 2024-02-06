#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <fstream>

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

class TCPServer {
private:
    int serverSocket;
    bool isAuthenticated = false; //This will denote if user is authed or not


    //this is just for testing!!!!!
    bool authenticate(const std::string& username, const std::string& password) {
    const std::string validUsername = "admin";
    const std::string validPassword = "password";

    return (username == validUsername && password == validPassword);
    };

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

    void run() {
        int clientSocket = accept(serverSocket, NULL, NULL);
        while (true) {

            // Read data from the client
            char buffer[1024];
            ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (bytesRead <= 0) {
                std::cerr << "Error reading from client." << std::endl;
                break; //temporary fix for endless looping 
            }

            // Null-terminate the received data
            buffer[bytesRead] = '\0';

            // Fix for telnet control symbols
            for (ssize_t i = 0; i < bytesRead; ++i) {
                if (buffer[i] == '\r' || buffer[i] == '\n') {
                    buffer[i] = '\0';
                }
            }

            std::cout << "Received from client: " << buffer << std::endl;
            dumpBufferToLog(buffer, bytesRead);

            // Process the received message and perform actions based on it
            if (!isAuthenticated) {
                if (strncmp(buffer, "AUTH", 4) == 0) {
                    // Assuming the message format is "AUTH username password"
                    char* token = strtok(buffer + 5, " ");
                    std::string username = token ? token : "";
                    token = strtok(nullptr, " ");
                    std::string password = token ? token : "";

                    if (authenticate(username, password)) {
                        send(clientSocket, "Authentication successful\n", 27, 0);
                        isAuthenticated = true; //Set authentication flag to true 
                    } else {
                        send(clientSocket, "Authentication failed\n", 23, 0);
                    }
                } else {
                    send(clientSocket, "Authentication needed to proceed\n", 34, 0 );
                }
            } else {
                if (strcmp(buffer, "HELLO") == 0) {
                    send(clientSocket, "Hello, client!\n", 16, 0); //Hello 
                } else if (strcmp(buffer, "QUIT") == 0) {
                    send(clientSocket, "Quitting the program\n", 22, 0); //Quit function
                    close(clientSocket); 
                    break;
                } else {
                    send(clientSocket, "Unknown command\n", 17, 0);
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
