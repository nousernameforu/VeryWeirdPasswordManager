// FileHandeling.h

//Header file for handeling files and file transfers

#ifndef FILEHANDELING_H
#define FILEHANDELING_H

#include <iostream>
#include <fstream>
#include <arpa/inet.h>
#include <cstring>
#include <cerrno>
#include <thread>
#include "FileEncryption.h"

//Singleton attampt section

class FileHandeling;

class File_SingletonDestroyer {
private:
    FileHandeling* p_instance;

public:
    ~File_SingletonDestroyer();
    void initialize(FileHandeling* p);
};
//FileHandeling section


class FileHandeling {
private:
    static FileHandeling* p_instance;
    static File_SingletonDestroyer destroyer;
public:
    static FileHandeling* getInstance();
    FileHandeling();
    ~FileHandeling();
    bool sendFile(int clientSocket, const std::string& fileName);
    bool receiveFile(int clientSocket, const std::string& fileName);
};

#endif
