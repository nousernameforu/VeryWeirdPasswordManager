#include "TCPserver.h"

void TCPserver::dumpBufferToLog(const char* buffer, ssize_t size, int clientSocket) {
    std::ofstream logFile("commands_log.txt", std::ios::app); // Open log file in append mode

    if (logFile.is_open()) {
        logFile.write(buffer, size);  //need to add clientsocket to the output to know which client wrote what.

        logFile << std::endl;
        logFile.close();
        std::cout << "Buffer dumped to commands_log.txt" << std::endl;
    } else {
        std::cerr << "Error opening log file." << std::endl;
    }
}

TCPserver::TCPserver(int port) {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in serverAddress;
        memset(&serverAddress, 0, sizeof(serverAddress));
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = INADDR_ANY;
        serverAddress.sin_port = htons(port);

        bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
        listen(serverSocket, 5);

        std::cout << "Server listening on port " << port << "..." << std::endl;
};

TCPserver::~TCPserver(){
    close(serverSocket);
    for (auto& thread : clientThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
};

bool TCPserver::authCommand(int clientSocket, char* buffer) {
    char* token = strtok(buffer + 5, " ");
    std::string username = token ? token : "";
    token = strtok(nullptr, " ");
    std::string password = token ? token : "";
    UserAuthentication Auth;
    if (Auth.authenticateUser(username, password)) {
        send(clientSocket, "Authentication successful\n", 27, 0);
        clientAuthStatus[clientSocket] = true;  // Set authentication status for the client
    } else {
        send(clientSocket, "Authentication failed\n", 23, 0);
    }
    return true;
};

bool TCPserver::regCommand(int clientSocket, char* buffer) {
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
    return true;
}

bool TCPserver::sendFileCommand(int clientSocket, char* buffer) {
    char* token = strtok(buffer + 10, " ");
    std::string filePath = token ? token : "";
    FileHandeling fileTransfer;
    if (!fileTransfer.sendFile(clientSocket, filePath)) {   
        cerr << "Failed to send file!" << endl;
    }
    return true;
}

bool TCPserver::receiveFileCommand(int clientSocket, char* buffer) {
    char* token = strtok(buffer + 13, " ");
    std::string filePath = token ? token : "";
    FileHandeling fileTransfer;
    if (!fileTransfer.receiveFile(clientSocket, filePath)) {
        cerr << "Failed to receive file!" << endl;
    }
    return true;
}

bool TCPserver::helloCommand(int clientSocket) {
    send(clientSocket, "Hello, client!\n", 16, 0);
    return true;
}

bool TCPserver::quitCommand(int clientSocket) {
    send(clientSocket, "Quitting the program\n", 22, 0);
    close(clientSocket); 
    return true;
}

void TCPserver::handleClient(int clientSocket) {
    while(true){
        // Buffer to store received data
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));

        // Read data from the client
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead <= 0) {
            // Handle error or client disconnect
            std::cerr << "Error reading from client " << clientSocket << "."<< std::endl;
            close(clientSocket);
            return;
        }

        buffer[bytesRead] = '\0';

        for (ssize_t i = 0; i < bytesRead; ++i) {
            if (buffer[i] == '\r' || buffer[i] == '\n') {
                buffer[i] = '\0';
            }
        }

        dumpBufferToLog(buffer, bytesRead, clientSocket);

        // Process the received data (you can replace this with your logic)
        std::cout << "Received from client " << clientSocket << ": " << buffer << std::endl;
        // dumpBufferToLog(buffer, bytesRead);

        if (clientAuthStatus.find(clientSocket) == clientAuthStatus.end() || !clientAuthStatus[clientSocket]) {
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
            } else if (strncmp(buffer, "SEND_FILE", 9) == 0) {
                if (!sendFileCommand(clientSocket,buffer)){
                    cerr << "error" << endl;
                };
            } else if (strncmp(buffer, "RECEIVE_FILE", 12) == 0) {
                if (!receiveFileCommand(clientSocket,buffer)){
                    cerr << "error" << endl;
                };
            } else {
                // unknownCommand(clientSocket);
            }
        }
    }
}

void TCPserver::startAcceptingConnections() {
    while (true) {
        sockaddr_in clientAddress;
        socklen_t clientAddrLen = sizeof(clientAddress);

        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddrLen);

        if (clientSocket != -1) {
            std::thread clientThread(&TCPserver::handleClient, this, clientSocket);
            clientThreads.push_back(std::move(clientThread));
        }
    }
}