//
//  bill.cpp
//
//  Created by Chris Head on 4/10/22.
//


#include <iostream>
using namespace std;

int main(){
    int initialreading;
    int finalreading;
    string name;
    int month;
    
    cout << "Initial meter reading: ";
    cin >> initialreading;
    cout << "Final meter reading: ";
    cin >> finalreading;
    cin.ignore(10000, '\n');
    cout << "Customer name: ";
    getline(cin, name);
    cout << "Month number (1=Jan, 2=Feb, etc.): ";
    cin >> month;
    cout << "---" << endl;
    
    //We create a variable and set it to 0. If the code runs and the variable is still 0, the final line will print; if the variable is 1, the final line will not print. This is to avoid printing the bill amount after an error has occured.
    int print = 0;
    
    //We add a line that sets print = 1 for every error so that the final amount line does not print.
    if (initialreading < 0)
    {
        cout << "The initial meter reading must not be negative.";
        print = 1;
    }
    else if (finalreading < initialreading)
    {
        cout << "The final meter reading must be at least as large as the initial reading.";
        print = 1;
    }
    else if (name == "")
    {
        cout << "You must enter a customer name.";
        print = 1;
    }
    else if (month < 1 || month > 12)
    {
        cout << "The month number must be in the range 1 through 12.";
        print = 1;
    }
    
    int hcf = finalreading - initialreading;
    int difference;
    double amount;
    
    //We create a variable called difference and use it to charge different prices for HCF > 23 or HCF > 15 depending on the season - if the customer did not use more than the cut off amount (15 or 23 depending on season), we only use the first rate and the HCF.
    
    if (month >= 4 && month <= 10)
    {
        if (hcf > 23)
        {
            difference = hcf - 23;
            amount = (23 * 5.41) + (difference * 9.79);
        }
        else
            amount = hcf * 5.41;
    }
    else
    {
        if (hcf > 15)
    {
        difference = hcf - 15;
        amount = (15 * 5.41) + (difference * 7.77);
    }
        else
            amount = hcf * 5.41;
    }
    
    cout.setf(ios::fixed);
    cout.precision(2);
    
    //Checking to see if any errors occured/if we should print the last line.
    if (print == 0)
        cout << "The bill for " << name << " is $" << amount;
    
        

        
    
        
        
        
        
    
    
    
}
