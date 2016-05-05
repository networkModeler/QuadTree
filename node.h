////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// node.h : header file for quadtree node class
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NODE_H
#define NODE_H

#include <vector>
#include "point.h"
#include "heap.h"
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Node
{
    public:

        // Constructor
        Node(int xMin, int yMin, int xMax, int yMax);

        // Destructor
        void deepDelete();

        // Return the number of points contained in node
        int numPoints();

        // Insert a point into the node
        bool insertPoint(Point* point);

        // Return the index of the child that contains the given point
        int findChildIndex(Point* point);
        int findChildIndex(int x, int y);

        // Return the child with the given index
        Node* getChild(int index);

        // Find the leaf node that encloses a given point
        Node* findLeafNode(int x, int y);

        // Find the k nearest neighbors to a point
        bool findNearestNeighbors(int x, int y, Heap& heap);

        // Split a node into four children, redistribute points
        bool split();

        // Verify a vector of nearest neighbors
        bool verifyNearestNeighbors(Point* queryPoint, vector<Entry*>& neighbors, int& numPointsVerified);

        // Print a single node
        void print();

        // Print the points contained in a node
        void printPoints();

        // Print a node and all its descendants
        void printTree(int level);

    private:

        int xMin;
        int yMin;
        int xMax;
        int yMax;

        // TODO: a given node only needs either points or children, not both
        vector<Point*>  points;
        vector<Node*>   children;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif  NODE_H