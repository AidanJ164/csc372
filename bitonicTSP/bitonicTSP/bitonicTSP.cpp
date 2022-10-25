#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <chrono>
using namespace std;

struct City{
    double x = 0;
    double y = 0;
};

struct Path{
    double distance = -1;
    vector<int> path;
};

Path betterTSP( vector<City> cities, vector<vector<Path>>& distances, int i, int j, int next, int numCities);
double betterTSP( vector<City> cities, vector<vector<double>>& best, int i, int j);
vector<int> bruteForceTSP(vector<City> cities, int numCities, double& bestDist);
double dist(City city1, City city2);
bool isValid(vector<int> set);
vector<vector<int>> genSubsets(int numCities);
bool readFile( string fileName, vector<City>& cities, int& numCities );
bool outputPath( string fileName, Path finalPath, int numCities );



int main(int argc, char**argv)
{
    int i, j;
    int numCities;
    vector<City> cities;
    vector<vector<Path>> distances; 
    double bestDist = 0;
    vector<int> path;
    string fileName = argv[1];
    Path final;

    if ( !readFile( fileName, cities, numCities ) )
        return -1;
    
    fileName.erase(fileName.end() - 3, fileName.end());

    path = bruteForceTSP(cities, numCities, bestDist);
    for ( i = 0; i < numCities; i++) 
    {
        cout << path[i] << " ";
    }
    cout << endl << bestDist;

    // Inititalize distance matrix
    distances.resize(numCities);
    for (i = 0; i < numCities; i++ )
    {
        distances[i].resize(numCities);
        for (j = 0; j < numCities; j++)
        {
            distances[i][j].distance = -1;
        }
    }

    final = betterTSP(cities, distances, 0, 0, 1, numCities);
    outputPath(fileName, final, numCities);

    return 0;
}

Path betterTSP( vector<City> cities, vector<vector<Path>>& distances, int i, int j, int next, int numCities)
{
    Path final;
    Path inci, incj;

    if ( distances[i][j].distance != -1 )
    {
        return distances[i][j];
    }

    if ( i == (numCities - 1 ) || j == (numCities - 1))
    {
        distances[i][j].distance = dist(cities[i],cities[j]);
        return distances[i][j];
    }

    inci = betterTSP(cities, distances, next, j, next + 1, numCities);
    inci.distance += dist(cities[i],cities[next]); 
    incj = betterTSP(cities, distances, i, next, next + 1, numCities);
    incj.distance += dist(cities[j],cities[next]);

    if (inci.distance < incj.distance)
    {
        distances[i][j]= inci;
        return distances[i][j];
    }

    distances[i][j] = incj;
    distances[i][j].path.push_back(j);
    return distances[i][j];
}

// best alg, O(n^2), cant return path
double betterTSP( vector<City> cities, vector<vector<double>>& best, int i, int j)
{
     if (best[i][j] != -1)
    {
        return best[i][j];
    }

    double temp;

    if ( i < (j -1) )
    {
        best[i][j] = betterTSP(cities, best, i, j - 1) + dist(cities[j - 1], cities[j]); 
        return best[i][j];
    }

    best[i][j] = betterTSP(cities, best, i - 1, i) + dist(cities[i - 1],cities[j]);
    for ( int k = i - 2; k >= 0; k-- )
    {
        temp = betterTSP(cities, best, k, i) + dist(cities[k], cities[j]);
        
        if (temp < best[i][j])
        {
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

    fin.close();

    return true;
}

bool outputPath( string fileName, Path finalPath, int numCities )
{
    int i;
    ofstream fout;
    vector<bool> used(numCities);

    fout.open(fileName + ".out", ios::out | ios::trunc);
    if (!fout.is_open())
    {
        cout << "Could not open " << fileName + ".out";
        return false;
    }


    for (i = 0; i < numCities; i++ )
    {
        used[i] = false;
    }

    for (i = finalPath.path.size() - 1; i >= 0; i--)
    {
        fout << finalPath.path[i] << " ";
        used[finalPath.path[i]] = true;
    }

    for (i = numCities - 1; i >= 0; i-- )
    {
        if (!used[i])
        {
            fout << i << " ";
        }
    }
    fout << endl << finalPath.distance;

    fout.close();
    return true;
}