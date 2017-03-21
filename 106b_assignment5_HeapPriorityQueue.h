//Lab Code
#ifndef _heappriorityqueue_h
#define _heappriorityqueue_h

#include <iostream>
#include <string>
#include "PQEntry.h"
using namespace std;

/*
 * For documentation of each member, see VectorPriorityQueue.h.
 */

//Changes were only made to private member variables and functions
class HeapPriorityQueue {
public:
    HeapPriorityQueue();
    ~HeapPriorityQueue();
    void changePriority(string value, int newPriority);
    void clear();
    string dequeue();
    void enqueue(string value, int priority);
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    friend ostream& operator <<(ostream& out, const HeapPriorityQueue& queue);

private:
    void doubleAllocation(); //Similar to array, I add in a doubleAllocation method
    void sortParentsAndChildren(int pos); //used during enqueue to put the child in the proper spot
    void dequeueBubbleDown(int bubblingThisDown); //used during dequeue
    void swap(int check, int bubblingThisDown, PQEntry parent, PQEntry child);

    void remove(); //Similar to array, I add in a doubleAllocation method

    PQEntry * array; //Similar to array, I have a pointer to the start of the array
    //I also have private member variables for the array's current size and capacity
    int arrayCapacity;
    int usedSize;
};

#endif
