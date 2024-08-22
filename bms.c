#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Account {
    char first_name[15];
    char last_name[15];
    char username[30];
    char email[50];
    char password[14];
    int account_number;
    int account_balance;
};

void premenu();
void menu(struct Account a);
void login(struct Account a, char x);
void create_account();
bool validate_email();
int generate_account_number();
void deposit_money();
void withdraw_money();
void transfer_money();
void manage_account();

int main() {
    premenu();
    return 0;
};

void premenu() {
    struct Account a;
    char c[10];
    int choice;

    printf("\n\nSelection option: Login (1) Create Account (2): ");
    fgets(c, sizeof(c), stdin);
    choice = atoi(c);
    switch (choice) {
        case 1:
            // need to initialise a values before login in to pass in correct struct
            login(a, 2);
            break;
        case 2:
            create_account();
            break;
        default:
            printf("################ INVALID INPUT ################");
            premenu();
    }
};

void login(struct Account a, char x) {
    char username[30];
    char password[14];
    FILE* file;

    file = fopen("Account-Details.txt", "r");
    if (file == NULL) {
        fputs("Error... File does not exist", stderr);
        exit(1);
    }

    //struct Account a;

    bool success = false;
    bool valid_username = false;
    bool valid_password = false;
    char valid = 0;
    char buffer[100];
    char* get_string;

    fflush(stdin);

    printf("\nLogin below:\n");
    printf("Username: ");
    fgets(username, 30, stdin);
    printf("Password: ");
    fgets(password, 14, stdin);

    while (!valid_username || !valid_password) {
        fgets(buffer, sizeof(buffer), file);
 
        if (strcmp(buffer, "Username:\n") == 0) {
            get_string = fgets(buffer, sizeof(buffer), file);
            if (strcmp(get_string, username) == 0) {
                valid_username = true;
            }
        } else if (strcmp(buffer, "Password:\n") == 0) {
            get_string = fgets(buffer, sizeof(buffer), file);
            if (strcmp(get_string, password) == 0) {
                valid_password = true;
            }
        } else if (feof(file)) {
            printf("\nInvalid username or password\n");
            login(a, x);
        }
    }

    // x == 1 means has 'a' already initialised, x == 2 means not initialised
    if (x == 1) {
        menu(a);
    }
    if (x == 2) {
        
    }

    fclose(file);

    menu(a);
};

/*
    This function is called when the user selects the option to create an account in the premenu() function.
    It has the role of collecting accurate and valid user input to then append this information to the 
    Account-Details.txt file.
*/
void create_account() {
    struct Account a;
    FILE* accounts;
    
    accounts = fopen("Account-Details.txt", "a");
    if (accounts == NULL) {
        fputs("Error... File does not exist", stderr);
        exit(1);
    }
    
    printf("\nAccount Creation:\nEnter your first name: ");
    fgets(a.first_name, sizeof(a.first_name), stdin);
    printf("\nEnter your last name: ");
    fgets(a.last_name, sizeof(a.last_name), stdin);

    printf("\nEnter a username: ");
    fgets(a.username, sizeof(a.username), stdin);
    
    printf("\nEnter your new email: ");
    fgets(a.email, sizeof(a.email), stdin);
    bool isValid = false;
    isValid = validate_email(a.email);
    while (!isValid) {
        printf("\n################ INVALID INPUT ################\nEnter different email: ");
        fgets(a.email, sizeof(a.email), stdin);
        isValid = validate_email(a.email);
    }

    printf("\nEnter a new password (14 Characters Max): ");
    fgets(a.password, sizeof(a.password), stdin);

    printf("\n################ GENERATING ACC. NO. ################\n");
    a.account_number = generate_account_number();
    printf("Your account number is %d\n", a.account_number);
    a.account_balance = 0;

    printf("\nAccount successfully created\n");

    //fprintf(accounts, "\n\n-----------------------------------\nAccount Number:\n%d\n\nFirst Name:\n%s\nLast Name:\n%s\nUsername:\n%s\nPassword:\n%s\nEmail:\n%s\nAccount Balance:\n%d\n-----------------------------------", a.account_number, a.first_name, a.last_name, a.username, a.password, a.email, a.account_balance);
    fprintf(accounts, "Username: %s Password: %s AccountNum: %d AccountBal: %d FirstName: %s LastName: %s Email: %s\n", a.username, a.password, a.account_number, a.account_balance, a.first_name, a.last_name, a.email);
    fclose(accounts);

    login(a, 1);
};

/*
    This function has the role of validating whether the input from the user for the email section is correct.
    It achieves this by splitting the string into an array of strings to validate each section and also detects
    whether invalid characters have been inputted.
*/
bool validate_email(char* email) {
    int atCount = 0;
    
    for (int i = 0; i < strlen(email); i++) {
        if (email[i] == '@') {
            atCount++;
        }
        if (email[i] == ' ' || email[i] == '/' || email[i] == ':'
            || email[i] == ';' || email[i] == '<' || email[i] == '>'
            || email[i] == ',' || email[i] == '[' || email[i] == ']') {
            return false;
        }
    }

    if (atCount == 1) {
        if (email[0] != '@') {
            char* dot = strchr(email, '.');

            if (dot != NULL && dot > strchr(email, '@')) {
                return true;
            }
        }
    }

    return false;
};

/*
    Function used to generate a unique account number for each account created. Has a while loop to search
    through the Account-Details.txt file to detect whether the number that was generated was unique.
*/
int generate_account_number() {
    int lower = 10000000, upper = 100000000;
    int number = (rand() % (upper - lower)) + lower;
    char buffer[100];
    int get_number;

    bool isDuplicate = false;
    int duplicate_count = 0;
    bool success = false;

    FILE* file;
    file = fopen("Account-Details.txt", "r");

    if (file == NULL) {
        printf("ERROR file empty or wrong file");
        exit(0);
    }
    while (!success) {
        fgets(buffer, sizeof(buffer), file);
        
        if (strcmp(buffer, "Account Number:\n") == 0) {
            get_number = atoi(fgets(buffer, sizeof(buffer), file));
            if (get_number == number) {
                fclose(file);
                isDuplicate = true;
                number = (rand() % (upper - lower)) + lower;
                file = fopen("Account-Details.txt", "r");
                isDuplicate = false;
            }
        }
        else if (feof(file) && !isDuplicate) {
            success = true;
        }
    }

    fclose(file);

    return number;
}

/*
    This function is called once the user has properly logged into the system. Its purpose is to collect user
    input to then access different parts of the system through functions in order to carry out operations
    such as depositing money or changing account details.
*/
void menu(struct Account a) {
    int choice;
    char c[10];

    printf("\nMenu:\nWelcome back %s!\n", a.first_name);
    printf("Current Balance: %d\n", a.account_balance);
    printf("1. Deposit Money\n");
    printf("2. Withdraw Money\n");
    printf("3. Transfer Money\n");
    printf("4. Manage Account\n");
    printf("5. Exit\n");

    printf("\nChoose option: ");
    fgets(c, sizeof(c), stdin);
    choice = atoi(c);
    switch (choice) {
        case 1:
            deposit_money(a);
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
            menu(a);
    }
};

void deposit_money(struct Account a) {
    //struct Account a;
    int amount = 0;
    char c[10];

    printf("\nDeposit Money menu:\n");
    printf("Current Balance: %d\n", a.account_balance);
    printf("Deposit money: ");
    fgets(c, sizeof(c), stdin);
    amount = atoi(c);
    while (amount <= 0) {
        printf("\nIncorrect input. Enter amount to deposit: ");
        fgets(c, sizeof(c), stdin);
        amount = atoi(c);
    }
    a.account_balance += amount;
    printf("Account balance: %d and amount: %d", a.account_balance, amount);
    menu(a);
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