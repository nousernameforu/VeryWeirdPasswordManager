// UserAuthentication.h

//Header file for the library

#ifndef USERAUTHENTICATION_H
#define USERAUTHENTICATION_H

#include <iostream>
#include <sqlite3.h>
#include <string>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <random>

using namespace std;

class UserAuthentication {
private:
    string generateRandomSalt(int length);
    sqlite3* db;
public:
    UserAuthentication();
    ~UserAuthentication();
    bool authenticateUser(const string& username, const string& password);
    bool registerUser(const string& username, const string& password);
};

#endif
