#ifndef FILEHANDELING_H
#define FILEHANDELING_H

#include <iostream>
#include <fstream>
#include <arpa/inet.h>
#include <cstring>
#include <cerrno>
#include <thread>
#include <QFile>
#include <QTextStream>
#include <QDataStream>
#include <QDebug>
#include <QIODevice>

class FileHandeling;

class File_SingletonDestroyer {
private:
    FileHandeling* p_instance;

public:
    ~File_SingletonDestroyer();
    void initialize(FileHandeling* p);
};

class FileHandeling {
private:
    static FileHandeling* p_instance;
    static File_SingletonDestroyer destroyer;
public:
    static FileHandeling* getInstance();
    FileHandeling();
    ~FileHandeling();
    bool sendFile(int clientSocket, const QString& filePath);
    bool receiveFile(int clientSocket, const QString& filePath);
};

#endif
