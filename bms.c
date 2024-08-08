#include <stdio.h>
#include <stdlib.h>
#define MAX_LENGTH 256

struct Account {
    char first_name[15];
    char last_name[15];
    char password[14];
    int account_number;

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

int main() {
    premenu();
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
    struct Account a;

    printf("\nEnter details below:\nAccount number: ");
    scanf("%d", &a.account_number);
    printf("Password: ");
    scanf("%s", &a.password);
};

/* 
    - Need to fix email sscanf(). Function validate_email(email) is running as expected but is validating
    slightly wrong emails such as 'jasdf@asdf' but will correctly find emails like 'sdf'
    - Need to fix account number generator to wider values
    - Then need to store this information into Account-Details.txt by writing it to it
*/
void create_account() {
    struct Account a;
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

    printf("Account successfully created");
};

int validate_email(char* email) {
    struct Account a;
    return sscanf(email, "%[_a-zA-Z0-9.]@%[_a-zA-Z0-9.]", a.user, a.domain);
};

// In the future need to add a check to see if account number already exists
int generate_account_number() {
    int lower, upper, number;
    lower = 10000000;
    upper = 100000000;
    number = (rand() % (upper - lower + 1)) + lower;
    return number;
}

void menu() {
    printf("\nSuccessfully logged in");
};