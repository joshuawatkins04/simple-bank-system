#include <stdio.h>
#include <stdlib.h>

struct Account {
    char name[40];
    char password[15];
    int account_number;
    int balance;
    int deposit_amount;
};

void premenu();
void menu();
void login();
void create_account();
int validate_email();

int main() {
    premenu();
    
    struct Account a;
    menu();

    return 0;
}

void premenu() {
    int choice;

    printf("\n\nSelection option: Login (1) Create Account (2):\n");
    scanf("%d", &choice);
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

void login() {
    int account_number;
    char password[20];

    printf("\nEnter details below:\nAccount number: ");
    scanf("%d", &account_number);
    printf("Password: ");
    scanf("%s", &password);
};

void create_account() {
    #define MAX_LENGTH 256
    char* email = malloc(MAX_LENGTH);
    char user[10];
    char domain[4];
    int valid;

    //FILE *fptr;
    
    /* 
        - Need to fix email sscanf(). Function validate_email(email) is running as expected but is validating
        slightly wrong emails such as 'jasdf@asdf' but will correctly find emails like 'sdf'
        - Need to implement other account creation detail features such as collecting first/last name
        password and generate 8 digit long account number
        - Then need to store this information into Account-Details.txt by writing it to it
    */

    // Get valid details First Name, Last Name, email, Password, Auto generate an 8 digit long account number
    printf("\nAccount Creation:\nEnter your new email: ");
    scanf("%s", email);
    valid = validate_email(email);
    while (valid != 2) {\
        printf("\n################ INVALID INPUT ################\nEnter different email: ");
        scanf("%s", email);
        valid = validate_email(email);
    }

    //fptr = fopen("Account-Details.txt", "w"); 
    
    printf("Account successfully created");
};

int validate_email(char* email) {
    char user[10];
    char domain[4];
    return sscanf(email, "%[_a-zA-Z0-9.]@%[_a-zA-Z0-9.]", user, domain);
};

void menu() {
    printf("\nSuccessfully logged in");
};