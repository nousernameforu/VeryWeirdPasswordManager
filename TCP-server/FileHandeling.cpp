#include "FileHandeling.h"

//function to check if the filepath is not escaping the allowed dir
bool FileHandeling::isValidFilePath(const string& filePath) {
    filesystem::path fullPath(filePath);
    filesystem::path allowedDirectory(baseDirectory);

    return filesystem::equivalent(fullPath, allowedDirectory / fullPath.filename());
}

bool FileHandeling::sendFile(int clientSocket, const string& fileName) {
    string filePath = baseDirectory + fileName;

    if (!isValidFilePath(filePath)) {
        cerr << "Invalid file path: " << filePath << endl;
        return false;
    }

    ifstream file(filePath, ios::binary);
    if (!file.is_open()) {
        cerr << "Error opening file: " << strerror(errno) << endl;
        return false;
    }

    // Get the file size
    file.seekg(0, ios::end);
    streampos fileSize = file.tellg();
    file.seekg(0, ios::beg);

    // Prepare buffer for file content
    char buffer[1024];
    ssize_t bytesRead;

    // Send file size to the client
    send(clientSocket, &fileSize, sizeof(fileSize), 0);

    // Send file content in chunks
    while (!file.eof()) {
        file.read(buffer, sizeof(buffer));
        bytesRead = file.gcount();

        if (bytesRead > 0) {
            send(clientSocket, buffer, bytesRead, 0);
        }
    }

    file.close();
    return true;
}

bool FileHandeling::receiveFile(int clientSocket, const string& fileName) {
    string filePath = baseDirectory + fileName;

    if (!isValidFilePath(filePath)) {
        cerr << "Invalid file path: " << filePath << endl;
        return false;
    }


    ofstream file(filePath, ios::binary);
    if (!file.is_open()) {
        cerr << "Error opening file: " << strerror(errno) << endl;
        return false;
    }

    // Receive file size from the client
    streampos fileSize;
    recv(clientSocket, &fileSize, sizeof(fileSize), 0);

    // Receive file content in chunks
    char buffer[1024];
    ssize_t bytesReceived = 0;
    ssize_t totalBytesReceived = 0;

    while (totalBytesReceived < fileSize) {
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            cerr << "Error receiving file content." << endl;
            file.close();
            return false;
        }

        totalBytesReceived += bytesReceived;
        file.write(buffer, bytesReceived);
    }

    file.close();
    return true;
}