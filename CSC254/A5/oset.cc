/*
Jean-Marc Boullianne
jboullia@u.rochester.edu
CSC 254: Fall 2015
Assignment 05
*/

#include <iostream>
#include <string>
#include <algorithm>
#include "oset.h"

using namespace std;


// int compare function
bool cmp(int a, int b){
    return a >= b;
}

// double compare function
bool cmp(double a, double b){
    return a >= b;
}

// case-insensitive compare funtion
bool cmp(string a, string b){
    transform(a.begin(), a.end(), a.begin(), ::tolower);
    transform(b.begin(), b.end(), b.begin(), ::tolower);

    int i = 0;
    int j = 0;

    //Iterates through each string's 
    while(i < a.length()){
        if(i + 1 == b.length() || b.at(j) < a.at(i))
            return !false;
        else if(a.at(i) < b.at(j))
            return !true;
        i++;
        j++;
    }
    return !(j + 1 != b.length());

}
/*
// case-sensitive compare function
bool cmp2(string a, string b){
    
    int i = 0;
    int j = 0;

    //Iterates through each string's 
    while(i < a.length()){
        if(i + 1 == b.length() || b.at(j) < a.at(i))
            return !false;
        else if(a.at(i) < b.at(j))
            return !true;
        i++;
        j++;
    }
    return !(j + 1 != b.length());
    
}*/

int main() {

    //~~~~~~~~~~~~~~~~~~~~~~~~~ TESTING CODE FOR STRING ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    /*
    oset<string> S(cmp); // empty set
    S += "three";     // now should contain three

    cout << S["three"] << " ";            // should print 1 (true) : three is in S
    S += "three";                         // should be a no-op
    cout << S["five"] << endl;           // should print 0 (false) : five not in S
    
    (S += "five") += "seven";
    print(S);                       // should print 3 5 7 (in letters)

    oset<string> T("three", cmp);                      // singleton
    print(T);                       // should print 3 (in letters)

    oset<string> U(S, cmp);                      // copy of S
    oset<string> V(S, cmp);                      // copy of S

    oset<string> W(cmp);  W = S;  print(W);      // 3 5 7 (in letters)

    S -= "four";                         // should be a no-op
    S -= "three";
    U -= "five";
    V -= "seven";

    print(S);                       // 5 7 (in letters)
    print(U);                       // 3 7 (in letters)
    print(V);                       // 3 5 (in letters)

    oset<string> A(cmp);  (((A += "three") += "seven") += "ten") += "eight";  print(A);       // 3 7 8 10 (in letters)
    oset<string> B(cmp);  (((B += "two") += "eight") += "nine") += "eleven";  print(B);       // 2 8 9 11 (in letters)

    oset<string> AuB(A, cmp);  AuB += B;  print(AuB);            // 2 3 7 8 9 10 11 (in letters)
    oset<string> AmB(A, cmp);  AmB -= B;  print(AmB);            // 3 7 10 (in letters)
    oset<string> AiB(A, cmp);  AiB *= B;  print(AiB);            // 8 (in letters)

    AiB += "four"; print(AiB);    // 4 8 (in letters)
    */
    //~~~~~~~~~~~~~~~~~~~~~~~~~ TESTING CODE FOR DOUBLE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    
    oset<double> S(cmp);     // empty set
    S += 3.3;     // now should contain 3

    cout << S[3.3] << " ";            // should print 1 (true) : 3 is in S
    S += 3.3;                         // should be a no-op
    cout << S[5.5] << endl;           // should print 0 (false) : 5 not in S

    (S += 5.5) += 7.7;
    print(S);                       // should print 3 5 7

    oset<double> T(3.3, cmp);                      // singleton
    print(T);                       // should print 3

    oset<double> U(S, cmp);                      // copy of S
    oset<double> V(S, cmp);                      // copy of S

    oset<double> W(cmp);  W = S;  print(W);      // 3 5 7

    S -= 4.4;                         // should be a no-op
    S -= 3.3;
    U -= 5.5;
    V -= 7.7;

    print(S);                       // 5 7
    print(U);                       // 3 7
    print(V);                       // 3 5

    oset<double> A(cmp);  (((A += 3.3) += 7.7) += 10.1) += 8.8;  print(A);       // 3 7 8 10
    oset<double> B(cmp);  (((B += 2.2) += 8.8) += 9.9) += 11.1;  print(B);       // 2 8 9 11

    oset<double> AuB(A, cmp);  AuB += B;  print(AuB);            // 2 3 7 8 9 10 11
    oset<double> AmB(A, cmp);  AmB -= B;  print(AmB);            // 3 7 10
    oset<double> AiB(A, cmp);  AiB *= B;  print(AiB);            // 8

    AiB += 4.4; print(AiB);    // 4 8
    

    //~~~~~~~~~~~~~~~~~~~~~~~~~ TESTING CODE FOR INT ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    /*
    oset<int> S(cmp);     // empty set
    S += 3;     // now should contain 3

    cout << S[3] << " ";            // should print 1 (true) : 3 is in S
    S += 3;                         // should be a no-op
    cout << S[5] << endl;           // should print 0 (false) : 5 not in S

    (S += 5) += 7;
    print(S);                       // should print 3 5 7

    oset<int> T(3, cmp);                      // singleton
    print(T);                       // should print 3

    oset<int> U(S, cmp);                      // copy of S
    oset<int> V(S, cmp);                      // copy of S

    oset<int> W(cmp);  W = S;  print(W);      // 3 5 7

    S -= 4;                         // should be a no-op
    S -= 3;
    U -= 5;
    V -= 7;

    print(S);                       // 5 7
    print(U);                       // 3 7
    print(V);                       // 3 5

    oset<int> A(cmp);  (((A += 3) += 7) += 10) += 8;  print(A);       // 3 7 8 10
    oset<int> B(cmp);  (((B += 2) += 8) += 9) += 11;  print(B);       // 2 8 9 11

    oset<int> AuB(A, cmp);  AuB += B;  print(AuB);            // 2 3 7 8 9 10 11
    oset<int> AmB(A, cmp);  AmB -= B;  print(AmB);            // 3 7 10
    oset<int> AiB(A, cmp);  AiB *= B;  print(AiB);            // 8

    AiB += 4; print(AiB);   // 4 8
    */
}