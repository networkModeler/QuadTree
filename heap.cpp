////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// heap.cpp : class for fixed size heap
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "heap.h"
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Constructor
Heap::Heap(int maxSize)
    : maxSize(maxSize)
{
    // N/A
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Return if the size of the heap equals the maxSize
bool Heap::full()
{
    return (this->heap.size() >= (size_t)this->maxSize);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Return the top element of the heap
Entry* Heap::top()
{
    return this->heap.top();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Push an element onto the heap
bool Heap::pushOrReject(Entry* entry)
{
    // Check if there's room in the heap
    if (this->heap.size() < (size_t)this->maxSize)
    {
        // Push the new point onto the heap
        this->heap.push(entry);
        return true;
    }

    // Check if the new entry is closer than the entry at the top of the heap
    else if (this->heap.top()->getDistance() > entry->getDistance())
    {
        Entry* topEntry = this->heap.top();
        this->heap.pop();
        this->heap.push(entry);
        delete topEntry;
        return true;
    }

    // Reject the new entry
    else
    {
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Return true if no members in the heap
void Heap::toVector(vector<Entry*>& neighbors)
{
    while (!this->heap.empty())
    {
        neighbors.push_back(this->heap.top());
        this->heap.pop();
    }

    std::reverse(neighbors.begin(), neighbors.end());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////