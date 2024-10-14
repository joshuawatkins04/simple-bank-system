#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

sqlite3* open_database();
void create_table(sqlite3* db);
void insert_user(sqlite3* db, const char* first_name, const char* last_name, 
        const char* username, const char* password, const char* email, 
        int account_number, int balance);
int verify_login(sqlite3* db, const char* username, const char* password);
int generate_unique_account_number(sqlite3* db);

#endif