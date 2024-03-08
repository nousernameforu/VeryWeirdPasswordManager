#include "FileHandeling.h"

//Singleton attampt section

// InstanceDestroyer::~InstanceDestroyer(){delete p_instance;}
// void InstanceDestroyer::initialize(FileHandeling * p) {p_instance = p;}

//Singleton attampt section end


//function to check if the filepath is not escaping the allowed dir
bool FileHandeling::isValidFilePath(const string& filePath, const string& baseDirectory) {
        filesystem::path fullPath(filePath);

        // Check if the file exists before creating the base directory path
        if (!filesystem::exists(fullPath)) {
            cerr << "File does not exist: " << filePath << endl;
            return false;
        }

        filesystem::path allowedDirectory(baseDirectory);

        return filesystem::equivalent(fullPath, allowedDirectory / fullPath.filename());
    }

FileHandeling::FileHandeling() {
    std::cout << "[FileHandle] Constructor ran. " << endl;
    //here should be something
}

FileHandeling::~FileHandeling() {
    std::cout << "[FileHandle] Destructor ran. " << endl;
}

bool FileHandeling::sendFile(int clientSocket, const string& fileName, const string& baseDirectory) {    
    string filePath = baseDirectory + fileName;

    if (!isValidFilePath(filePath, baseDirectory)) {
        cerr << "Invalid file path: " << filePath << endl;
    };

    ifstream fileToSend(filePath, ios::binary);
    if (!fileToSend.is_open()) {
        perror("Error opening file");
    }

    std::cout << "Trying to send file <" << filePath << "> " << endl;

    // Send file size
    fileToSend.seekg(0, ios::end);
    int fileSize = fileToSend.tellg();
    fileToSend.seekg(0, ios::beg);
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

bool FileHandeling::receiveFile(int clientSocket, const string& fileName, const string& baseDirectory) {
    string filePath = baseDirectory + fileName;
    if (!isValidFilePath(filePath, baseDirectory)) {
        cerr << "Invalid file path: " << filePath << endl;
    };
    int fileSize;
    recv(clientSocket, &fileSize, sizeof(fileSize) - 1, 0);
    std::cout << "Trying to receive a file from client" << endl;
    // Receive file data
    ofstream fileToReceive(filePath, ios::binary);
    char buffer[1024];
    int bytesRead;

    while (fileSize > 0) {
        bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        fileToReceive.write(buffer, bytesRead);
        fileSize -= bytesRead;
    }

    fileToReceive.close();
    std::cout << "The file has been received successfully" << endl;
    return true;
}