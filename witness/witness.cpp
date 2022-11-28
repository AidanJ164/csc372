#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

typedef long long int lli;

int findNonWitnesses(lli num);
bool isPrime(lli n);
bool isWitness(lli n, lli base);
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
            fout << num << " " << findNonWitnesses(num) << endl;
        }
        fin >> num;
    }
    
    fin.close();
    return 0;
}

int findNonWitnesses(lli num)
{
    int i, numWitnesses = 0;

    for (i = 2; i < num - 1; i++)
    {
        if (!isWitness(num, i))
        {
            numWitnesses++;
        }
    }

    return numWitnesses;
}

bool isPrime(lli n)
{
    int i;

    for (i = 2; i <= sqrt(n); i++)
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
}

