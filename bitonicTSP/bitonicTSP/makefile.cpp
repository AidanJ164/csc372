#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    ofstream fout;

    fout.open("sample.in");

    for (int i = 0; i < 50; i++ )
    {
        fout << i << " " << rand() % 50 << endl;
    }

    fout.close();

    return 0;
}