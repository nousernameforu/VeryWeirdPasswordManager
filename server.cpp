#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

class TCPServer {
private:
    int serverSocket;

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
                close(clientSocket);
                continue;
            }

            // Null-terminate the received data
            buffer[bytesRead] = '\0';
            std::cout << "Received from client: " << buffer << std::endl;

            

            // Process the received message and perform actions based on it
            if (strncmp(buffer, "AUTH", 4) == 0) {
                // Assuming the message format is "AUTH username password"
                char* token = strtok(buffer + 5, " ");
                std::string username = token ? token : "";
                token = strtok(nullptr, " ");
                std::string password = token ? token : "";

                if (authenticate(username, password)) {
                    send(clientSocket, "Authentication successful\n", 27, 0);
                } else {
                    send(clientSocket, "Authentication failed\n", 23, 0);
                }
            } else if (strcmp(buffer, "HELLO") == 0) {
                send(clientSocket, "Hello, client!\n", 15, 0); //Hello 
            } else if (strcmp(buffer, "QUIT") == 0) {
                send(clientSocket, "Quitting the program\n", 20, 0); //Quit function
                close(clientSocket); 
                break;
            } else {
                send(clientSocket, "Unknown command\n", 16, 0);
            }
        }
    }
};


int main() {
    TCPServer server(8080);
    server.run();

    return 0;
}
