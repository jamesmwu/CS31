//
//  main.cpp
//  hero
//
//  Created by James Wu on 4/22/22.
//

#include <iostream>
#include <cassert>
using namespace std;

//Functions

//Returns true if parameter is syntactically correct
bool hasProperSyntax(string tune){
   
    //Before anything just check to see if there's any white space. White space is invalid syntax.
    for(size_t i = 0; i < tune.size(); i++){
        string temp;
        temp += tune.at(i);
        
        if(temp == " ")
            return false;
    }
    
    bool currentIndexHasNumber = false;

    //Iterate through the string, assuming string length > 0
    for(size_t i = 0; i < tune.size(); i++){
        char current = tune.at(i);
        char next1;
        
        //If the current index has number, see if the next index is also a number (2 digits).
        if(currentIndexHasNumber){
            //First ensure that the string doesn't end after the number, in which case there is invalid syntax
            if(i+1 >= tune.size())
                return false;
            //If the next character isn't a slash or a digit, invalid character detected.
            else if(tune.at(i+1) != '/' && !isdigit(tune.at(i+1)))
                return false;
            //If the next character is a slash, reset and continue testing for new beats.
            else if(tune.at(i+1) == '/')
                currentIndexHasNumber = false;
            
            //If the end of this if block is reached, that means the next character is also a number. In that case, we can continue iterating through the loop.
        }
        else{
            //Determine if the character is accepted
            switch(current){
                //These beats are accepted
                case 'g':
                case 'G':
                case 'r':
                case 'R':
                case 'y':
                case 'Y':
                case 'b':
                case 'B':
                case 'o':
                case 'O':
                    
                    //If we're at the end of the string and there's no / to follow the beat, invalid syntax
                    if(i+1 >= tune.size())
                        return false;
                    else{
                        next1 = tune.at(i+1);
                        
                        //If the next character isn't a slash and isn't a digit, invalid syntax
                        if(next1 != '/' && !isdigit(next1))
                            return false;
                        
                        else if(isdigit(next1))
                        //This is insurance in case there's a two digit number following the beat.
                        currentIndexHasNumber = true;
                        
                        //If the end of this if block is reached, that means the next character is a slash.
                    }
                    
                    break;
                    
                // slash is accepted
                case '/':
                    break;
                default:
                    return false;
            }
        }
    }
    
    //If the entire string has been iterated through and there are no issues found in syntax, then true is returned.
    return true;
}


/*
 
- If "tune" is convertible, set instructions to conversion of tune, leave badBeat unchanged, return 0. In all other cases, leave instructions unchanged. (CASE 0) // COMPLETE
 - If tune is syntactically incorrect, leave badBeat unchanged, return 1. (CASE 1) // COMPLETE
 - If different beat is detected during sustained beat, set badBeat equal to the number of that beat and return 2. (CASE 2) // COMPLETE
 - If sustained beat has length less than 2, set badBeat equal to number of that beat and return 3. (CASE 3) // COMPLETE
 - If tune ends prematurely, then set badBeat equal to number of beats + 1 and return 4. (CASE 4) // COMPLETE
 
 */
int convertTune(string tune, string& instructions, int& badBeat){
    
    //To help count the amount of beats iterated throughout the tune.
    int countBeat = 0;
    
    //String that will contain converted tune.
    string appendInstructions = "";
    
    //Determine if the numbers specified are double digit or not.
    bool doubleDigits = false;
    int doubleDigitNumber = 0;
    
    //Tune must be syntactically correct, return 1 if incorrect.
    if(!hasProperSyntax(tune)){
        return 1;
    }

    //Determine if tune is convertible, returning 2, 3, and 4 for appropriate cases if not. Otherwise, set instructions to conversion and return 0.
    //Recall every tune that is being iterated here is syntactically correct, at least.
    for(int i = 0; i < tune.size(); i++){
        char current = tune.at(i);
        
        if(current == '/'){
            countBeat++;
        }
        //All of these errors are dependent on a number (because everything is syntactically correct). Iterate until you find a number.
        else if(isdigit(current)){
            //This gives us value of the current number, because the difference between current and 0 will be the int value (difference in ASCII equals difference in actual numerical value)
            int currentNum = current - '0';
            
            //If currentNum is less than 2, there needs to be an adjacent digit otherwise not convertible.
            if(currentNum < 2 && doubleDigits == false){
                
                //Determine if there is an adjacent digit. Since this is syntactically correct, next char HAS to either be a digit or slash.
                char next1 = tune.at(i+1);
                if(next1 == '/'){
                    //currentNum is less than 2 and the next char is a /, so this is not convertible. (CASE 3)
                    //badBeat should equal the number of the beat the tune is currently on. That will equal the current beat + 1.
                    badBeat = countBeat + 1;
                    return 3;
                }
                else{
                    doubleDigits = true;
                    doubleDigitNumber = currentNum * 10;
                }

            }
            //Valid number-- determine if the amount of beats following it is correct.
            else{
                
                //Determine if there's a double digit number
                char next1 = tune.at(i+1);
                if(isdigit(next1)){
                    doubleDigitNumber = currentNum * 10;
                    doubleDigits = true;
                }
                else{
                    
                    int currentIndex = i+1;
                    int tempCounter = countBeat;
                    
                    currentNum += doubleDigitNumber;
                    doubleDigits = false;
                    doubleDigitNumber = 0;
                    
                    //Catch the specific test case 00.
                    if(currentNum == 0){
                        //currentNum is less than 2 and the next char is a /, so this is not convertible. (CASE 3)
                        //badBeat should equal the number of the beat the tune is currently on. That will equal the current beat + 1.
                        badBeat = countBeat + 1;
                        return 3;
                    }
                    
                    //Iterate the amount of times specified by the number-- this is how many beats should be detected.
                    for(int j = 0; j < currentNum; j++){
                        
                        tempCounter++;

                        //This conditional catches the out of bounds error (that occurs due to a premature ending). The program is expecting there to be another beat, but does not find one because the index it's looking for does not exist. Case 4.
                        if(currentIndex + 1 > tune.size()){
                            //Add two here because tempCounter hasn't had the chance to iterate one more time-- the beat where issue is encountered shuold be at location of next index (which doesn't exist), so add one.
                            badBeat = tempCounter++;
                            return 4;
                        }
                        //If the beat is interrupted by a non-slash character, case 2. Return 2, set badBeat equal to location of interruption. THIS IS ONLY VALID ASSUMING THAT THE TUNE IS NOT ENDING PREMATURELY. If tune ends prematurely, return 4.
                        else if(tune.at(currentIndex) != '/'){
                            //Need to add one here because the beat where the issue is encountered should be at the location of the next index, so add one.
                            badBeat = tempCounter++;
                            return 2;
                        }

                        currentIndex++;
                    }
                }

            }
        }
    }
    
    //If program is executing code here, that means the tune is convertible!
    //Write out instructinos.
        
    for(int i = 0; i < tune.size(); i++){
        char current = tune.at(i);
        
        //If the character is a letter AND the next thing following that letter isn't a digit, then append the letter. This implies that there is a solo beat. Additionally, this shouldn't result in an overflow error because if the current char isn't a letter, the conditional will immediately terminate without testing if the next entry is a number.
        if(isalpha(current) && !isdigit(tune.at(i+1))){
            //Append
            switch(current){
                case 'g':
                case 'G':
                    appendInstructions += "g";
                    break;
                case 'r':
                case 'R':
                    appendInstructions += "r";
                    break;
                case 'y':
                case 'Y':
                    appendInstructions += "y";
                    break;
                case 'b':
                case 'B':
                    appendInstructions += "b";
                    break;
                case 'o':
                case 'O':
                    appendInstructions += "o";
                    break;
                default:
                    cerr<<"Something went wrong"<<endl;
                    
            }
            //These are for solo notes, so they will always be followed by a slash.
            //Iterate index up by one to account for the slash being appended.
            i++;
        }
        //If the beat is a slash, append "x".
        else if(current == '/'){
            appendInstructions += "x";
        }
        //There is a sustained beat. All these tunes must be convertible (previous loop above would've caught exceptions by now.)
        else {
            
            string note = "";
            
            switch(current){
                case 'g':
                case 'G':
                    note = "G";
                    break;
                case 'r':
                case 'R':
                    note = "R";
                    break;
                case 'y':
                case 'Y':
                    note = "Y";
                    break;
                case 'b':
                case 'B':
                    note = "B";
                    break;
                case 'o':
                case 'O':
                    note = "O";
                    break;
                default:
                    cerr<<"Something went wrong"<<endl;
            }
            
            //Iterate up by one and reset current to hold the number
            i++;
            current = tune.at(i);
            
            int currentNum = current - '0';
            //If there is a double digit, note necessary amount of beats. Then put index right before the slash of converted string
            if(isdigit(tune.at(i+1))){
                currentNum = currentNum * 10;
                currentNum += tune.at(i+1) - '0';
                i += 2;
            }
            else
                i++;
            
            //Iterate through amount of slashes and append to instructions.
            for(int j = i; j < currentNum + i; j++){
                appendInstructions += note;
            }
            
            //Moves index to account for slashes appended. Minus one because i is still going to be increased by the loop after this.
            i += currentNum - 1;
        }
    }
    
    
    instructions = appendInstructions;
    return 0;
}

int main(int argc, const char * argv[]) {
    
    string instructions = "Default";
    int badBeat = -999;
    
   //Test whitespace
       string test1 = "";
       string test2 = "     ";
    
    assert(convertTune(test1, instructions, badBeat) == 0 && instructions == "" && badBeat == -999);
    
    instructions = "Default";
    
    assert(convertTune(test2, instructions, badBeat) == 1 && instructions == "Default" && badBeat == -999);

    instructions = "Default";
    badBeat = -999;
    
   //Case 1: syntactically incorrect
       string test3 = "lol";
       string test4 = "r/3o/y";
    
    assert(convertTune(test3, instructions, badBeat) == 1 && instructions == "Default" && badBeat == -999);
    assert(convertTune(test4, instructions, badBeat) == 1 && instructions == "Default" && badBeat == -999);
    
   //Case 2: Different beat detected during sustained beat
       string test5 = "r/Y/g3///o5//r/g6//////o2//y/";
       string test6 = "r3//y/b2//o2//";
    
    assert(convertTune(test5, instructions, badBeat) == 2  && instructions == "Default" && badBeat == 8);
    assert(convertTune(test6, instructions, badBeat) == 2 && instructions == "Default" && badBeat == 3);

    badBeat = -999;
    
   //Case 3: Sustained beat has length less than 2
       string test7 = "o2//y00/r6//////";
       string test8 = "r5/////G/b1//o/";
    
    assert(convertTune(test7, instructions, badBeat) == 3 && instructions == "Default" && badBeat == 3);
    assert(convertTune(test8, instructions, badBeat) == 3 && instructions == "Default" && badBeat == 7);

    badBeat = -999;
    
   //Case 4: Tune ends prematurely
       string test9 = "o2//y8////";
       string test10 = "g/B/O/r/b5//";
    
    assert(convertTune(test9, instructions, badBeat) == 4 && instructions == "Default" && badBeat == 7);
    assert(convertTune(test10, instructions, badBeat) == 4 && instructions == "Default" && badBeat == 7);

    badBeat = -999;

   //Case 0: Convertible tune
       string test11 = "G/";
       string test12 = "///";
       string test13 = "r/Y/g6//////y10//////////r4////";
       string test14 = "r3///y/b/O2//";
       string test15 = "r2//b20////////////////////g03///r05/////y/";

    assert(convertTune(test11, instructions, badBeat) == 0 && instructions == "g" && badBeat == -999);
    assert(convertTune(test12, instructions, badBeat) == 0 && instructions == "xxx" && badBeat == -999);
    assert(convertTune(test13, instructions, badBeat) == 0 && instructions == "ryGGGGGGYYYYYYYYYYRRRR" && badBeat == -999);
    assert(convertTune(test14, instructions, badBeat) == 0 && instructions == "RRRybOO" && badBeat == -999);
    assert(convertTune(test15, instructions, badBeat) == 0 && instructions == "RRBBBBBBBBBBBBBBBBBBBBGGGRRRRRy" && badBeat == -999);


    
    cerr << "All tests successful"<<endl;
    
    return 0;
}
