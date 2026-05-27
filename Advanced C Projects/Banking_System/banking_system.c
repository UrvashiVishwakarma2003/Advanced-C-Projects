/*
================================================================================
DESCRIPTION:
Simple banking system with basic features for managing customer accounts.

FEATURES:
1. Create New Account - Open new bank account with minimum ₹500
2. Deposit Money - Add money to existing account
3. Withdraw Money - Remove money from account (maintain ₹500 minimum)
4. Check Balance - View current balance of account
5. View All Accounts - Display all customers and their balances
6. Delete Account - Remove an existing account
7. Exit - Close the program

FILES CREATED:
- bank_data.txt : Stores all account information permanently
================================================================================
*/

#include <stdio.h>      // For printf(), scanf(), fgets() - input/output functions
#include <stdlib.h>     // For system(), exit() - system functions
#include <string.h>     // For strcspn(), strcpy() - string operations
#include <conio.h>      // For getch() - to wait for key press (Windows only)

/*
================================================================================
STRUCTURE: Account
PURPOSE: Stores information for a single bank account
================================================================================
*/

struct Account
{
    int accNo;          // Unique account number (1001, 1002, etc.)
    char name[50];      // Customer full name
    int balance;        // Current account balance in rupees
};

/*
================================================================================
GLOBAL VARIABLES
================================================================================
*/

struct Account accounts[100];   // Array to store up to 100 accounts
int totalAccounts = 0;          // Counter for number of accounts

/*
================================================================================
FUNCTION: saveToFile()
PURPOSE: Save all account data to "bank_data.txt" file
PARAMETERS: None
RETURNS: Nothing
================================================================================
*/

void saveToFile()
{
    FILE *file = fopen("bank_data.txt", "w");   // Open file in write mode
    
    if(file != NULL)                            // Check if file opened successfully
    {
        fprintf(file, "%d\n", totalAccounts);   // Save total account count
        
        for(int i = 0; i < totalAccounts; i++)  // Loop through all accounts
        {
            fprintf(file, "%d\n", accounts[i].accNo);     // Save account number
            fprintf(file, "%s\n", accounts[i].name);      // Save customer name
            fprintf(file, "%d\n", accounts[i].balance);   // Save balance
        }
        
        fclose(file);                           // Close the file
    }
}

/*
================================================================================
FUNCTION: loadFromFile()
PURPOSE: Load all account data from "bank_data.txt" file
PARAMETERS: None
RETURNS: Nothing
================================================================================
*/

void loadFromFile()
{
    FILE *file = fopen("bank_data.txt", "r");   // Open file in read mode
    
    if(file != NULL)                            // If file exists
    {
        fscanf(file, "%d", &totalAccounts);     // Read total account count
        
        for(int i = 0; i < totalAccounts; i++)  // Loop through all accounts
        {
            fscanf(file, "%d", &accounts[i].accNo);     // Read account number
            fscanf(file, " %[^\n]", accounts[i].name);  // Read name (with spaces)
            fscanf(file, "%d", &accounts[i].balance);   // Read balance
        }
        
        fclose(file);                           // Close the file
    }
}

/*
================================================================================
FUNCTION: createAccount()
PURPOSE: Create a new bank account
PARAMETERS: None
RETURNS: Nothing
================================================================================
*/

void createAccount()
{
    system("cls");                              // Clear the screen
    
    printf("\n========================================\n");
    printf("        CREATE NEW ACCOUNT\n");
    printf("========================================\n");
    
    if(totalAccounts >= 100)                    // Check if bank is full
    {
        printf("\nBank is full! Cannot create more accounts.\n");
        printf("\nPress any key...");
        getch();                                // Wait for key press
        return;
    }
    
    struct Account newAcc;                      // Temporary account variable
    
    // Generate account number (starts from 1001)
    newAcc.accNo = 1001 + totalAccounts;
    
    printf("\nAccount Number: %d\n", newAcc.accNo);
    
    printf("Enter Customer Name: ");
    fflush(stdin);                              // Clear input buffer
    fgets(newAcc.name, 50, stdin);              // Read name with spaces
    newAcc.name[strcspn(newAcc.name, "\n")] = 0; // Remove newline character
    
    printf("Enter Initial Deposit (Minimum Rs.500): ");
    scanf("%d", &newAcc.balance);
    
    if(newAcc.balance < 500)                    // Check minimum balance
    {
        printf("\nMinimum balance required is Rs.500!\n");
        printf("\nPress any key...");
        getch();
        return;
    }
    
    accounts[totalAccounts] = newAcc;           // Add to accounts array
    totalAccounts++;                            // Increase account count
    saveToFile();                               // Save to file
    
    printf("\n========================================\n");
    printf("     ACCOUNT CREATED SUCCESSFULLY!\n");
    printf("========================================\n");
    printf("Account Number: %d\n", newAcc.accNo);
    printf("Customer Name: %s\n", newAcc.name);
    printf("Balance: Rs.%d\n", newAcc.balance);
    
    printf("\nPress any key...");
    getch();
}

/*
================================================================================
FUNCTION: findAccount()
PURPOSE: Find account by account number
PARAMETERS: accNo - account number to search
RETURNS: Index of account in array, or -1 if not found
================================================================================
*/

int findAccount(int accNo)
{
    for(int i = 0; i < totalAccounts; i++)      // Loop through all accounts
    {
        if(accounts[i].accNo == accNo)          // If account number matches
        {
            return i;                           // Return index position
        }
    }
    return -1;                                  // Account not found
}

/*
================================================================================
FUNCTION: depositMoney()
PURPOSE: Deposit money into an account
PARAMETERS: None
RETURNS: Nothing
================================================================================
*/

void depositMoney()
{
    system("cls");
    
    printf("\n========================================\n");
    printf("           DEPOSIT MONEY\n");
    printf("========================================\n");
    
    int accNo, amount;
    
    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);
    
    int index = findAccount(accNo);             // Find account
    
    if(index == -1)                             // If account not found
    {
        printf("\nAccount not found!\n");
        printf("\nPress any key...");
        getch();
        return;
    }
    
    printf("Account Holder: %s\n", accounts[index].name);
    printf("Current Balance: Rs.%d\n", accounts[index].balance);
    
    printf("\nEnter Amount to Deposit: Rs.");
    scanf("%d", &amount);
    
    if(amount <= 0)                             // Check valid amount
    {
        printf("\nInvalid amount!\n");
        printf("\nPress any key...");
        getch();
        return;
    }
    
    accounts[index].balance += amount;          // Add money to balance
    saveToFile();                               // Save changes to file
    
    printf("\n========================================\n");
    printf("        DEPOSIT SUCCESSFUL!\n");
    printf("========================================\n");
    printf("Amount Deposited: Rs.%d\n", amount);
    printf("New Balance: Rs.%d\n", accounts[index].balance);
    
    printf("\nPress any key...");
    getch();
}

/*
================================================================================
FUNCTION: withdrawMoney()
PURPOSE: Withdraw money from an account
PARAMETERS: None
RETURNS: Nothing
================================================================================
*/

void withdrawMoney()
{
    system("cls");
    
    printf("\n========================================\n");
    printf("          WITHDRAW MONEY\n");
    printf("========================================\n");
    
    int accNo, amount;
    
    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);
    
    int index = findAccount(accNo);             // Find account
    
    if(index == -1)                             // If account not found
    {
        printf("\nAccount not found!\n");
        printf("\nPress any key...");
        getch();
        return;
    }
    
    printf("Account Holder: %s\n", accounts[index].name);
    printf("Current Balance: Rs.%d\n", accounts[index].balance);
    
    printf("\nEnter Amount to Withdraw: Rs.");
    scanf("%d", &amount);
    
    if(amount <= 0)                             // Check valid amount
    {
        printf("\nInvalid amount!\n");
        printf("\nPress any key...");
        getch();
        return;
    }
    
    // Check if minimum balance of ₹500 remains after withdrawal
    if(amount > accounts[index].balance - 500)
    {
        printf("\nInsufficient balance! Minimum Rs.500 required.\n");
        printf("\nPress any key...");
        getch();
        return;
    }
    
    accounts[index].balance -= amount;          // Subtract money from balance
    saveToFile();                               // Save changes to file
    
    printf("\n========================================\n");
    printf("        WITHDRAWAL SUCCESSFUL!\n");
    printf("========================================\n");
    printf("Amount Withdrawn: Rs.%d\n", amount);
    printf("New Balance: Rs.%d\n", accounts[index].balance);
    
    printf("\nPress any key...");
    getch();
}

/*
================================================================================
FUNCTION: checkBalance()
PURPOSE: Display current balance of an account
PARAMETERS: None
RETURNS: Nothing
================================================================================
*/

void checkBalance()
{
    system("cls");
    
    printf("\n========================================\n");
    printf("          CHECK BALANCE\n");
    printf("========================================\n");
    
    int accNo;
    
    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);
    
    int index = findAccount(accNo);             // Find account
    
    if(index == -1)                             // If account not found
    {
        printf("\nAccount not found!\n");
        printf("\nPress any key...");
        getch();
        return;
    }
    
    printf("\n========================================\n");
    printf("Account Number: %d\n", accounts[index].accNo);
    printf("Account Holder: %s\n", accounts[index].name);
    printf("Current Balance: Rs.%d\n", accounts[index].balance);
    printf("========================================\n");
    
    printf("\nPress any key...");
    getch();
}

/*
================================================================================
FUNCTION: viewAllAccounts()
PURPOSE: Display all accounts in the bank
PARAMETERS: None
RETURNS: Nothing
================================================================================
*/

void viewAllAccounts()
{
    system("cls");
    
    printf("\n========================================\n");
    printf("          ALL ACCOUNTS\n");
    printf("========================================\n");
    
    if(totalAccounts == 0)                      // If no accounts exist
    {
        printf("\nNo accounts found!\n");
    }
    else
    {
        printf("\n========================================\n");
        printf("Acc No | Name                | Balance\n");
        printf("========================================\n");
        
        for(int i = 0; i < totalAccounts; i++)  // Loop through all accounts
        {
            printf(" %5d | %-19s | Rs.%d\n", 
                   accounts[i].accNo, 
                   accounts[i].name, 
                   accounts[i].balance);
        }
        printf("========================================\n");
        printf("Total Accounts: %d\n", totalAccounts);
    }
    
    printf("\nPress any key...");
    getch();
}

/*
================================================================================
FUNCTION: deleteAccount()
PURPOSE: Delete an existing account
PARAMETERS: None
RETURNS: Nothing
================================================================================
*/

void deleteAccount()
{
    system("cls");
    
    printf("\n========================================\n");
    printf("          DELETE ACCOUNT\n");
    printf("========================================\n");
    
    int accNo;
    
    printf("\nEnter Account Number to Delete: ");
    scanf("%d", &accNo);
    
    int index = findAccount(accNo);             // Find account
    
    if(index == -1)                             // If account not found
    {
        printf("\nAccount not found!\n");
        printf("\nPress any key...");
        getch();
        return;
    }
    
    printf("\nAccount Found:\n");
    printf("Account Holder: %s\n", accounts[index].name);
    printf("Balance: Rs.%d\n", accounts[index].balance);
    
    char confirm;
    printf("\nAre you sure? (Y/N): ");
    scanf(" %c", &confirm);
    
    if(confirm == 'Y' || confirm == 'y')
    {
        // Shift all accounts after this one to fill the gap
        for(int i = index; i < totalAccounts - 1; i++)
        {
            accounts[i] = accounts[i + 1];
        }
        totalAccounts--;                        // Decrease account count
        saveToFile();                           // Save changes to file
        
        printf("\n========================================\n");
        printf("        ACCOUNT DELETED SUCCESSFULLY!\n");
        printf("========================================\n");
    }
    else
    {
        printf("\nDeletion cancelled.\n");
    }
    
    printf("\nPress any key...");
    getch();
}

/*
================================================================================
FUNCTION: main()
PURPOSE: Main program entry point - displays menu and handles user choice
PARAMETERS: None
RETURNS: 0 on successful execution
================================================================================
*/

int main()
{
    int choice;                                 // Variable for menu choice
    
    loadFromFile();                             // Load existing accounts from file
    
    do
    {
        system("cls");                          // Clear screen
        
        printf("\n========================================\n");
        printf("     SIMPLE BANK MANAGEMENT SYSTEM\n");
        printf("========================================\n");
        printf("\n");
        printf(" 1. Create New Account\n");
        printf(" 2. Deposit Money\n");
        printf(" 3. Withdraw Money\n");
        printf(" 4. Check Balance\n");
        printf(" 5. View All Accounts\n");
        printf(" 6. Delete Account\n");
        printf(" 7. Exit\n");
        printf("\n");
        printf("========================================\n");
        printf(" Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice)                          // Execute based on user choice
        {
            case 1: createAccount(); break;
            case 2: depositMoney(); break;
            case 3: withdrawMoney(); break;
            case 4: checkBalance(); break;
            case 5: viewAllAccounts(); break;
            case 6: deleteAccount(); break;
            case 7:
                printf("\nThank you for using Bank System!\n");
                break;
            default:
                printf("\nInvalid choice! Try again.\n");
                printf("\nPress any key...");
                getch();
        }
        
    } while(choice != 7);                       // Loop until user chooses exit
    
    return 0;                                   // Program ends successfully
}