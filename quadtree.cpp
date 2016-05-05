////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// quadtree.cpp : N nearest neighbors
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "heap.h"
#include "quadtree.h"
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Constructor
Quadtree::Quadtree(int xMin, int yMin, int xMax, int yMax, int maxNodeSize)
    : xMin(xMin), yMin(yMin), xMax(xMax), yMax(yMax), maxNodeSize(maxNodeSize)
{
    cout << "constructing quadtree \n";

    // Create the root node
    this->root = new Node(xMin, yMin, xMax, yMax);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Find the leaf node that encloses a given point
Node* Quadtree::findLeafNode(Point* point)
{
    // Get the point's coordinates
    int x = point->getX();
    int y = point->getY();

    // Check if point is outside of the quadtree
    if (x < this->xMin || x > this->xMax || y < this->yMin || y > this->yMax)
    {
        cout << "ERROR:  point is outside the bounds of the quadtree" << endl;
        return NULL;
    }

    return this->root->findLeafNode(x, y);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Insert a Point into the quadtree
bool Quadtree::insert(Point* point)
{
    // Find the leaf node that should contain this point
    Node* node = this->findLeafNode(point);
    if (!node)
    {
        return false;
    }

    // Check if this node should be split
    if (node->numPoints() < this->maxNodeSize)
    {
        node->insertPoint(point);
    }
    else
    {
        if (node->split())
        {
            int childIndex = node->findChildIndex(point);
            Node* childNode = node->getChild(childIndex);
            childNode->insertPoint(point);
        }
        else
        {
            node->insertPoint(point);
            return false;
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Insert random points into quadtree
bool Quadtree::randomPoints(int numPoints)
{
    srand(0);
    for (int i = 0; i < numPoints; ++i)
    {
        int x = rand() % this->xMax;
        int y = rand() % this->yMax;

        Point* pointPtr = new Point(x, y);

        this->insert(pointPtr);
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Find the k nearest neighbors to a point
bool Quadtree::findNearestNeighbors(Point* point, int k, vector<Entry*>& neighbors)
{
    cout << "\n";
    cout << "finding k nearest neighbors for ";
    point->print();

    // Get the point's coordinates
    int x = point->getX();
    int y = point->getY();

    // Check if point is outside of the quadtree
    if (x < this->xMin || x > this->xMax || y < this->yMin || y > this->yMax)
    {
        cout << "ERROR:  point is outside the bounds of the quadtree" << endl;
        return false;
    }

    // Find the nearest neighbors
    Heap heap = Heap(k);
    this->root->findNearestNeighbors(x, y, heap);
    heap.toVector(neighbors);

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Verify a vector of nearest neighbors
bool Quadtree::verifyNearestNeighbors(Point* point, vector<Entry*>& neighbors, int& numPointsVerified)
{
    cout << endl;
    cout << "verifying nearest neighbors" << endl;

    if (this->root != NULL)
    {
        return this->root->verifyNearestNeighbors(point, neighbors, numPointsVerified);
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Print the tree
void Quadtree::print()
{
    this->root->printTree(0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Destructor
Quadtree::~Quadtree()
{
    if (this->root != NULL)
    {
        this->root->deepDelete();
        delete this->root;
        this->root = NULL;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

