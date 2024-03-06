//
//  jitter.cpp
//  jitter
//
//  Created by Chris Head on 5/13/22.
//

#include <iostream>
#include <cstring>
#include <cassert>
using namespace std;

const int MAX_WORD_LENGTH = 20;

int editStandards(int distance[], char word1[][MAX_WORD_LENGTH+1], char word2[][MAX_WORD_LENGTH+1], int nStandards);
int determineMatchLevel(int distance[], char word1[][MAX_WORD_LENGTH+1], char word2[][MAX_WORD_LENGTH+1], int nStandards, const char jeet[]);
void compWord(char editJeet[], char word[], int& k);


int editStandards(int distance[], char word1[][MAX_WORD_LENGTH+1], char word2[][MAX_WORD_LENGTH+1], int nStandards){
    
    int counter = 0;
    int total = nStandards;
    
    
    if (nStandards < 0)
        nStandards = 0;
    
    //first for loop checks for all of the cases in which the c string contains an error
    for (int i = 0; i < nStandards - counter; i++)
    {
        int standardForm = 1;    //will change to 0 if any violations are made
        
        if (distance[i] < 0)
        {
            standardForm = 0;
        }
        if (word1[i][0] == '\0' || word2[i][0] == '\0') //a word has no characters
        {
            standardForm = 0;
        }
        
        //next for loop checks to see if first word contains only letters
        for (int k = 0; word1[i][k] != '\0'; k++){
            if (isalpha(word1[i][k]))
            {
                tolower(word1[i][k]);
                continue;
            }
            else
            {
                standardForm = 0;
            }
        }
        
        //same as last loop but for second word
        for (int j = 0; word2[i][j] != '\0'; j++){
            if (isalpha(word2[i][j]))
            {
                tolower(word2[i][j]);
                continue;
            }
            else
            {
                standardForm = 0;
            }
        }
        
        //checks for the case in which there are 2 of the same standards
            for (int a = 0; a < nStandards - counter; a++){
                if (a != i){
                if ((strcmp(word1[a], word1[i]) == 0 && strcmp(word2[a], word2[i]) == 0))
                {
                    if (distance[a] >= distance[i])
                    {
                        standardForm = 0;
                    }
                    else
                    {
                        strcpy(word1[a], word1[i]);
                        strcpy(word2[a], word2[i]);
                        distance[a] = distance[i];
                        standardForm = 0;
                    }
            
                }
                }
        }
        
        
        //in case of non standard form
        if (standardForm == 0)
        {
            strcpy(word1[i], word1[nStandards - counter - 1]);
            strcpy(word2[i], word2[nStandards - counter - 1]);
            distance[i] = distance[nStandards - counter - 1];
            i--;
            counter++;
        }
    }
   
    total -= counter;
    return total;
    
}




int determineMatchLevel(int distance[], char word1[][MAX_WORD_LENGTH+1], char word2[][MAX_WORD_LENGTH+1], int nStandards, const char jeet[]){
    char editJeet[280];
    char word[MAX_WORD_LENGTH+1] = {'\0'};     //had to set equal to fix errors
    int count = 0;   //keeps a different count for our temporary array
    int space = 0;   //to get rid of double spaces
    int numMatches = 0;
     
    
    if (nStandards < 0)
        nStandards = 0;
    
    
    
    //loop copies jeet into temporary array with no extraneous characters
    for (int i = 0; jeet[i] != '\0'; i++)
    {
        if (isalpha(jeet[i]))
        {
            editJeet[count] = tolower(jeet[i]);
            count++;
            space = 0;
        }
        if (jeet[i] == ' ')
        {
            if (space == 1)    //skips the extra space if there are two spaces
            {
                continue;
            }
            else
            {
                editJeet[count] = jeet[i];
                count++;
                space = 1;
            }
        }
    }
    editJeet[count] = '\0';
    
    
    
    
    for (int i = 0; i < nStandards; i++)
    {
        int k = 0;        //corresponds with the int k in the compWord function
        int breaker = 0;  //used to break out of the while loop
        
        
        //loop to check if words are in the jeet and within the correct distance from eachother
        while (breaker == 0 && editJeet[k] != '\0')
        {
        compWord(editJeet, word, k);
        
        if (strcmp(word1[i], word) == 0)
        {
            
            for (int j = 0; j < distance[i]; j++)
            {
                compWord(editJeet, word, k);
                
                if(strcmp(word2[i], word) == 0)
                {
                    numMatches++;
                    breaker = 1;
                    break;
                }
            }
        }
        
        }
    }
    
    return numMatches;
}


//function to store the current word in an array for comparison
void compWord(char editJeets[], char words[], int& k){
    
    
    
    int index = 0;
    while (editJeets[k] != '\0')
    {
        if (editJeets[k] == ' ')
        {
            k++;
            break;
        }
        if (islower(editJeets[k]))
        {
            words[index] = editJeets[k];
            index++;
            k++;
        }
    }
    
    words[index] = '\0';
    
    
}




int main() {
    const int TEST1_NSTANDARDS = 4;
               int test1dist[TEST1_NSTANDARDS] = {
                   2,             4,         1,         13
               };
               char test1w1[TEST1_NSTANDARDS][MAX_WORD_LENGTH+1] = {
                   "eccentric",   "space",  "electric", "were"
               };
               char test1w2[TEST1_NSTANDARDS][MAX_WORD_LENGTH+1] = {
                   "billionaire", "capsule", "car",     "eccentric"
               };
               assert(determineMatchLevel(test1dist, test1w1, test1w2, TEST1_NSTANDARDS,
                   "The eccentric outspoken billionaire launched a space station cargo capsule.") == 2);
               assert(determineMatchLevel(test1dist, test1w1, test1w2, TEST1_NSTANDARDS,
                   "The eccentric outspoken billionaire launched    a space capsule.") == 2);
               assert(determineMatchLevel(test1dist, test1w1, test1w2, TEST1_NSTANDARDS,
                   "**** 2022 ****") == 0);
               assert(determineMatchLevel(test1dist, test1w1, test1w2, TEST1_NSTANDARDS,
                   "  It's an ELECTRIC car!") == 1);
               assert(determineMatchLevel(test1dist, test1w1, test1w2, TEST1_NSTANDARDS,
                   "space space capsule space capsule capsule") == 1);
               assert(determineMatchLevel(test1dist, test1w1, test1w2, TEST1_NSTANDARDS,
                   "Two eccentric billionaires were space-capsule riders.") == 0);
               cout << "All tests succeeded" << endl;
}
