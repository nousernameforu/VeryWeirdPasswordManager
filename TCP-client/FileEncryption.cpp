#include "FileEncryption.h"

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
FileEncryption* FileEncryption::getInstance(const std::string& keyFileName) {
    if (!p_instance) {
        p_instance = new FileEncryption(keyFileName);
        destroyer.initialize(p_instance);
    }
    return p_instance;
}

void FileEncryption::loadKey(const std::string& keyFileName) {
    std::ifstream keyFile(keyFileName, std::ios::binary);
    if (!keyFile.is_open()) {
        std::cerr << "Error opening key file." << std::endl;
        return;
    }

    keyFile.seekg(0, std::ios::end);
    std::streamsize keySize = keyFile.tellg();
    keyFile.seekg(0, std::ios::beg);

    key.resize(keySize);
    keyFile.read(reinterpret_cast<char*>(key.data()), keySize);

    keyFile.close();
};

void FileEncryption::generateKeyFile(const std::string& keyFileName) {
    std::ofstream keyFile(keyFileName, std::ios::binary);
    if (!keyFile.is_open()) {
        std::cerr << "Error creating key file." << std::endl;
        return;
    }

    // Generate a random key
    unsigned char randomKey[AES_BLOCK_SIZE];
    RAND_bytes(randomKey, AES_BLOCK_SIZE);

    keyFile.write(reinterpret_cast<char*>(randomKey), AES_BLOCK_SIZE);

    keyFile.close();
}

bool fileExists(const std::string& fileName) {
    std::ifstream file(fileName);
    return file.good();
}

FileEncryption::FileEncryption(const std::string& keyFileName) {
    loadKey(keyFileName);
};

void FileEncryption::encryptFile(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream inStream(inputFile, std::ios::binary);
    std::ofstream outStream(outputFile, std::ios::binary);

    if (!inStream.is_open()) {
        std::cerr << "Error opening input file " << inputFile << std::endl;
        return;
    }

    if (!outStream.is_open()) {
        std::cerr << "Error opening output file." << outputFile << std::endl;
        return;
    }

    // Generate IV (Initialization Vector)
    unsigned char iv[AES_BLOCK_SIZE];
    RAND_bytes(iv, AES_BLOCK_SIZE);

    outStream.write(reinterpret_cast<char*>(iv), AES_BLOCK_SIZE);

    AES_KEY encKey;
    AES_set_encrypt_key(key.data(), 128, &encKey);

    unsigned char inBuffer[AES_BLOCK_SIZE];
    unsigned char outBuffer[AES_BLOCK_SIZE];

    while (inStream.read(reinterpret_cast<char*>(inBuffer), AES_BLOCK_SIZE)) {
        AES_encrypt(inBuffer, outBuffer, &encKey);
        outStream.write(reinterpret_cast<char*>(outBuffer), AES_BLOCK_SIZE);
    }

    // Handle PKCS#7 padding for the last block
    size_t bytesRead = inStream.gcount();
    if (bytesRead > 0 && bytesRead < AES_BLOCK_SIZE) {
        // Calculate padding value
        size_t paddingSize = AES_BLOCK_SIZE - bytesRead;
        for (size_t i = bytesRead; i < AES_BLOCK_SIZE; ++i) {
            inBuffer[i] = static_cast<unsigned char>(paddingSize);
        }

        // Encrypt the padded block
        AES_encrypt(inBuffer, outBuffer, &encKey);
        outStream.write(reinterpret_cast<char*>(outBuffer), AES_BLOCK_SIZE);
    }

    inStream.close();
    outStream.close();
};

void FileEncryption::decryptFile(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream inStream(inputFile, std::ios::binary);
    std::ofstream outStream(outputFile, std::ios::binary);

    if (!inStream.is_open() || !outStream.is_open()) {
        std::cerr << "Error opening files." << std::endl;
        return;
    }

    // Read IV from the beginning of the file
    unsigned char iv[AES_BLOCK_SIZE];
    inStream.read(reinterpret_cast<char*>(iv), AES_BLOCK_SIZE);

    AES_KEY decKey;
    AES_set_decrypt_key(key.data(), 128, &decKey);

    unsigned char inBuffer[AES_BLOCK_SIZE];
    unsigned char outBuffer[AES_BLOCK_SIZE];

    size_t bytesRead = 0;

    while (inStream.read(reinterpret_cast<char*>(inBuffer), AES_BLOCK_SIZE)) {
        AES_decrypt(inBuffer, outBuffer, &decKey);

        // Handle PKCS#7 padding removal
        bytesRead = inStream.gcount();
        if (bytesRead < AES_BLOCK_SIZE) {
            size_t paddingSize = static_cast<size_t>(outBuffer[AES_BLOCK_SIZE - 1]);
            bytesRead -= paddingSize;
        }

        outStream.write(reinterpret_cast<char*>(outBuffer), bytesRead);
    }


    inStream.close();
    outStream.close();
}
