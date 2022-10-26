#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
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
vector<int> bruteForceTSP(vector<City> cities, int numCities, double& bestDist);
double dist(City city1, City city2);
bool isValid(vector<int> set);
vector<vector<int>> genSubsets(int numCities);
bool readFile( string fileName, vector<City>& cities, int& numCities );
bool outputPath( string fileName, Path finalPath, int numCities );

int main(int argc, char**argv)
{
    int i, j, numCities;
    Path final;
    string fileName;
    vector<City> cities;
    vector<vector<Path>> distances; 

    // Make sure there is correct number of arguments
    if (argc != 2)
    {
        cout << "Usage: .\\bitonicTSP.exe [filename]";
        return 0;
    }

    fileName =  argv[1];

    // Read in list of cities
    if ( !readFile( fileName, cities, numCities ) )
    {
        return 0;
    }
    
    // Erase the .in in the file name
    fileName.erase(fileName.end() - 3, fileName.end());

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

    // Calc optimized path and distance
    final = betterTSP(cities, distances, 0, 0, 1, numCities);

    // Output the path to file and make sure it works.
    if (!outputPath(fileName, final, numCities))
    {
        cout << "Outputting to " << fileName + ".out" << " failed.";
        return 0;
    }

    return 0;
}

// My version for the TSP algorithm
Path betterTSP( vector<City> cities, vector<vector<Path>>& distances, int i, int j, int next, int numCities)
{
    Path final, inci, incj;

    // If we have been here before, return
    if ( distances[i][j].distance != -1 )
    {
        return distances[i][j];
    }

    // Base case, reached end of the cities
    if ( i == (numCities - 1 ) || j == (numCities - 1))
    {
        distances[i][j].distance = dist(cities[i],cities[j]);
        return distances[i][j];
    }

    // Create path where i goes to next node
    inci = betterTSP(cities, distances, next, j, next + 1, numCities);
    inci.distance += dist(cities[i],cities[next]); 

    // Create path where j goes to next node
    incj = betterTSP(cities, distances, i, next, next + 1, numCities);
    incj.distance += dist(cities[j],cities[next]);

    // Compare the distances and return the shortest
    if (inci.distance < incj.distance)
    {
        distances[i][j]= inci;
        return distances[i][j];
    }

    // If j is shorter, push j onto the path
    distances[i][j] = incj;
    distances[i][j].path.push_back(j);
    return distances[i][j];
}

// Return the distance between two cities
double dist(City city1, City city2) {
    double x = city1.x - city2.x;
    double y = city1.y - city2.y;
    x = pow(x,2);
    y = pow(y,2);
    return sqrt(x + y);
}


/*****************************************************************************
* Brute Force Method Functions
******************************************************************************/
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

// Read the cities from the file into a vector
bool readFile( string fileName, vector<City>& cities, int& numCities ) 
{
    int i;
    ifstream fin;

    // Open file
    fin.open( fileName, ios::in );
    if ( !fin.is_open() )
    {
        cout << "Could not open " << fileName;
        return false;
    }

    // Get total number of cities
    fin >> numCities;
    cities.resize(numCities);
    for ( i = 0; i < numCities; i++ )
    {
        fin >> cities[i].x;
        fin >> cities[i].y;
    }

    // Close file
    fin.close();

    return true;
}

// Output the path and distance to a file
bool outputPath( string fileName, Path finalPath, int numCities )
{
    int i;
    ofstream fout;
    vector<bool> used(numCities);

    // Open output file
    fout.open(fileName + ".out", ios::out | ios::trunc);
    if (!fout.is_open())
    {
        cout << "Could not open " << fileName + ".out";
        return false;
    }

    // Initialize used array
    for (i = 0; i < numCities; i++ )
    {
        used[i] = false;
    }

    // Print path going right
    for (i = finalPath.path.size() - 1; i >= 0; i--)
    {
        fout << finalPath.path[i] << " ";
        used[finalPath.path[i]] = true;
    }

    // Print returning path
    for (i = numCities - 1; i >= 0; i-- )
    {
        if (!used[i])
        {
            fout << i << " ";
        }
    }
    
    // Print optimal distance
    fout << endl << finalPath.distance;

    // Close the file
    fout.close();

    return true;
}