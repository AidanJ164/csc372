#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

typedef long long int lli;

int findNonWitnesses(lli num);
bool isPrime(lli n);
//bool isWitness(lli n, lli base);
lli isWitness(lli base, lli e, lli n, bool& witness, lli start);
lli modPow(lli base, lli e, lli n);

int main(int argv, char** argc)
{
    ifstream fin;
    ofstream fout;
    lli num;
    string fileName;

    fin.open("witness.in");
    if (!fin.is_open())
    {
        cout << "Could not open witness.in";
        return 0;
    }

    fout.open("witness.out");
    if(!fout.is_open())
    {
        cout << "Could not open witness.out";
        return 0;
    }

    fin >> num;
    while (num > 2)
    {
        if (isPrime(num))
        {
            fout << num << " is prime" << endl;
        }
        else
        {
            cout << num << ": ";
            fout << num << " " << findNonWitnesses(num) << endl;
            cout << endl;
        }
        fin >> num;
    }

    fin.close();
    fout.close();
    return 0;
}

int findNonWitnesses(lli num)
{
    int i, numWitnesses = 0;
    bool witness;

    for (i = 2; i < num - 1; i++)
    {
        witness = false;
        //if (!isWitness(num, i))
        
        if(isWitness(i, num -1, num, witness, num - 1) == 1 && !witness)
        {
            cout << i << " ";
            numWitnesses++;
        }
    }

    return numWitnesses;
}

bool isPrime(lli n)
{
    int i;

    if (n % 2 == 0)
    {
        return false;
    }

    for (i = 3; i <= sqrt(n); i += 2)
    {
        if (n % i == 0)
        {
            return false;
        }
    }

    return true;
}

lli modPow(lli base, lli e, lli n)
{
    lli temp;

    if (e == 0)
    {
        return 1;
    }

    if (e % 2 == 1)
    {
        return (base * modPow(base, e - 1, n)) % n;
    }

    temp = modPow(base, e / 2, n);
    return (temp * temp) % n;
}

lli isWitness(lli base, lli e, lli n, bool& witness, lli start)
{
    lli temp;
    lli prev;

    if (e == 0)
    {
        return 1;
    }

    if (e % 2 == 1)
    {
        return (base * isWitness(base, e - 1, n, witness, start)) % n;
    }

    prev = isWitness(base, e / 2, n, witness, start);
    temp = (prev * prev) % n;
    if (prev != 1 && prev != n - 1 && temp == 1 && e != start)
    {
        witness = true;
    }

    return temp;
}

/*
bool isWitness(lli n, lli base)
{
    int k = 1, j, m;
    lli temp;
    
    while( (n - 1) % (int)pow(2,k) == 0)
    {
        k++;
    }
    k--;
    m = (n - 1) / (int)pow(2,k);

    temp = modPow(base, m, n);
    if (temp == 1 || temp == (n - 1))
    {
        return false;
    }

    for (j = 1; j < k; j++)
    {
        temp = (temp * temp) % n;
        if (temp == (n - 1))
        {
            return false;
        }
    }

    return true;
}*/