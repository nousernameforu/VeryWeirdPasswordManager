#include "UserAuthentication.h"

using namespace std;

// Constructor opens the sqlite database
UserAuthentication::UserAuthentication() : db(nullptr) {
    if (sqlite3_open("/database/temporary_passwords.db", &db) != SQLITE_OK) {
        cerr << "Error opening database\n";
        //error if not open
    }
}

// Destructor will close the connection
UserAuthentication::~UserAuthentication() {
    if (db) {
        sqlite3_close(db);
    }
}

//User authentication based on the inputs 
bool UserAuthentication::authenticateUser(const string& username, const string& password) {
    if (sqlite3_open("/database/temporary_passwords.db", &db) != SQLITE_OK) {
        cerr << "Error opening database\n";
        return false;
    }

    string query = "SELECT * FROM users WHERE username = '" + username + "' AND password = '" + password + "';";
    sqlite3_stmt* statement;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &statement, nullptr) != SQLITE_OK) {
        cerr << "Error preparing SQL statement\n";
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

//User registration based on the inputs
bool UserAuthentication::registerUser(const string& username, const string& password) {
    if (sqlite3_open("/database/temporary_passwords.db", &db) != SQLITE_OK) {
        cerr << "Error opening database\n";
        return false;
    }

    string query = "INSERT INTO users (username, password) VALUES ('" + username + "', '" + password + "');";
    if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        cerr << "Error executing SQL statement\n";
        sqlite3_close(db);
        return false;
    }

    sqlite3_close(db);
    return true;
}