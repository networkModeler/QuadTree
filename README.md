# QuadTree

This program implements a quadtree as a solution to the problem:
- given one million points in a two dimensional region
- given an arbitrary query point in that same region
- find the k nearest neighbors to the query point

Initially, I had considered doing an expanding search
that starts from the quadtree node that contains the query point.
However, identifying which nodes are adjacent to any given node
is rather complicated in a quadtree, where the levels are not balanced.
This would be a headache to code, test, and support.
 
Instead, I settled on a less precise method:
- visit the quadtree nodes in an order that approximates increasing distance to the query point
- keep a running list of the k nearest neighbors, update the list as needed
- using the running list, eliminate from consideration nodes that are further than the furthest point in the list
The idea is that if we visit the nodes in a good enough order, we should be able to prune many nodes from processing.

In this implementation I use the following:
- depth first search to the node containing the query point, then pop back up and process the siblings at each level
- priority queue to hold the running list of k nearest neighbors, with extra tooling to limit it to k entries
- prune nodes by calculating the minimum possible distance from the query point to the node's bounding box

An example command line to run this test:
  quadtree xMin 0 yMin 0 xMax 1000 yMax 1000 maxNodeSize 10 numPoints 1000000 k 50

  xMin, yMin, xMax, yMax: the bounding box for the entire quadtree (and the points it can contain)
  maxNodeSize: the maximum number of points a quadtree node can contain before it must be split into four children
  numPoints: the number of random points to generate and insert into the quadtree
  k: the number of nearest neighbor points to find

Compiled with Microsoft Visual C++, tested on Windows 7.

Dale Wong
