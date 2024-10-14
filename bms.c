#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <openssl/evp.h>
#include "database.h"

void premenu();
void menu();
void login();
void create_account();
void hash_password(const char* password, char* hash_output);
bool validate_email(const char* email);
void deposit_money();
void withdraw_money();
void transfer_money();
void manage_account();
void flush_input();


int main() {
    sqlite3* db = open_database();
    create_table(db);
    sqlite3_close(db);

    premenu();
}

void premenu() {
    int choice;

    printf("\n\nSelection option: Login (1) Create Account (2):\n");

    while (true) {
        if (scanf("%d", &choice) == 1 && (choice == 1 || choice == 2)) {
            break;
        }
        printf("################ INVALID INPUT ################");
        while (getchar() != '\n');
    }

    switch (choice) {
        case 1:
            login();
            break;
        case 2:
            create_account();
            break;
    }
};

void login() {
    sqlite3* db = open_database();

    char username[30], password[14];

    printf("\nLogin below:\n");

    printf("Username: ");
    scanf("%s", username);
    flush_input();

    printf("Password: ");
    scanf("%s", password);
    flush_input();

    if (verify_login(db, username, password)) {
        printf("Login successful!\n");
    } else {
        printf("Invalid username or password.\n");
        login();
    }

    sqlite3_close(db);

    menu();
};

void create_account() {
    sqlite3* db = open_database();
    char hashed_password[65], email[40], username[30], first_name[15], last_name[15], password[14];
    int account_number, balance = 0;

    printf("\nAccount Creation:\n");

    printf("Enter your first name: ");
    scanf("%14s", first_name);
    flush_input();

    printf("Enter your last name: ");
    scanf("%14s", last_name);
    flush_input();

    printf("Enter a username: ");
    scanf("%29s", username);
    flush_input();

    bool valid_email = false;
    while (!valid_email) {
        printf("Enter your email: ");
        scanf("%39s", email);
        flush_input();

        if (validate_email(email)) {
            valid_email = true;
        } else {
            printf("################ INVALID INPUT ################\n");
        }
    }

    printf("Enter a new password (14 Characters Max): ");
    scanf("%13s", password);
    hash_password(password, hashed_password);
    flush_input();

    account_number = generate_unique_account_number(db);
    if (account_number == -1) {
        printf("Error generating account number.\n");
        sqlite3_close(db);
        return;
    }

    insert_user(db, first_name, last_name, username, hashed_password, email, account_number, balance);
    printf("\nAccount successfully created. Your account number is %d\n", account_number);

    sqlite3_close(db);

    menu();
};

void hash_password(const char* password, char* hash_output) {
    unsigned char hash[32];
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();

    EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);
    EVP_DigestUpdate(mdctx, password, strlen(password));
    EVP_DigestFinal_ex(mdctx, hash, NULL);
    EVP_MD_CTX_free(mdctx);

    for (int i = 0; i < sizeof(hash); i++) {
        sprintf(hash_output + (i * 2), "%02x", hash[i]);
    }
}

bool validate_email(const char* email) {
    int at_count = 0;
    int dot_count = 0;
    const char* at_ptr = NULL;

    for (int i = 0; i < strlen(email); i++) {
        if (email[i] == '@') {
            at_count++;
            at_ptr = &email[i];
        } else if (email[i] == '.') {
            dot_count++;
        }
        if (isspace(email[i]) || strchr("/:;<>[]", email[i])) {
            return false;
        }
    }

    if (at_count == 1 && dot_count >= 1 && at_ptr && strchr(at_ptr, '.')) {
        return true;
    }

    return false;
};

void flush_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void menu() {
    int choice;

    while (true) {
        printf("\nMenu:\n");
        printf("1. Deposit Money\n");
        printf("2. Withdraw Money\n");
        printf("3. Transfer Money\n");
        printf("4. Manage Account\n");
        printf("5. Exit\n");

        printf("\nChoose option: ");
        if (scanf("%d", &choice) != 1) {
            printf("################ INVALID INPUT ################\n");
            flush_input();
            continue;
        }

        flush_input();

        switch (choice) {
            case 1:
                deposit_money();
                break;
            case 2:
                withdraw_money();
                break;
            case 3:
                transfer_money();
                break;
            case 4:
                manage_account();
                break;
            case 5:
                exit(0);
            default:
                printf("################ INVALID INPUT ################");
                break;
        }
    }
};

void deposit_money() {
    printf("\nDeposit Money menu:\n");

};

void withdraw_money() {
    printf("\nWithrdaw Money menu:\n");
};

void transfer_money() {
    printf("\nTransfer Money menu:\n");
};

void manage_account() {
    printf("\nManage Account menu:\n");
};