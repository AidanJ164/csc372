/************************************************************************
 * Aidan Justice
 * CSC 372 - Analysis of Algorithms
 * 
 * Count the number of nonwitnesses from integers given from "witness.in"
 * 
 * Compiled with:
 *      g++ -o witness witness.cpp
 * 
 * Execute with:
 *      .\witness.exe
************************************************************************/
#include <cmath>
#include <fstream>
#include <iostream>
using namespace std;

typedef long long int lli;

int findNonWitnesses(lli num);
bool isPrime(lli n);
lli isWitness(lli base, lli e, lli n, bool& witness, lli start);
lli modPow(lli base, lli e, lli n);

int main(int argv, char** argc)
{
    ifstream fin;
    ofstream fout;
    lli num;
    string fileName;

    // Open input file
    fin.open("witness.in");
    if (!fin.is_open())
    {
        cout << "Could not open witness.in";
        return 0;
    }

    // Open output file
    fout.open("witness.out");
    if(!fout.is_open())
    {
        cout << "Could not open witness.out";
        return 0;
    }

    // Read from file until 2 or less is read in
    fin >> num;
    while (num > 2)
    {
        // Check if number is prime
        if (isPrime(num))
        {
            fout << num << " is prime" << endl;
        }
        // If not prime, count number of nonwitnesses
        else
        {
            fout << num << " " << findNonWitnesses(num) << endl;
        }
        fin >> num;
    }

    // Close files
    fin.close();
    fout.close();
    return 0;
}

// Given an integer, find the amount of nonwitnesses from 2 to n - 2
int findNonWitnesses(lli num)
{
    int i, numWitnesses = 0;
    bool witness;

    for (i = 2; i < num - 1; i++)
    {
        witness = false;
        
        // If i is a not a witness, increment the counter
        if(isWitness(i, num -1, num, witness, num - 1) == 1 && !witness)
        {
            numWitnesses++;
        }
    }

    return numWitnesses;
}

bool isPrime(lli n)
{
    int i;

    // If even, not prime
    if (n % 2 == 0)
    {
        return false;
    }

    // Check odd numbers from 3 to sqrt(n)
    for (i = 3; i <= sqrt(n); i += 2)
    {
        if (n % i == 0)
        {
            return false;
        }
    }

    return true;
}

/************************************************************************
 * isWitness is a modified version of the modPow function written in
 * class. It does the normal recursion for the Fermat Test, however,
 * if the number is squared, it checks to see if the number is a witness
 * or not. 
************************************************************************/
lli isWitness(lli base, lli e, lli n, bool& witness, lli start)
{
    lli temp;
    lli prev;

    // Base case
    if (e == 0)
    {
        return 1;
    }

    // If e is odd
    if (e % 2 == 1)
    {
        return (base * isWitness(base, e - 1, n, witness, start)) % n;
    }

    // e is divisible by 2, check to see if the base is valid witness
    prev = isWitness(base, e / 2, n, witness, start);
    temp = (prev * prev) % n;
    if (prev != 1 && prev != n - 1 && temp == 1 && e != start)
    {
        witness = true;
    }

    return temp;
}