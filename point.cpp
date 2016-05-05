////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// point.cpp : class for 2D point
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "point.h"
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Constructor
Point::Point(int x, int y)
    : x(x), y(y)
{
    // N/A
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Get the point's x coordinate
int Point::getX()
{
    return this->x;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Get the point's y coordinate
int Point::getY()
{
    return this->y;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Calculate the distance
double Point::distance(int x, int y)
{
    double xDistance = double(abs(this->x - x));
    double yDistance = double(abs(this->y - y));
    double distance = sqrt((xDistance * xDistance) + (yDistance * yDistance));

    return distance;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Calculate the distance
double Point::distance(Point* otherPoint)
{
    int otherX = otherPoint->getX();
    int otherY = otherPoint->getY();
    double distance = this->distance(otherX, otherY);

    return distance;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Calculate the distance
double Point::distance(int xMin, int yMin, int xMax, int yMax)
{
    // Set the x coordinate of the closest point on the bounding box
    int xTarget = this->x;
    if (this->x <= xMin)
    {
        xTarget = xMin;
    }
    else if (this->x >= xMax)
    {
        xTarget = xMax;
    }

    // Set the y coordinate of the closest point on the bounding box
    int yTarget = this->y;
    if (this->y <= yMin)
    {
        yTarget = yMin;
    }
    else if (this->y >= yMax)
    {
        yTarget = yMax;
    }

    // Calculate the distance
    double distance = this->distance(xTarget, yTarget);

    return distance;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Print
void Point::print()
{
    cout << "(" << this->x << "," << this->y << ")" << endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
