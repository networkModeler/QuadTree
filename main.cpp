////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// main.cpp : test driver for quadtree
//
// This program implements a quadtree as a solution to the problem:
// - given one million points in a two dimensional region
// - given an arbitrary query point in that same region
// - find the k nearest neighbors to the query point
//
// Initially, I had considered doing an expanding search
// that starts from the quadtree node that contains the query point.
// However, identifying which nodes are adjacent to any given node
// is rather complicated in a quadtree, where the levels are not balanced.
// This would be a headache to code, test, and support.
// 
// Instead, I settled on a less precise method:
// - visit the quadtree nodes in an order that approximates increasing distance to the query point
// - keep a running list of the k nearest neighbors, update the list as needed
// - using the running list, eliminate from consideration nodes that are further than the furthest point in the list
// The idea is that if we visit the nodes in a good enough order, we should be able to prune many nodes from processing.
//
// In this implementation I use the following:
// - depth first search to the node containing the query point, then pop back up and process the siblings at each level
// - priority queue to hold the running list of k nearest neighbors, with extra tooling to limit it to k entries
// - prune nodes by calculating the minimum possible distance from the query point to the node's bounding box
//
// This file implements the test driver.  It takes arguments from the command line, constructs a quadtree of random
// points, finds the k nearest neighbors, and verifies the returned list of nearest neighbors.
//
// An example command line to run this test:
//   quadtree xMin 0 yMin 0 xMax 1000 yMax 1000 maxNodeSize 10 numPoints 1000000 k 50
//
//   xMin, yMin, xMax, yMax: the bounding box for the entire quadtree (and the points it can contain)
//   maxNodeSize: the maximum number of points a quadtree node can contain before it must be split into four children
//   numPoints: the number of random points to generate and insert into the quadtree
//   k: the number of nearest neighbor points to find
//
// Compiled with Microsoft Visual C++, tested on Windows 7.
//
// Dale Wong
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <ctime>
#include "arguments.h"
#include "heap.h"
#include "quadtree.h"
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
    // Capture the arguments
    Arguments args = Arguments(argc, argv);
    args.print();

    string value;
    int xMin = args.get("xMin", value) ? stoi(value) : 0;
    int yMin = args.get("yMin", value) ? stoi(value) : 0;
    int xMax = args.get("xMax", value) ? stoi(value) : 0;
    int yMax = args.get("yMax", value) ? stoi(value) : 0;
    int maxNodeSize = args.get("maxNodeSize", value) ? stoi(value) : 0;
    int numPoints = args.get("numPoints", value) ? stoi(value) : 0;
    int k = args.get("k", value) ? stoi(value) : 0;

    // Create a quadtree
    clock_t start = clock();
    Quadtree quadtree = Quadtree(xMin, yMin, xMax, yMax, maxNodeSize);
    quadtree.randomPoints(numPoints);
    //quadtree.print();
    clock_t finish = clock();
    cout << "elapsed seconds: " << double(finish - start) / CLOCKS_PER_SEC << endl;

    // Find the k nearest neighbors
    start = clock();
    vector<Entry*> neighbors;
    int xMid = xMin + (xMax - xMin) / 2;
    int yMid = yMin + (yMax - yMin) / 2;
    Point point = Point(xMid, yMid);
    quadtree.findNearestNeighbors(&point, k, neighbors);
    finish = clock();
    cout << "elapsed seconds: " << double(finish - start) / CLOCKS_PER_SEC << endl;

    for (auto iter = neighbors.begin(); iter != neighbors.end(); ++iter)
    {
        (*iter)->print();
    }

    // Verify the vector of nearest neighbors
    start = clock();
    int numPointsVerified = 0;
    if (quadtree.verifyNearestNeighbors(&point, neighbors, numPointsVerified))
    {
        cout << "passed verification" << "\n";
    }
    else
    {
        cout << "failed verification" << "\n";
    }
    cout << "number of points verified: " << numPointsVerified << "\n";
    finish = clock();
    cout << "elapsed seconds: " << double(finish - start) / CLOCKS_PER_SEC << endl;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
