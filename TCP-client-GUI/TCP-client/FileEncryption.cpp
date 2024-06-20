#include "FileEncryption.h"
#include "qaesencryption.h"

//Singleton section

// Initialize static members
FileEncryption* FileEncryption::p_instance = nullptr;
SingletonDestroyer FileEncryption::destroyer;

// Implementation of SingletonDestroyer destructor
SingletonDestroyer::~SingletonDestroyer() {
    delete p_instance;
    std::cout << "[File_signletonDestroyer] Destructor ran." << std::endl;
}

// Implementation of SingletonDestroyer initialize method
void SingletonDestroyer::initialize(FileEncryption* p) {
    p_instance = p;
}

// Implementation of FileHandeling getInstance method
FileEncryption* FileEncryption::getInstance() {
    if (!p_instance) {
        p_instance = new FileEncryption();
        destroyer.initialize(p_instance);
    }
    return p_instance;
}

bool FileEncryption::encryptFile(const QString &inputFilePath, const QString &outputFilePath, const QByteArray &key) {
    QFile inputFile(inputFilePath);
    if (!inputFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open input file for reading";
        return false;
    }

    // Open output file
    QFile outputFile(outputFilePath);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open output file for writing";
        inputFile.close();
        return false;
    }

    while (!inputFile.atEnd()) {
//        QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);
        QByteArray chunk = inputFile.read(1024); // Adjust chunk size as needed
        QByteArray iv;
        iv.resize(1024);

        // Generate random bytes for IV
        for (int i = 0; i < 1024; ++i) {
            quint32 randomValue = QRandomGenerator::global()->generate();
            iv[i] = static_cast<char>(randomValue & 0xFF); // Take the least significant byte
        }
        QByteArray hashIV = QCryptographicHash::hash(iv, QCryptographicHash::Md5);
        QByteArray hashKey = QCryptographicHash::hash(key, QCryptographicHash::Sha256);
//        QByteArray encryptedChunk = encryption.encode(chunk, hashKey, hashIV);

        outputFile.write(chunk);
    }

    inputFile.close();
    outputFile.close();
    return true;
}

// Decrypts a file with a given key
bool FileEncryption::decryptFile(const QString &inputFilePath, const QString &outputFilePath, const QByteArray &key) {
    QFile inputFile(inputFilePath);
    if (!inputFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open input file for reading";
        return false;
    }

    // Open output file
    QFile outputFile(outputFilePath);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open output file for writing";
        inputFile.close();
        return false;
    }

    while (!inputFile.atEnd()) {
//        QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);
        QByteArray chunk = inputFile.read(1024); // Adjust chunk size as needed
        QByteArray iv;
        iv.resize(1024);

        // Generate random bytes for IV
        for (int i = 0; i < 1024; ++i) {
            quint32 randomValue = QRandomGenerator::global()->generate();
            iv[i] = static_cast<char>(randomValue & 0xFF); // Take the least significant byte
        }
        QByteArray hashIV = QCryptographicHash::hash(iv, QCryptographicHash::Md5);
        QByteArray hashKey = QCryptographicHash::hash(key, QCryptographicHash::Sha256);
//        QByteArray encryptedChunk = encryption.decode(chunk, hashKey, hashIV);

        outputFile.write(chunk);
    }

    inputFile.close();
    outputFile.close();
    return true;
}

