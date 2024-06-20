#ifndef TCPHANDLER_H
#define TCPHANDLER_H

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>


class tcpHandler;

class tcpHandler_SingletonDestroyer{
private:
    tcpHandler* p_instance;
public:
    ~tcpHandler_SingletonDestroyer();
    void initialize(tcpHandler* p);
};

class tcpHandler {
private:
    static tcpHandler* p_instance;
    static tcpHandler_SingletonDestroyer destroyer;
public:
    static tcpHandler* getInstance();
    int connectToServer();
    int disconnectTCPsocket(int tcpSocket);
    int sendCommand(int tcpSocket, std::string message);
    std::string recevieCommand(int tcpSocket);

    void showConnectionPopup();
};
#endif
