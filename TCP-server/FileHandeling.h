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

//Singleton attampt section

// class InstanceDestroyer
// {
//     private:
//         FileHandeling * p_instance;
//     public:
//         ~InstanceDestroyer();
//         void initialize(FileHandeling * p);
// };

//FileHandeling part


class FileHandeling {
private:
    static FileHandeling * p_instance;
//this will be our directory for user files
    const string baseDirectory = "/client-files/";
    bool isValidFilePath(const string& filePath, const std::string& baseDirectory);

public:
    FileHandeling();
    ~FileHandeling();
    bool sendFile(int clientSocket, const string& fileName, const string& baseDirectory);
    bool receiveFile(int clientSocket, const string& fileName, const string& baseDirectory);
};

#endif
