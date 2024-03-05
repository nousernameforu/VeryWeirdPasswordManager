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

//Singleton attampt section

class UserAuthentication;

class SingletonDestroyer {
private:
    UserAuthentication* p_instance;

public:
    ~SingletonDestroyer();
    void initialize(UserAuthentication* p);
};

//UserAuthentication part

class UserAuthentication {
private:
    static UserAuthentication* p_instance;
    static SingletonDestroyer destroyer;
    string generateRandomSalt(int length);
    sqlite3* db;

public:
    UserAuthentication();
    ~UserAuthentication();
    bool authenticateUser(const string& username, const string& password);
    bool registerUser(const string& username, const string& password);
    static UserAuthentication* getInstance();

};

#endif
