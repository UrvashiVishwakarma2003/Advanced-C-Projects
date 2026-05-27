/*
 ============================================
 CALENDAR MANAGEMENT SYSTEM
 ============================================
 
 Features:
 - Display full year calendar
 - Display specific month calendar
 - Save calendar to file
 - Add notes/events on specific dates
 - View holidays
 - Change year
 
 Author: Calendar System
 Version: 3.0
 ============================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Global variables
char events[12][31][200];
int eventCount[12][31];

// Function declarations
int get_1st_weekDay(int year);
void displayFullYear(int year);
void displaySpecificMonth(int year, int month);
void saveCalendarToFile(int year);
void addEvent(int year, int month, int day);
void viewEvents(int year, int month, int day);
void viewHolidays(int year);
int isLeapYear(int year);
void navigationMenu();

// ============================================
// Check leap year
// ============================================
int isLeapYear(int year) {
    return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
}

// ============================================
// Get first weekday of year (0 = Sunday)
// ============================================
int get_1st_weekDay(int year) {
    int d;
    d = (year + (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400) % 7;
    return d;
}

// ============================================
// Display full year calendar
// ============================================
void displayFullYear(int year) {
    int month, day, daysInMonth;
    int weekDay = 0, startingDay;
    char *months[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };
    
    int monthDay[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if(isLeapYear(year)) {
        monthDay[1] = 29;
    }
    
    startingDay = get_1st_weekDay(year);
    
    printf("\n\n==================== CALENDAR %d ====================\n", year);
    printf("\n");
    
    for(month = 0; month < 12; month++) {
        daysInMonth = monthDay[month];
        
        printf("\n     ----------- %s %d -----------\n", months[month], year);
        printf("     Sun  Mon  Tue  Wed  Thu  Fri  Sat\n");
        printf("     ");
        
        for(weekDay = 0; weekDay < startingDay; weekDay++) {
            printf("     ");
        }
        
        for(day = 1; day <= daysInMonth; day++) {
            printf("%5d", day);
            
            if(++weekDay > 6) {
                printf("\n     ");
                weekDay = 0;
            }
        }
        printf("\n");
        startingDay = weekDay;
    }
    
    printf("\n================= Calendar Generated =================\n");
}

// ============================================
// Display specific month calendar
// ============================================
void displaySpecificMonth(int year, int month) {
    char *months[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };
    
    int monthDay[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if(isLeapYear(year)) {
        monthDay[1] = 29;
    }
    
    int startingDay = get_1st_weekDay(year);
    for(int i = 0; i < month; i++) {
        startingDay = (startingDay + monthDay[i]) % 7;
    }
    
    printf("\n\n==================== %s %d ====================\n", months[month], year);
    printf("     Sun  Mon  Tue  Wed  Thu  Fri  Sat\n");
    printf("     ");
    
    for(int weekDay = 0; weekDay < startingDay; weekDay++) {
        printf("     ");
    }
    
    for(int day = 1; day <= monthDay[month]; day++) {
        printf("%5d", day);
        
        if(eventCount[month][day-1] > 0) {
            printf("*");
        } else {
            printf(" ");
        }
        
        if((startingDay + day) % 7 == 0) {
            printf("\n     ");
        }
    }
    printf("\n\n========================================================\n");
    
    printf("\n[Events this month]:\n");
    int hasEvents = 0;
    for(int day = 1; day <= monthDay[month]; day++) {
        if(eventCount[month][day-1] > 0) {
            printf("   - Day %d: %s\n", day, events[month][day-1]);
            hasEvents = 1;
        }
    }
    if(!hasEvents) {
        printf("   No events scheduled this month.\n");
    }
}

// ============================================
// Add event for specific date
// ============================================
void addEvent(int year, int month, int day) {
    int monthDay[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if(isLeapYear(year)) {
        monthDay[1] = 29;
    }
    
    if(day < 1 || day > monthDay[month]) {
        printf("\n[ERROR] Invalid date!\n");
        return;
    }
    
    printf("\n========== ADD EVENT ==========\n");
    printf("Enter event description: ");
    getchar();
    fgets(events[month][day-1], 200, stdin);
    events[month][day-1][strcspn(events[month][day-1], "\n")] = 0;
    eventCount[month][day-1] = 1;
    
    printf("\n[SUCCESS] Event added successfully!\n");
}

// ============================================
// View events for specific date
// ============================================
void viewEvents(int year, int month, int day) {
    int monthDay[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if(isLeapYear(year)) {
        monthDay[1] = 29;
    }
    
    if(day < 1 || day > monthDay[month]) {
        printf("\n[ERROR] Invalid date!\n");
        return;
    }
    
    printf("\n========== EVENTS FOR %d/%d/%d ==========\n", day, month+1, year);
    
    if(eventCount[month][day-1] > 0) {
        printf("[NOTE] %s\n", events[month][day-1]);
    } else {
        printf("No events scheduled for this date.\n");
    }
    printf("========================================\n");
}

// ============================================
// View major holidays
// ============================================
void viewHolidays(int year) {
    printf("\n========== MAJOR HOLIDAYS %d ==========\n", year);
    printf("[HOL] New Year's Day     : January 1\n");
    printf("[HOL] Republic Day       : January 26\n");
    printf("[HOL] Holi               : March (Date varies)\n");
    printf("[HOL] Independence Day   : August 15\n");
    printf("[HOL] Gandhi Jayanti     : October 2\n");
    printf("[HOL] Diwali             : October/November (Date varies)\n");
    printf("[HOL] Christmas          : December 25\n");
    
    int a = year % 19;
    int b = year % 4;
    int c = year % 7;
    int d = (19 * a + 24) % 30;
    int e = (2 * b + 4 * c + 6 * d + 5) % 7;
    int easterDay = d + e + 22;
    
    if(easterDay > 31) {
        printf("[HOL] Easter             : April %d\n", easterDay - 31);
    } else {
        printf("[HOL] Easter             : March %d\n", easterDay);
    }
    printf("========================================\n");
}

// ============================================
// Save calendar to file
// ============================================
void saveCalendarToFile(int year) {
    char filename[100];
    sprintf(filename, "calendar_%d.txt", year);
    
    FILE *file = fopen(filename, "w");
    if(file == NULL) {
        printf("\n[ERROR] Error creating file!\n");
        return;
    }
    
    int month, day, daysInMonth;
    int weekDay = 0, startingDay;
    char *months[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };
    
    int monthDay[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if(isLeapYear(year)) {
        monthDay[1] = 29;
    }
    
    startingDay = get_1st_weekDay(year);
    
    fprintf(file, "\n\n==================== CALENDAR %d ====================\n", year);
    
    for(month = 0; month < 12; month++) {
        daysInMonth = monthDay[month];
        fprintf(file, "\n     ----------- %s %d -----------\n", months[month], year);
        fprintf(file, "     Sun  Mon  Tue  Wed  Thu  Fri  Sat\n");
        fprintf(file, "     ");
        
        for(weekDay = 0; weekDay < startingDay; weekDay++) {
            fprintf(file, "     ");
        }
        
        for(day = 1; day <= daysInMonth; day++) {
            fprintf(file, "%5d", day);
            if(++weekDay > 6) {
                fprintf(file, "\n     ");
                weekDay = 0;
            }
        }
        fprintf(file, "\n");
        startingDay = weekDay;
    }
    
    fclose(file);
    printf("\n[SUCCESS] Calendar saved to '%s' successfully!\n", filename);
}

// ============================================
// Main navigation menu
// ============================================
void navigationMenu() {
    int year, choice, month, day;
    
    printf("\n========================================\n");
    printf("   WELCOME TO CALENDAR SYSTEM   \n");
    printf("========================================\n");
    printf("Enter year (e.g., 2024): ");
    scanf("%d", &year);
    
    while(1) {
        printf("\n========== MAIN MENU ==========\n");
        printf("| 1. Show Full Year Calendar  |\n");
        printf("| 2. Show Specific Month      |\n");
        printf("| 3. Add Event/Note           |\n");
        printf("| 4. View Events              |\n");
        printf("| 5. View Holidays            |\n");
        printf("| 6. Save Calendar to File    |\n");
        printf("| 7. Change Year              |\n");
        printf("| 8. Exit                     |\n");
        printf("================================\n");
        printf("Choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                displayFullYear(year);
                break;
                
            case 2:
                printf("\nEnter month (1-12): ");
                scanf("%d", &month);
                if(month >= 1 && month <= 12) {
                    displaySpecificMonth(year, month-1);
                } else {
                    printf("\n[ERROR] Invalid month!\n");
                }
                break;
                
            case 3:
                printf("\nEnter date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                if(month >= 1 && month <= 12 && day >= 1) {
                    addEvent(year, month-1, day);
                } else {
                    printf("\n[ERROR] Invalid date!\n");
                }
                break;
                
            case 4:
                printf("\nEnter date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                if(month >= 1 && month <= 12 && day >= 1) {
                    viewEvents(year, month-1, day);
                } else {
                    printf("\n[ERROR] Invalid date!\n");
                }
                break;
                
            case 5:
                viewHolidays(year);
                break;
                
            case 6:
                saveCalendarToFile(year);
                break;
                
            case 7:
                printf("\nEnter new year: ");
                scanf("%d", &year);
                printf("\n[SUCCESS] Year changed to %d\n", year);
                break;
                
            case 8:
                printf("\n[INFO] Thank you for using Calendar System!\n");
                return;
                
            default:
                printf("\n[ERROR] Invalid choice! Please try again.\n");
        }
        
        printf("\nPress Enter to continue...");
        getchar();
        getchar();
    }
}

// ============================================
// Main function
// ============================================
int main() {
    for(int i = 0; i < 12; i++) {
        for(int j = 0; j < 31; j++) {
            eventCount[i][j] = 0;
            strcpy(events[i][j], "");
        }
    }
    
    navigationMenu();
    
    return 0;
}