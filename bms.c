#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_LENGTH 256

struct Account {
    char first_name[15];
    char last_name[15];
    char username[30];
    char password[14];
    char email[40];
    int account_number;
    int account_balance;
};

void premenu();
void menu();
void login();
bool read_account_from_file(FILE* file, struct Account* account);
bool validate_login(struct Account* account, char* input_username, char* input_password);
void create_account();
bool validate_email(const char* email);
int generate_unique_account_number(FILE* file);
bool check_duplicate(FILE* file, int account_number);
void write_account_to_file(FILE* file, struct Account* account);
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

    fflush(stdin);

    printf("\nLogin below:\n");
    printf("Username: ");
    if (fgets(input_username, sizeof(input_username), stdin) == NULL) {
        printf("Error reading username.\n");
        fclose(file);
    }
    input_username[strcspn(input_username, "\n")] = 0;

    printf("Password: ");
    fgets(input_password, 14, stdin);
    input_password[strcspn(input_password, "\n")] = 0;

    bool login_success = false;
    while (read_account_from_file(file, &account)) {
        if (validate_login(&account, input_username, input_password)) {
            printf("Login successful!\n");
            login_success = true;
            break;
        }
    }

    if (!login_success) {
        printf("Invalid username or password.\n");
    }

    fclose(file);
};

bool read_account_from_file(FILE* file, struct Account* account) {
    return fread(account, sizeof(struct Account), 1, file) == 1;
}

bool validate_login(struct Account* account, char* input_username, char* input_password) {
    return (
        strcmp(account->username, input_username) == 0 &&
        strcmp(account->password, input_password) == 0
    );
}

/* 
    - Need to fix email sscanf(). Function validate_email(email) is running as expected but is validating
    slightly wrong emails such as 'jasdf@asdf' but will correctly find emails like 'sdf'
*/
void create_account() {
    struct Account new_account;
    FILE* accounts_file;

    printf("\nAccount Creation:\n");

    printf("Enter your first name: ");
    scanf("%s", new_account.first_name);
    flush_input();

    printf("\nEnter your last name: ");
    scanf("%s", new_account.last_name);
    flush_input();

    printf("\nEnter a username: ");
    scanf("%s", new_account.username);
    flush_input();

    bool valid_email = false;
    while (!valid_email) {
        printf("Enter your email: ");
        scanf("%s", new_account.email);
        flush_input();

        if (validate_email(new_account.email)) {
            valid_email = true;
        } else {
            printf("################ INVALID INPUT ################\n");
        }
    }

    accounts_file = fopen("Account-Details.txt", "a+");
    if (accounts_file == NULL) {
        fprintf(stderr, "Error... Could not open Account-Details.txt.\n");
        exit(1);
    }

    printf("\nEnter a new password (14 Characters Max): ");
    scanf("%14s", new_account.password);
    flush_input();

    new_account.account_number = generate_unique_account_number(accounts_file);
    new_account.account_balance = 0;

    printf("\nAccount successfully created. Your account number is %d\n", new_account.account_number);

    write_account_to_file(accounts_file, &new_account);

    fclose(accounts_file);
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

int generate_unique_account_number(FILE* file) {
    int lower = 10000000, upper = 100000000;
    int account_number;

    do {
        account_number = (rand() % (upper - lower)) + lower;
    } while (check_duplicate(file, account_number));

    return account_number;
}

bool check_duplicate(FILE* file, int account_number) {
    struct Account existing_account;
    rewind(file);

    while (fread(&existing_account, sizeof(struct Account), 1, file)) {
        if (existing_account.account_number == account_number) {
            return true;
        }
    }

    return false;
}

void write_account_to_file(FILE* file, struct Account* account) {
    fprintf(file, "Account Number:\n%d\nFirst Name:\n%s\nLast Name:\n%s\nUsername:\n%s\nPassword:\n%s\nEmail:\n%s\nAccount Balance:\n%d\n\n",
        account->account_number,
        account->first_name,
        account->last_name,
        account->username,
        account->password,
        account->email,
        account->account_balance
    );
}

void flush_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void menu() {
    int choice;

    printf("\nMenu:\n");
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
            menu();
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