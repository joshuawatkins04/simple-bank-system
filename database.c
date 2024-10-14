#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

sqlite3* open_database() {
    sqlite3* db;
    if (sqlite3_open("user_data.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        exit(1);
    }
    return db;
}

void create_table(sqlite3* db) {
    const char* sql = 
        "CREATE TABLE IF NOT EXISTS Users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "first_name TEXT, "
        "last_name TEXT, "
        "username TEXT UNIQUE, "
        "password TEXT, "
        "email TEXT, "
        "account_number INTEGER, "
        "balance INTEGER);";

    char* err_msg = NULL;
    if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        exit(1);
    }
}

void insert_user(sqlite3* db, const char* first_name, const char* last_name,
        const char* username, const char* password, const char* email, 
        int account_number, int balance) {
    
    char sql[512];
    snprintf(sql, sizeof(sql),
        "INSERT INTO Users (first_name, last_name, username, password, email, account_number, balance) "
        "VALUES ('%s', '%s', '%s', '%s', '%s', %d, %d);",
        first_name, last_name, username, password, email, account_number, balance
    );

    char* err_msg = NULL;
    if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        exit(1);
    }
}

int verify_login(sqlite3* db, const char* username, const char* password) {
    char sql[256];
    snprintf(sql, sizeof(sql),
        "SELECT * FROM Users WHERE username = '%s' AND password = '%s';",
        username, password
    );

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to execute query: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    int login_success = sqlite3_step(stmt) == SQLITE_ROW;
    sqlite3_finalize(stmt);
    return login_success;
}

int generate_unique_account_number(sqlite3* db) {
    int lower = 10000000, upper = 100000000;
    int account_number;
    char sql[128];
    sqlite3_stmt* stmt;

    while (1) {
        account_number = (rand() % (upper - lower + 1)) + lower;

        snprintf(sql, sizeof(sql),
            "SELECT 1 FROM Users WHERE account_number = %d;", account_number
        );

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
            return -1;
        }

        if (sqlite3_step(stmt) != SQLITE_ROW) {
            sqlite3_finalize(stmt);
            break;
        }

        sqlite3_finalize(stmt);
    }

    return account_number;
}