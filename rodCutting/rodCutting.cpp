#include <iostream>
using namespace std;

int bestCut(int n, int p[]) {
    if ( n == 0 )
        return 0;
    if ( n == 1 )
        return p[1];
    
    int bestSoFar = p[n];
    int bestTemp;
    for (int i = 0; i < n - 1; i++ ) {
        bestTemp = bestCut(i, p) + bestCut(n - i, p );
        bestSoFar = (bestTemp > bestSoFar) ? bestTemp : bestSoFar;
    }

    return bestSoFar;
}

int main() {
    int p[] = {0, 1, 5, 8, 9};
    int size = 4;

    cout << bestCut(size, p);
    
    return 0;
}