//
//  main.cpp
//  Project 5
//
//  Created by James Wu on 5/9/22.
//

#include <iostream>
#include <cassert>
#include <cstring>
using namespace std;

const int MAX_WORD_LENGTH = 20;
const int MAX_JEET_LENGTH = 280;

/**
 distance[] : Array of distance values
 word1[][]: Array of C-Strings, where each string has a max length of MAX_WORD_LENGTH (not including 0 byte)
 word2[][]: Same as above
 nStandards: How many total collections there are
 */
int editStandards(int distance[],
                  char word1[][MAX_WORD_LENGTH+1],
                  char word2[][MAX_WORD_LENGTH+1],
                  int nStandards){
    
    if(nStandards < 0)
        nStandards = 0;
    
    //Note: nStandards is amount of standards to be returned
    
    //Iterate through C-strings, accounting for 0 byte
    for(int i = 0; i < nStandards; i++){
        
        //Distance is not positive, delete entry and move everything over. Alternatively, if string is empty, delete entry as well.
        //We don't care about what's at the end of the array.
        if(distance[i] < 0 || strcmp(word1[i], "")==0 || strcmp(word2[i], "")==0){
            //Move every element in every array to the left by one, starting where specified to get rid of the entire collection
            for(int j = i; j < nStandards - 1; j++){
                distance[j] = distance[j+1];
                strcpy(word1[j], word1[j+1]);
                strcpy(word2[j], word2[j+1]);
            }
            //One collection has been removed, return one less item
            nStandards--;
            //Catch the case if next item is also negative
            i--;
            continue;
        }
        
        //Iterate over each character in the string for word1-- if uppercase, change to lower. If non-alphabetical, remove entire collection.
        for(int j = 0; word1[i][j] != '\0'; j++){
            //If word contains non-alphabetic character, remove every element in collection
            if(!isalpha(word1[i][j])){
                //Move every element in every array to the left by one, starting where specified to get rid of the entire collection
                for(int j = i; j < nStandards - 1; j++){
                    distance[j] = distance[j+1];
                    strcpy(word1[j], word1[j+1]);
                    strcpy(word2[j], word2[j+1]);
                }
                //One collection has been removed, return one less item
                nStandards--;
                //Catch the case if next item is also negative
                i--;
                continue;
            }
            //Otherwise, make character lowercase
            else{
                word1[i][j] = tolower(word1[i][j]);
            }
        }
        
        //Iterate over each character in string for word2. If uppercase, change to lower. If non-alpha, remove collection.
        for(int j = 0; word2[i][j] != '\0'; j++){
            //If word contains non-alphabetic character, remove every element in collection
            if(!isalpha(word2[i][j])){
                //Move every element in every array to the left by one, starting where specified to get rid of the entire collection
                for(int j = i; j < nStandards - 1; j++){
                    distance[j] = distance[j+1];
                    strcpy(word1[j], word1[j+1]);
                    strcpy(word2[j], word2[j+1]);
                }
                //One collection has been removed, return one less item
                nStandards--;
                //Catch the case if next item is also negative
                i--;
                continue;
            }
            //Otherwise, make character lowercase
            else{
                word2[i][j] = tolower(word2[i][j]);
            }
        }
    }
    
    //Remove any duplicate standards
    for(int i = 0; i < nStandards-1; i++){
        
        for(int j = i+1; j < nStandards; j++){
            //If two standards have the same w1 and w2 values, remove the one whose distance is less
            if(strcmp(word1[i], word1[j]) == 0 && strcmp(word2[i], word2[j])==0){
                
                int pos;
                
                //Assign pos to the lesser distance (the position we want to remove)
                if(distance[i] > distance[j]){
                    pos = j;
                }
                else
                    //distance values are either equal (in which case it doesn't matter which is removed) or dist[i] is less than dist[j]
                    pos = i;
                                
                //Move every element in every array to the left by one, starting where specified to get rid of the entire collection
                //Removes the index at the smaller value
                for(pos; pos < nStandards - 1; pos++){
                    distance[pos] = distance[pos+1];
                    strcpy(word1[pos], word1[pos+1]);
                    strcpy(word2[pos], word2[pos+1]);
                }
                //One collection has been removed, return one less item
                nStandards--;
                //Catch the case if next item also has a copy
                i--;
                continue;
            }
        }
    }
    
    
    return nStandards;
}


//Assume every jeet is already in standard form.
/**
 distance[] : Array of distance values
 word1[][]: Array of C-Strings, where each string has a max length of MAX_WORD_LENGTH (not including 0 byte)
 word2[][]: Same as above
 nStandards: How many total collections there are
 jeet[]: C String containing entire text of a jeet. Length of every jeet (excluding 0 byte)
 */
//Returns number of match standards that the jeet matches
//NOTE: Unsure if this works for the case where the jeet is all spaces.
int determineMatchLevel(const int distance[],
                        const char word1[][MAX_WORD_LENGTH+1],
                        const char word2[][MAX_WORD_LENGTH+1],
                        int nStandards,
                        const char jeet[]){
    if(nStandards < 0)
        nStandards = 0;
        
    //Make an array of C-Strings
    char wordsArray[MAX_JEET_LENGTH + 1][MAX_JEET_LENGTH + 1];
    
    //Initialize wordsArray to null byte
    for(int i = 0; i < MAX_JEET_LENGTH + 1; i++){
        strcpy(wordsArray[i], "\0");
    }
    
    int wordsArrayIndex = 0;
    int currentWordIndex = 0;
    
    //Separate each word in jeet and put it into an array
        
    bool entryIsBlank = true;
    
    //Temp C-String to hold the current word
    char currentWord[MAX_JEET_LENGTH+1] = "\0";
    
    //Loop through each character of the jeet
    for(int i = 0; i < strlen(jeet) + 1; i++){
  
        //If character of jeet is alphabetical, append lowercase version
        if(isalpha(jeet[i])){
            currentWord[currentWordIndex] = tolower(jeet[i]);
            currentWordIndex++;
            entryIsBlank = false;
        }
        //If char is a space or 0 byte, end current word
        else if((isspace(jeet[i]) || jeet[i] == '\0') && entryIsBlank == false){
            currentWord[currentWordIndex] = '\0';
            strcpy(wordsArray[wordsArrayIndex], currentWord);
            wordsArrayIndex++;
            currentWordIndex = 0;
            entryIsBlank = true;
        }
        //Otherwise, the char is a number or symbol or consecutive space. Skip this.
        else{
            continue;
        }
        
    }
    
    //Now, every word in the jeet is contained in wordsArray.
    
//    Debugging code to print wordsArray
//    for(int k = 0; k < MAX_JEET_LENGTH + 1; k++){
//        if(strcmp(wordsArray[k], "") == 0)
//            break;
//        else
//            cout << wordsArray[k] << endl;
//    }

    int totalMatches = 0;
    
    //Now, iterate through word1 array and compare to see if each standard is present
    for(int k = 0; k < nStandards; k++){
        for(int z = 0; z < MAX_JEET_LENGTH + 1; z++){
            
            bool breakAll = false;
//            cerr << "word 1: " << word1[k] << endl;
//            cerr << "wordsArray: " << wordsArray[z] << endl;
//            cerr << "word 2: " << word2[k] << endl;
//            cerr << endl;
//            If the jeet is at its end, leave the loop
            if(strcmp(wordsArray[z], "") == 0)
                break;
            else if(strcmp(wordsArray[z], word1[k]) == 0){
                //Iterate "distance" amount of times through jeet to see if word2 exists
                for(int d = z+1; d <= z + distance[k]; d++){
//                    cerr << "wordsArray: " << wordsArray[d] << endl;
                    if(strcmp(wordsArray[d], word2[k]) == 0){
                        totalMatches++;
                        breakAll = true;
                        break;
                    }
                }
            }
            //This means that a match was found and we can move on to the next standard
            if(breakAll)
                break;
        }
    }
    
    return totalMatches;
}



int main(int argc, const char * argv[]) {

    //EDIT STANDARDS FUNCTION TEST
    //Remove standards where distance is not positive, word contains no characters, contains a character that isn't a letter, and duplicate standards.
    //Return amount of number of standards in set
    const int EDITSTANDARDS_NSTANDARDS = 10;
    int editStandardsDistance[EDITSTANDARDS_NSTANDARDS] = {4, 10, 2, 0, -1, 5, 1, 2, 60, 22};
    char word1[EDITSTANDARDS_NSTANDARDS][MAX_WORD_LENGTH + 1] = {"Super", "Duper", "Awesome", "super", "f&mily", "", "", "12324", "super", "hello" };
    char word2[EDITSTANDARDS_NSTANDARDS][MAX_WORD_LENGTH + 1] = {"man", "california", "8wesome", "hii", "boogie", "po", "panda_", "thisIsValid?", "trebuchet", "simp" };
    assert(editStandards(editStandardsDistance, word1, word2, EDITSTANDARDS_NSTANDARDS) == 5);
//    //Print array to see if contents have been shifted
//    for(int i = 0; i < EDITSTANDARDS_NSTANDARDS; i++){
//        cout << editStandardsDistance[i] << " " << word1[i] << " " << word2[i] << endl;
//    }
    
    //These tests test nonpositive distances for editStandards
    const int nStandards = 5;
    int editStandardsDistance1[nStandards] = {0, 1, -1, 2, -3};
    int editStandardsDistance2[nStandards] = {-1, 0, 0, 3, 5};
    int editStandardsDistance3[nStandards] = {-1, -2, -3, -4, -5};
    char case1Word1[nStandards][MAX_WORD_LENGTH + 1] = {"hi", "this", "is", "very", "valid"};
    char case1Word2[nStandards][MAX_WORD_LENGTH + 1] = {"yes", "this", "is", "valid", "string"};
    //Comment out the assertion you want to test (that way you can see one by one if it's valid, or else the array will get shifted around)
//    assert(editStandards(editStandardsDistance1, case1Word1, case1Word2, nStandards) == 3);
//    for(int i = 0; i < nStandards; i++){
//        cout << editStandardsDistance1[i] << " " << case1Word1[i] << " " << case1Word2[i] << endl;
//    }
    
//    assert(editStandards(editStandardsDistance2, case1Word1, case1Word2, nStandards) == 4);
//    for(int i = 0; i < nStandards; i++){
//        cout << editStandardsDistance2[i] << " " << case1Word1[i] << " " << case1Word2[i] << endl;
//    }
//
//    assert(editStandards(editStandardsDistance3, case1Word1, case1Word2, nStandards) == 0);
//    for(int i = 0; i < nStandards; i++){
//        cout << editStandardsDistance1[i] << " " << case1Word1[i] << " " << case1Word2[i] << endl;
//    }
//    assert(editStandards(editStandardsDistance1, case1Word1, case1Word2, -5) == 0); //Test nStandards negative-- should equal 0

    //Test empty string
    char empty[nStandards][MAX_WORD_LENGTH+1] = {"", "Hi", "", "", "lol"};
    char empty2[nStandards][MAX_WORD_LENGTH+1] = {"", "", "", "", ""};
    char nonEmptyWord2[nStandards][MAX_WORD_LENGTH+1] = {"a", "b", "c", "d", "e"};
    int validDist[nStandards] = {1, 2, 3, 4, 5};
    //Comment out the assertions you want to test
//    assert(editStandards(validDist, empty, nonEmptyWord2, nStandards) == 2);
//    for(int i = 0; i < nStandards; i++){
//        cout << validDist[i] << " " << empty[i] << " " << nonEmptyWord2[i] << endl;
//    }
//    assert(editStandards(validDist, empty2, nonEmptyWord2, nStandards) == 0);
//    for(int i = 0; i < nStandards; i++){
//        cout << validDist[i] << " " << empty2[i] << " " << nonEmptyWord2[i] << endl;
//    }
    
    //Test non-alpha chars
    int newDist[3] = {5, 6, 20};
    char nonAlpha1[3][MAX_WORD_LENGTH+1] = {"&Hi", "1234", "Test_HI"};
    char nonAlpha2[3][MAX_WORD_LENGTH+1] = {"valid", "not valid", "thisValid"};
    char alpha[3][MAX_WORD_LENGTH+1] = {"valid", "valid", "valid"};
    //Comment out assertions you want to test
//    assert(editStandards(newDist, nonAlpha1, alpha, 3) == 0);
//    for(int i = 0; i < 3; i++){
//        cout << newDist[i] << " " << nonAlpha1[i] << " " << alpha[i] << endl;
//    }
    
//    assert(editStandards(newDist, nonAlpha2, alpha, 3) == 2);
//    for(int i = 0; i < 3; i++){
//        cout << newDist[i] << " " << nonAlpha2[i] << " " << alpha[i] << endl;
//    }
    
    //Test duplicate w1 and w2
    int dupDist[6] = {5, 10, 15, 20, 25, 100};
    char word1Dup1[6][MAX_WORD_LENGTH+1] = {"I", "Am", "super", "happy", "I", "Am"};
    char word2Dup1[6][MAX_WORD_LENGTH+1] = {"or", "am", "I", "test", "or", "Am"};
    char word1Dup2[6][MAX_WORD_LENGTH+1] = {"Boot", "foot", "Boot", "foot", "Boot", "foot"};
    char word2Dup2[6][MAX_WORD_LENGTH+1] = {"Foot", "Boot", "FOOT", "FOOT", "Boot", "foot"};
    //Comment out assertions you want to test
//    assert(editStandards(dupDist, word1Dup1, word2Dup1, 6) == 4);
//    for(int i = 0; i < 6; i++){
//        cout << dupDist[i] << " " << word1Dup1[i] <<" " << word2Dup1[i]<<endl;
//    }
    
//    assert(editStandards(dupDist, word1Dup2, word2Dup2, 6) == 4);
//    for(int i = 0; i < 6; i++){
//        cout << dupDist[i] << " " << word1Dup2[i] <<" " << word2Dup2[i]<<endl;
//    }

    
    //DETERMINE MATCH LEVEL FUNCTION TEST
    //Return match level of the jeet, treating uppercase chars as lowercase and ignoring non-alphabetic characters
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
    
    
    //Custom cases with more jeets
    const int TEST2_NSTANDARDS = 8;
    int test2dist[TEST2_NSTANDARDS] = {2, 30, 2, 3, 2, 3, 10, 3};
    char test2w1[TEST2_NSTANDARDS][MAX_WORD_LENGTH+1] = {
        "have",   "you",  "i", "d123arth", "wise", "thought", "not", "tragedy"
    };
    char test2w2[TEST2_NSTANDARDS][MAX_WORD_LENGTH+1] = {
        "heard", "i", "thought", "the", "i", "not", "have", "you"
    };
    assert(determineMatchLevel(test2dist, test2w1, test2w2, TEST2_NSTANDARDS, "Have you heard the tragedy of Darth Plagueis the wise? I thought not.") == 5);
    
    const int TEST3_NSTANDARDS = 3;
    int test3dist[TEST3_NSTANDARDS] = {1, 2, 3};
    char test3w1[TEST3_NSTANDARDS][MAX_WORD_LENGTH + 1] = {"hi", "how", "hi"};
    char test3w2[TEST3_NSTANDARDS][MAX_WORD_LENGTH + 1] = {"there", "how", "you"};
    
//    cout << determineMatchLevel(test3dist, test3w1, test3w2, TEST3_NSTANDARDS, "Hi there how are you") << endl;
    
    assert(determineMatchLevel(test3dist, test3w1, test3w2, TEST3_NSTANDARDS, "Hi there how are you") == 1);
    
    cout << "All tests succeeded" << endl;
    
    return 0;
}
