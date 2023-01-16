/*
--------------------------------------------------------------------------------------------
Program 3: Best Wordle Word

Course: CS 211, Fall 2022. Thurs 8am lab
System: Mac using Xcode & Replit
Author: William Angelakos & Srijita Banerjee
--------------------------------------------------------------------------------------------
 */
//  bestWordle.c
//  Author: William Angelakos & Srijita Banerjee
//
//  Links to wordle dictionary words at:
//    https://www.reddit.com/r/wordle/comments/s4tcw8/a_note_on_wordles_word_list/
//

#include <stdio.h>
#include <stdlib.h>   // for exit( -1)
#include <string.h>

/*
 structure to hold word and the word's score together
 taken is the blanking out process we use
 if taken[0] == 1, that means the first letter of the word is blanked out
 */
typedef struct wordInfo wordCountStruct;
struct wordInfo
{
    char word[6];
    int score;
    int taken[5];
};


//-----------------------------------------------------------------------------------------
// Comparator for use in built-in qsort(..) function.  Parameters are declared to be a
// generic type, so they will match with anything.
// This is a two-part comparison.  First the scores are compared.  If they are the same,
// then the words themselves are also compared, so that the results are in descending
// order by score, and within score they are in alphabetic order.
int compareFunction( const void * a, const void * b) {
    // Before using parameters we have cast them into the actual type they are in our program
    // and then extract the numerical value used in comparison
    int firstScore = ((wordCountStruct *) a)->score;
    int secondScore = ((wordCountStruct *) b)->score;

    // If scores are different, then that's all we need for our comparison.
    if (firstScore != secondScore) {
        // We reverse the values, so the result is in descending vs. the otherwise ascending order
        // return firstScore - secondScore;   // ascending order
        return secondScore - firstScore;      // descending order
    }
    else {
        // Scores are equal, so check words themselves, to put them in alphabetical order
        // Hint: return the value of strcmp( parameter a word,  parameter b word)
        return strcmp( ((wordCountStruct *) a)->word, ((wordCountStruct *) b)->word);
    }
} //end compareFunction(..)


// -----------------------------------------------------------------------------------------
/*
 countFileElements counts the number of words in a file
 the input is a pointer to the fileName
 the output is an integer of the number of elements in the file
 this function will open and close the file itself
 */
int countFileElements(const char *fileName){
    
    //stores the number of elements in the list
    int count = 0;
    
    // open the file
    FILE *inFilePtr  = fopen(fileName, "r");  // Connect logical name to filename
    char inputString[ 81];
    
    // Sanity check: ensure file open worked correctly
    if( inFilePtr == NULL )
    {
        printf("Error: could not open %s for reading\n", fileName);
        exit(-1);    // must include stdlib.h
    }
    
    // Read each word from file and count it.
    while( fscanf( inFilePtr, "%s", inputString) != EOF)
    {
        count++;
    }
    
    // Close the file
    fclose( inFilePtr);
    
    //return the number of elements in the
    return count;
}

// -----------------------------------------------------------------------------------------
/*
 readFileElements will read a file and export the words into a wordCountStruct array
 this function takes the a pointer to a file name, a pointer to an array of wordCountStruct,
 and a pointer to the ammount of elements in the file
 this will return nothing, however it will intilize elements within the wordCountStruct array
 this function will open and close the file itself
 */
void readFileElements(const char *fileName, wordCountStruct *wordList, const int *wordCountSize)
{
    //open the file
    FILE *inFilePtr  = fopen(fileName, "r");  // Connect logical name to filename
    char inputString[ 81];
    
    // Sanity check: ensure file open worked correctly
    if( inFilePtr == NULL )
    {
        printf("Error: could not open %s for reading\n", fileName);
        exit(-1);    // must include stdlib.h
    }
    
    // Read each word from file and add to the list
    int index = 0;
    while( fscanf( inFilePtr, "%s", inputString) != EOF)
    {
        strcpy( wordList[index].word, inputString);
        wordList[index].word[5] = '\0';
        wordList[index].score = 0;
        index++;
    }
    // Close the file
    fclose( inFilePtr);
}

// -----------------------------------------------------------------------------------------
/*
 concatLists will combind append source list to dest list, both being an array of
 wordCountStructs. It will also sort the list into alphabetical order
 this function takes a destination list, destionation list size (before appending source list),
 source list, and source list size
 this function returns nothing
 */
void concatLists(wordCountStruct *destList, int *destSize, wordCountStruct *sourceList, int *sourceSize)
{
    //index to start appending in dest list
    int destIndex = *destSize;
    //beginning of source list
    int sourceIndex = 0;
    //final size of dest list
    int finalSize = *destSize + *sourceSize;
    
    //will append all of source list into dest list
    for( sourceIndex = 0; destIndex < finalSize; sourceIndex++, destIndex++)
    {
        strcpy( destList[destIndex].word, sourceList[sourceIndex].word);
    }
    
    //sort list into alphabetical order
    qsort( destList, finalSize, sizeof( wordCountStruct), compareFunction);
    
    *destSize += *sourceSize;
}

// -----------------------------------------------------------------------------------------
/*
 firstWordOnly takes a destination list, destionation list size (before appending source list),
 source list, and source list size
 this function returns nothing
 this function will iterate thourgh every element in guess list and score it based on its
 similarities to the words in answerList
 This funciton will also sort the struct array
 */
void firstWordsOnly( wordCountStruct *guessList, int *guessListSize, wordCountStruct *answerList, int *answerListSize)
{
    // index of where you are in the guesses list
    int guessIndex = 0;
    // index of where you are in the answers list
    int answerIndex = 0;
    // index of what character you are in in the guess word
    int guessWordIndex = 0;
    // index of what character you are in in the answer word
    int answerWordIndex = 0;
    // index of where you are when iterating through to initilize the taken struct data member
    int takenIndex = 0;

    // iterating through each guess word
    for( guessIndex = 0; guessIndex < *guessListSize; guessIndex++)
    {
        // initilizing all guess word score to zero
        guessList[guessIndex].score = 0;

        // iterating through each word in the answer list for each word in the guess list
        for( answerIndex = 0; answerIndex < *answerListSize; answerIndex++)
        {
            // initilizing all answer and guess word taken elements to be zero
            for(takenIndex = 0; takenIndex < 5; takenIndex++)
            {
                answerList[answerIndex].taken[takenIndex] = 0;
                guessList[guessIndex].taken[takenIndex] = 0;
            }
            
            // iterating through each character in the guess word
            for( guessWordIndex = 0; guessWordIndex < 5; guessWordIndex++)
            {
                // if the guess word and answer word have the same letter at guess word character index
                if( guessList[guessIndex].word[guessWordIndex] == answerList[answerIndex].word[guessWordIndex])
                {
                    // adding score to guess word
                    guessList[guessIndex].score += 3;
                    // initilizing these characters to be used
                    answerList[answerIndex].taken[guessWordIndex] = 1;
                    guessList[guessIndex].taken[guessWordIndex] = 1;
                }
            }
            
            // iterating through each character in guess word
            for( guessWordIndex = 0; guessWordIndex < 5; guessWordIndex++)
            {
                // iterating through each character in the answer word for each character in the guess word
                for( answerWordIndex = 0; answerWordIndex < 5; answerWordIndex++)
                {
                    // if the characters match and both characters have not already been used
                    if( guessList[guessIndex].word[guessWordIndex] == answerList[answerIndex].word[answerWordIndex] &&
                        answerList[answerIndex].taken[answerWordIndex] == 0 &&
                        guessList[guessIndex].taken[guessWordIndex] == 0)
                    {
                        // adding score to guess word
                        guessList[guessIndex].score += 1;
                        // initilizing the characeers to be used
                        answerList[answerIndex].taken[answerWordIndex] = 1;
                        guessList[guessIndex].taken[guessWordIndex] = 1;
                    }
                }
            }
        }
    }
    
    //sort the list into proper order
    qsort( guessList, *guessListSize, sizeof( wordCountStruct), compareFunction);
    
    
}

// -----------------------------------------------------------------------------------------
/*
 firstAndSecondWord takes a destination list, destionation list size (before appending source list),
 source list, and source list size
 this function returns nothing
 this function will iterate thourgh every element in guess list and score it based on its
 similarities to the words in answerList minus the similarities from the best first word that
 is passed by giving its index in firstWordPosition
 This funciton will also sort the struct array
 */
void firstAndSecondWord( wordCountStruct *guessList, int *guessListSize, wordCountStruct *answerList, int *answerListSize, int *firstWordPosition)
{
    // index that holds location in guess list
    int guessIndex = 0;
    // index that holds location in the answers list
    int answerIndex = 0;
    // index that holds location in guess list word
    int guessWordIndex = 0;
    // index that holds the location in the answer list word
    int answerWordIndex = 0;
    // index that holds location when initilizing taken variables
    int takenIndex = 0;
    // index of the top score words
    int firstWordIndex = *firstWordPosition;
    
    // finding the top first word scores
    firstWordsOnly(guessList, guessListSize, answerList, answerListSize);
    
    // iterating for each word in guess list
    for( guessIndex = 0; guessIndex < *guessListSize; guessIndex++)
    {
        // initilzing all guess scores to be zero
        guessList[guessIndex].score = 0;
        
        // iterating for each word in the answer list
        for( answerIndex = 0; answerIndex < *answerListSize; answerIndex++)
        {
            // iterating through the taken array
            for(takenIndex = 0; takenIndex < 5; takenIndex++)
            {
                // initilizing all values to be zero
                answerList[answerIndex].taken[takenIndex] = 0;
                guessList[guessIndex].taken[takenIndex] = 0;
                guessList[firstWordIndex].taken[takenIndex] = 0;
            }
            
            // iterating through taken list
            // also, will erase characters that were used by the top scoring word
            for( takenIndex = 0; takenIndex < 5; takenIndex++)
            {
                // if the top scoring word has the same character as the answer word at some index
                if( guessList[firstWordIndex].word[takenIndex] == answerList[answerIndex].word[takenIndex])
                {
                    guessList[firstWordIndex].taken[takenIndex] = 1;
                    answerList[answerIndex].taken[takenIndex] = 1;
                }
                // if the top scoring word has the same character as the answer word, at two different indexs
                for( guessWordIndex = 0; guessWordIndex < 5; guessWordIndex++)
                {
                    if( guessList[firstWordIndex].word[takenIndex] == answerList[answerIndex].word[guessWordIndex]
                       && guessList[firstWordIndex].taken[takenIndex] == 0
                       && answerList[answerIndex].taken[guessWordIndex] == 0)
                    {
                        guessList[firstWordIndex].taken[takenIndex] = 1;
                        answerList[answerIndex].taken[guessWordIndex] = 1;
                    }
                }
            }
            
            // initilizng top scoring word to have no erased values
            for(takenIndex = 0; takenIndex < 5; takenIndex++)
            {
                guessList[firstWordIndex].taken[takenIndex] = 0;
            }
            
            // iterating through each character in the guess word
            for( guessWordIndex = 0; guessWordIndex < 5; guessWordIndex++)
            {
                // if the guess word and the answer word have the same character at some index
                if( guessList[guessIndex].word[guessWordIndex] == answerList[answerIndex].word[guessWordIndex]
                   && answerList[answerIndex].taken[guessWordIndex] == 0
                   && guessList[guessIndex].taken[guessWordIndex] == 0)
                {
                    // add score
                    guessList[guessIndex].score += 3;
                    // initilize characters to be used
                    answerList[answerIndex].taken[guessWordIndex] = 1;
                    guessList[guessIndex].taken[guessWordIndex] = 1;
                }
            }
            // iterating through each character in the guess word
            for( guessWordIndex = 0; guessWordIndex < 5; guessWordIndex++)
            {
                // initilizing through each character in the answer word for each character in the guess word
                for( answerWordIndex = 0; answerWordIndex < 5; answerWordIndex++)
                {
                    // if the character is the same at different indexs
                    if( guessList[guessIndex].word[guessWordIndex] == answerList[answerIndex].word[answerWordIndex] &&
                       answerList[answerIndex].taken[answerWordIndex] == 0 &&
                       guessList[guessIndex].taken[guessWordIndex] == 0)
                    {
                        // add score
                        guessList[guessIndex].score += 1;
                        // initilize both to be used
                        answerList[answerIndex].taken[answerWordIndex] = 1;
                        guessList[guessIndex].taken[guessWordIndex] = 1;
                    }
                }
            }
        }
    }
    
    //sort the list into order
    qsort( guessList, *guessListSize, sizeof( wordCountStruct), compareFunction);
}

// -----------------------------------------------------------------------------------------
/*
 menuOption1 takes a pointer to the answer file name, answer file size,  answersList
 wordCountStruct, guesses file name, guesses size and guesses list wordCountStruct
 this funciton will not return anything
 it will open, read and store all of the elements from the file
 it will also find the top ranked words and print them to the terminal
 */
void menuOption1(char *answersFileName, int *answersSize, wordCountStruct *answersList, char *guessesFileName, int *guessesSize, wordCountStruct *guessesList)
{
    // counting elements in both files
    *answersSize = countFileElements(answersFileName);
    *guessesSize = countFileElements(guessesFileName);
    
    //printing file names and sizes
    printf("%s has %d words\n", answersFileName, *answersSize);
    printf("%s has %d words\n", guessesFileName, *guessesSize);
    
    // reading and storing the answers list
    answersList = malloc( sizeof( wordCountStruct) * *answersSize);
    readFileElements(answersFileName, answersList, answersSize);
    
    // reading and stroing the guesses list
    guessesList = malloc( sizeof( wordCountStruct) * (*answersSize + *guessesSize));
    readFileElements(guessesFileName, guessesList, guessesSize);
    
    // adding the answers list to the guesses lsit
    concatLists(guessesList, guessesSize, answersList, answersSize);
    
    // finding the top scoring first words
    firstWordsOnly(guessesList, guessesSize, answersList, answersSize);
    
    // holds the top score value
    int maxScore = guessesList[0].score;
    // holds the number of words with the top score
    int numberOfMaxScore = 0;
    
    // printing out each element that has the top score in the correct format
    printf("\nWords and scores for top first words:");
    while (maxScore == guessesList[numberOfMaxScore].score)
    {
        printf("\n%s %d", guessesList[numberOfMaxScore].word, guessesList[numberOfMaxScore].score);
        numberOfMaxScore++;
    }
    printf("\n");
}

// -----------------------------------------------------------------------------------------
/*
 menuOption1 takes a pointer to the answer file name, answer file size,  answersList
 wordCountStruct, guesses file name, guesses size and guesses list wordCountStruct
 this funciton will not return anything
 it will open, read and store all of the elements from the file
 it will also find the top ranked first and second words and print them to the terminal
 */
void menuOption2(char *answersFileName, int *answersSize, wordCountStruct *answersList, char *guessesFileName, int *guessesSize, wordCountStruct *guessesList)
{
    // counting the elements in both files
    *answersSize = countFileElements(answersFileName);
    *guessesSize = countFileElements(guessesFileName);
    
    // printing the file names and number of elements
    printf("%s has %d words\n", answersFileName, *answersSize);
    printf("%s has %d words\n", guessesFileName, *guessesSize);
    
    // reading and storing the answer list
    answersList = malloc( sizeof( wordCountStruct) * *answersSize);
    readFileElements(answersFileName, answersList, answersSize);
    
    // reading and storing the guesses list
    guessesList = malloc( sizeof( wordCountStruct) * (*answersSize + *guessesSize));
    readFileElements(guessesFileName, guessesList, guessesSize);
    
    // adding answers list to the guesses lsit
    concatLists(guessesList, guessesSize, answersList, answersSize);
    
    // finding the top scoring first words
    firstWordsOnly(guessesList, guessesSize, answersList, answersSize);
    
    // holds top first word score
    int maxScore = guessesList[0].score;
    // holds the top second word score
    int maxSecondScore = 0;
    // holds the number of word wiht the top first word score
    int numberOfMaxScore = 0;
    // index for iterating through top first words
    int firstWordIndex = 0;
    // index for iterating through top second words
    int secondWordIndex = 0;
    
    // finding the number of top first words
    printf("\nWords and scores for top first words and second words:");
    while (maxScore == guessesList[numberOfMaxScore].score)
    {
        numberOfMaxScore++;
    }
    
    // iterating through top first words
    for( firstWordIndex = 0; firstWordIndex < numberOfMaxScore; firstWordIndex++)
    {
        // re-finding top first words and printing it
        firstWordsOnly(guessesList, guessesSize, answersList, answersSize);
        printf("\n%s %d\n", guessesList[firstWordIndex].word, guessesList[firstWordIndex].score);
        
        //finding top second word for each first word
        firstAndSecondWord(guessesList, guessesSize, answersList, answersSize, &firstWordIndex);
        
        // iterating through top second words for each top first word
        secondWordIndex = 0;
        maxSecondScore = guessesList[0].score;
        while( maxSecondScore == guessesList[secondWordIndex].score)
        {
            // prinitng top second words
            printf("\t%s %d", guessesList[0].word, guessesList[secondWordIndex].score);
            secondWordIndex++;
        }
        
    }
    
    printf("\n");
}


// -----------------------------------------------------------------------------------------
int main() {
    char answersFileName[81];  // Stores the answers file name
    char guessesFileName[81];  // Stores the guesses file name
    // Set default file names
    strcpy(answersFileName, "answersTiny.txt");
    strcpy(guessesFileName, "guessesTiny.txt");
    
    printf("Default file names are %s and %s\n", answersFileName, guessesFileName);

    // Finding the size of the answers and guesses files
    int answersSize = 0;
    int guessesSize = 0;
    
    // delcating wordCountStructs for boht lists
    wordCountStruct *answersList = NULL;
    wordCountStruct *guessesList = NULL;
    
    // Display menu, to allow partial credit for different program components
    int menuOption = 0;
    do
    {
        printf("\n");
        printf("Menu Options:\n");
        printf("  1. Display best first words only\n");
        printf("  2. Display best first and best second words\n");
        printf("  3. Change answers and guesses filenames\n");
        printf("  4. Exit\n");
        printf("Your choice: ");
        scanf("%d", &menuOption);

        // Handle menu option 3 to exit the program
        if (menuOption == 4)
        {
            exit(1); // Exit the program
        }
        
        else if (menuOption == 3)
        {
            // Change file names.  Menu will then be redisplayed.
            printf("Enter new answers and guesses filenames: ");
            scanf("%s %s", answersFileName, guessesFileName);
        }
        else if (menuOption == 2)
        {
            menuOption2(answersFileName, &answersSize, answersList, guessesFileName, &guessesSize, guessesList);
            
        }
        else if (menuOption == 1)
        {
            menuOption1(answersFileName, &answersSize, answersList, guessesFileName, &guessesSize, guessesList);
        }
    } while( menuOption == 3);

    printf("Done\n");
    return 0; //keep C happy
} // end main()
