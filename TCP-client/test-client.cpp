#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <fstream>

using namespace std;

//receiveFile
bool receiveFileFromServer(int tcpSocket) {
    int fileSize;
    recv(tcpSocket, &fileSize, sizeof(fileSize), 0);
    cout << "Trying to receive a file from server" << endl;
    // Receive file data
    std::ofstream receivedFile("received_file.txt", std::ios::binary);
    char buffer[1024];
    int bytesRead;

    while (fileSize > 0) {
        bytesRead = recv(tcpSocket, buffer, sizeof(buffer), 0);
        receivedFile.write(buffer, bytesRead);
        fileSize -= bytesRead;
    }

    receivedFile.close();
    cout << "The file has been received successfully" << endl;
    return true;
}

bool sendFileToServer(int tcpSocket) {
    std::ifstream fileToSend("received_file.txt", std::ios::binary);
    if (!fileToSend.is_open()) {
        perror("Error opening file");
    }

    cout << "Trying to send file <" << "received_file.txt" << "> " << endl;

    // Send file size
    fileToSend.seekg(0, std::ios::end);
    int fileSize = fileToSend.tellg();
    fileToSend.seekg(0, std::ios::beg);
    send(tcpSocket, &fileSize, sizeof(fileSize), 0);

    // Send file data
    char buffer[1024];
    while (!fileToSend.eof()) {
        fileToSend.read(buffer, sizeof(buffer));
        send(tcpSocket, buffer, fileToSend.gcount(), 0);
    }

    fileToSend.close();

    std::cout << "File sent successfully" << endl;
    return true;
}


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

        send(tcpSocket, message.c_str(), message.size(), 0);

        if (message == "Exit") {
            cout << "Exiting client." << endl;
            break;
        } else
 
        if (strncmp(charArray, "SEND_FILE", 9) == 0) {
            if(!receiveFileFromServer(tcpSocket) == true){
                cerr << "Failed to receive file from server." << endl;
            };
        } else 
        
        if (strncmp(charArray, "RECEIVE_FILE", 12) == 0) {
            if(!sendFileToServer(tcpSocket) == true){
                cerr << "Failed to send file to the server." << endl;
            };
        } else {

        char buffer[1024];
        ssize_t bytesRead = recv(tcpSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            cerr << "Error reading from server." << endl;
            break;
        }

        buffer[bytesRead] = '\0'; // Null-terminate the received data
        cout << "Received from server: " << buffer << endl;
        }
    }

    close(tcpSocket);
    return 0;
}