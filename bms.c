#include <stdio.h>

struct Account {
    char name[40];
    char password[15];
    int account_number;
    int balance;
    int deposit_amount;
};

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
    }
};

void login() {
    int account_number;
    char password[20];
    printf("\nEnter details below:\nAccount number: ");
    scanf("%d", &account_number);
    printf("\nPassword: ");
    scanf("%s", &password);
};

void create_account() {

};

void menu() {
    printf("Successfully logged in");
};

int main() {
    premenu();
    
    struct Account a;
    menu();

    return 0;
}