#include <iostream>
#include <string>
using namespace std;

int minTemp( int temp1, int temp2, int temp3 ) {
    int min = temp1;
    if ( temp2 < min )
        min = temp2;
    if ( temp3 < min )
        min = temp3;

    return min;
}

int editDistance( string a, int n, string b, int k, int table[][50] ){
    if (table[n][k] >= 0)
        return table[n][k];
    if ( n == 0 )
        return k;
    if ( k == 0 )
        return n;

    int result, temp1, temp2, temp3;
    
    if (a[n] == b[k] ) {
        result = editDistance( a, n - 1, b, k - 1, table);
        table[n][k] = result;
        return result;
    }

    temp1 = editDistance( a, n, b, k -1, table );
    temp2 = editDistance( a, n - 1, b, k, table );
    temp3 = editDistance( a, n - 1, b, k -1, table );
    result = minTemp(temp1, temp2, temp3) + 1;
    table[n][k] = result;
    return result;
}

int main() {
    string a = "Hello";
    string b = "Hello! This aidan!";
    int table[50][50];

    for (int i = 0; i < 50; i++)
        for (int j = 0; j < 50; j++)    
            table[i][j] = -1;

    cout << editDistance(a, a.length() - 1, b, b.length() - 1, table) << endl;

    return 0;
}