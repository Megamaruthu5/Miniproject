#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int account_no;
    char first_name[25];
    char last_name[25];
    float balance;
} Customer;

void new_customer();
void display_customers();
void update_customer();
void deposit_amount();
void withdraw_amount();

int main() {
    int choice;
    while (1) {
        printf("\n====================================\n");
        printf("1. Add New Customer\n");
        printf("2. Display Customer Details\n");
        printf("3. Update Customer Name\n");
        printf("4. Deposit\n");
        printf("5. Withdraw\n");
        printf("6. Exit\n");
        printf("====================================\n");
        printf("Select option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: new_customer(); break;
            case 2: display_customers(); break;
            case 3: update_customer(); break;
            case 4: deposit_amount(); break;
            case 5: withdraw_amount(); break;
            case 6: exit(0);
            default: printf("Invalid option.\n");
        }
    }
    return 0;
}

void new_customer() {
    FILE *fb = fopen("customer.dat", "ab+");
    FILE *ftxt = fopen("customer.txt", "a");
    Customer c;
    int exists = 0, acc_no;

    if (!fb || !ftxt) {
        printf("File access error.\n");
        return;
    }

    printf("Enter Account Number: ");
    scanf("%d", &acc_no);

    rewind(fb);
    while (fread(&c, sizeof(Customer), 1, fb)) {
        if (c.account_no == acc_no) {
            exists = 1;
            break;
        }
    }

    if (exists) {
        printf("Account number already exists.\n");
        fclose(fb); fclose(ftxt);
        return;
    }

    c.account_no = acc_no;
    printf("Enter First Name: ");
    scanf("%s", c.first_name);
    printf("Enter Last Name: ");
    scanf("%s", c.last_name);
    printf("Enter Balance: ");
    scanf("%f", &c.balance);

    fwrite(&c, sizeof(Customer), 1, fb);
    fprintf(ftxt, "%d %s %s %.2f\n", c.account_no, c.first_name, c.last_name, c.balance);
    printf("Customer added.\n");

    fclose(fb);
    fclose(ftxt);
}

void display_customers() {
    FILE *fb = fopen("customer.dat", "rb");
    FILE *ftxt = fopen("customer.txt", "w");
    Customer c;

    if (!fb || !ftxt) {
        printf("File access error.\n");
        return;
    }

    printf("\nCustomer Records:\n");
    printf("-------------------------------------\n");

    while (fread(&c, sizeof(Customer), 1, fb)) {
        printf("Account: %d\nName: %s %s\nBalance: %.2f\n", c.account_no, c.first_name, c.last_name, c.balance);
        printf("-------------------------------------\n");
        fprintf(ftxt, "%d %s %s %.2f\n", c.account_no, c.first_name, c.last_name, c.balance);
    }

    fclose(fb);
    fclose(ftxt);
}

void update_customer() {
    FILE *fb = fopen("customer.dat", "rb+");
    Customer c;
    int acc_no, found = 0;

    if (!fb) {
        printf("File access error.\n");
        return;
    }

    printf("Enter Account Number: ");
    scanf("%d", &acc_no);

    while (fread(&c, sizeof(Customer), 1, fb)) {
        if (c.account_no == acc_no) {
            found = 1;
            printf("Current Name: %s %s\n", c.first_name, c.last_name);
            printf("New First Name: ");
            scanf("%s", c.first_name);
            printf("New Last Name: ");
            scanf("%s", c.last_name);

            fseek(fb, -(long)sizeof(Customer), SEEK_CUR);
            fwrite(&c, sizeof(Customer), 1, fb);
            printf("Customer updated.\n");
            break;
        }
    }

    fclose(fb);

    if (!found) {
        printf("Account not found.\n");
    } else {
        display_customers();
    }
}

void deposit_amount() {
    FILE *fb = fopen("customer.dat", "rb+");
    Customer c;
    int acc_no, found = 0;
    float amount;

    if (!fb) {
        printf("File access error.\n");
        return;
    }

    printf("Enter Account Number: ");
    scanf("%d", &acc_no);

    while (fread(&c, sizeof(Customer), 1, fb)) {
        if (c.account_no == acc_no) {
            found = 1;
            printf("Current Balance: %.2f\n", c.balance);
            printf("Deposit Amount: ");
            scanf("%f", &amount);

            if (amount <= 0) {
                printf("Invalid amount.\n");
                fclose(fb);
                return;
            }

            c.balance += amount;
            fseek(fb, -(long)sizeof(Customer), SEEK_CUR);
            fwrite(&c, sizeof(Customer), 1, fb);
            printf("Deposit complete. New Balance: %.2f\n", c.balance);
            break;
        }
    }

    fclose(fb);
    if (!found) printf("Account not found.\n");
}

void withdraw_amount() {
    FILE *fb = fopen("customer.dat", "rb+");
    Customer c;
    int acc_no, found = 0;
    float amount;

    if (!fb) {
        printf("File access error.\n");
        return;
    }

    printf("Enter Account Number: ");
    scanf("%d", &acc_no);

    while (fread(&c, sizeof(Customer), 1, fb)) {
        if (c.account_no == acc_no) {
            found = 1;
            printf("Current Balance: %.2f\n", c.balance);
            printf("Withdraw Amount: ");
            scanf("%f", &amount);

            if (amount <= 0 || amount > c.balance) {
                printf("Invalid amount.\n");
                fclose(fb);
                return;
            }

            c.balance -= amount;
            fseek(fb, -(long)sizeof(Customer), SEEK_CUR);
            fwrite(&c, sizeof(Customer), 1, fb);
            printf("Withdrawal complete. New Balance: %.2f\n", c.balance);
            break;
        }
    }

    fclose(fb);
    if (!found) printf("Account not found.\n");
}
