/*
================================================================================
PROJECT: LIBRARY MANAGEMENT SYSTEM
LANGUAGE: C PROGRAMMING
================================================================================

DESCRIPTION:
Complete library system for managing books, members, and book issuing/returning.

FEATURES:
- Add new books
- Add new members
- Issue book to member
- Return book
- View all books
- View all members
- View issued books
- Search book by ID or name
- Search member by ID or name
- Calculate fine for late return
- Save data to files

================================================================================
*/

#include <stdio.h>      // For printf, scanf - input/output
#include <stdlib.h>     // For system, exit - system functions
#include <string.h>     // For strcpy, strcmp - string operations
#include <conio.h>      // For getch - keyboard input
#include <time.h>       // For time - date functions

/*
================================================================================
STRUCTURES
================================================================================
*/

// Structure for date
struct Date
{
    int day;
    int month;
    int year;
};

// Structure for book
struct Book
{
    int id;                 // Book ID
    char title[100];        // Book title
    char author[50];        // Author name
    char publisher[50];     // Publisher name
    int quantity;           // Total quantity
    int available;          // Available copies
    int issued;             // Issued copies
};

// Structure for member
struct Member
{
    int id;                 // Member ID
    char name[50];          // Member name
    char phone[12];         // Phone number
    char email[50];         // Email address
    char address[100];      // Address
    int booksIssued;        // Number of books currently issued
};

// Structure for transaction (issue/return)
struct Transaction
{
    int id;                 // Transaction ID
    int bookId;             // Book ID
    int memberId;           // Member ID
    struct Date issueDate;  // Date when book was issued
    struct Date dueDate;    // Due date (usually 14 days after issue)
    struct Date returnDate; // Date when book was returned
    int status;             // 1=Issued, 2=Returned
    int fine;               // Fine amount if late
};

/*
================================================================================
GLOBAL VARIABLES
================================================================================
*/

struct Book books[200];             // Array to store books
int bookCount = 0;                  // Total books
struct Member members[300];         // Array to store members
int memberCount = 0;                // Total members
struct Transaction transactions[1000]; // Array to store transactions
int transactionCount = 0;           // Total transactions
int nextBookId = 1001;              // Next book ID
int nextMemberId = 2001;            // Next member ID
int nextTransactionId = 3001;       // Next transaction ID

/*
================================================================================
FUNCTION: getCurrentDate()
PURPOSE: Get current date
================================================================================
*/

void getCurrentDate(struct Date *date)
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    date->day = tm->tm_mday;
    date->month = tm->tm_mon + 1;
    date->year = tm->tm_year + 1900;
}

/*
================================================================================
FUNCTION: calculateDueDate()
PURPOSE: Calculate due date (14 days after issue date)
================================================================================
*/

void calculateDueDate(struct Date issueDate, struct Date *dueDate)
{
    dueDate->day = issueDate.day + 14;
    dueDate->month = issueDate.month;
    dueDate->year = issueDate.year;
    
    if(dueDate->day > 30)
    {
        dueDate->day = dueDate->day - 30;
        dueDate->month++;
    }
    
    if(dueDate->month > 12)
    {
        dueDate->month = 1;
        dueDate->year++;
    }
}

/*
================================================================================
FUNCTION: calculateFine()
PURPOSE: Calculate fine for late return (₹5 per day)
================================================================================
*/

int calculateFine(struct Date dueDate, struct Date returnDate)
{
    int fine = 0;
    
    // Convert dates to days for simple calculation
    int dueDays = dueDate.year * 365 + dueDate.month * 30 + dueDate.day;
    int returnDays = returnDate.year * 365 + returnDate.month * 30 + returnDate.day;
    
    int lateDays = returnDays - dueDays;
    
    if(lateDays > 0)
    {
        fine = lateDays * 5;  // ₹5 per day fine
    }
    
    return fine;
}

/*
================================================================================
FUNCTION: saveDataToFile()
PURPOSE: Save all data to files
================================================================================
*/

void saveDataToFile()
{
    // Save books
    FILE *file = fopen("books.dat", "wb");
    if(file != NULL)
    {
        fwrite(&bookCount, sizeof(int), 1, file);
        fwrite(books, sizeof(struct Book), bookCount, file);
        fwrite(&nextBookId, sizeof(int), 1, file);
        fclose(file);
    }
    
    // Save members
    file = fopen("members.dat", "wb");
    if(file != NULL)
    {
        fwrite(&memberCount, sizeof(int), 1, file);
        fwrite(members, sizeof(struct Member), memberCount, file);
        fwrite(&nextMemberId, sizeof(int), 1, file);
        fclose(file);
    }
    
    // Save transactions
    file = fopen("transactions.dat", "wb");
    if(file != NULL)
    {
        fwrite(&transactionCount, sizeof(int), 1, file);
        fwrite(transactions, sizeof(struct Transaction), transactionCount, file);
        fwrite(&nextTransactionId, sizeof(int), 1, file);
        fclose(file);
    }
}

/*
================================================================================
FUNCTION: loadDataFromFile()
PURPOSE: Load all data from files
================================================================================
*/

void loadDataFromFile()
{
    // Load books
    FILE *file = fopen("books.dat", "rb");
    if(file != NULL)
    {
        fread(&bookCount, sizeof(int), 1, file);
        fread(books, sizeof(struct Book), bookCount, file);
        fread(&nextBookId, sizeof(int), 1, file);
        fclose(file);
    }
    
    // Load members
    file = fopen("members.dat", "rb");
    if(file != NULL)
    {
        fread(&memberCount, sizeof(int), 1, file);
        fread(members, sizeof(struct Member), memberCount, file);
        fread(&nextMemberId, sizeof(int), 1, file);
        fclose(file);
    }
    
    // Load transactions
    file = fopen("transactions.dat", "rb");
    if(file != NULL)
    {
        fread(&transactionCount, sizeof(int), 1, file);
        fread(transactions, sizeof(struct Transaction), transactionCount, file);
        fread(&nextTransactionId, sizeof(int), 1, file);
        fclose(file);
    }
}

/*
================================================================================
FUNCTION: addBook()
PURPOSE: Add new book to library
================================================================================
*/

void addBook()
{
    system("cls");
    printf("\n========================================\n");
    printf("           ADD NEW BOOK\n");
    printf("========================================\n");
    
    if(bookCount >= 200)
    {
        printf("\nCannot add more books! Limit reached.\n");
        printf("\nPress any key...");
        getch();
        return;
    }
    
    struct Book newBook;
    
    newBook.id = nextBookId++;
    
    printf("\nBook ID: %d\n", newBook.id);
    printf("Enter Book Title: ");
    fflush(stdin);
    fgets(newBook.title, 100, stdin);
    newBook.title[strcspn(newBook.title, "\n")] = 0;
    
    printf("Enter Author Name: ");
    fgets(newBook.author, 50, stdin);
    newBook.author[strcspn(newBook.author, "\n")] = 0;
    
    printf("Enter Publisher Name: ");
    fgets(newBook.publisher, 50, stdin);
    newBook.publisher[strcspn(newBook.publisher, "\n")] = 0;
    
    printf("Enter Quantity: ");
    scanf("%d", &newBook.quantity);
    
    newBook.available = newBook.quantity;
    newBook.issued = 0;
    
    books[bookCount] = newBook;
    bookCount++;
    saveDataToFile();
    
    printf("\n========================================\n");
    printf("        BOOK ADDED SUCCESSFULLY!\n");
    printf("========================================\n");
    printf("Book ID: %d\n", newBook.id);
    printf("Title: %s\n", newBook.title);
    
    printf("\nPress any key...");
    getch();
}

/*
================================================================================
FUNCTION: viewAllBooks()
PURPOSE: Display all books
================================================================================
*/

void viewAllBooks()
{
    system("cls");
    printf("\n========================================\n");
    printf("           ALL BOOKS\n");
    printf("========================================\n");
    
    if(bookCount == 0)
    {
        printf("\nNo books found!\n");
    }
    else
    {
        printf("\n================================================================================\n");
        printf("ID     | Title                          | Author              | Available | Total\n");
        printf("================================================================================\n");
        
        for(int i = 0; i < bookCount; i++)
        {
            printf("%-6d | %-30s | %-19s | %-9d | %d\n", 
                   books[i].id,
                   books[i].title,
                   books[i].author,
                   books[i].available,
                   books[i].quantity);
        }
        printf("================================================================================\n");
        printf("Total Books: %d\n", bookCount);
    }
    
    printf("\nPress any key...");
    getch();
}

/*
================================================================================
FUNCTION: searchBook()
PURPOSE: Search book by ID or title
================================================================================
*/

void searchBook()
{
    system("cls");
    printf("\n========================================\n");
    printf("           SEARCH BOOK\n");
    printf("========================================\n");
    
    int choice;
    printf("\nSearch by:\n");
    printf("1. Book ID\n");
    printf("2. Book Title\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    
    if(choice == 1)
    {
        int id;
        printf("\nEnter Book ID: ");
        scanf("%d", &id);
        
        int found = 0;
        for(int i = 0; i < bookCount; i++)
        {
            if(books[i].id == id)
            {
                printf("\n========================================\n");
                printf("           BOOK DETAILS\n");
                printf("========================================\n");
                printf("Book ID: %d\n", books[i].id);
                printf("Title: %s\n", books[i].title);
                printf("Author: %s\n", books[i].author);
                printf("Publisher: %s\n", books[i].publisher);
                printf("Total Quantity: %d\n", books[i].quantity);
                printf("Available: %d\n", books[i].available);
                printf("Issued: %d\n", books[i].issued);
                printf("========================================\n");
                found = 1;
                break;
            }
        }
        
        if(!found)
        {
            printf("\nBook not found!\n");
        }
    }
    else if(choice == 2)
    {
        char title[100];
        printf("\nEnter Book Title: ");
        fflush(stdin);
        fgets(title, 100, stdin);
        title[strcspn(title, "\n")] = 0;
        
        int found = 0;
        printf("\n================================================================================\n");
        printf("ID     | Title                          | Author              | Available\n");
        printf("================================================================================\n");
        
        for(int i = 0; i < bookCount; i++)
        {
            if(strstr(books[i].title, title) != NULL)
            {
                printf("%-6d | %-30s | %-19s | %d\n", 
                       books[i].id,
                       books[i].title,
                       books[i].author,
                       books[i].available);
                found = 1;
            }
        }
        
        if(!found)
        {
            printf("\nNo book found with title: %s\n", title);
        }
        printf("================================================================================\n");
    }
    
    printf("\nPress any key...");
    getch();
}

/*
================================================================================
FUNCTION: addMember()
PURPOSE: Add new library member
================================================================================
*/

void addMember()
{
    system("cls");
    printf("\n========================================\n");
    printf("           ADD NEW MEMBER\n");
    printf("========================================\n");
    
    if(memberCount >= 300)
    {
        printf("\nCannot add more members! Limit reached.\n");
        printf("\nPress any key...");
        getch();
        return;
    }
    
    struct Member newMember;
    
    newMember.id = nextMemberId++;
    
    printf("\nMember ID: %d\n", newMember.id);
    printf("Enter Member Name: ");
    fflush(stdin);
    fgets(newMember.name, 50, stdin);
    newMember.name[strcspn(newMember.name, "\n")] = 0;
    
    printf("Enter Phone Number: ");
    scanf("%s", newMember.phone);
    
    printf("Enter Email: ");
    scanf("%s", newMember.email);
    
    printf("Enter Address: ");
    fflush(stdin);
    fgets(newMember.address, 100, stdin);
    newMember.address[strcspn(newMember.address, "\n")] = 0;
    
    newMember.booksIssued = 0;
    
    members[memberCount] = newMember;
    memberCount++;
    saveDataToFile();
    
    printf("\n========================================\n");
    printf("        MEMBER ADDED SUCCESSFULLY!\n");
    printf("========================================\n");
    printf("Member ID: %d\n", newMember.id);
    printf("Name: %s\n", newMember.name);
    
    printf("\nPress any key...");
    getch();
}

/*
================================================================================
FUNCTION: viewAllMembers()
PURPOSE: Display all members
================================================================================
*/

void viewAllMembers()
{
    system("cls");
    printf("\n========================================\n");
    printf("           ALL MEMBERS\n");
    printf("========================================\n");
    
    if(memberCount == 0)
    {
        printf("\nNo members found!\n");
    }
    else
    {
        printf("\n================================================================================\n");
        printf("ID     | Name                      | Phone      | Books Issued\n");
        printf("================================================================================\n");
        
        for(int i = 0; i < memberCount; i++)
        {
            printf("%-6d | %-25s | %-10s | %d\n", 
                   members[i].id,
                   members[i].name,
                   members[i].phone,
                   members[i].booksIssued);
        }
        printf("================================================================================\n");
        printf("Total Members: %d\n", memberCount);
    }
    
    printf("\nPress any key...");
    getch();
}

/*
================================================================================
FUNCTION: searchMember()
PURPOSE: Search member by ID or name
================================================================================
*/

void searchMember()
{
    system("cls");
    printf("\n========================================\n");
    printf("           SEARCH MEMBER\n");
    printf("========================================\n");
    
    int choice;
    printf("\nSearch by:\n");
    printf("1. Member ID\n");
    printf("2. Member Name\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    
    if(choice == 1)
    {
        int id;
        printf("\nEnter Member ID: ");
        scanf("%d", &id);
        
        int found = 0;
        for(int i = 0; i < memberCount; i++)
        {
            if(members[i].id == id)
            {
                printf("\n========================================\n");
                printf("           MEMBER DETAILS\n");
                printf("========================================\n");
                printf("Member ID: %d\n", members[i].id);
                printf("Name: %s\n", members[i].name);
                printf("Phone: %s\n", members[i].phone);
                printf("Email: %s\n", members[i].email);
                printf("Address: %s\n", members[i].address);
                printf("Books Issued: %d\n", members[i].booksIssued);
                printf("========================================\n");
                found = 1;
                break;
            }
        }
        
        if(!found)
        {
            printf("\nMember not found!\n");
        }
    }
    else if(choice == 2)
    {
        char name[50];
        printf("\nEnter Member Name: ");
        fflush(stdin);
        fgets(name, 50, stdin);
        name[strcspn(name, "\n")] = 0;
        
        int found = 0;
        printf("\n================================================================================\n");
        printf("ID     | Name                      | Phone      | Books Issued\n");
        printf("================================================================================\n");
        
        for(int i = 0; i < memberCount; i++)
        {
            if(strstr(members[i].name, name) != NULL)
            {
                printf("%-6d | %-25s | %-10s | %d\n", 
                       members[i].id,
                       members[i].name,
                       members[i].phone,
                       members[i].booksIssued);
                found = 1;
            }
        }
        
        if(!found)
        {
            printf("\nNo member found with name: %s\n", name);
        }
        printf("================================================================================\n");
    }
    
    printf("\nPress any key...");
    getch();
}

/*
================================================================================
FUNCTION: issueBook()
PURPOSE: Issue a book to a member
================================================================================
*/

void issueBook()
{
    system("cls");
    printf("\n========================================\n");
    printf("           ISSUE BOOK\n");
    printf("========================================\n");
    
    int bookId, memberId;
    
    printf("\nEnter Book ID: ");
    scanf("%d", &bookId);
    
    int bookIndex = -1;
    for(int i = 0; i < bookCount; i++)
    {
        if(books[i].id == bookId)
        {
            bookIndex = i;
            break;
        }
    }
    
    if(bookIndex == -1)
    {
        printf("\nBook not found!\n");
        printf("\nPress any key...");
        getch();
        return;
    }
    
    if(books[bookIndex].available <= 0)
    {
        printf("\nBook is not available! All copies are issued.\n");
        printf("\nPress any key...");
        getch();
        return;
    }
    
    printf("\nBook Title: %s\n", books[bookIndex].title);
    printf("Available Copies: %d\n", books[bookIndex].available);
    
    printf("\nEnter Member ID: ");
    scanf("%d", &memberId);
    
    int memberIndex = -1;
    for(int i = 0; i < memberCount; i++)
    {
        if(members[i].id == memberId)
        {
            memberIndex = i;
            break;
        }
    }
    
    if(memberIndex == -1)
    {
        printf("\nMember not found!\n");
        printf("\nPress any key...");
        getch();
        return;
    }
    
    printf("\nMember Name: %s\n", members[memberIndex].name);
    
    struct Transaction newTransaction;
    
    newTransaction.id = nextTransactionId++;
    newTransaction.bookId = bookId;
    newTransaction.memberId = memberId;
    
    getCurrentDate(&newTransaction.issueDate);
    calculateDueDate(newTransaction.issueDate, &newTransaction.dueDate);
    newTransaction.status = 1;
    newTransaction.fine = 0;
    
    // Update book availability
    books[bookIndex].available--;
    books[bookIndex].issued++;
    
    // Update member books issued count
    members[memberIndex].booksIssued++;
    
    transactions[transactionCount] = newTransaction;
    transactionCount++;
    saveDataToFile();
    
    printf("\n========================================\n");
    printf("        BOOK ISSUED SUCCESSFULLY!\n");
    printf("========================================\n");
    printf("Transaction ID: %d\n", newTransaction.id);
    printf("Issue Date: %02d/%02d/%d\n", newTransaction.issueDate.day,
           newTransaction.issueDate.month, newTransaction.issueDate.year);
    printf("Due Date: %02d/%02d/%d\n", newTransaction.dueDate.day,
           newTransaction.dueDate.month, newTransaction.dueDate.year);
    
    printf("\nPress any key...");
    getch();
}

/*
================================================================================
FUNCTION: returnBook()
PURPOSE: Return a book and calculate fine if late
================================================================================
*/

void returnBook()
{
    system("cls");
    printf("\n========================================\n");
    printf("           RETURN BOOK\n");
    printf("========================================\n");
    
    int transactionId;
    printf("\nEnter Transaction ID: ");
    scanf("%d", &transactionId);
    
    int transIndex = -1;
    for(int i = 0; i < transactionCount; i++)
    {
        if(transactions[i].id == transactionId && transactions[i].status == 1)
        {
            transIndex = i;
            break;
        }
    }
    
    if(transIndex == -1)
    {
        printf("\nTransaction not found or book already returned!\n");
        printf("\nPress any key...");
        getch();
        return;
    }
    
    // Get book and member details
    int bookId = transactions[transIndex].bookId;
    int memberId = transactions[transIndex].memberId;
    
    int bookIndex = -1, memberIndex = -1;
    
    for(int i = 0; i < bookCount; i++)
    {
        if(books[i].id == bookId)
        {
            bookIndex = i;
            break;
        }
    }
    
    for(int i = 0; i < memberCount; i++)
    {
        if(members[i].id == memberId)
        {
            memberIndex = i;
            break;
        }
    }
    
    printf("\nBook Title: %s\n", books[bookIndex].title);
    printf("Member Name: %s\n", members[memberIndex].name);
    printf("Issue Date: %02d/%02d/%d\n", transactions[transIndex].issueDate.day,
           transactions[transIndex].issueDate.month, transactions[transIndex].issueDate.year);
    printf("Due Date: %02d/%02d/%d\n", transactions[transIndex].dueDate.day,
           transactions[transIndex].dueDate.month, transactions[transIndex].dueDate.year);
    
    getCurrentDate(&transactions[transIndex].returnDate);
    
    // Calculate fine
    transactions[transIndex].fine = calculateFine(transactions[transIndex].dueDate, 
                                                   transactions[transIndex].returnDate);
    
    if(transactions[transIndex].fine > 0)
    {
        printf("\nLATE RETURN! Fine: ₹%d\n", transactions[transIndex].fine);
    }
    
    transactions[transIndex].status = 2;
    
    // Update book availability
    if(bookIndex != -1)
    {
        books[bookIndex].available++;
        books[bookIndex].issued--;
    }
    
    // Update member books issued count
    if(memberIndex != -1)
    {
        members[memberIndex].booksIssued--;
    }
    
    saveDataToFile();
    
    printf("\n========================================\n");
    printf("        BOOK RETURNED SUCCESSFULLY!\n");
    printf("========================================\n");
    printf("Return Date: %02d/%02d/%d\n", transactions[transIndex].returnDate.day,
           transactions[transIndex].returnDate.month, transactions[transIndex].returnDate.year);
    
    if(transactions[transIndex].fine > 0)
    {
        printf("Fine Amount: ₹%d\n", transactions[transIndex].fine);
    }
    
    printf("\nPress any key...");
    getch();
}

/*
================================================================================
FUNCTION: viewIssuedBooks()
PURPOSE: Display all currently issued books
================================================================================
*/

void viewIssuedBooks()
{
    system("cls");
    printf("\n========================================\n");
    printf("           ISSUED BOOKS\n");
    printf("========================================\n");
    
    int found = 0;
    printf("\n================================================================================\n");
    printf("Trans ID | Book Title                  | Member Name             | Due Date\n");
    printf("================================================================================\n");
    
    for(int i = 0; i < transactionCount; i++)
    {
        if(transactions[i].status == 1)
        {
            char bookTitle[100] = "Unknown";
            char memberName[50] = "Unknown";
            
            for(int j = 0; j < bookCount; j++)
            {
                if(books[j].id == transactions[i].bookId)
                {
                    strcpy(bookTitle, books[j].title);
                    break;
                }
            }
            
            for(int j = 0; j < memberCount; j++)
            {
                if(members[j].id == transactions[i].memberId)
                {
                    strcpy(memberName, members[j].name);
                    break;
                }
            }
            
            printf("%-8d | %-25s | %-23s | %02d/%02d/%d\n", 
                   transactions[i].id,
                   bookTitle,
                   memberName,
                   transactions[i].dueDate.day,
                   transactions[i].dueDate.month,
                   transactions[i].dueDate.year);
            found = 1;
        }
    }
    
    if(!found)
    {
        printf("\nNo books currently issued!\n");
    }
    printf("================================================================================\n");
    
    printf("\nPress any key...");
    getch();
}

/*
================================================================================
FUNCTION: mainMenu()
PURPOSE: Display main menu
================================================================================
*/

void mainMenu()
{
    int choice;
    
    do
    {
        system("cls");
        printf("\n========================================\n");
        printf("       LIBRARY MANAGEMENT SYSTEM\n");
        printf("========================================\n");
        printf("\n");
        printf(" BOOK MANAGEMENT\n");
        printf(" 1. Add New Book\n");
        printf(" 2. View All Books\n");
        printf(" 3. Search Book\n");
        printf("\n");
        printf(" MEMBER MANAGEMENT\n");
        printf(" 4. Add New Member\n");
        printf(" 5. View All Members\n");
        printf(" 6. Search Member\n");
        printf("\n");
        printf(" TRANSACTIONS\n");
        printf(" 7. Issue Book\n");
        printf(" 8. Return Book\n");
        printf(" 9. View Issued Books\n");
        printf("\n");
        printf(" 10. Exit\n");
        printf("\n");
        printf("========================================\n");
        printf("Books: %d | Members: %d | Issued: ?\n", bookCount, memberCount);
        printf("========================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice)
        {
            case 1: addBook(); break;
            case 2: viewAllBooks(); break;
            case 3: searchBook(); break;
            case 4: addMember(); break;
            case 5: viewAllMembers(); break;
            case 6: searchMember(); break;
            case 7: issueBook(); break;
            case 8: returnBook(); break;
            case 9: viewIssuedBooks(); break;
            case 10:
                printf("\nThank you for using Library Management System!\n");
                break;
            default:
                printf("\nInvalid choice!\n");
                printf("\nPress any key...");
                getch();
        }
        
    } while(choice != 10);
}

/*
================================================================================
FUNCTION: main()
PURPOSE: Program entry point
================================================================================
*/

int main()
{
    loadDataFromFile();
    mainMenu();
    return 0;
}