#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 256

struct Account {
    char first_name[15];
    char last_name[15];
    char password[14];
    int account_number;
    int account_balance;

    char* email;
    char user[10];
    char domain[4];
    int valid;
};

void premenu();
void menu();
void login();
void create_account();
int validate_email();
int generate_account_number();
void deposit_money();
void withdraw_money();
void transfer_money();
void manage_account();

int main() {
    struct Account a;


    printf("%d", generate_account_number());

    premenu(a);


    return 0;
}

void premenu(struct Account a) {
    int choice;

    printf("\n\nSelection option: Login (1) Create Account (2):\n");
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            login();
            break;
        case 2:
            create_account(a);
            break;
        default:
            printf("################ INVALID INPUT ################");
            premenu(a);
    }
};

void login(void) {
    printf("\nEnter details below:\nAccount number: ");
    //scanf("%d", &a.account_number);
    printf("Password: ");
    //scanf("%s", &a.password);
};

/* 
    - Need to fix email sscanf(). Function validate_email(email) is running as expected but is validating
    slightly wrong emails such as 'jasdf@asdf' but will correctly find emails like 'sdf'
    - Need to fix account number generator to wider values
    - Then need to store this information into Account-Details.txt by writing it to it
*/
void create_account(struct Account a) {
    a.email = malloc(MAX_LENGTH);
    
    // Name
    printf("\nAccount Creation:\nEnter your first and last name: ");
    scanf("%s %s", &a.first_name, &a.last_name);

    // Email
    printf("\nEnter your new email: ");
    scanf("%s", a.email);
    a.valid = validate_email(a.email);
    while (a.valid != 2) {\
        printf("\n################ INVALID INPUT ################\nEnter different email: ");
        scanf("%s", a.email);
        a.valid = validate_email(a.email);
    }
    
    // Password
    printf("\nEnter a new password (14 Characters Max): ");
    scanf("%14s", &a.password);

    // Randomly generated account number
    printf("\n################ GENERATING ACC. NO. ################\n");
    a.account_number = generate_account_number();
    printf("Your account number is %d\n", a.account_number);
    a.account_balance = 0;

    printf("Account successfully created");

    FILE* accounts;
    accounts = fopen("Account-Details.txt", "a");
    fprintf(accounts, "Account Number: %d\nFirst Name: %s\nLast Name: %s\nPassword: %s\nAccount Balance: %d\n", a.account_number, a.first_name, a.last_name, a.password, a.account_balance);
    fclose(accounts);

    menu(a);
};

int validate_email(char* email) {
    struct Account a;
    return sscanf(email, "%[_a-zA-Z0-9.]@%[_a-zA-Z0-9.]", a.user, a.domain);
};

// In the future need to add a check to see if account number already exists
int generate_account_number() {
    int lower = 10000000, upper = 100000000;
    //int number = (rand() % (upper - lower)) + lower;
    int number = 10008380;
    char line[100];
    char found = 0;
    char valid = 0;
    char str_number[sizeof(number)];

    sprintf(str_number, "%d", number);

    FILE* file;
    file = fopen("Account-Details.txt", "r");

    while (fread(line, sizeof(line), 1, file)) {
        if (strcmp(str_number, ))
    }

    // Check if account number already exists to avoid duplicate account numbers
    // FILE* file;
    // do {
    //     file = fopen("Account-Details.txt", "r");
    //     if (file == NULL) {
    //         printf("ERROR file empty or wrong file");
    //         exit(0);
    //     }
    //     // while (fgets(line, sizeof(line), file)) {
    //     //     printf(line);
    //     //     if (strstr(line, str_number)) {
    //     //         printf("Number found ");
    //     //     }
    //     // }
    //     while (fgets(line, sizeof(line), file)) {
    //         fscanf(file, , line);
    //     }
    //     if (!found) {
    //         printf("Number not found");
    //         valid = 1;
    //     } 
    // } while (!valid);

    // fclose(file);
    return number;
}

void menu(struct Account a) {
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