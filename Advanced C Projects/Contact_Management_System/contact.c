/*
================================================================================
PROJECT: CONTACT MANAGEMENT SYSTEM
LANGUAGE: C PROGRAMMING
================================================================================

DESCRIPTION:
Complete phonebook/contact management system to store and manage contacts.
Add, view, search, edit, delete contacts with phone numbers and emails.

FEATURES:
- Add new contact (Name, Phone, Email, Address)
- View all contacts
- Search contact by name or phone number
- Edit/Update contact details
- Delete contact
- Save contacts to file
- Load contacts from file

================================================================================
*/

#include <stdio.h>      // For printf, scanf - input/output
#include <stdlib.h>     // For system, exit - system functions
#include <string.h>     // For strcpy, strcmp - string operations
#include <conio.h>      // For getch - keyboard input

/*
================================================================================
STRUCTURES
================================================================================
*/

// Structure for contact
struct Contact
{
    int id;                 // Unique ID for contact
    char name[50];          // Contact name
    char phone[15];         // Phone number
    char email[50];         // Email address
    char address[100];      // Home address
};

/*
================================================================================
GLOBAL VARIABLES
================================================================================
*/

struct Contact contacts[500];   // Array to store contacts
int contactCount = 0;           // Total number of contacts
int nextId = 1;                 // Next contact ID

/*
================================================================================
FUNCTION: saveContactsToFile()
PURPOSE: Save all contacts to file
================================================================================
*/

void saveContactsToFile()
{
    FILE *file = fopen("contacts.dat", "wb");
    
    if(file != NULL)
    {
        fwrite(&contactCount, sizeof(int), 1, file);
        fwrite(contacts, sizeof(struct Contact), contactCount, file);
        fwrite(&nextId, sizeof(int), 1, file);
        fclose(file);
    }
}

/*
================================================================================
FUNCTION: loadContactsFromFile()
PURPOSE: Load all contacts from file
================================================================================
*/

void loadContactsFromFile()
{
    FILE *file = fopen("contacts.dat", "rb");
    
    if(file != NULL)
    {
        fread(&contactCount, sizeof(int), 1, file);
        fread(contacts, sizeof(struct Contact), contactCount, file);
        fread(&nextId, sizeof(int), 1, file);
        fclose(file);
    }
    else
    {
        contactCount = 0;
        nextId = 1;
    }
}

/*
================================================================================
FUNCTION: addContact()
PURPOSE: Add new contact
================================================================================
*/

void addContact()
{
    system("cls");
    printf("\n========================================\n");
    printf("           ADD NEW CONTACT\n");
    printf("========================================\n");
    
    if(contactCount >= 500)
    {
        printf("\nContact list is full! Cannot add more.\n");
        printf("\nPress any key...");
        getch();
        return;
    }
    
    struct Contact newContact;
    
    newContact.id = nextId++;
    
    printf("\nContact ID: %d\n", newContact.id);
    
    printf("Enter Name: ");
    fflush(stdin);
    fgets(newContact.name, 50, stdin);
    newContact.name[strcspn(newContact.name, "\n")] = 0;
    
    printf("Enter Phone Number: ");
    scanf("%s", newContact.phone);
    
    printf("Enter Email: ");
    scanf("%s", newContact.email);
    
    printf("Enter Address: ");
    fflush(stdin);
    fgets(newContact.address, 100, stdin);
    newContact.address[strcspn(newContact.address, "\n")] = 0;
    
    contacts[contactCount] = newContact;
    contactCount++;
    saveContactsToFile();
    
    printf("\n========================================\n");
    printf("     CONTACT ADDED SUCCESSFULLY!\n");
    printf("========================================\n");
    printf("Contact ID: %d\n", newContact.id);
    printf("Name: %s\n", newContact.name);
    printf("Phone: %s\n", newContact.phone);
    
    printf("\nPress any key...");
    getch();
}

/*
================================================================================
FUNCTION: viewAllContacts()
PURPOSE: Display all contacts
================================================================================
*/

void viewAllContacts()
{
    system("cls");
    printf("\n========================================\n");
    printf("           ALL CONTACTS\n");
    printf("========================================\n");
    
    if(contactCount == 0)
    {
        printf("\nNo contacts found!\n");
    }
    else
    {
        printf("\n================================================================================\n");
        printf("ID   | Name                      | Phone        | Email\n");
        printf("================================================================================\n");
        
        for(int i = 0; i < contactCount; i++)
        {
            printf("%-4d | %-25s | %-12s | %s\n", 
                   contacts[i].id,
                   contacts[i].name,
                   contacts[i].phone,
                   contacts[i].email);
        }
        printf("================================================================================\n");
        printf("Total Contacts: %d\n", contactCount);
    }
    
    printf("\nPress any key...");
    getch();
}

/*
================================================================================
FUNCTION: searchContact()
PURPOSE: Search contact by name or phone number
================================================================================
*/

void searchContact()
{
    system("cls");
    printf("\n========================================\n");
    printf("           SEARCH CONTACT\n");
    printf("========================================\n");
    
    int choice;
    printf("\nSearch by:\n");
    printf("1. Name\n");
    printf("2. Phone Number\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    
    if(choice == 1)
    {
        char name[50];
        printf("\nEnter Name: ");
        fflush(stdin);
        fgets(name, 50, stdin);
        name[strcspn(name, "\n")] = 0;
        
        int found = 0;
        printf("\n================================================================================\n");
        printf("ID   | Name                      | Phone        | Email\n");
        printf("================================================================================\n");
        
        for(int i = 0; i < contactCount; i++)
        {
            if(strstr(contacts[i].name, name) != NULL)
            {
                printf("%-4d | %-25s | %-12s | %s\n", 
                       contacts[i].id,
                       contacts[i].name,
                       contacts[i].phone,
                       contacts[i].email);
                found = 1;
            }
        }
        
        if(!found)
        {
            printf("\nNo contact found with name: %s\n", name);
        }
        printf("================================================================================\n");
    }
    else if(choice == 2)
    {
        char phone[15];
        printf("\nEnter Phone Number: ");
        scanf("%s", phone);
        
        int found = 0;
        printf("\n================================================================================\n");
        printf("ID   | Name                      | Phone        | Email\n");
        printf("================================================================================\n");
        
        for(int i = 0; i < contactCount; i++)
        {
            if(strcmp(contacts[i].phone, phone) == 0)
            {
                printf("%-4d | %-25s | %-12s | %s\n", 
                       contacts[i].id,
                       contacts[i].name,
                       contacts[i].phone,
                       contacts[i].email);
                found = 1;
            }
        }
        
        if(!found)
        {
            printf("\nNo contact found with phone: %s\n", phone);
        }
        printf("================================================================================\n");
    }
    
    printf("\nPress any key...");
    getch();
}

/*
================================================================================
FUNCTION: viewContactDetails()
PURPOSE: View full details of a specific contact
================================================================================
*/

void viewContactDetails()
{
    system("cls");
    printf("\n========================================\n");
    printf("           VIEW CONTACT DETAILS\n");
    printf("========================================\n");
    
    int id;
    printf("\nEnter Contact ID: ");
    scanf("%d", &id);
    
    int found = 0;
    for(int i = 0; i < contactCount; i++)
    {
        if(contacts[i].id == id)
        {
            printf("\n========================================\n");
            printf("           CONTACT DETAILS\n");
            printf("========================================\n");
            printf("Contact ID: %d\n", contacts[i].id);
            printf("Name: %s\n", contacts[i].name);
            printf("Phone: %s\n", contacts[i].phone);
            printf("Email: %s\n", contacts[i].email);
            printf("Address: %s\n", contacts[i].address);
            printf("========================================\n");
            found = 1;
            break;
        }
    }
    
    if(!found)
    {
        printf("\nContact not found!\n");
    }
    
    printf("\nPress any key...");
    getch();
}

/*
================================================================================
FUNCTION: editContact()
PURPOSE: Edit/Update contact details
================================================================================
*/

void editContact()
{
    system("cls");
    printf("\n========================================\n");
    printf("           EDIT CONTACT\n");
    printf("========================================\n");
    
    int id;
    printf("\nEnter Contact ID to edit: ");
    scanf("%d", &id);
    
    int index = -1;
    for(int i = 0; i < contactCount; i++)
    {
        if(contacts[i].id == id)
        {
            index = i;
            break;
        }
    }
    
    if(index == -1)
    {
        printf("\nContact not found!\n");
        printf("\nPress any key...");
        getch();
        return;
    }
    
    printf("\nCurrent Details:\n");
    printf("Name: %s\n", contacts[index].name);
    printf("Phone: %s\n", contacts[index].phone);
    printf("Email: %s\n", contacts[index].email);
    printf("Address: %s\n", contacts[index].address);
    
    printf("\nEnter New Details (Press Enter to keep same):\n");
    
    char newName[50], newPhone[15], newEmail[50], newAddress[100];
    
    printf("Name [%s]: ", contacts[index].name);
    fflush(stdin);
    fgets(newName, 50, stdin);
    newName[strcspn(newName, "\n")] = 0;
    if(strlen(newName) > 0)
        strcpy(contacts[index].name, newName);
    
    printf("Phone [%s]: ", contacts[index].phone);
    scanf("%s", newPhone);
    if(strlen(newPhone) > 0)
        strcpy(contacts[index].phone, newPhone);
    
    printf("Email [%s]: ", contacts[index].email);
    scanf("%s", newEmail);
    if(strlen(newEmail) > 0)
        strcpy(contacts[index].email, newEmail);
    
    printf("Address [%s]: ", contacts[index].address);
    fflush(stdin);
    fgets(newAddress, 100, stdin);
    newAddress[strcspn(newAddress, "\n")] = 0;
    if(strlen(newAddress) > 0)
        strcpy(contacts[index].address, newAddress);
    
    saveContactsToFile();
    
    printf("\n========================================\n");
    printf("     CONTACT UPDATED SUCCESSFULLY!\n");
    printf("========================================\n");
    
    printf("\nPress any key...");
    getch();
}

/*
================================================================================
FUNCTION: deleteContact()
PURPOSE: Delete a contact
================================================================================
*/

void deleteContact()
{
    system("cls");
    printf("\n========================================\n");
    printf("           DELETE CONTACT\n");
    printf("========================================\n");
    
    int id;
    printf("\nEnter Contact ID to delete: ");
    scanf("%d", &id);
    
    int index = -1;
    for(int i = 0; i < contactCount; i++)
    {
        if(contacts[i].id == id)
        {
            index = i;
            break;
        }
    }
    
    if(index == -1)
    {
        printf("\nContact not found!\n");
        printf("\nPress any key...");
        getch();
        return;
    }
    
    printf("\nContact Found:\n");
    printf("Name: %s\n", contacts[index].name);
    printf("Phone: %s\n", contacts[index].phone);
    
    char confirm;
    printf("\nAre you sure you want to delete this contact? (Y/N): ");
    scanf(" %c", &confirm);
    
    if(confirm == 'Y' || confirm == 'y')
    {
        // Shift all contacts after this one
        for(int i = index; i < contactCount - 1; i++)
        {
            contacts[i] = contacts[i + 1];
        }
        contactCount--;
        saveContactsToFile();
        
        printf("\n========================================\n");
        printf("     CONTACT DELETED SUCCESSFULLY!\n");
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
FUNCTION: deleteAllContacts()
PURPOSE: Delete all contacts
================================================================================
*/

void deleteAllContacts()
{
    system("cls");
    printf("\n========================================\n");
    printf("           DELETE ALL CONTACTS\n");
    printf("========================================\n");
    
    if(contactCount == 0)
    {
        printf("\nNo contacts to delete!\n");
        printf("\nPress any key...");
        getch();
        return;
    }
    
    printf("\nTotal Contacts: %d\n", contactCount);
    
    char confirm;
    printf("\nWARNING: This will delete ALL contacts!\n");
    printf("Are you sure? (Y/N): ");
    scanf(" %c", &confirm);
    
    if(confirm == 'Y' || confirm == 'y')
    {
        contactCount = 0;
        nextId = 1;
        saveContactsToFile();
        
        printf("\n========================================\n");
        printf("     ALL CONTACTS DELETED SUCCESSFULLY!\n");
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
        printf("       CONTACT MANAGEMENT SYSTEM\n");
        printf("========================================\n");
        printf("\n");
        printf(" 1. Add New Contact\n");
        printf(" 2. View All Contacts\n");
        printf(" 3. Search Contact\n");
        printf(" 4. View Contact Details\n");
        printf(" 5. Edit Contact\n");
        printf(" 6. Delete Contact\n");
        printf(" 7. Delete All Contacts\n");
        printf(" 8. Exit\n");
        printf("\n");
        printf("========================================\n");
        printf("Total Contacts: %d\n", contactCount);
        printf("========================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice)
        {
            case 1: addContact(); break;
            case 2: viewAllContacts(); break;
            case 3: searchContact(); break;
            case 4: viewContactDetails(); break;
            case 5: editContact(); break;
            case 6: deleteContact(); break;
            case 7: deleteAllContacts(); break;
            case 8:
                printf("\nThank you for using Contact Management System!\n");
                break;
            default:
                printf("\nInvalid choice!\n");
                printf("\nPress any key...");
                getch();
        }
        
    } while(choice != 8);
}

/*
================================================================================
FUNCTION: main()
PURPOSE: Program entry point
================================================================================
*/

int main()
{
    loadContactsFromFile();
    mainMenu();
    return 0;
}