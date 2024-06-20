#ifndef FILEENCRYPTION_H
#define FILEENCRYPTION_H

#include <iostream>
#include <QFile>
#include <QDebug>
#include <QRandomGenerator>
#include <QCryptographicHash>
#include "qaesencryption.h"

class FileEncryption;

class SingletonDestroyer {
private:
    FileEncryption* p_instance;

public:
    ~SingletonDestroyer();
    void initialize(FileEncryption* p);
};

class FileEncryption {
private:
    static FileEncryption* p_instance;
    static SingletonDestroyer destroyer;

public:
    static FileEncryption* getInstance();
    bool encryptFile(const QString &inputFilePath, const QString &outputFilePath, const QByteArray &key);
    bool decryptFile(const QString &inputFilePath, const QString &outputFilePath, const QByteArray &key);
};


#endif // FILEENCRYPTION_H
