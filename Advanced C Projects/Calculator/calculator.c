/*
 ============================================
 ADVANCED CALCULATOR SYSTEM
 ============================================
 Features:
 - Basic operations (Add, Subtract, Multiply, Divide)
 - Modulus and Power
 - Square root
 - Percentage calculation
 - Factorial
 - Memory functions (M+, M-, MR, MC)
 - History tracking
 - Scientific calculations (sin, cos, tan)
 - Continue with result feature
 ============================================
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

// Define PI manually if not defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Global variables for memory and history
double memory = 0;
char history[100][200];
int historyCount = 0;

// Function declarations
double division(double a, double b);
double modulus(double a, double b);
void print_menu();
void clear_buffer();
double squareRoot(double a);
double percentage(double a, double b);
long long factorial(int n);
void addToHistory(char operation[], double a, double b, double result);
void showHistory();
void memoryAdd(double value);
void memorySubtract(double value);
void memoryRecall();
void memoryClear();
void scientificMenu();
void continueWithResult(double *lastResult);

int main()
{
    int choice;
    double first, second;
    double result;
    double lastResult = 0;
    int useLastResult = 0;

    while (1)
    {
        print_menu();
        
        if (scanf("%d", &choice) != 1)
        {
            printf("\n[ERROR] Invalid input! Please enter a number.\n");
            clear_buffer();
            continue;
        }

        if (choice == 15)
        {
            printf("\n[INFO] Exiting calculator...\n");
            printf("[INFO] Total calculations done: %d\n", historyCount);
            break;
        }

        if (choice < 1 || choice > 15)
        {
            printf("\n[ERROR] Invalid choice! Try again.\n");
            continue;
        }

        // Handle special cases first
        if (choice == 7)  // Square root
        {
            printf("\nEnter number: ");
            if (scanf("%lf", &first) != 1)
            {
                printf("[ERROR] Invalid input!\n");
                clear_buffer();
                continue;
            }
            result = squareRoot(first);
            if (!isnan(result))
            {
                printf("\n[RESULT] Square root of %.2f = %.2f\n", first, result);
                addToHistory("SQRT", first, 0, result);
                lastResult = result;
            }
            continue;
        }
        
        if (choice == 8)  // Percentage
        {
            printf("\nEnter number: ");
            if (scanf("%lf", &first) != 1)
            {
                printf("[ERROR] Invalid input!\n");
                clear_buffer();
                continue;
            }
            printf("Enter percentage: ");
            if (scanf("%lf", &second) != 1)
            {
                printf("[ERROR] Invalid input!\n");
                clear_buffer();
                continue;
            }
            result = percentage(first, second);
            printf("\n[RESULT] %.2f%% of %.2f = %.2f\n", second, first, result);
            addToHistory("PCT", first, second, result);
            lastResult = result;
            continue;
        }
        
        if (choice == 9)  // Factorial
        {
            printf("\nEnter a non-negative integer: ");
            int n;
            if (scanf("%d", &n) != 1)
            {
                printf("[ERROR] Invalid input!\n");
                clear_buffer();
                continue;
            }
            if (n < 0)
            {
                printf("[ERROR] Factorial not defined for negative numbers!\n");
                continue;
            }
            result = factorial(n);
            printf("\n[RESULT] %d! = %.0f\n", n, (double)result);
            addToHistory("FACT", n, 0, result);
            lastResult = result;
            continue;
        }
        
        if (choice == 10)  // Scientific
        {
            scientificMenu();
            continue;
        }
        
        if (choice == 11)  // History
        {
            showHistory();
            continue;
        }
        
        if (choice == 12)  // Memory operations
        {
            int memChoice;
            printf("\n===== MEMORY OPERATIONS =====\n");
            printf("1. Memory Add (M+)\n");
            printf("2. Memory Subtract (M-)\n");
            printf("3. Memory Recall (MR)\n");
            printf("4. Memory Clear (MC)\n");
            printf("Choice: ");
            scanf("%d", &memChoice);
            
            switch(memChoice)
            {
                case 1:
                    printf("Enter value to add: ");
                    double val;
                    scanf("%lf", &val);
                    memoryAdd(val);
                    break;
                case 2:
                    printf("Enter value to subtract: ");
                    scanf("%lf", &val);
                    memorySubtract(val);
                    break;
                case 3:
                    memoryRecall();
                    break;
                case 4:
                    memoryClear();
                    break;
                default:
                    printf("[ERROR] Invalid memory choice!\n");
            }
            continue;
        }
        
        if (choice == 13)  // Continue with result
        {
            if (lastResult == 0)
            {
                printf("\n[INFO] No previous result available!\n");
                continue;
            }
            printf("\n[INFO] Continuing with last result: %.2f\n", lastResult);
            first = lastResult;
            useLastResult = 1;
            
            printf("Enter second number: ");
            if (scanf("%lf", &second) != 1)
            {
                printf("[ERROR] Invalid input!\n");
                clear_buffer();
                continue;
            }
            
            printf("\n1. Add\n2. Subtract\n3. Multiply\n4. Divide\n");
            printf("Choose operation: ");
            int op;
            scanf("%d", &op);
            
            switch(op)
            {
                case 1:
                    result = first + second;
                    printf("\n[RESULT] %.2f + %.2f = %.2f\n", first, second, result);
                    addToHistory("ADD", first, second, result);
                    break;
                case 2:
                    result = first - second;
                    printf("\n[RESULT] %.2f - %.2f = %.2f\n", first, second, result);
                    addToHistory("SUB", first, second, result);
                    break;
                case 3:
                    result = first * second;
                    printf("\n[RESULT] %.2f * %.2f = %.2f\n", first, second, result);
                    addToHistory("MUL", first, second, result);
                    break;
                case 4:
                    result = division(first, second);
                    if (!isnan(result))
                    {
                        printf("\n[RESULT] %.2f / %.2f = %.2f\n", first, second, result);
                        addToHistory("DIV", first, second, result);
                    }
                    break;
                default:
                    printf("[ERROR] Invalid operation!\n");
            }
            lastResult = result;
            continue;
        }
        
        if (choice == 14)  // Clear history
        {
            historyCount = 0;
            printf("\n[SUCCESS] History cleared!\n");
            continue;
        }
        
        // Regular operations
        if (!useLastResult)
        {
            printf("\nEnter first number: ");
            if (scanf("%lf", &first) != 1)
            {
                printf("[ERROR] Invalid input!\n");
                clear_buffer();
                continue;
            }
        }
        else
        {
            useLastResult = 0;
        }
        
        printf("Enter second number: ");
        if (scanf("%lf", &second) != 1)
        {
            printf("[ERROR] Invalid input!\n");
            clear_buffer();
            continue;
        }
        
        switch (choice)
        {
        case 1:
            result = first + second;
            printf("\n[RESULT] %.2f + %.2f = %.2f\n", first, second, result);
            addToHistory("ADD", first, second, result);
            break;
            
        case 2:
            result = first - second;
            printf("\n[RESULT] %.2f - %.2f = %.2f\n", first, second, result);
            addToHistory("SUB", first, second, result);
            break;
            
        case 3:
            result = first * second;
            printf("\n[RESULT] %.2f * %.2f = %.2f\n", first, second, result);
            addToHistory("MUL", first, second, result);
            break;
            
        case 4:
            result = division(first, second);
            if (!isnan(result))
            {
                printf("\n[RESULT] %.2f / %.2f = %.2f\n", first, second, result);
                addToHistory("DIV", first, second, result);
            }
            break;
            
        case 5:
            result = modulus(first, second);
            if (!isnan(result))
            {
                printf("\n[RESULT] %.2f %% %.2f = %.2f\n", first, second, result);
                addToHistory("MOD", first, second, result);
            }
            break;
            
        case 6:
            result = pow(first, second);
            printf("\n[RESULT] %.2f ^ %.2f = %.2f\n", first, second, result);
            addToHistory("POW", first, second, result);
            break;
        }
        
        lastResult = result;
    }
    
    return 0;
}

// Clear input buffer
void clear_buffer()
{
    while (getchar() != '\n');
}

// Division function with zero check
double division(double a, double b)
{
    if (b == 0)
    {
        printf("[ERROR] Division by zero not allowed!\n");
        return NAN;
    }
    return a / b;
}

// Modulus function using fmod
double modulus(double a, double b)
{
    if (b == 0)
    {
        printf("[ERROR] Modulus by zero not allowed!\n");
        return NAN;
    }
    return fmod(a, b);
}

// Square root function
double squareRoot(double a)
{
    if (a < 0)
    {
        printf("[ERROR] Square root of negative number not allowed!\n");
        return NAN;
    }
    return sqrt(a);
}

// Percentage calculation
double percentage(double a, double b)
{
    return (a * b) / 100;
}

// Factorial calculation
long long factorial(int n)
{
    if (n == 0 || n == 1)
        return 1;
    
    long long result = 1;
    for (int i = 2; i <= n; i++)
    {
        result *= i;
    }
    return result;
}

// Add calculation to history
void addToHistory(char operation[], double a, double b, double result)
{
    if (historyCount < 100)
    {
        if (strcmp(operation, "SQRT") == 0)
            sprintf(history[historyCount], "%d. SQRT(%.2f) = %.2f", historyCount + 1, a, result);
        else if (strcmp(operation, "PCT") == 0)
            sprintf(history[historyCount], "%d. %.2f%% of %.2f = %.2f", historyCount + 1, b, a, result);
        else if (strcmp(operation, "FACT") == 0)
            sprintf(history[historyCount], "%d. %d! = %.0f", historyCount + 1, (int)a, result);
        else if (strcmp(operation, "SIN") == 0)
            sprintf(history[historyCount], "%d. sin(%.2f) = %.4f", historyCount + 1, a, result);
        else if (strcmp(operation, "COS") == 0)
            sprintf(history[historyCount], "%d. cos(%.2f) = %.4f", historyCount + 1, a, result);
        else if (strcmp(operation, "TAN") == 0)
            sprintf(history[historyCount], "%d. tan(%.2f) = %.4f", historyCount + 1, a, result);
        else if (strcmp(operation, "LN") == 0)
            sprintf(history[historyCount], "%d. ln(%.2f) = %.4f", historyCount + 1, a, result);
        else if (strcmp(operation, "LOG") == 0)
            sprintf(history[historyCount], "%d. log10(%.2f) = %.4f", historyCount + 1, a, result);
        else if (strcmp(operation, "EXP") == 0)
            sprintf(history[historyCount], "%d. e^%.2f = %.4f", historyCount + 1, a, result);
        else if (strcmp(operation, "MOD") == 0)
            sprintf(history[historyCount], "%d. %.2f %% %.2f = %.2f", historyCount + 1, a, b, result);
        else
            sprintf(history[historyCount], "%d. %.2f %s %.2f = %.2f", historyCount + 1, a, operation, b, result);
        
        historyCount++;
    }
}

void showHistory()
{
    printf("\n========== CALCULATION HISTORY ==========\n");
    if (historyCount == 0)
    {
        printf("No calculations yet!\n");
    }
    else
    {
        for (int i = 0; i < historyCount; i++)
        {
            printf("%s\n", history[i]);
        }
        printf("\nTotal calculations: %d\n", historyCount);
    }
    printf("=========================================\n");
}

// Memory functions
void memoryAdd(double value)
{
    memory += value;
    printf("[SUCCESS] Added %.2f to memory. Memory = %.2f\n", value, memory);
}

void memorySubtract(double value)
{
    memory -= value;
    printf("[SUCCESS] Subtracted %.2f from memory. Memory = %.2f\n", value, memory);
}

void memoryRecall()
{
    printf("[INFO] Memory value = %.2f\n", memory);
}

void memoryClear()
{
    memory = 0;
    printf("[SUCCESS] Memory cleared!\n");
}

void scientificMenu()
{
    int sciChoice;
    double angle, result;
    
    printf("\n========== SCIENTIFIC CALCULATOR ==========\n");
    printf("1. Sine (sin)\n");
    printf("2. Cosine (cos)\n");
    printf("3. Tangent (tan)\n");
    printf("4. Natural Log (ln)\n");
    printf("5. Log base 10 (log10)\n");
    printf("6. Exponential (e^x)\n");
    printf("Choice: ");
    scanf("%d", &sciChoice);
    
    printf("Enter value: ");
    scanf("%lf", &angle);
    
    switch(sciChoice)
    {
        case 1:
            result = sin(angle * M_PI / 180.0);  // Convert to radians
            printf("\n[RESULT] sin(%.2f) = %.4f\n", angle, result);
            addToHistory("SIN", angle, 0, result);
            break;
        case 2:
            result = cos(angle * M_PI / 180.0);
            printf("\n[RESULT] cos(%.2f) = %.4f\n", angle, result);
            addToHistory("COS", angle, 0, result);
            break;
        case 3:
            if (cos(angle * M_PI / 180.0) == 0)
                printf("[ERROR] Tan undefined for this angle!\n");
            else
            {
                result = tan(angle * M_PI / 180.0);
                printf("\n[RESULT] tan(%.2f) = %.4f\n", angle, result);
                addToHistory("TAN", angle, 0, result);
            }
            break;
        case 4:
            if (angle <= 0)
                printf("[ERROR] Natural log only for positive numbers!\n");
            else
            {
                result = log(angle);
                printf("\n[RESULT] ln(%.2f) = %.4f\n", angle, result);
                addToHistory("LN", angle, 0, result);
            }
            break;
        case 5:
            if (angle <= 0)
                printf("[ERROR] Log only for positive numbers!\n");
            else
            {
                result = log10(angle);
                printf("\n[RESULT] log10(%.2f) = %.4f\n", angle, result);
                addToHistory("LOG", angle, 0, result);
            }
            break;
        case 6:
            result = exp(angle);
            printf("\n[RESULT] e^%.2f = %.4f\n", angle, result);
            addToHistory("EXP", angle, 0, result);
            break;
        default:
            printf("[ERROR] Invalid scientific choice!\n");
    }
}

// Print calculator menu
void print_menu()
{
    printf("\n===========================================\n");
    printf("        ADVANCED CALCULATOR\n");
    printf("===========================================\n");
    printf("| BASIC OPERATIONS                       |\n");
    printf("| 1. Addition                            |\n");
    printf("| 2. Subtraction                         |\n");
    printf("| 3. Multiplication                      |\n");
    printf("| 4. Division                            |\n");
    printf("| 5. Modulus                             |\n");
    printf("| 6. Power                               |\n");
    printf("|-----------------------------------------|\n");
    printf("| ADVANCED FUNCTIONS                     |\n");
    printf("| 7. Square Root                         |\n");
    printf("| 8. Percentage                          |\n");
    printf("| 9. Factorial                           |\n");
    printf("| 10. Scientific (sin, cos, tan, log)   |\n");
    printf("|-----------------------------------------|\n");
    printf("| UTILITIES                              |\n");
    printf("| 11. Show History                       |\n");
    printf("| 12. Memory Operations (M+, M-, MR, MC) |\n");
    printf("| 13. Continue with Last Result          |\n");
    printf("| 14. Clear History                      |\n");
    printf("| 15. Exit                               |\n");
    printf("===========================================\n");
    printf("Enter your choice: ");
}