//
//  array.cpp
//  Project 4
//
//  Created by James Wu on 4/30/22.
//

#include <iostream>
#include <string>
#include <cassert>
using namespace std;

//Functions
//Returns number of strings = to target
int enumerate(const string a[], int n, string target){
    if(n < 0)
        return -1;
    
    int counter = 0;
    
    for(int i = 0; i < n; i++){
        if(a[i] == target)
            counter++;
    }
    
    return counter;
    
}

//Returns position of string = to target
int findMatch(const string a[], int n, string target){

    if(n == 0)
        return 0;
    
    for(int i = 0; i < n; i++){
        if(a[i] == target)
            return i;
    }
    
    return -1;
    
}

//Returns true if string is found. Sets "begin" to where it's first found, sets "end" to where the amount of consecutive target strings end.
bool findRun(const string a[], int n, string target, int& begin, int& end){
    
    bool firstOccurrence = true;
    
    for(int i = 0; i < n; i++){
        //If the target is identified and it's the first time it's identified
        if(a[i] == target && firstOccurrence){
            //Set begin/end equal to index, change firstOccurrence to false
            begin = i;
            end = i;
            firstOccurrence = false;
            //Now iterate through the remaining amount of consecutive strings
            for(int j = i; j < n; j++){
                //Set end to the final index where the consecutive strings end
                if(a[j] == target)
                    end = j;
                else
                    break;
            }
        }
    }
    
    //If firstOccurrence is false, that means at least one occurrence of string was found
    if(!firstOccurrence)
        return true;
    
    return false;
}

//Return position of smallest VALUE string in array
int findMin(const string a[], int n){
    //0 case logic: If there are 0 elements, there are no positions to return. Therefore return -1
    if(n <= 0)
        return -1;
    
    int min = 0;
    string minVal = a[0];
    
    //Find the minimum value and set minVal equal to it.
    for(int i = 0; i < n; i++){
        string temp = a[i];
        if(temp < minVal){
            min = i;
            minVal = temp;
        }
    }
    
    return min;
}

//Move specified position to end of specified length
int moveToEnd(string a[], int n, int pos){
    //0 Case logic: If there are no elements, how can I move a specified position to the end??
    if(n <= 0 || pos < 0 || pos >= n)
        return -1;
    
    string specified = a[pos];
    //Move every element in the array to the left by one, starting at where the specified position is
    for(int i = pos; i < n - 1; i++){
        a[i] = a[i+1];
    }
    //Set last element equal to the initially removed element
    a[n - 1] = specified;
    
    return pos;
}

//Move specified position to beginning of specified length
int moveToBeginning(string a[], int n, int pos){
    //0 Case logic: If there are no elements, how can you move a specified position to the beginning?
    if(n <= 0 || pos < 0 || pos >= n)
        return -1;
    
    string specified = a[pos];
    //Move every element in the array to the right by one, starting at where the specified position is
    for(int i = pos; i > 0; i--){
        a[i] = a[i-1];
    }
    //Set first element equal to initially removed element
    a[0] = specified;
    
    return pos;
}


//Return position of first corresponding elements in a1 and a2 that aren't equal, or whichever position is less than the other.
int findDifference(const string a1[], int n1, const string a2[], int n2){
    //0 Case logic: The spec says "If the arrays are equal up to the point where one or both runs out, return whichever value of n1 and n2 is less than or equal to the other." So if one array's length is 0, then function should return whichever value of n1 and n2 is less than or equal to the other.
    if(n1 < 0 || n2 < 0)
        return -1;
    else if(n1 == 0 || n2 == 0)
        return 0;
    
    int i = 0;
    
    if(n1 > n2){
        for(i = 0; i < n2; i++){
            if(a1[i] != a2[i])
                return i;
        }
        return i;
    }
    else if(n2 > n1){
        for(i = 0; i < n1; i++){
            if(a1[i] != a2[i])
                return i;
        }
        return i;
    }
    //n1 = n2
    else{
        for(i = 0; i < n1; i++){
            if(a1[i] != a2[i])
                return i;
        }
        return i;
    }
}

//Return number of retained items
int removeDups(string a[], int n){
    int i = 0;
    
    //0 Case logic: The spec says: "For every sequence of consecutive identical items in a, retain only one item of that sequence. [...] Return the number of retained items." So if there are no elements, retain items in the sequence-- which is 0. Return that number.
    if(n < 0)
        return -1;
    
    //Iterate through array up to second to last specified position
    for(i; i < n; i++){
        int indexTracker = 0;
  
        //Debugging code
//        cout << "n = " << n << endl;
//        cout << "i = " << i << endl;
//        for(int j = 0; j < n; j++){
//            cout << a[j] << " ";
//        }
//        cout << endl;
        
        //If current index equals next index
        if(a[i] == a[i+1] && i + 2 != n){
            //move duplicates to the end
            while(a[i] == a[i+1]){
                moveToEnd(a, n, i+1);
                indexTracker--; //Make sure that we won't iterate over the same things again
            }
            n += indexTracker;
        }
        else if(i + 2 == n){
            i++;
            break;
        }
    }
    
    return i;
}

//Return true if all n2 elements of a2 appear in a1 in the same order (not necessarily consecutively)
bool subsequence(const string a1[], int n1, const string a2[], int n2){
    
    //Ensure that a1 will contain at least the same amount of elements as a2.
    if(n2 > n1 || n2 < 0 || n1 < 0)
        return false;
    
    //Catch the case where there are 0 elements-- every sequence has 0 elements
    if(n2 == 0)
        return true;
    
    //Represents index of a2. This will increment every time it finds the value in a2, which will eventually iterate through a2.
    int index = 0;
    
    //Iterate through a1
    for(int i = 0; i < n1; i++){
        if(a1[i] == a2[index])
            index++;
    }
    
    //If index has iterated all the way to the same number of elements of n2, then it has found every consecutive value
    return (index == n2);
}


//Arrange elements in increasing (nondecreasing) order and return number of elements placed
int makeMerger(const string a1[], int n1, const string a2[], int n2, string result[], int max){
    
    //0 Case logic: If n1 and n2 have NO elements, how can the elements be arranged in nondecreasing order? There are no elements. That means a1 and a2 are not in nondecreasing order (because there is no order at all).
    if(n1 + n2 > max || n1 <= 0 || n2 <= 0)
        return -1;
    
    //Determine if a1 and a2 are in nondecreasing order
    for(int i = 0; i < n1-1; i++){
        if(a1[i] > a1[i+1])
            return -1;
    }
    for(int i = 0; i < n2-1; i++){
        if(a2[i] > a2[i+1])
            return -1;
    }
    
    
    
    //Append all entries in a1 to result
    for(int i = 0; i < n1; i++){
        result[i] = a1[i];
    }
    
    //Append all entries in a2 to result. "n1" is index where the result array leaves off.
    int index = 0;
    for(int j = n1; j < n1 + n2; j++){
        result[j] = a2[index];
        index++;
    }
    
    //Now, sort the results array so that elements are arranged in nondecreasing order
    //Iterate through the results array
    for(int k = 0; k < n1 + n2; k++){
        
        //Sort the array in ascending order:
        for(int h = k+1; h < n1 + n2; h++){
            //Swap values until the smallest value is brought to the position "k". As the parent loop iterates, this will gradually sort every single item in the array so it's ascending.
            if(result[k] > result[h]){
                string temp = result[k];
                result[k] = result[h];
                result[h] = temp;
            }
        }
    }
    
    return (n1 + n2);
}

//Return position of first element that is >= divider or "n" if there are no such elements
int divide(string a[], int n, string divider){
   //0 Case logic: The spec says: "Return the position of the first element that, after the rearrangement, is not < divider, or n if there are no such elements." So in the event that there are 0 elements, there are no such elements that are less than divider. So return 0.
    if(n < 0)
        return -1;
    
    //If you sort all the elements of the array in ascending order, then naturally everything less than divider will come before everything bigger than divider.
    for(int i = 0; i < n; i++){
        for(int j = i+1; j < n; j++){
            if(a[i] > a[j]){
                string temp = a[i];
                a[i] = a[j];
                a[j] = temp;
            }
        }
    }
    
    //Now determine the position of the first element that is >= divider
    for(int i = 0; i < n; i++){
        if(a[i] >= divider)
            return i;
    }
    
    //There is no such element that is >= divider so return n
    return n;
    
}



int main(int argc, const char * argv[]) {
        
    //enumerate Test
    string enumerateArray[10] = {"James", "jAmEs", "David", "Nicole", "Ashley", "Phillip", "Phillip", "Phillip"};
    assert(enumerate(enumerateArray, -1, "James") == -1); // Test negative case
    assert(enumerate(enumerateArray, 0, "James") == 0); // Test 0 case
    assert(enumerate(enumerateArray, 3, "James") == 1); // Test for case
    assert(enumerate(enumerateArray, 3, "Ashley") == 0); // Test for range
    assert(enumerate(enumerateArray, 10, "Ashley") == 1);
    assert(enumerate(enumerateArray, 10, "Phillip") == 3);
    assert(enumerate(enumerateArray, 10, "jAmEs") == 1);
    
    //findMatch Test
    string findMatchArray[10] = {"James", "jAmEs", "David", "Nicole", "Ashley", "Phillip", "Phillip", "Phillip"};
    assert(findMatch(findMatchArray, -1, "James") == -1); // Test negative case
    assert(findMatch(findMatchArray, 0, "James") == 0); // Test 0 case
    assert(findMatch(findMatchArray, 3, "James") == 0); // Test for case
    assert(findMatch(findMatchArray, 3, "Nicole") == -1); // Test for no string found in range
    assert(findMatch(findMatchArray, 10, "Phillip") == 5); // Test for multiple repeats
    assert(findMatch(findMatchArray, 10, "Nicole") == 3);
    assert(findMatch(findMatchArray, 10, "jAmEs") == 1);
    
    //findRun Test
    string findRunArray[10] = {"James", "James", "jAmEs", "James", "James", "Phillip", "Phillip", "Phillip"};
    int b = 999;
    int e = -999;
    assert(findRun(findRunArray, -1, "James", b, e) == false); // Test negative case
    assert(findRun(findRunArray, 0, "James", b, e) == false); // Test 0 case
    assert(findRun(findRunArray, 3, "James", b, e) == true && b == 0 && e == 1); // Test for case
    assert(findRun(findRunArray, 10, "James", b, e) == true && b == 0 && e == 1); // Test for multiple consecutive strings
    assert(findRun(findRunArray, 10, "Phillip", b, e) == true && b == 5 && e == 7);
    assert(findRun(findRunArray, 10, "Ashley", b, e) == false && b == 5 && e == 7); // Test for string not found
    
    //findMin Test
    string findMinArray[5] = {"Kaylyn", "Eric", "Eugene", "Nareh", "Eric"};
    assert(findMin(findMinArray, -1) == -1); // Test negative case
    assert(findMin(findMinArray, 0) == -1); // No elements part of the array
    assert(findMin(findMinArray, 5) == 1); // Test for multiple occurrences
    assert(findMin(findMinArray, 4) == 1);
    
    //moveToEnd Test
    string moveToEnd1[5] = {"Kaylyn", "Eric", "Eugene", "Nareh", "James"};
    assert(moveToEnd(moveToEnd1, -1, 2) == -1); // Test negative case
    assert(moveToEnd(moveToEnd1, 0, 2) == -1); // Test 0 case
    assert(moveToEnd(moveToEnd1, 5, 2) == 2 && moveToEnd1[4] == "Eugene");
    assert(moveToEnd(moveToEnd1, 5, 0) == 0 && moveToEnd1[4] == "Kaylyn");
    assert(moveToEnd(moveToEnd1, 5, 4) == 4 && moveToEnd1[4] == "Kaylyn"); // Test moving last element to end again
    
    //moveToBeginning Test
    string moveToBeginning1[5] = {"Kaylyn", "Eric", "Eugene", "Nareh", "James"};
    assert(moveToBeginning(moveToBeginning1, -1, 2) == -1); // Test negative case
    assert(moveToBeginning(moveToBeginning1, 0, 2) == -1); // Test 0 case
    assert(moveToBeginning(moveToBeginning1, 5, 2) == 2);
    assert(moveToBeginning(moveToBeginning1, 5, 4) == 4 && moveToBeginning1[0] == "James");
    assert(moveToBeginning(moveToBeginning1, 5, 0) == 0 && moveToBeginning1[0] == "James"); // Test moving first element to end again
    
    //findDifference Test
    string array1[5] = {"James", "Phillip", "Luke", "Aaron", "Erin"};
    string array2[5] = {"James", "Jaclyn", "Phillip", "Luke", "Aaron"};
    string array3[5] = {"James", "Phillip", "Luke", "Aaron", "Aaron"};
    assert(findDifference(array1, -1, array2, 5) == -1); // Test negative case
    assert(findDifference(array1, 5, array2, -1) == -1); // Test negative case
    assert(findDifference(array1, 0, array2, 0) == 0); // Test 0 case
    assert(findDifference(array1, 5, array2, 0) == 0); // Test 0 case
    assert(findDifference(array1, 5, array2, 5) == 1);
    assert(findDifference(array1, 5, array3, 5) == 4);
    
    //removeDups Test
    string removeDupsArray[10] = {"James", "James", "James", "Mario", "Mario", "Luigi", "Luigi", "Thomas", "James", "James"};
    string removeDupsArray1[10] ={"James", "James", "Mario", "James", "James", "Mario", "James", "James", "Mario", "Mario"};
    assert(removeDups(removeDupsArray, -1) == -1); // Test negative case
    assert(removeDups(removeDupsArray, 0) == 0); // Test 0 case
    assert(removeDups(removeDupsArray, 10) == 5 && removeDupsArray[0] == "James" && removeDupsArray[1] == "Mario" && removeDupsArray[2] == "Luigi" && removeDupsArray[3] == "Thomas" && removeDupsArray[4] == "James");
    assert(removeDups(removeDupsArray1, 10) == 6);
    
    //subsequence Test
    string s1[5] = {"James", "Kat", "Alex", "Phillip", "Dom"};
    string s2[3] = {"James", "Phillip", "Dom"};
    string s3[6] = {"One", "Two", "Three", "Kat", "Alex", "Six"};
    assert(subsequence(s1, -1, s2, 0) == false); // Test negative case
    assert(subsequence(s1, 0, s2, -1) == false); // Test negative case
    assert(subsequence(s1, 0, s2, 0) == true); // Test 0 case
    assert(subsequence(s1, 5, s2, 3) == true);
    assert(subsequence(s1, 5, s3, 6) == false); // Test different bounds
    assert(subsequence(s1, 5, s3, 5) == false);
    
    //makeMerger Test
    string x[5] = { "amy", "elena", "elena", "ketanji", "samuel" };
    string x1[5] = {"elena", "ketanji", "bob", "amy", "samuel"};
    string y[4] = { "clarence", "elena", "john", "sonia" };
    string z[20];
    assert(makeMerger(x, -1, y, -1, z, 20) == -1); // Test negative case
    assert(makeMerger(x, 0, y, -1, z, 20) == -1); // Test negative case
    assert(makeMerger(x, -1, y, 0, z, 20) == -1); // Test negative case
    assert(makeMerger(x, 0, y, 0, z, -1) == -1); // Test negative case
    assert(makeMerger(x, 0, y, 0, z, 20) == -1); // Test 0 case
    assert(makeMerger(x, 5, y, 4, z, 20)==9);
    assert(makeMerger(x1, 5, y, 4, z, 20)== -1); // Test case where array is not nondecreasing
    assert(makeMerger(x, 5, y, 4, z, 3)==-1); // Test case where results have more than max elements

    //divide Test
    string dividerTest[5] = {"e", "k", "b", "a", "s"};
    assert(divide(dividerTest, -1, "c") == -1); // Test negative case
    assert(divide(dividerTest, 0, "c") == 0); // Test 0 case
    assert(divide(dividerTest, 5, "c") == 2);
    assert(divide(dividerTest, 5, "z") == 5); // Test if no item greater than divider
    assert(divide(dividerTest, 5, "m") == 4);
    
    cout << "All tests successful" << endl;
    
    //enumerate Test
     string d[9] = {
         "clarence", "neil", "amy", "amy", "ketanji", "ketanji", "ketanji", "amy", "amy"
     };
     assert(enumerate(d, 9, "amy") == 4);     // returns 4
     assert(enumerate(d, 5, "ketanji") == 1); // returns 1
     assert(enumerate(d, 9, "brett") == 0);   // returns 0
     assert(enumerate(d, 0, "amy") == 0);
     assert(enumerate(d, -500, "amy") == -1);
     
     
     //findMatch test
     assert(findMatch(d, 9, "ketanji") == 4);
     assert(findMatch(d, 4, "ketanji") == -1);
     
     //findRun test
     int b1;
     int e1;
     assert(findRun(d, 9, "amy", b1, e1) == true && b1 == 2 && e1==3);
     assert(findRun(d, 9, "neil", b1, e1) == true && b1 == 1 && e1 == 1);
     assert(findRun(d, 9, "samuel", b1, e1) == false && b1 == 1 && e1 == 1);
     
     //findMin test
     string people[5] = { "john", "sonia", "samuel", "elena", "neil" };
     assert(findMin(people, 5) == 3);
     
     //moveToEnd test
     assert(moveToEnd(people, 4, 1) == 1);
    //    for(int i = 0; i < 5; i++){
    //        cout << people[i] << endl;
    //    }
     
     //moveToBeginning test
     assert(moveToBeginning(people, 5, 2) == 2);
    //    for(int i = 0; i < 5; i++){
    //        cout << people[i] << endl;
    //    }
     
     //findDifference test
     string people1[5] = { "john", "sonia", "samuel", "elena", "neil" };
     string bench[6] = { "john", "sonia", "clarence", "elena", "neil", "samuel" };
     
     assert(findDifference(people1, 5, bench, 6) == 2);
     assert(findDifference(people1, 2, bench, 1) == 1);
     
     //removeDups test
     string example[9] = {
         "clarence", "neil", "amy", "amy", "ketanji", "ketanji", "ketanji", "amy", "amy"
     };
     assert(removeDups(example, 9) == 5);
     
     //subsequence test // More testing needed
     string big[10] = { "elena", "john", "amy", "ketanji", "neil", "amy" };
     string little1[10] = { "john", "ketanji", "neil" };
     assert(subsequence(big, 6, little1, 3) == true);  // returns true
     string little2[10] = { "amy", "john" };
     assert(subsequence(big, 6, little2, 2) == false);  // returns false
     string little3[10] = { "john", "amy", "amy" };
     assert(subsequence(big, 6, little3, 3) == true); // returns true
     string little4[10] = { "john", "john", "amy" };
     assert(subsequence(big, 6, little4, 3) == false);  // returns false
     assert(subsequence(big, 6, little4, 0) == true);  // returns true

     //makeMerger test
     string x3[5] = { "amy", "elena", "elena", "ketanji", "samuel" };
     string y3[4] = { "clarence", "elena", "john", "sonia" };
     string z3[20];
     assert(makeMerger(x3, 5, y3, 4, z3, 20) == 9);  // returns 9
     
     //divide test
     string sc[6] = { "john", "amy", "samuel", "elena", "sonia", "neil" };
     assert(divide(sc, 6, "ketanji") == 3); // returns 3
     
     string sc2[4] = { "john", "sonia", "amy", "neil" };
     assert(divide(sc2, 4, "neil") == 2);  //  returns 2
     
     cout << "All tests successful" << endl;
     
     
     //Smallberg tests
     string h[7] = { "neil", "sonia", "john", "amy", "", "elena", "john" };
     assert(enumerate(h, 7, "john") == 2);
     assert(enumerate(h, 7, "") == 1);
     assert(enumerate(h, 7, "brett") == 0);
     assert(enumerate(h, 0, "john") == 0);
     assert(findMatch(h, 7, "john") == 2);
     assert(findMatch(h, 2, "john") == -1);
     int bg;
     int en;
     assert(findRun(h, 7, "amy", bg, en) && bg == 3 && en == 3);

     string g[4] = { "neil", "sonia", "amy", "elena" };
     assert(findMin(g, 4) == 2);
     assert(findDifference(h, 4, g, 4) == 2);
     assert(subsequence(h, 7, g, 4));
     assert(moveToEnd(g, 4, 1) == 1 && g[1] == "amy" && g[3] == "sonia");

     string f[4] = { "elena", "amy", "sonia", "john" };
     assert(moveToBeginning(f, 4, 2) == 2 && f[0] == "sonia" && f[2] == "amy");

     string elena[5] = { "elena", "elena", "elena", "sonia", "sonia" };
     assert(removeDups(elena, 5) == 2 && elena[1] == "sonia");

     string xander[4] = { "john", "john", "samuel", "sonia" };
     string ymir[4] = { "amy", "elena", "john", "ketanji" };
     string zoop[10];
     assert(makeMerger(xander, 4, ymir, 4, zoop, 10) == 8 && zoop[5] == "ketanji");

     assert(divide(h, 7, "john") == 3);

     cout << "All tests succeeded" << endl;

    return 0;
}




