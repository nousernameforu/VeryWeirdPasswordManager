//UserAuthentication.cpp

#include "UserAuthentication.h"

//Singleton attampt section

// Initialize static members
UserAuthentication* UserAuthentication::p_instance = nullptr;
SingletonDestroyer UserAuthentication::destroyer;

// Implementation of SingletonDestroyer destructor
SingletonDestroyer::~SingletonDestroyer() {
    delete p_instance;
    cout << "[signletonDestroyer] Destructor ran." << endl;
}

// Implementation of SingletonDestroyer initialize method
void SingletonDestroyer::initialize(UserAuthentication* p) {
    p_instance = p;
}

// Implementation of UserAuthentication getInstance method
UserAuthentication* UserAuthentication::getInstance() {
    if (!p_instance) {
        p_instance = new UserAuthentication();
        destroyer.initialize(p_instance);
    }
    return p_instance;
}

//Singleton attampt section end



//Constructor opens the sqlite database
UserAuthentication::UserAuthentication() : db(nullptr) {
    if (sqlite3_open("/database/temporary_passwords.db", &db) != SQLITE_OK) {
        cerr << "Error opening database\n";
        //error if not open
    }
    cout << "[userAuthentication] Constructor ran." << endl;
}

//Destructor will close the connection
UserAuthentication::~UserAuthentication() {
    if (db) {
        sqlite3_close(db);
    }
    cout << "[userAuthentication] Destructor ran." << endl;
}

//PRIVATE method to generate random salt
string UserAuthentication::generateRandomSalt(int length) {
    const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const int charsetSize = sizeof(charset) - 1;

    random_device rd;
    default_random_engine rng(rd());
    uniform_int_distribution<int> distribution(0, charsetSize - 1);

    string salt;
    for (int i = 0; i < length; ++i) {
        salt += charset[distribution(rng)];
    }

    return salt;
}

//User authentication based on the inputs 
bool UserAuthentication::authenticateUser(const string& username, const string& password) {
    string query = "SELECT password, salt FROM users WHERE username = ?;";
    sqlite3_stmt* statement;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &statement, nullptr) != SQLITE_OK) {
        cout << "test: " << sqlite3_prepare_v2(db, query.c_str(), -1, &statement, nullptr) << endl;
        cerr << "Error preparing SQL statement\n";
        sqlite3_close(db);
        return false;
    }

    //bind the username as a parameter
    if (sqlite3_bind_text(statement, 1, username.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
        cerr << "Error binding parameter\n";
        sqlite3_finalize(statement);
        sqlite3_close(db);
        return false;
    }

    int result = sqlite3_step(statement);

    //check if the user exists
    if (result != SQLITE_ROW) {
        sqlite3_finalize(statement);
        sqlite3_close(db);
        return false;
    }

    //fetch the hashed password and salt from the result set
    const char *hashedPassword = reinterpret_cast<const char*>(sqlite3_column_text(statement, 0));
    const char* salt = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));

    //concatenate the password and salt

    string concatenatedPassword = password + salt;

    //hash the concatenated password using SHA-256
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLength;

    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, EVP_sha256(), nullptr);
    EVP_DigestUpdate(mdctx, concatenatedPassword.c_str(), concatenatedPassword.length());
    EVP_DigestFinal_ex(mdctx, hash, &hashLength);
    EVP_MD_CTX_free(mdctx);

    //convert hash to string
    string hashedInput(reinterpret_cast<const char*>(hash), hashLength);

    //check if password matches
    bool isAuthenticated = (hashedInput == hashedPassword);

    sqlite3_finalize(statement);
    return isAuthenticated;

}


//User registration based on the inputs
bool UserAuthentication::registerUser(const string& username, const string& password) {
    //generate a random salt
    string salt = generateRandomSalt(64);

    //goncatenate the password and salt
    string concatenatedPassword = password + salt;

    //hash the concatenated password using SHA-256
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLength;

    EVP_MD_CTX* mdctx = nullptr;
    const EVP_MD* md = EVP_sha256();

    if (!(mdctx = EVP_MD_CTX_create())) {
        cerr << "Error creating EVP_MD_CTX\n";
        return false;
    }

    if (EVP_DigestInit_ex(mdctx, md, nullptr) != 1 ||
        EVP_DigestUpdate(mdctx, concatenatedPassword.c_str(), concatenatedPassword.length()) != 1 ||
        EVP_DigestFinal_ex(mdctx, hash, &hashLength) != 1) {
        cerr << "Error hashing password\n";
        EVP_MD_CTX_destroy(mdctx);
        return false;
    }

    EVP_MD_CTX_destroy(mdctx);

    //convert the hash to a string
    string hashedPassword(reinterpret_cast<const char*>(hash), hashLength);

    //insert the username, hashed password, and salt into the database
    string query = "INSERT INTO users (username, password, salt) VALUES ('" + username + "', '" + hashedPassword + "', '" + salt + "');";
    if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, nullptr) != SQLITE_OK) {
        cerr << "Error executing SQL statement\n";
        return false;
    }

    return true;
}