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

Path betterTSP( vector<City> cities, vector<vector<Path>>& best, int i, int j);
//double betterTSP( vector<City> cities, vector<vector<double>>& best, vector<int>& path, int i, int j);
double betterTSP( vector<City> cities, vector<vector<double>>& distances, vector<int>& path, int i, int j, int next, int numCities);
//double betterTSP( vector<City> cities, vector<vector<double>>& best, vector<int>& ipath, vector<int>& jpath, int i, int j);
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
    vector<vector<double>> best; 
    vector<vector<Path>> best2;
    vector<int> path;
    string fileName = argv[1];
    ofstream fout;

    Path final;

    if ( !readFile( fileName, cities, numCities ) )
        return -1;
    
    fileName.erase(fileName.end() - 3, fileName.end());
    fout.open(fileName + ".csv", ios::out | ios::trunc);
    if (!fout.is_open())
    {
        cout << " didnt open" << endl;
        return -1;
    }

    bestPath = bruteForceTSP(cities, numCities, bestDist);

    cout << "Final Path: {"; 
    for (i = 0; i < numCities; i++)
    {
        cout << bestPath[i] << " ";
    }    
    cout << "}" << endl << "Distance: " << bestDist << endl;


    best.resize(numCities);
    path.clear();
    for (i = 0; i < numCities; i++ )
    {
        best[i].resize(numCities);
        for (j = 0; j < numCities; j++)
        {
            best[i][j] = -1;
        }
    }
    cout << endl << "Slower TSP: " << betterTSP(cities, best, path, 0, 0, 1, numCities) << endl;

    for( i = 0; i < path.size(); i++)
    {
        cout << path[i] << " ";
    }

    cout << endl;

    for (i = 0; i < numCities; i++)
    {
        for ( j = 0; j < numCities; j++)
        {
            cout << setw(11) << best[i][j];
        }
        cout << endl;
    }

    best2.resize(numCities);
    for (i = 0; i < numCities; i++ )
    {
        best2[i].resize(numCities);
        for (j = 0; j < numCities; j++)
        {
            best2[i][j].distance = -1;
        }
    }
    best2[0][1].distance = dist(cities[0],cities[1]);
    //best2[0][1].path.push_back(1);
    cout << final.distance << endl;

    final =  betterTSP(cities, best2, numCities - 1, numCities - 1);
    cout << final.distance << endl;

    for (i = 0; i < numCities; i++)
    {
        for ( j = 0; j < numCities; j++)
        {
            cout << setw(11) << best2[i][j].distance;
        }
        cout << endl;
    }

    for (i = 0; i < final.path.size(); i++ )
    {
        cout << final.path[i] << " ";
    }
    cout << endl;
    for (i = 0; i < best2[numCities - 1][numCities - 1].path.size(); i++)
    {
        cout << best2[numCities - 1][numCities - 1].path[i] << " ";
    }
    /*
    for ( numCities = 3; numCities < 200; numCities++ )
    {
        cout << "here";
        best.resize(numCities);
        auto start = chrono::high_resolution_clock::now();

        for (i = 0; i < numCities; i++)
        {
            best[i].resize(numCities);
            for (j = 0; j < numCities; j++ )
            {
                best[i][j] = -1;
            }
        }
        best[0][1] = dist(cities[0], cities[1]);

        fout << numCities << "," << betterTSP(cities, best, path, numCities - 1, numCities - 1) << ",";

        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(stop-start);
        fout << duration.count() << endl;
    }*/
    fout.close();

    return 0;
}

// Another attempt, saving the best path along with the best distance.
Path betterTSP( vector<City> cities, vector<vector<Path>>& best, int i, int j)
{
    Path path;
    Path tempPath;
    Path tempPath2;

    if (best[i][j].distance != -1)
    {
        return best[i][j];
    }

    double temp;

    if ( i < (j - 1) )
    {
        best[i][j] = betterTSP(cities, best, i, j - 1);
        best[i][j].distance += dist(cities[j - 1], cities[j]);
        
        //best[i][j].path.push_back(j - 1);

        return best[i][j];
    }

    best[i][j] = betterTSP(cities, best, i - 1, i);
    best[i][j].distance += dist(cities[i - 1],cities[j]);
    int tempIndex = i - 1;
    for ( int k = i - 2; k >= 0; k-- )
    {
        tempPath = betterTSP(cities, best, k, i);
        tempPath.distance += dist(cities[k], cities[j]);

        if (tempPath.distance < best[i][j].distance)
        {
            tempIndex = k;
            best[i][j] = tempPath;
        }
    }

    best[i][j].path.push_back(tempIndex);
    return best[i][j];
}

// Slower tsp, probably O(2^n), faster with dynamic programming
double betterTSP( vector<City> cities, vector<vector<double>>& distances, vector<int>& path, int i, int j, int next, int numCities)
{
    if ( i == (numCities - 1 ) || j == (numCities - 1))
    {
        return dist(cities[i],cities[j]);
    }

    double inci, incj;
    vector<int> path1 = path;
    vector<int> path2 = path;

    inci = betterTSP(cities, distances, path1, next, j, next + 1, numCities) + dist(cities[i],cities[next]);
    incj = betterTSP(cities, distances, path2, i, next, next + 1, numCities) + dist(cities[j],cities[next]);

    if (inci < incj)
    {
        path = path1;
        //path.push_back(i);
        //path.insert(path.begin(), path1.begin() + path.size(), path1.end());
        distances[i][j] = inci;
        return inci;
    }

    path = path2;
    path.push_back(j);
    //path.insert(path.begin(), path2.begin() + path.size(), path1.end());
    distances[i][j] = incj;
    return incj;
}

/*
// best alg, O(n^2), cant return path
double betterTSP( vector<City> cities, vector<vector<double>>& best, vector<int>& path, int i, int j)
{
     if (best[i][j] != -1)
    {
        return best[i][j];
    }

    double temp;

    if ( i < (j -1) )
    {
        best[i][j] = betterTSP(cities, best, path, i, j - 1) + dist(cities[j - 1], cities[j]); 
        return best[i][j];
    }

    best[i][j] = betterTSP(cities, best, path, i - 1, i) + dist(cities[i - 1],cities[j]);
    for ( int k = i - 2; k >= 0; k-- )
    {
        temp = betterTSP(cities, best, path, k, i) + dist(cities[k], cities[j]);
        
        if (temp < best[i][j])
        {
            best[i][j] = temp;
        }
    }
    return best[i][j];
}*/

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
