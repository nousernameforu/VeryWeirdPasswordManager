// TCPserver.h

//Header file for the library

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <iostream>
#include <arpa/inet.h>
#include <vector>
#include <thread>
#include <unistd.h>

#include <unordered_map>

#include "UserAuthentication.h"
#include "FileHandeling.h"

using namespace std;

//Singleton attampt section

// class InstanceDestroyer
// {
//     private:
//         TCPserver * p_instance;
//     public:
//         ~InstanceDestroyer();
//         void initialize(TCPserver * p);
// };

//TCPserver part

class TCPserver {
    private:
        int serverSocket; //initial server socket
        vector<thread> clientThreads; //Multiple client support is enabled by this vector
        std::unordered_map<int, bool> clientAuthStatus;  // Map to store client authentication status
        void dumpBufferToLog(const char* buffer, ssize_t size, int clientSocket); //Will dump commands to a log file
        //UserAuthentication* Auth = UserAuthentication::getInstance();

        FileHandeling fileReception;
        const string baseDirectory = "/client-files/";

        std::unordered_map<int, std::string> clientUsernames;  // Map to store authenticated usernames

    
    public:
        TCPserver(int port);
        ~TCPserver();
        bool authCommand(int clientSocket, char* buffer);
        bool regCommand(int clientSocket, char* buffer);
        bool helloCommand(int clientSocket);
        bool quitCommand(int clientSocket);
        bool sendFileCommand(int clientSocket, char* buffer);
        bool receiveFileCommand(int clientSocket, char* buffer);

        void handleClient(int clientSocket);
        void startAcceptingConnections();


};

#endif