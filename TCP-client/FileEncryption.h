//FileEncryption.h

#ifndef FILEENCRYPTION_H
#define FILEENCRYPTION_H

#include <iostream>
#include <fstream>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <vector>

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
    void loadKey(const std::string& keyFileName);
    
    std::vector<unsigned char> key;
    static FileEncryption* p_instance;
    static SingletonDestroyer destroyer;
public:

    void generateKeyFile(const std::string& keyFileName);

    static FileEncryption* getInstance(const std::string& keyFileName);
    FileEncryption(const std::string& keyFileName);
    void encryptFile(const std::string& inputFile, const std::string& outputFile);
    void decryptFile(const std::string& inputFile, const std::string& outputFile);
};

#endif