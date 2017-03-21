//Part of lab
#ifndef _linkedpriorityqueue_h
#define _linkedpriorityqueue_h

#include <cstddef>    // for NULL
#include <iostream>
#include <string>
#include "ListNode.h"
#include "PQEntry.h"
using namespace std;

/*
 * For documentation of each member, see VectorPriorityQueue.h.
 */
class LinkedPriorityQueue {
public:
    LinkedPriorityQueue();
    ~LinkedPriorityQueue();
    void changePriority(string value, int newPriority);
    void clear();
    string dequeue();
    void enqueue(string value, int priority);
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    friend ostream& operator <<(ostream& out, const LinkedPriorityQueue& queue);

    //I only add in two private variables/methods. I add in a pointer to head of the ListNode and then a method
    //called lowerPriority that determines if the given value and priority form a lower priority.
private:
    ListNode* head;
    bool lowerPriority(ListNode * node, string value, int priority);
    void checkEnqueueOneElement(string value, int priority, ListNode * newNode);
    void enqueueSizeOverOne(ListNode * currNode, ListNode * newNode, string value, int priority);

};

#endif
