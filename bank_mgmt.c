#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---------------------------
// Structures
// ---------------------------
typedef struct Transaction {
    char detail[50];
    struct Transaction *next;
} Transaction;

typedef struct Account {
    int acc_no;
    char name[50];
    float balance;
    Transaction *transactions; // Queue of transactions
    struct Account *next;
} Account;

typedef struct DeletedStack {
    Account *account;
    struct DeletedStack *next;
} DeletedStack;

// ---------------------------
// Global variables
// ---------------------------
Account *head = NULL;          // Linked list head
DeletedStack *deletedTop = NULL; // Stack top for deleted accounts

// ---------------------------
// Queue Operations (Transactions)
// ---------------------------
void enqueue(Transaction **front, const char *detail) {
    Transaction *newT = (Transaction *)malloc(sizeof(Transaction));
    strcpy(newT->detail, detail);
    newT->next = NULL;

    if (*front == NULL) {
        *front = newT;
    } else {
        Transaction *temp = *front;
        while (temp->next) temp = temp->next;
        temp->next = newT;
    }
}

void displayTransactions(Transaction *front) {
    if (front == NULL) {
        printf("No transactions.\n");
        return;
    }
    printf("Transaction History:\n");
    while (front) {
        printf(" - %s\n", front->detail);
        front = front->next;
    }
}

// ---------------------------
// Stack Operations (Deleted Accounts)
// ---------------------------
void pushDeleted(Account *acc) {
    DeletedStack *newNode = (DeletedStack *)malloc(sizeof(DeletedStack));
    newNode->account = acc;
    newNode->next = deletedTop;
    deletedTop = newNode;
}

Account* popDeleted() {
    if (deletedTop == NULL) return NULL;
    DeletedStack *temp = deletedTop;
    Account *acc = temp->account;
    deletedTop = deletedTop->next;
    free(temp);
    return acc;
}

// ---------------------------
// Linked List Operations (Accounts)
// ---------------------------
void createAccount(int acc_no, char name[], float balance) {
    Account *newAcc = (Account *)malloc(sizeof(Account));
    newAcc->acc_no = acc_no;
    strcpy(newAcc->name, name);
    newAcc->balance = balance;
    newAcc->transactions = NULL;
    newAcc->next = NULL;

    if (head == NULL) {
        head = newAcc;
    } else {
        Account *temp = head;
        while (temp->next) temp = temp->next;
        temp->next = newAcc;
    }
    printf("✅ Account created: %s (A/C No: %d)\n", name, acc_no);
}

Account* searchAccount(int acc_no) {
    Account *temp = head;
    while (temp) {
        if (temp->acc_no == acc_no) return temp;
        temp = temp->next;
    }
    return NULL;
}

void displayAccounts() {
    if (head == NULL) {
        printf("No accounts found.\n");
        return;
    }
    Account *temp = head;
    printf("\n--- Accounts List ---\n");
    while (temp) {
        printf("A/C No: %d, Name: %s, Balance: %.2f\n", temp->acc_no, temp->name, temp->balance);
        temp = temp->next;
    }
}

void deposit(int acc_no, float amount) {
    Account *acc = searchAccount(acc_no);
    if (acc) {
        acc->balance += amount;
        char detail[50];
        sprintf(detail, "Deposited %.2f", amount);
        enqueue(&acc->transactions, detail);
        printf("💰 %.2f deposited. New Balance: %.2f\n", amount, acc->balance);
    } else {
        printf("Account not found.\n");
    }
}

void withdraw(int acc_no, float amount) {
    Account *acc = searchAccount(acc_no);
    if (acc) {
        if (acc->balance >= amount) {
            acc->balance -= amount;
            char detail[50];
            sprintf(detail, "Withdrew %.2f", amount);
            enqueue(&acc->transactions, detail);
            printf("💸 %.2f withdrawn. New Balance: %.2f\n", amount, acc->balance);
        } else {
            printf("Insufficient balance.\n");
        }
    } else {
        printf("Account not found.\n");
    }
}

void deleteAccount(int acc_no) {
    Account *temp = head, *prev = NULL;
    while (temp) {
        if (temp->acc_no == acc_no) {
            if (prev) prev->next = temp->next;
            else head = temp->next;

            pushDeleted(temp);
            printf("🗑️ Account %d deleted.\n", acc_no);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    printf("Account not found.\n");
}

void restoreAccount() {
    Account *acc = popDeleted();
    if (acc) {
        acc->next = head;
        head = acc;
        printf("♻️ Account %d restored.\n", acc->acc_no);
    } else {
        printf("No account to restore.\n");
    }
}

void showTransactions(int acc_no) {
    Account *acc = searchAccount(acc_no);
    if (acc) {
        displayTransactions(acc->transactions);
    } else {
        printf("Account not found.\n");
    }
}

// ---------------------------
// Main Menu
// ---------------------------
int main() {
    int choice, acc_no;
    char name[50];
    float amount, balance;

    while (1) {
        printf("\n--- Bank Menu ---\n");
        printf("1. Create Account\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Delete Account\n");
        printf("5. Restore Account\n");
        printf("6. Show Accounts\n");
        printf("7. Show Transactions\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter Account No: ");
                scanf("%d", &acc_no);
                printf("Enter Name: ");
                scanf("%s", name);
                printf("Enter Initial Balance: ");
                scanf("%f", &balance);
                createAccount(acc_no, name, balance);
                break;
            case 2:
                printf("Enter Account No: ");
                scanf("%d", &acc_no);
                printf("Enter Deposit Amount: ");
                scanf("%f", &amount);
                deposit(acc_no, amount);
                break;
            case 3:
                printf("Enter Account No: ");
                scanf("%d", &acc_no);
                printf("Enter Withdrawal Amount: ");
                scanf("%f", &amount);
                withdraw(acc_no, amount);
                break;
            case 4:
                printf("Enter Account No to Delete: ");
                scanf("%d", &acc_no);
                deleteAccount(acc_no);
                break;
            case 5:
                restoreAccount();
                break;
            case 6:
                displayAccounts();
                break;
            case 7:
                printf("Enter Account No: ");
                scanf("%d", &acc_no);
                showTransactions(acc_no);
                break;
            case 0:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}