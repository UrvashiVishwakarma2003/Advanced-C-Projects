/*
 ============================================
       USER MANAGEMENT SYSTEM IN C
 ============================================
 Features:
 - Add new user with ID, Name, Email, Password
 - View all users
 - Delete user by ID
 - Login/Password authentication
 - Data stored in "users.txt" file
 ============================================
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// Yeh structure ek user ki saari information store karta hai
struct User {
    int id;                 // User unique identifier
    char name[50];         // User ka naam
    char email[50];        // User ka email address
    char password[50];     // User ka password (plain text for demo)
};

void addUser();           // Naya user add karne ke liye
void viewUsers();         // Saare users dekhne ke liye
void deleteUser();        // User delete karne ke liye
void loginUser();         // User login karne ke liye
void changePassword();    // Password change karne ke liye
int userExists(int id);   // Check karta hai ki user exist karta hai ya nahi

// FUNCTION: addUser()
// WORKING: User se input leke naya user file mein save karta hai
void addUser() {
    struct User u;
    FILE *file = fopen("users.txt", "a");  // 'a' = append mode, file ke end mein likhega
    
    // Error checking - agar file open nahi hui
    if (file == NULL) {
        printf(" Error: File open nahi ho raha!\n");
        return;
    }
    
    printf("\n========== ADD NEW USER ==========\n");
    
    printf("Enter User ID: ");                       // User ID input
    scanf("%d", &u.id);
    
    if (userExists(u.id)) {                          // Check agar ID already exist karti hai
        printf(" Error: User ID %d already exists!\n", u.id);
        fclose(file);
        return;
    }
    
    printf("Enter User Name: ");
    scanf("%s", u.name);
    
    printf("Enter User Email: ");
    scanf("%s", u.email);
    
    printf("Enter User Password: ");
    scanf("%s", u.password);
    
    // File mein save karna
    fprintf(file, "%d %s %s %s\n", u.id, u.name, u.email, u.password);
    fclose(file);
    
    printf(" User added successfully!\n");
    printf("   ID: %d | Name: %s\n", u.id, u.name);
}

// FUNCTION: viewUsers()
// WORKING: users.txt file se saare users ko read karke display karta hain 
void viewUsers() {
    struct User u;
    FILE *file = fopen("users.txt", "r");  // 'r' = read mode
    
    // Check karo file hai ya nahi
    if (file == NULL) {
        printf("\n❌ No users found! Database empty hai.\n");
        return;
    }
    
    printf("\n========== ALL USERS LIST ==========\n");
    printf("| ID | Name              | Email              | Password |\n");
    printf("|----|-------------------|--------------------|----------|\n");
    
    // EOF = End Of File, jab tak file khatam nahi hoti tab tak read karo
    while (fscanf(file, "%d %s %s %s", &u.id, u.name, u.email, u.password) != EOF) {
        printf("| %-2d | %-17s | %-18s | %-8s |\n", 
               u.id, u.name, u.email, u.password);
    }
    
    fclose(file);
    printf("====================================\n");
}

// FUNCTION: userExists()
// WORKING: Check karta hai ki given ID ka user exist karta hai ya nahi
// PARAMETER: int id - user ki ID
// RETURN: 1 agar exist karta hai, 0 agar nahi
int userExists(int id) {
    struct User u;
    FILE *file = fopen("users.txt", "r");
    
    if (file == NULL) {
        return 0;  // File hi nahi hai to user definitely nahi hoga
    }
    
    // Saare users check karo
    while (fscanf(file, "%d %s %s %s", &u.id, u.name, u.email, u.password) != EOF) {
        if (u.id == id) {
            fclose(file);
            return 1;  // User mil gaya
        }
    }
    
    fclose(file);
    return 0;  // User nahi mila
}

// FUNCTION: deleteUser()
// WORKING: User ID se user ko delete karta hai
// Temporary file use karta hai (temp.txt) phir rename kar deta hai
void deleteUser() {
    int id, found = 0;
    struct User u;
    
    printf("\n========== DELETE USER ==========\n");
    printf("Enter User ID to delete: ");
    scanf("%d", &id);

    FILE *file = fopen("users.txt", "r");                     // Original file read karne ke liye
    if (file == NULL) {
        printf(" No users found!\n");
        return;
    }
    
    // Temporary file write karne ke liye (jisme delete wala user nahi hoga)
    FILE *temp = fopen("temp.txt", "w");
    
    // Saare users read karo, delete wale ko chhod ke baaki sab temp file mein likho
    while (fscanf(file, "%d %s %s %s", &u.id, u.name, u.email, u.password) != EOF) {
        if (u.id == id) {
            found = 1;  // User mil gaya jise delete karna hai
            continue;   // Is user ko skip karo (matlab temp mein mat likho)
        }
        // Baaki users ko temp file mein likho
        fprintf(temp, "%d %s %s %s\n", u.id, u.name, u.email, u.password);
    }
    
    fclose(file);
    fclose(temp);
    
    // Purani file ko delete karo aur temporary file ko rename karo
    remove("users.txt");
    rename("temp.txt", "users.txt");
    
    if (found) {
        printf(" User with ID %d deleted successfully!\n", id);
    } else {
        printf(" User with ID %d not found!\n", id);
    }
}

// FUNCTION: loginUser()
// WORKING: User ID aur Password check karta hai
// Agar match ho jaye to login successful
void loginUser() {
    int id, found = 0;
    char password[50];
    struct User u;
    
    printf("\n========== USER LOGIN ==========\n");
    printf("Enter User ID: ");
    scanf("%d", &id);
    printf("Enter Password: ");
    scanf("%s", password);
    
    FILE *file = fopen("users.txt", "r");
    
    if (file == NULL) {
        printf("❌ No users registered yet!\n");
        return;
    }
    
    // Saare users check karo
    while (fscanf(file, "%d %s %s %s", &u.id, u.name, u.email, u.password) != EOF) {
        if (u.id == id && strcmp(u.password, password) == 0) {
            found = 1;
            break;
        }
    }
    
    fclose(file);
    
    if (found) {
        printf("\n✅ LOGIN SUCCESSFUL!\n");
        printf("   Welcome back, %s!\n", u.name);
        printf("   Email: %s\n", u.email);
    } else {
        printf("\n❌ LOGIN FAILED!\n");
        printf("   Invalid User ID or Password!\n");
    }
}

// FUNCTION: changePassword()
// WORKING: User ka password change karta hai
// Pehle old password verify karta hai phir naya set karta hai
void changePassword() {
    int id, found = 0;
    char oldPass[50], newPass[50];
    struct User u;
    
    printf("\n========== CHANGE PASSWORD ==========\n");
    printf("Enter User ID: ");
    scanf("%d", &id);
    printf("Enter Old Password: ");
    scanf("%s", oldPass);
    
    FILE *file = fopen("users.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if (file == NULL) {
        printf("❌ No users found!\n");
        return;
    }
    
    // Read all users, update password for matching user
    while (fscanf(file, "%d %s %s %s", &u.id, u.name, u.email, u.password) != EOF) {
        if (u.id == id && strcmp(u.password, oldPass) == 0) {
            found = 1;
            printf("Enter New Password: ");
            scanf("%s", newPass);
            strcpy(u.password, newPass);  // set  new password
        }
        fprintf(temp, "%d %s %s %s\n", u.id, u.name, u.email, u.password);
    }
    
    fclose(file);
    fclose(temp);
    
    remove("users.txt");
    rename("temp.txt", "users.txt");
    
    if (found) {
        printf("Password changed successfully!\n");
    } else {
        printf("Invalid User ID or Password!\n");
    }
}

// Program yahan se start hoga
int main() {
    int choice;
    
    printf("\n========================================\n");
    printf("   WELCOME TO USER MANAGEMENT SYSTEM   \n");
    printf("========================================\n");
    
    // Infinite loop until user chooses exit
    while (1) {
        printf("\n========== MAIN MENU ==========\n");
        printf("| 1. Add New User              |\n");
        printf("| 2. View All Users            |\n");
        printf("| 3. Delete User               |\n");
        printf("| 4. User Login                |\n");
        printf("| 5. Change Password           |\n");
        printf("| 6. Exit                      |\n");
        printf("================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
    
        switch(choice) {                           // Switch case for menu options
            case 1:
                addUser();
                break;
            case 2:
                viewUsers();
                break;
            case 3:
                deleteUser();
                break;
            case 4:
                loginUser();
                break;
            case 5:
                changePassword();
                break;
            case 6:
                printf("\n Thank you for using User Management System!\n");
                printf("   Goodbye!\n\n");
                return 0;  // Program exit
            default:
                printf("\n Invalid choice! Please enter 1-6 only.\n");
        }
    }
    
    return 0;
}