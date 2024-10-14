#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "database.h"

#define MAX_LENGTH 256

struct Account {
    char email[40];
    char username[30];
    char first_name[15];
    char last_name[15];
    char password[14];
    int account_number;
    int account_balance;
};

void premenu();
void menu();
void login();
// bool read_account_from_file(FILE* file, struct Account* account);
// bool validate_login(struct Account* account, char* input_username, char* input_password);
void create_account();
bool validate_email(const char* email);
// bool check_duplicate(FILE* file, int account_number);
// void write_account_to_file(FILE* file, struct Account* account);
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
    }

    sqlite3_close(db);
};


/* 
    - Need to fix email sscanf(). Function validate_email(email) is running as expected but is validating
    slightly wrong emails such as 'jasdf@asdf' but will correctly find emails like 'sdf'
*/
void create_account() {
    sqlite3* db = open_database();
    char username[30], password[14], email[40];
    int account_number;
    

    printf("\nAccount Creation:\n");

    printf("Enter your first name: ");
    scanf("%s", &first_name);
    flush_input();

    printf("\nEnter your last name: ");
    scanf("%s", &last_name);
    flush_input();

    printf("\nEnter a username: ");
    scanf("%s", username);
    flush_input();

    bool valid_email = false;
    while (!valid_email) {
        printf("Enter your email: ");
        scanf("%s", email);
        flush_input();

        if (validate_email(email)) {
            valid_email = true;
        } else {
            printf("################ INVALID INPUT ################\n");
        }
    }

    printf("\nEnter a new password (14 Characters Max): ");
    scanf("%14s", password);
    flush_input();

    account_number = generate_unique_account_number(db);
    account_balance = 0;

    printf("\nAccount successfully created. Your account number is %d\n", account_number);

    // write_account_to_file(accounts_file, &new_account);

    // fclose(accounts_file);

    menu();
};

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

// bool check_duplicate(FILE* file, int account_number) {
//     struct Account existing_account;
//     rewind(file);

//     while (fread(&existing_account, sizeof(struct Account), 1, file)) {
//         if (existing_account.account_number == account_number) {
//             return true;
//         }
//     }

//     return false;
// }

// void write_account_to_file(FILE* file, struct Account* account) {
//     if (fprintf(file, "Account Number:\n%d\nFirst Name:\n%s\nLast Name:\n%s\nUsername:\n%s\nPassword:\n%s\nEmail:\n%s\nAccount Balance:\n%d\n\n",
//         account->account_number,
//         account->first_name,
//         account->last_name,
//         account->username,
//         account->password,
//         account->email,
//         account->account_balance)< 0) {
//         fprintf(stderr, "Error writing account info to file.\n");
//         exit(1);
//     }
// }

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


/* OLD CODE
void login(void) {
    char input_username[30];
    char input_password[14];
    struct Account account;
    FILE* file;

    file = fopen("Account-Details.txt", "r");
    if (file == NULL) {
        fputs("Error... File does not exist", stderr);
        exit(1);
    }

    printf("\nLogin below:\n");

    flush_input();
    printf("Username: ");
    if (fgets(input_username, sizeof(input_username), stdin) == NULL) {
        printf("Error reading username.\n");
        fclose(file);
        return;
    }
    input_username[strcspn(input_username, "\n")] = 0;

    printf("Password: ");
    if (fgets(input_password, sizeof(input_password), stdin) == NULL) {
        printf("Error reading password.\n");
        fclose(file);
        return;
    }
    input_password[strcspn(input_password, "\n")] = 0;

    bool login_success = false;
    while (read_account_from_file(file, &account)) {
        if (validate_login(&account, input_username, input_password)) {
            printf("Login successful!\n");
            login_success = true;
            menu();
            break;
        }
    }

    if (!login_success) {
        printf("Invalid username or password.\n");
    }

    fclose(file);
};

bool validate_login(struct Account* account, char* input_username, char* input_password) {
    return (
        strcmp(account->username, input_username) == 0 &&
        strcmp(account->password, input_password) == 0
    );
}

bool read_account_from_file(FILE* file, struct Account* account) {
    return fread(account, sizeof(struct Account), 1, file) == 1;
}
*/