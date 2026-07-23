#include <stdio.h>
#include <string.h>

struct Account
{
    int accNo;
    char name[50];
    float balance;
};

int main()
{
    struct Account acc;
    int choice;
    float amount;
    int created = 0;

    while (1)
    {
        printf("\n===== BANKING SYSTEM =====\n");
        printf("1. Create Account\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Check Balance\n");
        printf("5. Display Account Details\n");
        printf("6. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("\nEnter Account Number: ");
            scanf("%d", &acc.accNo);

            printf("Enter Account Holder Name: ");
            scanf(" %[^\n]", acc.name);

            printf("Enter Initial Balance: ");
            scanf("%f", &acc.balance);

            created = 1;

            printf("\nAccount Created Successfully!\n");
            break;

        case 2:
            if (!created)
            {
                printf("\nCreate an account first!\n");
                break;
            }

            printf("Enter Amount to Deposit: ");
            scanf("%f", &amount);

            acc.balance += amount;

            printf("Deposit Successful!\n");
            break;

        case 3:
            if (!created)
            {
                printf("\nCreate an account first!\n");
                break;
            }

            printf("Enter Amount to Withdraw: ");
            scanf("%f", &amount);

            if (amount <= acc.balance)
            {
                acc.balance -= amount;
                printf("Withdrawal Successful!\n");
            }
            else
            {
                printf("Insufficient Balance!\n");
            }
            break;

        case 4:
            if (!created)
            {
                printf("\nCreate an account first!\n");
                break;
            }

            printf("\nCurrent Balance: %.2f\n", acc.balance);
            break;

        case 5:
            if (!created)
            {
                printf("\nCreate an account first!\n");
                break;
            }

            printf("\n----- Account Details -----\n");
            printf("Account Number : %d\n", acc.accNo);
            printf("Account Holder : %s\n", acc.name);
            printf("Balance         : %.2f\n", acc.balance);
            break;

        case 6:
            printf("\nThank You for Using Banking System!\n");
            return 0;

        default:
            printf("\nInvalid Choice!\n");
        }
    }

    return 0;
}