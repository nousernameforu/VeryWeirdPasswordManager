#include "FileHandeling.h"

//function to check if the filepath is not escaping the allowed dir
bool FileHandeling::isValidFilePath(const std::string& filePath, const std::string& baseDirectory) {
        filesystem::path fullPath(filePath);

        // Check if the file exists before creating the base directory path
        if (!filesystem::exists(fullPath)) {
            std::cerr << "File does not exist: " << filePath << std::endl;
            return false;
        }

        filesystem::path allowedDirectory(baseDirectory);

        return filesystem::equivalent(fullPath, allowedDirectory / fullPath.filename());
    }


FileHandeling::FileHandeling(){

}

FileHandeling::~FileHandeling() {
    
}

bool FileHandeling::sendFile(int clientSocket, const string& fileName) {
    string filePath = baseDirectory + fileName;

    if (!isValidFilePath(filePath,baseDirectory)) {
        cerr << "Invalid file path: " << filePath << endl;
        return false;
    }

    std::ifstream fileToSend(filePath, std::ios::binary);
    if (!fileToSend.is_open()) {
        perror("Error opening file");
    }

    cout << "Trying to send file <" << fileName << "> " << endl;

    // Send file size
    fileToSend.seekg(0, std::ios::end);
    int fileSize = fileToSend.tellg();
    fileToSend.seekg(0, std::ios::beg);
    send(clientSocket, &fileSize, sizeof(fileSize), 0);

    // Send file data
    char buffer[1024];
    while (!fileToSend.eof()) {
        fileToSend.read(buffer, sizeof(buffer));
        send(clientSocket, buffer, fileToSend.gcount(), 0);
    }

    fileToSend.close();

    std::cout << "File sent successfully" << endl;
    return true;
}

bool FileHandeling::receiveFile(int clientSocket, const string& fileName) {
    string filePath = baseDirectory + fileName;

    if (!isValidFilePath(filePath,baseDirectory)) {
        cerr << "Invalid file path: " << filePath << endl;
        return false;
    }

    int fileSize;
    recv(clientSocket, &fileSize, sizeof(fileSize), 0);
    cout << "Trying to receive a file from client" << endl;
    // Receive file data
    std::ofstream file(filePath, std::ios::binary);
    char buffer[1024];
    int bytesRead;

    while (fileSize > 0) {
        bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        file.write(buffer, bytesRead);
        fileSize -= bytesRead;
    }

    file.close();
    cout << "The file has been received successfully" << endl;
    return true;
}