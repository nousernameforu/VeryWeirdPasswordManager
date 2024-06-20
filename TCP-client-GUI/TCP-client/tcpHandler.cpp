#include "tcpHandler.h"

//Singleton section

// Initialize static members
tcpHandler* tcpHandler::p_instance = nullptr;
tcpHandler_SingletonDestroyer tcpHandler::destroyer;

// Implementation of SingletonDestroyer destructor
tcpHandler_SingletonDestroyer::~tcpHandler_SingletonDestroyer() {
    delete p_instance;
    std::cout << "[FileEncryption_signletonDestroyer] Destructor ran." << std::endl;
}

// Implementation of SingletonDestroyer initialize method
void tcpHandler_SingletonDestroyer::initialize(tcpHandler* p) {
    p_instance = p;
}

// Implementation of FileHandeling getInstance method
tcpHandler* tcpHandler::getInstance() {
    if (!p_instance) {
        p_instance = new tcpHandler();
        destroyer.initialize(p_instance);
    }
    return p_instance;
}

int tcpHandler::connectToServer(){

    int tcpSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

    if (connect(tcpSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error connecting to the server." << std::endl;
        return 1;
    }
    return tcpSocket;
}

int tcpHandler::disconnectTCPsocket(int tcpSocket){
    close(tcpSocket);
    return 0;
}

int tcpHandler::sendCommand(int tcpSocket, std::string message){
    send(tcpSocket, message.c_str(), message.size(), 0);
    return 0;
}

std::string tcpHandler::recevieCommand(int tcpSocket) { //needs to be right after any send
    char MSGbuffer[1024];
    ssize_t bytesRead = recv(tcpSocket, MSGbuffer, sizeof(MSGbuffer), 0);
    if (bytesRead <= 0) {
        std::cerr << "Error reading from server." << std::endl;
        return "Error";
    }
    std::cout << "Received from server: " << MSGbuffer << std::endl;
    return MSGbuffer;
}

