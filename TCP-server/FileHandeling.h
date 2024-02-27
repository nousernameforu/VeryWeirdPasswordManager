// FileHandeling.h

//Header file for handeling files and file transfers

#ifndef FILEHANDELING_H
#define FILEHANDELING_H

#include <iostream>
#include <fstream>
#include <arpa/inet.h>
#include <cstring>
#include <cerrno>
#include <filesystem>

using namespace std;

class FileHandeling {
private:
//this will be our directory for user files
    const string baseDirectory = "/client-files/";
    bool isValidFilePath(const string& filePath, const std::string& baseDirectory);

public:
    FileHandeling();
    ~FileHandeling();
    bool sendFile(int clientSocket, const string& fileName);
    bool receiveFile(int clientSocket, const string& fileName);
};

#endif
