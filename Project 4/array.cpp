//
//  array.cpp
//  CS 31 project 4
//
//  Created by Chris Head on 5/1/22.
//

#include <iostream>
#include <string>
#include <cassert>
using namespace std;

int enumerate(const string a[], int n, string target);
int findMatch(const string a[], int n, string target);
bool findRun(const string a[], int n, string target, int& begin, int& end);
int findMin(const string a[], int n);
int moveToEnd(string a[], int n, int pos);
int moveToBeginning(string a[], int n, int pos);
int findDifference(const string a1[], int n1, const string a2[], int n2);
int removeDups(string a[], int n);
bool subsequence(const string a1[], int n1, const string a2[], int n2);
int makeMerger(const string a1[], int n1, const string a2[], int n2, string result[], int max);
int divide(string a[], int n, string divider);

int main()
{
            
}




int enumerate(const string a[], int n, string target){
    int total = 0;
    
    if (n < 0)
        return -1;
    
    for (int i = 0; i < n; i++){
        if (a[i] == target)
            total++;

    }
    
        return total;
         
         }



int findMatch(const string a[], int n, string target){
    int position = -1;
    
    if (n < 0)
        return -1;
    
    for (int i = 0; i < n; i++){
        if (a[i] == target){
            position = i;
            break;
        }
    }
    
    return position;
    
}
         
bool findRun(const string a[], int n, string target, int& begin, int& end){
    bool trueOrfalse = false;
    
    if (n < 0)
        return false;
    
    //first for loop to check if the string is in the array at all- if not, we return false and keep begin and end the same
    for (int i = 0; i < n; i++){
        if (a[i] == target){
            trueOrfalse = true;
            break;
        }
    }
    
    if (!trueOrfalse)
        return false;
    
    //outer loop checks for beginning of run, inner loop checks for end of run
    for (int i = 0; i < n; i++){
        if (a[i] == target)
        {
            begin = i;
            end = i;
            for (int k = i + 1; k < n; k++){
                if (a[k] != target)
                {
                    end = k - 1;
                    break;
                }
                else
                    end++;
                    
            }
            break;
        }
        
    }
    return true;
    
    
}


int findMin(const string a[], int n){
    int position = -1;
    string min = a[n - 1];   //sets current min to be last string in array
    
    //checks the array to see which string is <= the rest of the strings, goes backwards to find the string that fits this description with the earliest position
    for (int i = n - 1; i >= 0; i--){
        if (a[i] <= min){
            min = a[i];
            position = i;
        }
            
    }
    
    return position;
}


int moveToEnd(string a[], int n, int pos){
    
    if (n < 0 || pos >= n || pos < 0)  //make sure pos is in bounds
        return -1;
    
    //put the string we want to move in a new variable so we can change the current strings in the array and not lose the string we want
    string end = a[pos];
    
    //set every element in the array starting with pos one position to the left
    for (int i = pos; i < n - 1; i++){
        a[i] = a[i+1];
    }
    a[n - 1] = end;    //set the end of the array to be the string we want
    return pos;
}


int moveToBeginning(string a[], int n, int pos){
    
    if (n < 0 || pos >= n || pos < 0)
        return -1;
    
    //put the string we want to move in a new variable so we can change the current strings in the array and not lose the string we want
    string beginning = a[pos];
    
    //set every element in the array starting with pos one position to the right
    for (int i = pos; i > 0; i--){
        a[i] = a[i-1];
    }
    
    a[0] = beginning;    //set the beginning of the array to the string we want
    return pos;
}

int findDifference(const string a1[], int n1, const string a2[], int n2){
    if (n1 < 0 || n2 < 0)
        return -1;
    
    int loop;
    
    //we find which n is <= the other, to see how far in the arrays we should test and in case one is larger than the other and have the same elements up to their equal positions
    if (n1 <= n2){
        loop = n1;
    }
    else{
        loop = n2;
    }
    
    //find the difference and return the position
    for (int i = 0; i < loop; i++){
        if (a1[i] != a2[i])
            return i;
    }
    
    return loop;

    

}


int removeDups(string a[], int n){
    if (n < 0)
        return -1;
    
    int interestingItems = n;
    
    for (int i = 0; i < interestingItems; i++)
    {
        if (i != interestingItems - 1)  //check to make sure we don't go out of bounds
        {
            if (a[i] == a[i+1])
            {
                //if there is a duplicate in a row, we move the first one to the end and subtract 1 from k so that when the loop goes through we are on the same number- we also lower the number of interesting items we have by 1
                moveToEnd(a,n,i);
                i--;
                interestingItems--;
            }
        }
    }
    return interestingItems;
}


bool subsequence(const string a1[], int n1, const string a2[], int n2){
    if (n1 <= 0 || n2 > n1 || n2 < 0)           //impossible for a2 to be a subset of a1
        return false;
    
    int hit = 0;                //counts the number of times n2 matches with n1 or "hits"
    int k = 0;
    //k is for the inner for loop- we create it outside so that it does not reset to 0 everytime the for loop runs
    
    //outer for loop goes through the values in the smaller array
    for (int i = 0; i < n2; i++)
    {
        //inner loop goes through the values in the larger array for each value in the smaller-k moves to the next position for the next index of the outer loop
        for (;k < n1; k++)
        {
            if (a1[k] == a2[i])
            {
                hit++;
                k++;
                break;
            }
        }
    }
    
    if (hit == n2)
        return true;
    else
        return false;
}



int makeMerger(const string a1[], int n1, const string a2[], int n2, string result[], int max){
    
    if (n1 < 0 || n2 < 0 || n1 + n2 > max)
        return -1;
    
    //we check to see if both arrays are in nondecreasing order
    for (int i = 0; i < n1 - 1; i++){
        if (a1[i] > a1[i+1])
            return -1;
    }
    for (int i = 0; i < n2 - 1; i++){
        if (a2[i] > a2[i+1])
            return -1;
    }
    
    int i = 0;
    int k = 0;
    
    //outer for loop represents the spots in the result array
    //inner for loop loops through a1 and a2 and sees which has the lowest number available
    for (int j = 0; j < n1 + n2; j++)
    {
    if (i < n1 && k < n2)
    {
        if (a1[i] <= a2[k])
        {
            result[j] = a1[i];
            i++;
        }
        else
        {
            result[j] = a2[k];
            k++;
        }
    }
        
        //these if statements are for when one array ends and the other still has elements
        else if (i < n1)
        {
            result[j] = a1[i];
            i++;
        }
        else if (k < n2)
        {
            result[j] = a2[k];
            k++;
        }
    }
    
    return n1 + n2;
    
}

int divide(string a[], int n, string divider){
    if (n < 0)
        return -1;
    
    int position = -1;
    
    for (int i = 0; i < n; i++)
    {
        if (a[i] < divider)
            moveToBeginning(a,n,i);
    }
    for (int i = n - 1; i >= 0; i--)
    {
        if (a[i] > divider)
            moveToEnd(a,n,i);
    }
    for (int i = 0; i < n; i++)
    {
        if (a[i] >= divider)
        {
            position = i;
            break;
        }
    }
    
    if (position >= 0)
        return position;
    else
        return n;
}
