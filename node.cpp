////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// node.cpp : class for quadtree node
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "heap.h"
#include "node.h"
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Constructor
Node::Node(int xMin, int yMin, int xMax, int yMax)
    : xMin(xMin), yMin(yMin), xMax(xMax), yMax(yMax)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Return the number of points contained in node
int Node::numPoints()
{
    return this->points.size();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Insert a point into the node
bool Node::insertPoint(Point* point)
{
    try
    {
        this->points.push_back(point);
    }
    catch (bad_alloc)
    {
        cout << "ERROR: out of memory when inserting point" << endl;
        point->print();
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Return the index of the child that contains the given point
int Node::findChildIndex(int x, int y)
{
    // Calculate this node's midpoint, avoid possible integer overflow
    int xMid = this->xMin + (this->xMax - this->xMin) / 2;
    int yMid = this->yMin + (this->yMax - this->yMin) / 2;

    // Children are stored in Z order
    if (x < xMid && y < yMid)
    {
        return 0; // lower left
    }
    else if (x >= xMid && y < yMid)
    {
        return 1; // lower right
    }
    else if (x < xMid && y >= yMid)
    {
        return 2; // upper left
    }
    else
    {
        return 3; // upper right
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Return the index of the child that contains the given point
int Node::findChildIndex(Point* point)
{
    // Get the coordinates of the point
    int x = point->getX();
    int y = point->getY();

    // Calculate this node's midpoint, avoid possible integer overflow
    int xMid = this->xMin + (this->xMax - this->xMin) / 2;
    int yMid = this->yMin + (this->yMax - this->yMin) / 2;

    // Children are stored in Z order
    if (x < xMid && y < yMid)
    {
        return 0; // lower left
    }
    else if (x >= xMid && y < yMid)
    {
        return 1; // lower right
    }
    else if (x < xMid && y >= yMid)
    {
        return 2; // upper left
    }
    else
    {
        return 3; // upper right
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Return the child with the given index
Node* Node::getChild(int index)
{
    assert(index >= 0 && index <= 3);

    return this->children[index];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Find the leaf node that encloses a given point
Node* Node::findLeafNode(int x, int y)
{
    // Check if this node has children
    if (this->children.size() > 0)
    {
        // Find which of this node's children contains the point
        int childIndex = this->findChildIndex(x, y);
        return this->children[childIndex]->findLeafNode(x, y);
    }
    else
    {
        return this;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Find the k nearest neighbors to a point
bool Node::findNearestNeighbors(int x, int y, Heap& heap)
{
    // Check if this node has children
    if (this->children.size() > 0)
    {
        // Find which of this node's children contains the point
        int childIndex = this->findChildIndex(x, y);
        this->children[childIndex]->findNearestNeighbors(x, y, heap);

        // Try the other sibling nodes
        Point queryPoint = Point(x, y);
        for (int i = 0; i < 4; ++i)
        {
            if (i == childIndex) continue;
            Node* child = this->children[i];

            // If we haven't found k points yet, visit the sibling node
            if (!heap.full())
            {
                child->findNearestNeighbors(x, y, heap);
            }
            else
            {
                // We have k points already
                // Only visit sibling node if there's a possibility of a closer point in it than what we have so far
                double minNodeDistance = queryPoint.distance(child->xMin, child->yMin, child->xMax, child->yMax);
                double maxHeapDistance = heap.top()->getDistance();
                if (minNodeDistance < maxHeapDistance)
                {
                    child->findNearestNeighbors(x, y, heap);
                }
            }
        }

        return true;
    }
    else
    {
        for (auto iter = this->points.begin(); iter != this->points.end(); ++iter)
        {
            Point* currentPoint = *iter;
            int currentX = currentPoint->getX();
            int currentY = currentPoint->getY();
            double distance = currentPoint->distance(x, y);

            Entry* entry = new Entry(currentX, currentY, distance);
            if (!heap.pushOrReject(entry))
            {
                delete entry;
            }
        }

        return true;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Split a node into four children, redistribute points
bool Node::split()
{
    // Make sure this node is a leaf
    if (this->children.size() != 0)
    {
        cout << "ERROR: cannot split a non-leaf node" << endl;
        return false;
    }

    // Calculate this node's midpoint, avoid possible integer overflow
    int xMid = this->xMin + (this->xMax - this->xMin) / 2;
    int yMid = this->yMin + (this->yMax - this->yMin) / 2;

    // Create the child nodes, children are stored in Z order
    try
    {
        this->children.push_back(new Node(this->xMin, this->yMin, xMid, yMid));    // child 0, lower left
        this->children.push_back(new Node(xMid, this->yMin, this->xMax, yMid));    // child 1, lower right
        this->children.push_back(new Node(this->xMin, yMid, xMid, this->yMax));    // child 2, upper left
        this->children.push_back(new Node(xMid, yMid, this->xMax, this->yMax));    // child 3, upper right
    }
    catch (bad_alloc)
    {
        cout << "ERROR: out of memory when splitting node" << endl;
        this->print();
        return false;
    }

    // Copy the points to the appropriate child
    for (auto iter = this->points.begin(); iter != this->points.end(); ++iter)
    {
        Point* currentPointPtr = *iter;

        // Redistribute the point to the appropriate child
        int childIndex = this->findChildIndex(currentPointPtr);
        if (!this->children[childIndex]->insertPoint(currentPointPtr))
        {
            return false;
        }
    }

    // Delete the points from this node
    vector<Point*>().swap(this->points);

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Verify a vector of nearest neighbors
bool Node::verifyNearestNeighbors(Point* queryPoint, vector<Entry*>& neighbors, int& numPointsVerified)
{
    // Get the maximum distance amongst the neighbors
    double maxDistance = neighbors.back()->getDistance();

    // Check if any point is closer than those in the vector
    for (auto iter = this->points.begin(); iter != this->points.end(); ++iter)
    {
        Point* otherPoint = *iter;
        ++numPointsVerified;

        double otherDistance = queryPoint->distance(otherPoint);
        if (otherDistance < maxDistance)
        {
            // Check if this closer point is in the neighbors vector
            bool found = false;
            for (auto iter2 = neighbors.begin(); iter2 != neighbors.end(); ++iter2)
            {
                Entry* entry = *iter2;
                if (entry->equivalent(otherPoint))
                {
                    found = true;
                    break;
                }
            }

            // Verification failed
            if (!found)
            {
                return false;
            }
        }
    }

    // Recursive check
    for (auto iter = this->children.begin(); iter != this->children.end(); ++iter)
    {
        Node* nodePtr = *iter;
        if (!nodePtr->verifyNearestNeighbors(queryPoint, neighbors, numPointsVerified))
        {
            return false;
        }
    }

    // fall through, all children must have been verified
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Destructor
void Node::deepDelete()
{
    // Recursive delete
    for (auto iter = this->children.begin(); iter != this->children.end(); ++iter)
    {
        Node* nodePtr = *iter;
        nodePtr->deepDelete();
        delete nodePtr;
    }

    // Delete the points in this node
    for (auto iter = this->points.begin(); iter != this->points.end(); ++iter)
    {
        Point* pointPtr = *iter;
        delete pointPtr;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Print a single node
void Node::print()
{
    cout << "(" << this->xMin << "," << this->yMin << ")" 
         << "(" << this->xMax << "," << this->yMax << ")"
         << " " << this->points.size() << " points"
         << endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Print the points contained in a node
void Node::printPoints()
{
    for (auto iter = this->points.begin(); iter != this->points.end(); ++iter)
    {
        (*iter)->print();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Print
void Node::printTree(int level)
{
    for (int i = 0; i < level; ++i)
    {
        cout << "\t";
    }
    this->print();

    for (auto iter = this->children.begin(); iter != this->children.end(); ++iter)
    {
        Node* child = *iter;
        child->printTree(level + 1);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
