////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// quadtree.h : header file for quadtree class
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef QUADTREE_H
#define QUADTREE_H

#include "node.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Quadtree
{
    public:

        // Constructor
        Quadtree(int xMin, int yMin, int xMax, int yMax, int maxNodeSize);

        // Destructor
        ~Quadtree();

        // Find the leaf node that encloses a given point
        Node* findLeafNode(Point* point);

        // Insert a Point into the quadtree
        bool insert(Point* point);

        // Insert random points into quadtree
        bool randomPoints(int numPoints);

        // Find the k nearest neighbors to a point
        bool findNearestNeighbors(Point* point, int k, vector<Entry*>& neighbors);

        // Verify a vector of nearest neighbors
        bool verifyNearestNeighbors(Point* point, vector<Entry*>& neighbors, int& numPointsVerified);

        // Print the tree
        void print();

    private:

        int     xMin;
        int     yMin;
        int     xMax;
        int     yMax;
        int     maxNodeSize;
        int     numPoints;
        Node*   root;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif  QUADTREE_H