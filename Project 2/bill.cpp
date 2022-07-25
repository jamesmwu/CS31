//
//  main.cpp
//  Project 2
//
//  Created by James Wu on 4/8/22.
//

#include <iostream>
using namespace std;

int main(int argc, const char * argv[]) {
    
    //Variables inputted by user
    int initialMeterReading;
    int finalMeterReading;
    string customerName;
    int monthNumber;
    
    //Variables used in calculation / output
    double totalCost;
    string output;
    
    
    //Initial Meter Reading
    cout << "Initial meter reading: ";
    cin >> initialMeterReading;
    
    //Final Meter Reading
    cout << "Final meter reading: ";
    cin >> finalMeterReading;
    cin.ignore(10000, '\n');
    
    //Customer Name
    cout << "Customer name: ";
    getline(cin, customerName);
    
    //Month Number
    cout << "Month number (1=Jan, 2=Feb, etc.): ";
    cin >> monthNumber;
    
    //Three Hyphens indicating that output will soon follow
    cout << "---" << endl;
    
    //Calculations to determine total cost
    
    //Determine the hundred cubic feet of water used
    double hcf = finalMeterReading  - initialMeterReading;
    //Determine whether high or low season
    if(monthNumber >= 4 && monthNumber <= 10){
        //High season-- first 23 hcf billed at first tier, additional hcf billed at second tier
        //For high season: tier 1 = $5.41, tier 2 = $9.79
        double extraHcf = hcf - 23;
        
        //Find appropriate costs depending on if HCF is > 23 or not.
        if(extraHcf >= 0){
            //First 23 billed at $5.41, additional is billed at $9.79
            totalCost = (5.41 * 23) + (extraHcf * 9.79);
        } else {
            totalCost = 5.41 * hcf;
        }
    }
    else {
        //Low season-- first 15 hcf billed at first tier, additional hcf billed at second tier
        //For low season: tier 1 = $5.41, tier 2 = $7.77
        double extraHcf = hcf - 15;
        
        //Find appropriate costs depending on if HCF is > 15 or not.
        if(extraHcf >= 0){
            //First 23 billed at $5.41, additional is billed at $7.77
            totalCost = (15 * 5.41) + (extraHcf * 7.77);
        } else {
            totalCost = 5.41 * hcf;
        }
    }
    
    
    //Output reading either a bill or an error message if user entered invalid input
    
    if(initialMeterReading < 0)
        cout << "The initial meter reading must not be negative." << endl;
    else if(finalMeterReading < initialMeterReading)
        cout << "The final meter reading must be at least as large as the initial reading." << endl;
    else if(customerName == "")
        cout << "You must enter a customer name." << endl;
    else if(!(monthNumber >= 1 && monthNumber <= 12)) //Must determine if it's a non-integer
        cout << "The month number must be in the range 1 through 12." << endl;
    else {
        //Set precision to 2 decimal places.
        cout.setf(ios::fixed);
        cout.precision(2);
        cout << "The bill for " << customerName << " is $" << totalCost << endl;
    }

    //Note: is it ok for us to end this program with an endl...?
    
    return 0;
}
