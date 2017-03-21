//Alex

#ifndef _arraypriorityqueue_h
#define _arraypriorityqueue_h

#include <iostream>
#include <string>
#include "PQEntry.h"
using namespace std;

//ArrayPriorityQueue implementation -- not much is changed here from what we get in the lab
class ArrayPriorityQueue {
public:
    ArrayPriorityQueue();
    ~ArrayPriorityQueue();
    void changePriority(string value, int newPriority);
    void clear();
    string dequeue();
    void enqueue(string value, int priority);
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    friend ostream& operator <<(ostream& out, const ArrayPriorityQueue& queue);

//My changes and additions -- I add a method for doubleAllocation, similar to what we do in lecture.
/*I also add in the method of remove (used during dequeue) to remove an element from the array, and
then I have a method called bulkOfCheck which finds the minimum index when dequeueing, peeking, or peekPriority'ing,
as we are always looking for the entry with the smallest priority.*/
private: //private members
    void doubleAllocation();
    void remove(int indexMin);
    int bulkOfCheck() const;

    //with private member variables, I can keep track of the current size of the array and the current capacity
   int arrayCapacity;
   int usedSize;
   PQEntry * priorityQueue; //a pointer to the array, which stores PQEntry's

};

#endif
