#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct City{
    double x = 0;
    double y = 0;
    bool visited = false;
};

bool readFile( string fileName, vector<City>& cities, int& numCities );
double dist(City city1, City city2);
double bruteForceTSP(vector<City> cities, int numCities, int from, int to);

int main(int argc, char**argv)
{
    int numCities;
    vector<City> cities;
    vector<double> distances;
    string fileName = argv[1];

    if ( !readFile( fileName, cities, numCities ) )
        return -1;

    distances.resize(numCities);

    return 0;
}

/*double bruteForceTSP(vector<City> cities, int numCities, int from, int to) {
    if (cities[to].visited)
        return -1;
}*/

double dist(City city1, City city2) {
    double x = city1.x - city2.x;
    double y = city1.y - city2.y;
    x = pow(x,2);
    y = pow(y,2);
    return sqrt(x + y);
}

bool readFile( string fileName, vector<City>& cities, int& numCities ) {
    ifstream fin;
    int i;

    fin.open( fileName, ios::in );
    if ( !fin.is_open() )
        return false;

    fin >> numCities;
    cities.resize(numCities);
    for ( i = 0; i < numCities; i++ ){
            fin >> cities[i].x;
            fin >> cities[i].y;
    }

    return true;
}

