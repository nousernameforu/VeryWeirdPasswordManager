#include "FileHandeling.h"

//Singleton section

// Initialize static members
FileHandeling* FileHandeling::p_instance = nullptr;
File_SingletonDestroyer FileHandeling::destroyer;

// Implementation of SingletonDestroyer destructor
File_SingletonDestroyer::~File_SingletonDestroyer() {
    delete p_instance;
    std::cout << "[FileHandeling_signletonDestroyer] Destructor ran." << std::endl;
}


// Implementation of SingletonDestroyer initialize method
void File_SingletonDestroyer::initialize(FileHandeling* p) {
    p_instance = p;
}

// Implementation of FileHandeling getInstance method
FileHandeling* FileHandeling::getInstance() {
    if (!p_instance) {
        p_instance = new FileHandeling();
        destroyer.initialize(p_instance);
    }
    return p_instance;
}

FileHandeling::FileHandeling() {
    std::cout << "[FileHandle] Constructor ran. " << std::endl;
}

FileHandeling::~FileHandeling() {
    std::cout << "[FileHandle] Destructor ran. " << std::endl;
}

bool FileHandeling::sendFile(int clientSocket, const std::string& filePath) {
    
    const std::string HANDSHAKE_MESSAGE = "READY";

    char handshakeResponse[HANDSHAKE_MESSAGE.size() + 1];
    recv(clientSocket, handshakeResponse, HANDSHAKE_MESSAGE.size(), 0);
    handshakeResponse[HANDSHAKE_MESSAGE.size()] = '\0';
    if (HANDSHAKE_MESSAGE != handshakeResponse) {
        std::cerr << "Handshake failed." << std::endl;
        return false;
    }

    send(clientSocket, HANDSHAKE_MESSAGE.c_str(), HANDSHAKE_MESSAGE.size(), 0);

    //FileEncryption::getInstance("keyfile")->encryptFile(filePath + "_unencrypted.txt",filePath);

    std::ifstream fileToSend(filePath, std::ios::binary);
    if (!fileToSend.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return false;
    }

    std::cout << "Trying to send file <" << filePath << ">... " << std::endl;

    // Send file size
    fileToSend.seekg(0, std::ios::end);
    int fileSize = fileToSend.tellg();
    fileToSend.seekg(0, std::ios::beg);
    send(clientSocket, &fileSize, sizeof(fileSize), 0);
    std::cout << "Sent the file size: " << fileSize << std::endl;

    // Send file data
    char sendBuffer[1024];
    while (!fileToSend.eof()) {
        fileToSend.read(sendBuffer, sizeof(sendBuffer));
        send(clientSocket, sendBuffer, fileToSend.gcount(), 0);
    }

    fileToSend.close();
    std::cout << "File sent successfully" << std::endl;
    return true;
}

bool FileHandeling::receiveFile(int clientSocket, const std::string& filePath) {

    const std::string HANDSHAKE_MESSAGE = "READY";

    char handshakeMessage[HANDSHAKE_MESSAGE.size() + 1];
    recv(clientSocket, handshakeMessage, HANDSHAKE_MESSAGE.size(), 0);
    handshakeMessage[HANDSHAKE_MESSAGE.size()] = '\0';
    if (HANDSHAKE_MESSAGE != handshakeMessage) {
        std::cerr << "Handshake failed." << std::endl;
        return false;
    }
    
    send(clientSocket, HANDSHAKE_MESSAGE.c_str(), HANDSHAKE_MESSAGE.size(), 0);

    int fileSize = 0;
    recv(clientSocket, &fileSize, sizeof(fileSize), 0);
    std::cout << "Trying to receive a file..." << std::endl;

    // Receive file data
    std::ofstream fileToReceive(filePath, std::ios::binary);
    std::cout << "Opened the file" << std::endl;
    char receiveBuffer[1024];
    int bytesRead;
    std::cout << "Reading the sent file." << std::endl;
    while (fileSize > 0) {
        // Receive file data
        bytesRead = recv(clientSocket, receiveBuffer, sizeof(receiveBuffer), 0);
        std::cout << "Receivinig buffer: " << receiveBuffer << std::endl;
        // Write data to the file
        fileToReceive.write(receiveBuffer, bytesRead);
        fileSize -= bytesRead;
        std::cout << "Filesize is: " << fileSize << std::endl;

    }
    fileToReceive.close();
    std::cout << "The file has been received successfully" << std::endl;
    //FileEncryption::getInstance("keyfile")->decryptFile(filePath,filePath + "_unencrypted.txt");
    return true;
}

