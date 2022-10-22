#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

struct City{
    double x = 0;
    double y = 0;
};

double betterTSP( vector<City> cities, vector<vector<double>>& best, vector<vector<int>>& path, int i, int j);
//double betterTSP( vector<City> cities, vector<vector<double>>& best, int i, int j);
vector<int> bruteForceTSP(vector<City> cities, int numCities, double& bestDist);
double dist(City city1, City city2);
bool isValid(vector<int> set);
vector<vector<int>> genSubsets(int numCities);
bool readFile( string fileName, vector<City>& cities, int& numCities );


int main(int argc, char**argv)
{
    int i, j;
    int numCities;
    double bestDist;
    vector<City> cities;
    vector<int> bestPath;
    vector<vector<double>> best(numCities);
    vector<vector<int>> path(numCities);
    string fileName = argv[1];
    ofstream fout;

    if ( !readFile( fileName, cities, numCities ) )
        return -1;
    
    fileName.erase(fileName.end() - 3, fileName.end());
    fout.open(fileName + ".out");

    bestPath = bruteForceTSP(cities, numCities, bestDist);

    cout << "Final Path: {"; 
    for (i = 0; i < numCities; i++)
    {
        cout << bestPath[i] << " ";
    }    
    cout << "}" << endl << "Distance: " << bestDist << endl;

    // Initialize best array
    for (i = 0; i < numCities; i++ )
    {
        best[i].resize(numCities);
        path[i].resize(numCities);
        for (j = 0; j < numCities; j++)
        {
            best[i][j] = -1;
        }
    }

    //betterTSP(cities, numCities, bestDist);
    //cout << betterTSP(cities, best, numCities - 1, numCities - 1) << endl;
    cout << betterTSP(cities, best, path, numCities - 1, numCities - 1) << endl;


    for(i = 0; i < numCities; i++)
        cout << setw(11) << i;

    cout << endl;
    for (i = 0; i < numCities; i++ )
    {
        cout << i << " ";
        for (j = 0; j < numCities; j++)
        {
            cout << setw(11) << best[i][j] << " ";
        }
        cout << endl;
    }

    for (i = 0; i < numCities; i++ )
    {
        for (j = 0; j < numCities; j++)
        {
            cout << setw(11) << path[i][j];
        }
        cout << endl;
    }

    cout << dist(cities[1],cities[2]) << endl;
    cout << dist(cities[2],cities[3]) << endl;

    return 0;
}

double betterTSP( vector<City> cities, vector<vector<double>>& best, vector<vector<int>>& path, int i, int j)
{
    if (best[i][j] != -1)
    {
        return best[i][j];
    }

    if ( i == 0 && j == 1)
    {
        best[0][1] = dist(cities[0],cities[1]);
        path[0][1] = 1;
        return best[0][1];
    }

    double temp;

    if ( i < (j -1) )
    {
        best[i][j] = betterTSP(cities, best, path, i, j - 1) + dist(cities[j - 1], cities[j]); 
        path[i][j] = j - 1;
        return best[i][j];
    }

    best[i][j] = betterTSP(cities, best, path, i - 1, i) + dist(cities[i - 1],cities[j]);
    path[i][j] = i - 1;
    for ( int k = i - 2; k >= 0; k-- )
    {
        temp = betterTSP(cities, best, path, k, i) + dist(cities[k], cities[j]);
        
        //best[i][j] = (temp < best[i][j]) ? temp : best[i][j];
        if (temp < best[i][j])
        {
            path[i][j] = k;
            best[i][j] = temp;
        }
    }
    return best[i][j];
}

vector<int> bruteForceTSP(vector<City> cities, int numCities, double& bestDist)
{
    int i, j;
    double tempDist = 0;
    vector<int> bestPath(numCities);
    vector<int> currPath(numCities);
    vector<vector<int>> subsets;

    subsets = genSubsets(numCities);

    bestPath = subsets[0];
    bestDist = 0;
    for (i = 1; i < numCities; i++)
    {
        bestDist += dist(cities[i], cities[i - 1]);
    }   
    bestDist += dist(cities[numCities - 1], cities[0]);

    for (i = 1; i < subsets.size(); i++)
    {
        tempDist = 0;
        for (j = 1; j < numCities; j++)
        {
            tempDist += dist(cities[subsets[i][j]], cities[subsets[i][j - 1]]);
        }
        tempDist += dist(cities[subsets[i][0]], cities[subsets[i][numCities - 1]]);
        if (tempDist < bestDist)
        {
            bestDist = tempDist;
            bestPath = subsets[i];
        }
    }

    return bestPath;
}

double dist(City city1, City city2) {
    double x = city1.x - city2.x;
    double y = city1.y - city2.y;
    x = pow(x,2);
    y = pow(y,2);
    return sqrt(x + y);
}

bool isValid(vector<int> set)
{
    int i = 2;
    if (!(set[0] == 0 && set[1] == 1))
        return false;

    while( set[i] != set.size() - 1)
    {
        if (set[i] < set[i-1]) {
            return false;
        }
        i++;
    }
    i++;
    while( i < set.size())
    {
        if (set[i] > set[i - 1]) {
            return false;
        }
        i++;
    }
    return true;
}

vector<vector<int>> genSubsets(int numCities)
{
    int i;
    vector<int> subset(numCities);
    vector<vector<int>> allSubsets;
    
    for (i = 0; i < numCities; i++)
    {
        subset[i] = i;
    }

    do {
        if (isValid(subset))
        {
            allSubsets.push_back(subset);
        }
    } while ( next_permutation(subset.begin(), subset.end()));

    return allSubsets;
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
