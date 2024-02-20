#include <iostream>
#include <sqlite3.h>

int main() {
    sqlite3 *db;
    char *errMsg = 0;

    // Open database or create if it doesn't exist
    int rc = sqlite3_open("temporary_passwords.db", &db);

    if (rc) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    // Create a table named "users"
    const char *createTableQuery =
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT NOT NULL,"
        "password TEXT NOT NULL);";

    rc = sqlite3_exec(db, createTableQuery, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "Error creating table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Table created successfully." << std::endl;
    }

    // Insert sample data
    const char *insertDataQuery1 = "INSERT INTO users (username, password) VALUES ('user1', 'pass1');";
    const char *insertDataQuery2 = "INSERT INTO users (username, password) VALUES ('user2', 'pass2');";

    rc = sqlite3_exec(db, insertDataQuery1, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "Error inserting data: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Data inserted successfully." << std::endl;
    }

    rc = sqlite3_exec(db, insertDataQuery2, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "Error inserting data: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std
::cout << "Data inserted successfully." << std::endl;
}
// Close the database
sqlite3_close(db);

return 0;
}