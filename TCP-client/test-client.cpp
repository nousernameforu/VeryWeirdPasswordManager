#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <fstream>

#include "FileHandeling.h"

using namespace std;

int main() {
    int tcpSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

    if (connect(tcpSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        cerr << "Error connecting to the server." << endl;
        return 1;
    }

    cout << "Connected to the server." << endl;
    while (true) {
        string message;
        cout << "Enter message (type 'Exit' to quit): ";
        getline(cin, message);
        const char* charArray = message.c_str();
        

        if (message == "Exit") {
            cout << "Exiting client." << endl;
            break;
        } else
 
        if (strncmp(charArray, "SEND_FILE", 10) == 0) {
            send(tcpSocket, message.c_str(), message.size(), 0);
            if(!FileHandeling::getInstance()->sendFile(tcpSocket,"received_file") == true){
                cerr << "Failed to send file to the server." << endl;
            };
        } else 
        
        if (strncmp(charArray, "RECEIVE_FILE", 13) == 0) {
            send(tcpSocket, message.c_str(), message.size(), 0);
            if(!FileHandeling::getInstance()->receiveFile(tcpSocket,"received_file") == true){
                cerr << "Failed to receive file from server." << endl;
            };
        } else {
        send(tcpSocket, message.c_str(), message.size(), 0);
        char MSGbuffer[1024];
        ssize_t bytesRead = recv(tcpSocket, MSGbuffer, sizeof(MSGbuffer), 0);
        if (bytesRead <= 0) {
            cerr << "Error reading from server." << endl;
            break;
        }

        // MSGbuffer[bytesRead] += '\0'; // Null-terminate the received data
        cout << "Received from server: " << MSGbuffer << endl;
        }
    }

    close(tcpSocket);
    return 0;
}