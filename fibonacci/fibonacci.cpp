#include <iostream>
using namespace std;

int fibRec(int n) {
    if (n <= 2)
        return 1;
    
    return fibRec( n - 2) + fibRec( n - 1);
}

int fib(int n, int table[]) {
    if (table[n] >= 0)
        return table[n];

    table[n] = fib(n - 2, table) + fib(n - 1, table);

    return table[n];
}

int main()
{
    int n;
    int table[50];

    for ( int i = 0; i < 50; i++ )
        table[i] = -1;

    table[0] = 0;
    table[1] = 1;

    cin >> n;
    cout << endl << "Fib of " << n << ": " << fib(n, table) << endl;
    cout << "Recursion Fib of " << n << ": " << fibRec(n) << endl;

    //for (int i = 0; i <= n; i++ )
        //cout << i << ": " << table[i] << endl;

    return 0;
}