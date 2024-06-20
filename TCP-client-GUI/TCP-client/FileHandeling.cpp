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

bool FileHandeling::sendFile(int clientSocket, const QString& filePath) {

    const std::string HANDSHAKE_MESSAGE = "READY";

    char handshakeResponse[HANDSHAKE_MESSAGE.size() + 1];
    recv(clientSocket, handshakeResponse, HANDSHAKE_MESSAGE.size(), 0);
    handshakeResponse[HANDSHAKE_MESSAGE.size()] = '\0';
    if (HANDSHAKE_MESSAGE != handshakeResponse) {
        std::cerr << "Handshake failed." << std::endl;
        return false;
    }

    send(clientSocket, HANDSHAKE_MESSAGE.c_str(), HANDSHAKE_MESSAGE.size(), 0);
    // FileEncryption::getInstance()->encryptFile(filePath,filePath, "sdjlfjsdkl");

    // Open the file
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Error opening file";
        return false;
    }

    qDebug() << "Trying to send file <" << filePath << ">... ";

    // Send file size
    qint64 fileSize = file.size();
    send(clientSocket, reinterpret_cast<const char*>(&fileSize), sizeof(fileSize), 0);
    qDebug() << "Sent the file size: " << fileSize;

    // Send file data
    char sendBuffer[1024];
    qint64 bytesRead;
    while (!file.atEnd()) {
        bytesRead = file.read(sendBuffer, sizeof(sendBuffer));
        send(clientSocket, sendBuffer, bytesRead, 0);
    }

    file.close();
    qDebug() << "File sent successfully";
    return true;
}

bool FileHandeling::receiveFile(int clientSocket, const QString& filePath) {
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
    qDebug() << "Trying to receive a file...";

    // Receive file data
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Error opening file";
        return false;
    }

    qDebug() << "Opened the file";
    char receiveBuffer[1024];
    int bytesRead;
    qDebug() << "Reading the sent file.";
    while (fileSize > 0) {
        // Receive file data
        bytesRead = recv(clientSocket, receiveBuffer, sizeof(receiveBuffer), 0);
        // Write data to the file
        file.write(receiveBuffer, bytesRead);
        fileSize -= bytesRead;
        qDebug() << "Filesize is: " << fileSize;
    }

    file.close();
    qDebug() << "The file has been received successfully";
//    FileEncryption::getInstance("keyfile")->decryptFile(filePath,filePath + "_unencrypted.txt");
    return true;
}


