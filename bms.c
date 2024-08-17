#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_LENGTH 256

struct Account {
    char first_name[15];
    char last_name[15];
    char username[30];
    char password[14];
    int account_number;
    int account_balance;

    char* email;
};

void premenu();
void menu();
void login();
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
}

void premenu() {
    int choice;

    printf("\n\nSelection option: Login (1) Create Account (2):\n");
    //scanf("%d", &choice);
    fgets(choice, 15, stdin);
    switch (choice) {
        case 1:
            login();
            break;
        case 2:
            create_account();
            break;
        default:
            printf("################ INVALID INPUT ################");
            premenu();
    }
};

void login(void) {
    char username[30];
    char password[14];
    FILE* file;

    file = fopen("Account-Details.txt", "r");
    if (file == NULL) {
        fputs("Error... File does not exist", stderr);
        exit(1);
    }

    struct Account a;

    bool success = false;
    char buffer[100];
    char* get_string;

    fflush(stdin);

    printf("\nLogin below:\n");
    printf("Username: ");
    //scanf("%s", username);
    fgets(username, 30, stdin);
    // printf("\nPassword: ");
    // printf("\n");
    // fgets(password, 14, stdin);

    while (!success) {
        fgets(buffer, sizeof(buffer), file);
 
        if (strcmp(buffer, "Username:\n") == 0) {
            get_string = fgets(buffer, sizeof(buffer), file);
            printf("\n%s\n", get_string);
            if (strcmp(get_string, username) == 0) {
                printf("VALID username");
                success = true;
            }
        }
        else if (feof(file)) {
            printf("Invalid");
            success = true;
        }

        // if (strcmp(buffer, "Account Number:\n") == 0) {
        //     get_number = atoi(fgets(buffer, sizeof(buffer), file));
        //     if (get_number == number) {
    }

    fclose(file);

    // while (fread(&a, sizeof(a), 1, file)) {
    //     //if (strcmp(username, a.username) == 0 && strcmp(password, a.password) == 0) {
    //     if (strcmp(username, a.username) == 0) {
    //         printf("\nLogin Successful");
    //     }
    //     else {
    //         printf("\nIncorrect login details");
    //     }
    // }
    // fclose(file);
};

/* 
    - Need to fix email sscanf(). Function validate_email(email) is running as expected but is validating
    slightly wrong emails such as 'jasdf@asdf' but will correctly find emails like 'sdf'
*/
void create_account() {
    struct Account a;
    a.email = malloc(MAX_LENGTH);
    FILE* accounts;
    
    accounts = fopen("Account-Details.txt", "a");
    if (accounts == NULL) {
        fputs("Error... File does not exist", stderr);
        exit(1);
    }
    
    // Name
    printf("\nAccount Creation:\nEnter your first name: ");
    scanf("%s", a.first_name);
    printf("\nEnter your last name: ");
    scanf("%s", a.last_name);

    printf("\nEnter a username: ");
    scanf("%s", a.username);

    // Email
    printf("\nEnter your new email: ");
    scanf("%s", a.email);
    bool isValid = false;
    isValid = validate_email(a.email);
    while (!isValid) {
        printf("\n################ INVALID INPUT ################\nEnter different email: ");
        scanf("%s", a.email);
        isValid = validate_email(a.email);
    }
    
    // Password
    printf("\nEnter a new password (14 Characters Max): ");
    scanf("%14s", a.password);

    // Randomly generated account number
    printf("\n################ GENERATING ACC. NO. ################\n");
    a.account_number = generate_account_number();
    printf("Your account number is %d\n", a.account_number);
    a.account_balance = 0;

    printf("\nAccount successfully created\n");
    
    fprintf(accounts, "\n\nAccount Number:\n%d\nFirst Name:\n%s\nLast Name:\n%s\nUsername:\n%s\nPassword:\n%s\nEmail:\n%s\nAccount Balance:\n%d", a.account_number, a.first_name, a.last_name, a.username, a.password, a.email, a.account_balance);
    fclose(accounts);

    login();
};

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
            printf("FIXED: %d", number);
            success = true;
        }
    }

    fclose(file);

    return number;
}

void menu() {
    struct Account a;
    int choice;

    printf("\nMenu:\n");
    printf("Current Balance: %d\n", a.account_balance);
    printf("1. Deposit Money\n");
    printf("2. Withdraw Money\n");
    printf("3. Transfer Money\n");
    printf("4. Manage Account\n");
    printf("5. Exit\n");

    printf("\nChoose option: ");
    scanf("%d", &choice);
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
            menu(a);
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