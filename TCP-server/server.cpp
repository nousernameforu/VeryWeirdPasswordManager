#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <fstream>
#include <sqlite3.h>
/*
Will need to write a library for parcing an incomming command
Will need to write a library to manage a database 
*/

void dumpBufferToLog(const char* buffer, ssize_t size) {
    std::ofstream logFile("commands_log.txt", std::ios::app); // Open log file in append mode

    if (logFile.is_open()) {
        logFile.write(buffer, size);
        logFile << std::endl;
        logFile.close();
        std::cout << "Buffer dumped to commands_log.txt" << std::endl;
    } else {
        std::cerr << "Error opening log file." << std::endl;
    }
}

class TCPauth {
    public:
    bool authenticateUser(const std::string& username, const std::string& password) {
        // Use SQLite to query the database for the given username and password
        sqlite3* db;
         // Connect to the SQLite database in the Docker container
        if (sqlite3_open("/database/temporary_passwords.db", &db) != SQLITE_OK) {
            std::cerr << "Error opening database\n";
            return false;
        }

        std::string query = "SELECT * FROM users WHERE username = '" + username + "' AND password = '" + password + "';";
        sqlite3_stmt* statement;

        if (sqlite3_prepare_v2(db, query.c_str(), -1, &statement, nullptr) != SQLITE_OK) {
            std::cerr << "Error preparing SQL statement\n";
            sqlite3_close(db);
            return false;
        }

        int result = sqlite3_step(statement);

        // Check if the user exists and the password is correct
        bool isAuthenticated = (result == SQLITE_ROW);

        sqlite3_finalize(statement);
        sqlite3_close(db);

        return isAuthenticated;
    }

};

class TCPreg {
public:
    bool registerUser(const std::string& username, const std::string& password) {
        // Use SQLite to insert a new user into the database
        sqlite3* db;

        if (sqlite3_open("/database/temporary_passwords.db", &db) != SQLITE_OK) {
            std::cerr << "Error opening database\n";
            return false;
        }

        std::string query = "INSERT INTO users (username, password) VALUES ('" + username + "', '" + password + "');";
        if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
            std::cerr << "Error executing SQL statement\n";
            sqlite3_close(db);
            return false;
        }

        sqlite3_close(db);
        return true;
    }
};

class TCPServer {
private:
    int serverSocket;
    bool isAuthenticated = false;

public:
    TCPServer(int port) {
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in serverAddress;
        memset(&serverAddress, 0, sizeof(serverAddress));
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = INADDR_ANY;
        serverAddress.sin_port = htons(port);

        bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
        listen(serverSocket, 5);

        std::cout << "Server listening on port " << port << "..." << std::endl;
    }

    ~TCPServer() {
        close(serverSocket);
    }

    void run() {
        int clientSocket = accept(serverSocket, NULL, NULL);
        while (true) {
            char buffer[1024];
            ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (bytesRead <= 0) {
                std::cerr << "Error reading from client." << std::endl;
                break;
            }

            buffer[bytesRead] = '\0';

            for (ssize_t i = 0; i < bytesRead; ++i) {
                if (buffer[i] == '\r' || buffer[i] == '\n') {
                    buffer[i] = '\0';
                }
            }

            std::cout << "Received from client: " << buffer << std::endl;
            dumpBufferToLog(buffer, bytesRead);

            if (!isAuthenticated) {
                if (strncmp(buffer, "AUTH", 4) == 0) {
                    char* token = strtok(buffer + 5, " ");
                    std::string username = token ? token : "";
                    token = strtok(nullptr, " ");
                    std::string password = token ? token : "";
                    TCPauth TCPauth_user;
                    if (TCPauth_user.authenticateUser(username, password)) {
                        send(clientSocket, "Authentication successful\n", 27, 0);
                        isAuthenticated = true;
                    } else {
                        send(clientSocket, "Authentication failed\n", 23, 0);
                    }
                } else if (strncmp(buffer, "REG", 3) == 0) {
                    char* token = strtok(buffer + 4, " ");
                    std::string username = token ? token : "";
                    token = strtok(nullptr, " ");
                    std::string password = token ? token : "";
                    TCPreg TCPreg_user;
                    if (TCPreg_user.registerUser(username, password)) {
                        send(clientSocket, "User registration successful\n", 30, 0);
                    } else {
                        send(clientSocket, "User registration failed\n", 26, 0);
                    }
                } else {
                    send(clientSocket, "Authentication needed to proceed\n", 34, 0);
                }
            } else {
                if (strcmp(buffer, "HELLO") == 0) {
                    send(clientSocket, "Hello, client!\n", 16, 0); //Hello 
                } else if (strcmp(buffer, "QUIT") == 0) {
                    send(clientSocket, "Quitting the program\n", 22, 0); //Quit function
                    close(clientSocket); 
                    break;
                } else {
                    send(clientSocket, "Unknown command\n", 17, 0);
                }
            }
        }
    }
};

int main() {
    TCPServer server(8080);
    server.run();

    return 0;
}
