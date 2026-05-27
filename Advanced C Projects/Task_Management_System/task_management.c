/*
 ============================================
 TASK MANAGEMENT SYSTEM WITH USER MANAGEMENT
 ============================================
 Features:
 - Add new user with ID, Name, Email, Password
 - View all users
 - Delete user by ID
 - Login/Password authentication
 - Add custom tasks
 - View task progress with animation
 - Change task speed
 - Remove tasks
 - Data stored in "users.dat" file
 ============================================
*/

#include <stdio.h>      // For input-output functions like printf
#include <time.h>       // For time() used in random seed
#include <stdlib.h>     // For rand(), srand(), system()
#include <unistd.h>     // For sleep() function
#include <string.h>     // For string operations

const int BAR_LENGTH = 50;   // Total length of the progress bar
const int MAX_TASKS = 5;     // Total number of tasks

// User structure to store user information
typedef struct {
    int id;
    char username[50];
    char password[50];
    int taskCount;
} User;

// Task structure to represent a task
typedef struct {
    int id;         // Unique ID of the task
    int progress;   // Current progress (0 to 100)
    int step;       // Increment value for each update
    char name[50];  // Task name
} Task;

// Function declarations
void print_bar(Task task);
void clear_screen();
void addUser();
void viewUsers();           // NEW: View all users
void deleteUser();          // NEW: Delete user by ID
void loginUser();
void runTaskManager();
void saveUsers();
void loadUsers();
void addCustomTask(Task tasks[]);
void removeTask(Task tasks[]);

User users[100];        // Array to store users
int userCount = 0;      // Number of users
int currentUserId = -1; // Currently logged in user

int main()
{
    loadUsers();  // Load existing users from file
    
    int choice;
    
    while(1) {
        clear_screen();
        printf("\n========================================\n");
        printf("   WELCOME TO TASK MANAGEMENT SYSTEM   \n");
        printf("========================================\n");
        printf("| 1. Create New Account              |\n");
        printf("| 2. Login                           |\n");
        printf("| 3. View All Users                  |\n");
        printf("| 4. Delete User                     |\n");
        printf("| 5. Exit                            |\n");
        printf("========================================\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                addUser();
                break;
            case 2:
                loginUser();
                if(currentUserId != -1) {
                    runTaskManager();  // After login, run task manager
                }
                break;
            case 3:
                viewUsers();
                break;
            case 4:
                deleteUser();
                break;
            case 5:
                printf("\n Thank you for using Task Manager!\n");
                saveUsers();
                return 0;
            default:
                printf("\n Invalid choice! Press Enter to continue...");
                getchar();
                getchar();
        }
    }
    return 0;
}

// FUNCTION: addUser()
// Creates a new user account
void addUser() {
    clear_screen();
    printf("\n========== CREATE NEW ACCOUNT ==========\n");
    
    User newUser;
    newUser.id = userCount + 1;
    newUser.taskCount = 0;
    
    printf("Enter Username: ");
    scanf("%s", newUser.username);
    printf("Enter Password: ");
    scanf("%s", newUser.password);
    
    // Check if username already exists
    for(int i = 0; i < userCount; i++) {
        if(strcmp(users[i].username, newUser.username) == 0) {
            printf("\n Username already exists!\n");
            printf("Press Enter to continue...");
            getchar();
            getchar();
            return;
        }
    }
    
    users[userCount] = newUser;
    userCount++;
    saveUsers();
    
    printf("\n Account created successfully!\n");
    printf("   User ID: %d\n", newUser.id);
    printf("   Username: %s\n", newUser.username);
    printf("\nPress Enter to continue...");
    getchar();
    getchar();
}

// FUNCTION: viewUsers()
// Displays all registered users
void viewUsers() {
    clear_screen();
    printf("\n========== ALL REGISTERED USERS ==========\n");
    
    if(userCount == 0) {
        printf("\n No users found!\n");
    } else {
        printf("\n| ID | Username            | Tasks Done |\n");
        printf("  |----|---------------------|------------|\n");
        for(int i = 0; i < userCount; i++) {
            printf("| %-2d | %-19s | %-10d |\n", 
                   users[i].id, users[i].username, users[i].taskCount);
        }
        printf("\n Total Users: %d\n", userCount);
    }
    
    printf("\nPress Enter to continue...");
    getchar();
    getchar();
}

// FUNCTION: deleteUser()
// Deletes a user by ID
void deleteUser() {
    clear_screen();
    printf("\n========== DELETE USER ==========\n");
    
    if(userCount == 0) {
        printf("\n No users to delete!\n");
        printf("\nPress Enter to continue...");
        getchar();
        getchar();
        return;
    }
    
    int id, found = 0;
    printf("Enter User ID to delete: ");
    scanf("%d", &id);
    
    // Check if user exists
    for(int i = 0; i < userCount; i++) {
        if(users[i].id == id) {
            found = 1;
            // Shift all users after this one to left
            for(int j = i; j < userCount - 1; j++) {
                users[j] = users[j + 1];
            }
            userCount--;
            saveUsers();
            printf("\n User ID %d deleted successfully!\n", id);
            break;
        }
    }
    
    if(!found) {
        printf("\n User ID %d not found!\n", id);
    }
    
    printf("\nPress Enter to continue...");
    getchar();
    getchar();
}

// FUNCTION: loginUser()
// Authenticates user with username and password
void loginUser() {
    clear_screen();
    printf("\n========== USER LOGIN ==========\n");
    
    char username[50], password[50];
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);
    
    for(int i = 0; i < userCount; i++) {
        if(strcmp(users[i].username, username) == 0 && 
           strcmp(users[i].password, password) == 0) {
            currentUserId = i;
            printf("\n Login successful!\n");
            printf("   Welcome back, %s!\n", users[i].username);
            printf("\nPress Enter to continue...");
            getchar();
            getchar();
            return;
        }
    }
    
    printf("\n Invalid username or password!\n");
    printf("Press Enter to continue...");
    getchar();
    getchar();
    currentUserId = -1;
}

// FUNCTION: logoutUser()
// Logs out current user
void logoutUser() {
    currentUserId = -1;
    printf("\n Logged out successfully!\n");
    printf("Press Enter to continue...");
    getchar();
    getchar();
}

// FUNCTION: runTaskManager()
// Main task management system after login
void runTaskManager() {
    Task tasks[MAX_TASKS];
    srand(time(NULL));
    
    // Initialize tasks with names
    const char* taskNames[] = {"Download Files", "Process Data", "Upload Results", 
                                "Generate Report", "Send Notifications"};
    
    for(int i = 0; i < MAX_TASKS; i++) {
        tasks[i].id = i + 1;
        tasks[i].progress = 0;
        tasks[i].step = rand() % 5 + 1;
        strcpy(tasks[i].name, taskNames[i]);
    }
    
    int choice;
    int tasks_incomplete;
    
    while(1) {
        clear_screen();
        printf("\n========================================\n");
        printf("   TASK MANAGER - Welcome %s   \n", users[currentUserId].username);
        printf("========================================\n");
        printf("| 1. Start All Tasks                 |\n");
        printf("| 2. View Task Progress              |\n");
        printf("| 3. Add Custom Task                 |\n");
        printf("| 4. Remove Task                     |\n");
        printf("| 5. Change Task Speed               |\n");
        printf("| 6. Logout                          |\n");
        printf("========================================\n");
        printf("Choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                // Run all tasks with animation
                tasks_incomplete = 1;
                while(tasks_incomplete) {
                    tasks_incomplete = 0;
                    clear_screen();
                    printf("\n***** TASKS IN PROGRESS *****\n");
                    printf("User: %s\n\n", users[currentUserId].username);
                    
                    for(int i = 0; i < MAX_TASKS; i++) {
                        if(tasks[i].progress < 100) {
                            tasks[i].progress += tasks[i].step;
                            if(tasks[i].progress > 100) tasks[i].progress = 100;
                            tasks_incomplete = 1;
                        }
                        printf("%-20s: ", tasks[i].name);
                        print_bar(tasks[i]);
                    }
                    sleep(1);
                }
                printf("\n\n ALL TASKS COMPLETED! 🎉\n");
                users[currentUserId].taskCount++;
                saveUsers();
                printf("Press Enter to continue...");
                getchar();
                getchar();
                break;
                
            case 2:
                clear_screen();
                printf("\n========== CURRENT TASK STATUS ==========\n");
                for(int i = 0; i < MAX_TASKS; i++) {
                    printf("\nTask %d: %s\n", tasks[i].id, tasks[i].name);
                    printf("Progress: ");
                    print_bar(tasks[i]);
                }
                printf("\nPress Enter to continue...");
                getchar();
                getchar();
                break;
                
            case 3:
                addCustomTask(tasks);
                break;
                
            case 4:
                removeTask(tasks);
                break;
                
            case 5: { 
                int taskId, newStep;
                printf("\nEnter Task ID (1-%d): ", MAX_TASKS);
                scanf("%d", &taskId);
                if(taskId >= 1 && taskId <= MAX_TASKS) {
                    printf("Enter new speed (1-10): ");
                    scanf("%d", &newStep);
                    if(newStep >= 1 && newStep <= 10) {
                        tasks[taskId-1].step = newStep;
                        printf(" Task speed updated!\n");
                    } else {
                        printf(" Speed must be between 1 and 10!\n");
                    }
                } else {
                    printf(" Invalid Task ID!\n");
                }
                printf("Press Enter to continue...");
                getchar();
                getchar();
                break;
            }  
            
            case 6:
                logoutUser();
                return;
                
            default:
                printf("\n Invalid choice!\n");
                printf("Press Enter to continue...");
                getchar();
                getchar();
        }
    }
}

// FUNCTION: addCustomTask()
// Adds a new custom task to the list
void addCustomTask(Task tasks[]) {
    static int customTaskCount = 0;
    
    if(customTaskCount >= 5) {
        printf("\n Maximum custom tasks reached!\n");
        printf("Press Enter to continue...");
        getchar();
        getchar();
        return;
    }
    
    int newIndex = customTaskCount;
    if(newIndex >= MAX_TASKS) {
        printf("\n No space for more tasks!\n");
        return;
    }
    
    printf("\n========== ADD CUSTOM TASK ==========\n");
    printf("Enter task name: ");
    scanf(" %[^\n]", tasks[newIndex].name);
    tasks[newIndex].id = newIndex + 1;
    tasks[newIndex].progress = 0;
    tasks[newIndex].step = rand() % 5 + 1;
    
    customTaskCount++;
    printf("\n Custom task added successfully!\n");
    printf("Press Enter to continue...");
    getchar();
    getchar();
}

// FUNCTION: removeTask()
// Removes a task from the list
void removeTask(Task tasks[]) {
    int taskId;
    printf("\n========== REMOVE TASK ==========\n");
    printf("Enter Task ID to remove (1-%d): ", MAX_TASKS);
    scanf("%d", &taskId);
    
    if(taskId >= 1 && taskId <= MAX_TASKS) {
        strcpy(tasks[taskId-1].name, "Empty Slot");
        tasks[taskId-1].progress = 0;
        tasks[taskId-1].step = 0;
        printf("\n Task %d removed successfully!\n", taskId);
    } else {
        printf("\n Invalid Task ID!\n");
    }
    printf("Press Enter to continue...");
    getchar();
    getchar();
}

// FUNCTION: saveUsers()
// Saves user data to file
void saveUsers() {
    FILE *file = fopen("users.dat", "w");
    if(file == NULL) {
        printf("Error saving users!\n");
        return;
    }
    
    fprintf(file, "%d\n", userCount);
    for(int i = 0; i < userCount; i++) {
        fprintf(file, "%d %s %s %d\n", users[i].id, users[i].username, 
                users[i].password, users[i].taskCount);
    }
    fclose(file);
}

// FUNCTION: loadUsers()
// Loads user data from file
void loadUsers() {
    FILE *file = fopen("users.dat", "r");
    if(file == NULL) {
        userCount = 0;
        return;
    }
    
    fscanf(file, "%d", &userCount);
    for(int i = 0; i < userCount; i++) {
        fscanf(file, "%d %s %s %d", &users[i].id, users[i].username, 
               users[i].password, &users[i].taskCount);
    }
    fclose(file);
}

// FUNCTION: clear_screen()
// Clears terminal screen based on OS
void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// FUNCTION: print_bar()
// Prints progress bar for a task
void print_bar(Task task) {
    int bars_to_show = (task.progress * BAR_LENGTH) / 100;
    
    printf("[");
    for(int i = 0; i < BAR_LENGTH; i++) {
        if(i < bars_to_show) {
            // Different characters based on progress
            if(task.progress < 33) {
                printf("=");
            } else if(task.progress < 66) {
                printf("=");
            } else {
                printf("=");
            }
        } else {
            printf("=");
        }
    }
    printf("] %3d%%\n", task.progress);
}