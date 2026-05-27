/*
 ============================================
 ADVANCED NUMBER GUESSING GAME
 ============================================
 Features:
 - Multiple difficulty levels
 - Score system
 - Hint system
 - High score tracking
 - Play multiple rounds
 - Statistics tracking
 ============================================
*/

#include <stdio.h>      // For printf(), scanf() - input/output functions
#include <stdlib.h>     // For rand(), srand() - random number functions
#include <time.h>       // For time() - to get current time for random seed
#include <unistd.h>     // For sleep() - to pause program

// Global variables for statistics
int totalGames = 0;         // Total games played
int totalAttempts = 0;      // Total attempts across all games
int highScore = 999;        // Best score (lowest attempts)

// Function declarations
int getDifficulty();                    // Ask user for difficulty level
int playGame(int range, int maxAttempts);  // Main game logic
void showStatistics();                  // Display game statistics
void showRules();                       // Display game rules
void saveHighScore(int score);          // Save high score to file
int loadHighScore();                    // Load high score from file


int main()
{
    int choice;           // Variable to store user menu choice
    int continuePlaying;  // Variable to check if user wants to play again
    
    srand(time(NULL));    // Initialize random number generator
                          // time(NULL) gives current time in seconds
                          // Different seed = different random numbers
    
    // Load saved high score from file
    highScore = loadHighScore();
    
    printf("\n===========================================");
    printf("\n     WELCOME TO NUMBER GUESSING GAME");
    printf("\n===========================================");
    
    do
    {
        // Display main menu
        printf("\n\n========== MAIN MENU ==========");
        printf("\n 1. Play Game");
        printf("\n 2. View Rules");
        printf("\n 3. View Statistics");
        printf("\n 4. Exit");
        printf("\n===============================");
        printf("\n Enter your choice: ");
        
        scanf("%d", &choice);    // Read user's menu choice
        
        switch(choice)    // Switch case - executes different code based on choice
        {
            case 1:    // Play Game
                {
                    int difficulty = getDifficulty();  // Get difficulty level
                    int range, maxAttempts;
                    
                    // Set range and max attempts based on difficulty
                    switch(difficulty)
                    {
                        case 1:  // Easy
                            range = 30;      // Numbers 1 to 30
                            maxAttempts = 10; // 10 attempts allowed
                            printf("\n[EASY MODE] Number between 1-30, Max 10 attempts");
                            break;
                        case 2:  // Medium
                            range = 50;      // Numbers 1 to 50
                            maxAttempts = 7;  // 7 attempts allowed
                            printf("\n[MEDIUM MODE] Number between 1-50, Max 7 attempts");
                            break;
                        case 3:  // Hard
                            range = 100;     // Numbers 1 to 100
                            maxAttempts = 5;  // 5 attempts allowed
                            printf("\n[HARD MODE] Number between 1-100, Max 5 attempts");
                            break;
                        default:
                            range = 50;
                            maxAttempts = 7;
                    }
                    
                    int attempts = playGame(range, maxAttempts);  // Play the game
                    
                    if(attempts > 0)  // If game was won (not quit)
                    {
                        totalGames++;                    // Increase game count
                        totalAttempts += attempts;       // Add attempts to total
                        
                        // Check if this is new high score
                        if(attempts < highScore)
                        {
                            highScore = attempts;
                            saveHighScore(highScore);
                            printf("\n[CONGRATULATIONS] NEW HIGH SCORE!");
                        }
                    }
                }
                break;
                
            case 2:    // View Rules
                showRules();
                break;
                
            case 3:    // View Statistics
                showStatistics();
                break;
                
            case 4:    // Exit
                printf("\n Thank you for playing!");
                printf("\n Total games played: %d", totalGames);
                printf("\n Best score: %d attempts", highScore);
                printf("\n Goodbye!\n");
                break;
                
            default:   // Invalid choice
                printf("\n Invalid choice! Please enter 1-4");
        }
        
        if(choice != 4)  // If not exiting
        {
            printf("\n\n Play again? (1 for Yes / 0 for No): ");
            scanf("%d", &continuePlaying);
        }
        else
        {
            continuePlaying = 0;  // Exit the loop
        }
        
    } while(continuePlaying == 1);  // Loop continues if user wants to play again
    
    return 0;   
}

// FUNCTION: getDifficulty()
// PURPOSE: Ask user to select difficulty level
// RETURNS: 1=Easy, 2=Medium, 3=Hard
int getDifficulty()
{
    int diff;    // Variable to store difficulty choice
    
    printf("\n\n========== DIFFICULTY LEVELS ==========");
    printf("\n 1. Easy (1-30, 10 attempts)");
    printf("\n 2. Medium (1-50, 7 attempts)");
    printf("\n 3. Hard (1-100, 5 attempts)");
    printf("\n=======================================");
    printf("\n Choose difficulty (1-3): ");
    
    scanf("%d", &diff);   // Read user's choice
    
    // Validate input - if not 1-3, default to Medium
    if(diff < 1 || diff > 3)
    {
        printf("\n Invalid choice! Using Medium difficulty.");
        diff = 2;
        sleep(1);   // Pause for 1 second so user can read message
    }
    
    return diff;   // Return the chosen difficulty
}

// ============================================
// FUNCTION: playGame()
// PURPOSE: Main game logic - generates number and checks guesses
// PARAMETER1: range - maximum number (1 to range)
// PARAMETER2: maxAttempts - maximum guesses allowed
// RETURNS: Number of attempts taken, or 0 if game lost
// ============================================
int playGame(int range, int maxAttempts)
{
    int randomNum;      // Random number to guess
    int guess;          // User's guess
    int attempts = 0;   // Counter for number of guesses
    int score;          // Score for this game
    
    // Generate random number between 1 and range
    // rand() % range gives number from 0 to range-1, so add 1
    randomNum = rand() % range + 1;
    
    printf("\n\n========== GAME STARTED ==========");
    printf("\n I'm thinking of a number between 1 and %d", range);
    printf("\n You have %d attempts", maxAttempts);
    printf("\n Type -1 to quit the game");
    printf("\n==================================\n");
    
    // Game loop - runs until guess is correct or attempts exhausted
    do
    {
        printf("\n Enter your guess: ");
        scanf("%d", &guess);   // Take user input
        
        // Check if user wants to quit
        if(guess == -1)
        {
            printf("\n You quit the game!");
            printf("\n The number was: %d", randomNum);
            return 0;   // Return 0 indicating game was not won
        }
        
        attempts++;    // Increase attempt counter by 1
        
        // Provide hint every 3 attempts (but not on last attempt)
        if(attempts % 3 == 0 && attempts < maxAttempts)
        {
            if(randomNum % 2 == 0)   // Check if number is even
                printf(" [HINT] Number is even\n");
            else                      // Number is odd
                printf(" [HINT] Number is odd\n");
        }
        
        // Check if guess is correct, too high, or too low
        if(guess < randomNum)        // If guess is smaller than target
        {
            printf(" [TOO LOW] Guess a larger number");
            
            // Give additional hint if far off
            if(randomNum - guess > range/3)
                printf(" (You're far off)");
        }
        else if(guess > randomNum)   // If guess is greater than target
        {
            printf(" [TOO HIGH] Guess a smaller number");
            
            // Give additional hint if far off
            if(guess - randomNum > range/3)
                printf(" (You're far off)");
        }
        else                          // Guess is correct
        {
            // Calculate score based on attempts used
            score = maxAttempts - attempts + 1;
            if(score < 1) score = 1;
            
            printf("\n\n ===========================================");
            printf("\n [CONGRATULATIONS] YOU GUESSED IT!");
            printf("\n Number was: %d", randomNum);
            printf("\n Attempts taken: %d", attempts);
            printf("\n Your score: %d out of %d", score, maxAttempts);
            printf("\n ===========================================");
            
            return attempts;   // Return number of attempts taken
        }
        
        // Show remaining attempts
        printf("\n [Remaining attempts: %d]", maxAttempts - attempts);
        
        // Check if attempts are exhausted
        if(attempts >= maxAttempts && guess != randomNum)
        {
            printf("\n\n ===========================================");
            printf("\n [GAME OVER] You ran out of attempts!");
            printf("\n The number was: %d", randomNum);
            printf("\n Better luck next time!");
            printf("\n ===========================================");
            return 0;   // Return 0 indicating game was lost
        }
        
    } while(guess != randomNum);   // Continue until guess is correct
    
    return attempts;   // This line is actually never reached (loop condition handles it)
}

// FUNCTION: showRules()
// PURPOSE: Display game rules and instructions
void showRules()
{
    printf("\n\n========== GAME RULES ==========");
    printf("\n 1. Computer will think of a random number");
    printf("\n 2. You have to guess that number");
    printf("\n 3. After each guess, you'll get a hint:");
    printf("\n    - 'Too low' means guess a bigger number");
    printf("\n    - 'Too high' means guess a smaller number");
    printf("\n 4. You have limited attempts based on difficulty");
    printf("\n 5. Every 3rd attempt, you get an extra hint");
    printf("\n 6. Type -1 anytime to quit the game");
    printf("\n 7. Lower attempts = Higher score!");
    printf("\n=================================");
}

// FUNCTION: showStatistics()
// PURPOSE: Display game statistics
void showStatistics()
{
    float averageAttempts;   // Variable for average attempts
    
    printf("\n\n========== GAME STATISTICS ==========");
    
    if(totalGames == 0)   // If no games played yet
    {
        printf("\n No games played yet!");
        printf("\n Play a game to see statistics!");
    }
    else
    {
        // Calculate average attempts (typecast to float for division)
        averageAttempts = (float)totalAttempts / totalGames;
        
        printf("\n Total games played: %d", totalGames);
        printf("\n Total attempts made: %d", totalAttempts);
        printf("\n Average attempts per game: %.2f", averageAttempts);
        printf("\n Best score (lowest attempts): %d", highScore);
        
        // Give rating based on average performance
        if(averageAttempts <= 3)
            printf("\n Rating: [EXCELLENT] - 5 Stars");
        else if(averageAttempts <= 5)
            printf("\n Rating: [VERY GOOD] - 4 Stars");
        else if(averageAttempts <= 7)
            printf("\n Rating: [GOOD] - 3 Stars");
        else
            printf("\n Rating: [FAIR] - Keep practicing!");
    }
    printf("\n=====================================");
}

// FUNCTION: saveHighScore()
// PURPOSE: Save high score to a file
// PARAMETER: score - the high score to save
void saveHighScore(int score)
{
    FILE *file;   // File pointer variable
    
    file = fopen("highscore.txt", "w");   // Open file in write mode
                                          // "w" = write (creates new file or overwrites)
    
    if(file != NULL)   // Check if file opened successfully
    {
        fprintf(file, "%d", score);   // Write score to file
        fclose(file);                 // Close the file
    }
}

// FUNCTION: loadHighScore()
// PURPOSE: Load high score from file
// RETURNS: Saved high score or 999 if no file exists
int loadHighScore()
{
    FILE *file;           // File pointer variable
    int score = 999;      // Default high score (high means worse)
    
    file = fopen("highscore.txt", "r");   // Open file in read mode
                                          // "r" = read
    
    if(file != NULL)   // If file exists and opened successfully
    {
        fscanf(file, "%d", &score);   // Read score from file
        fclose(file);                 // Close the file
    }
    
    return score;   // Return the loaded score (or default 999)
}