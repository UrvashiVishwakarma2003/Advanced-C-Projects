/*
 ============================================
 DIGITAL CLOCK PROGRAM
 ============================================
 This program shows:
 - Current time (12 or 24 hour format)
 - Current date
 - Updates every second
 - Works on Windows, Linux, Mac
 ============================================
*/

#include <stdio.h>      // For printf(), scanf() - input/output functions
#include <stdlib.h>     // For system() - to clear screen
#include <time.h>       // For time(), localtime(), strftime() - time functions
#include <unistd.h>     // For sleep() - to pause program

void fill_time(char *, int);   // Gets current time and puts in buffer
void fill_date(char *);        // Gets current date and puts in buffer
int input_format();            // Asks user for 12/24 hour format
void clear_screen();           // Clears terminal screen

int main()
{
    char time[50];      // Array to store time string (like "14:30:25")
    char date[100];     // Array to store date string (like "Monday January 15 2024")
    
    int format;         // Variable to store format choice (1=24hr, 2=12hr)
    
    format = input_format();   // Call function to ask user for format
    while (1)   // 1 means true, so loop runs continuously
    {
        fill_time(time, format);   // Get current time in chosen format
        fill_date(date);           // Get current date
        
        clear_screen();            // Clear previous output from screen
        
        // Print time and date with nice formatting
        printf("\n ==============================================");
        printf("\n Current Time : %s", time);   // %s prints string from array
        printf("\n Current Date : %s", date);
        printf("\n ==============================================");
        
        sleep(1);   // Wait for 1 second before next update
    }
    
    return 0;   
}

// FUNCTION: clear_screen()
void clear_screen()
{
    #ifdef _WIN32              // If this is Windows operating system
        system("cls");         // "cls" is Windows command to clear screen
    #else                      // If this is Linux or Mac operating system
        system("clear");       // "clear" is Linux/Mac command to clear screen
    #endif
}

int input_format()
{
    int format;     // Variable to store user's choice
    printf("\n ==============================================");
    printf("\n ** Choose the time format ** ");
    printf("\n 1. 24 Hour Format (like 14:30:00)");
    printf("\n 2. 12 Hour Format (like 02:30:00 PM)");
    printf("\n Make a Choice (1/2) : ");
    
    scanf("%d", &format);   // Read integer input from user, store in 'format'
    return format;   // Send back the user's choice to main function
}

// FUNCTION: fill_date()
// PURPOSE: Gets current date and puts it into buffer
// PARAMETER: buffer - character array where date will be stored
void fill_date(char *buffer)
{
    time_t raw_time;           // Variable to store raw time (seconds since 1970)
    struct tm *current_time;   // Structure pointer to store broken-down time
    
    time(&raw_time);           // Get current time from system, store in raw_time
    
    current_time = localtime(&raw_time);   // Convert to local time (India time)
    strftime(buffer, 100, "%A %B %d %Y", current_time);                 // Format date: %A = full weekday, %B = full month name, %d = day, %Y = year    
}

// FUNCTION: fill_time()
// PURPOSE: Gets current time and puts it into buffer
// PARAMETER1: buffer - character array where time will be stored
void fill_time(char *buffer, int format)
{
    time_t raw_time;           // Variable to store raw time
    struct tm *current_time;   // Structure pointer for broken-down time
    
    time(&raw_time);           // Get current system time
    current_time = localtime(&raw_time);   // Convert to local time zone
    
    if (format == 1)  
    {
        strftime(buffer, 50, "%H:%M:%S", current_time);                   // %H = hour (00-23), %M = minute (00-59), %S = second (00-59)
    }
    else   
    {
        strftime(buffer, 50, "%I:%M:%S %p", current_time);              // %I = hour (01-12), %M = minute, %S = second, %p = AM/PM
    }
}