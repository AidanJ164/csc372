#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

struct Point
{
    double x;
    double y;
};

// y = mx + b
struct Line
{
    double xmax;    // Max x
    double xmin;    // Min x
    double m;       // Slope
    double b;       // y-intercept
    Point from;     // Point that line segment starts    
};

double area(vector<Point> p, int n);
double findBisect(vector<Point> points, vector<Line> lines);
vector<Point> findLeftPoints(vector<Point> points, double x);
double findMinX(vector<Point> points);
double findMaxX(vector<Point> points);
vector<Line> getLines(vector<Point> points);
vector<Point> getIntersectionPoints(vector<Point> points, vector<Line> lines, double x);
Point getPoint(Line line, double x);
bool readFile(ifstream& fin, vector<Point>& points);

int main(int argc, char** argv)
{
    double x;
    ifstream fin;
    int caseNum = 1;
    ofstream fout;
    string fileName;
    vector<Line> lines;
    vector<Point> points(4);

    // Check for an input file
    if (argc != 2)
    {
        cout << "Usage: .\\bisect.exe inputfile";
        return 0;
    }

    fileName = argv[1];

    // Open Input File
    fin.open(fileName);
    if (!fin.is_open())
    {
        cout << fileName << " could not be opened.";
        return 0;
    }

    // Open Output File
    fileName.erase(fileName.end() - 3, fileName.end());
    fout.open(fileName + ".out");
    if (!fout.is_open())
    {
        cout << fileName << " could not be opened.";
    }

    // Read points from file until we reach a -1
    while (readFile(fin, points))
    {
        // Create line segments
        lines = getLines(points);

        // Find bisecting vertical line
        x = findBisect(points, lines);

        // Output bisect
        fout << "Case " << caseNum << ": ";
        fout << fixed << setprecision(5) << x << endl;

        // Increment case number
        caseNum++;
    }

    // Close file
    fin.close();
    fout.close();

    return 0;
}

// Taken from d2l geometry.c
double area(vector<Point> p, int n)
{
    int i;  
    int j;  
    double result;

    result = 0;
    for (i = 0; i < n; i++)
    {
        j = (i + 1) % n;
        result += p[i].x * p[j].y;
        result -= p[i].y * p[j].x;
    }
    return fabs(result / 2);
}

double findBisect(vector<Point> points, vector<Line> lines)
{
    double Aleft, Aright, i, min, max, step, totalArea, x;
    vector<Point> allPoints;
    vector<Point> leftPoints;

    // Find total area of quadrilateral
    totalArea = area(points, 4);
    
    // Find min and max x
    min = findMinX(points);
    max = findMaxX(points);

    // Find middle x as vertical line
    x = (min + max) / 2;

    // Create a step to inc/dec from x
    step = x;

    do 
    {
        // Get points that intersect the quadrilateral
        allPoints = getIntersectionPoints(points, lines, x);

        // Find points left of the line
        leftPoints = findLeftPoints(allPoints, x);

        // Find areas of both sides
        Aleft = area(leftPoints, leftPoints.size());
        Aright = totalArea - Aleft;

        // Make the step smaller
        step /= 2;

        // Move x in the direction of the bigger area
        if (Aleft > Aright)
        {
            x -= step;
        }
        else if (Aleft < Aright)
        {
            x += step;
        }
        
    // Repeat until both sides have equal area
    } while( (Aleft > Aright + 0.000001) || (Aleft < Aright - 0.000001) );

    return x;
}

vector<Point> findLeftPoints(vector<Point> points, double x)
{
    int i;
    vector<Point> left;

    // Loop thru points adding to vector if x is less than given x
    for ( i = 0; i < points.size(); i++ )
    {
        if ( points[i].x <= x )
        {
            left.push_back(points[i]);
        }
    }

    return left;
}

double findMinX(vector<Point> points)
{
    int i;
    double x = points[0].x;

    // Loop thru all points and find the leftmost x
    for (i = 1; i < points.size(); i++)
    {
        if (points[i].x < x)
        {
            x = points[i].x;
        }
    }

    return x;
}

double findMaxX(vector<Point> points)
{
    int i;
    double x = points[0].x;

    // Loop thru all points and find the rightmost x
    for (i = 1; i < points.size(); i++)
    {
        if (points[i].x > x)
        {
            x = points[i].x;
        }
    }

    return x;
}

vector<Line> getLines(vector<Point> points)
{
    int i;
    vector<Line> lines(4);

    // Get line segments that make the quadrilateral
    for ( i = 0; i < 3; i++)
    {
        // Line segment runs from xmin to xmax
        lines[i].xmin = min(points[i].x, points[i + 1].x);
        lines[i].xmax = max(points[i].x, points[i + 1].x);

        // Get slope
        lines[i].m = (points[i+1].y - points[i].y) / (points[i+1].x - points[i].x);

        // Get y intercept
        lines[i].b = points[i].y - (lines[i].m * points[i].x);

        // Get point that line segment starts from
        lines[i].from = points[i];
    }

    // Line segment from last point to first point
    lines[3].xmin = min(points[3].x, points[0].x);
    lines[3].xmax = max(points[3].x, points[0].x);

    lines[3].m = (points[0].y - points[3].y) / (points[0].x - points[3].x);
    lines[3].b = points[3].y - (lines[3].m * points[3].x);

    lines[3].from = points[3];

    return lines;
}

vector<Point> getIntersectionPoints(vector<Point> points, vector<Line> lines, double x)
{
    int i;
    vector<Point> allPoints = points;

    // Add the points that are created from the intersection of the shape and x
    for ( i = lines.size() - 1; i >= 0; i-- )
    {
        if (x > lines[i].xmin && x < lines[i].xmax)
        {
            allPoints.emplace( allPoints.begin() + i + 1, getPoint(lines[i], x));
        }
    }

    return allPoints;
}

Point getPoint(Line line, double x)
{
    Point point;

    point.x = x;

    // Get y using a line equation and x
    point.y = ( x * line.m ) + line.b;

    return point;
}

bool readFile(ifstream& fin, vector<Point>& points)
{
    int i;

    // Read in four points
    for (i = 0; i < 4; i++)
    {
        fin >> points[i].x;
        fin >> points[i].y;

        // If the point is at -1, end of file has been reached
        if (points[i].x == -1)
        {
            return false;
        }
    }

    return true;
}