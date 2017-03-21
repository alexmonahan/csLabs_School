//Alex Monahan
//source -- Section 5 handout on LinkedLists
#include "LinkedPriorityQueue.h"
#include "ListNode.h"

/*When we make a new LinkedPriorityQueue, the first step is simply to create the node
for the head and set this node equal to NULL*/
LinkedPriorityQueue::LinkedPriorityQueue() {
    head = new ListNode;
    head = NULL;
}

/*The destructor is equivalent to calling the clear() method and we must free dynamically allocated memory.*/
LinkedPriorityQueue::~LinkedPriorityQueue() {
    clear();
}

/*If the queue is empty OR if the value cannot be found/does not have an urgent enough priority, we throw an error*/
/*We traverse the LinkedList until we find an entry with the given value and a new urgent priority. We then create
a replacement node with the new priority, and put this new node into the LinkedList in place of the old node.*/
void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    if(isEmpty()) {
        throw "The Priority Queue is empty. Try again.";
    }

    bool throwError = true; //keeps track of whether or not we can even change the priority of an element
    ListNode * replacementNode = new ListNode(value, newPriority, NULL);     /*Creation of the new node*/
    ListNode * curr = head;

    /*We traverse the LinkedList for the same value with a more urgent priority*/
    if(curr->value == value && (newPriority < curr->priority)){
        throwError = false;
        replacementNode -> next = head -> next;
        head = replacementNode;
    } else{
        while(curr->next != NULL){
            if(curr->next->value == value && (newPriority < curr->next->priority)){
                throwError = false;
                curr->next->priority = newPriority;
                break;
            }
            curr = curr->next;
        }
    }

    if(throwError){
        throw "This value is NOT in your priority queue or the new priority is NOT more urgent.";
    }
}

/*We delete the pointers to every node in the LinkedList during this method*/
void LinkedPriorityQueue::clear() {
    while(head != NULL){
        ListNode* throwAway = head -> next;
        delete head;
        head = throwAway;
    }
}

//If the array is empty, we throw an error. Otherwise, we return the value at the start of the linked list.
//To do this, we have two cases. First, if there is only one node in the queue, and, second, if more than one node is
//in the queue.
string LinkedPriorityQueue::dequeue() {
    if(isEmpty()) {
        throw "The Priority Queue is empty.";
    }

    ListNode* curr = head;
    string val = curr -> value;

    if(curr -> next == NULL){ //if only one entry in the queue
        delete curr;
        curr = NULL;
        head = NULL;
    } else {
        head = head -> next;
        delete curr;
        curr = NULL;
    }
    return val;
}

//enqueues if the size of linked list is currently 1
void LinkedPriorityQueue::checkEnqueueOneElement(string value, int priority, ListNode * newNode){
    if(lowerPriority(head, value, priority)){
        newNode -> next = NULL;
        head->next = newNode;
    } else {
        newNode -> next = head;
        head = newNode;
    }
}

/*We enqueue the node in the correct position. We traverse the LinkedList until we find the correct position in
the queue.*/
void LinkedPriorityQueue::enqueue(string value, int priority) {
    /*We create a new node*/
    ListNode * newNode = new ListNode(value, priority, NULL);

    /*If the queue is currently empty, the head is equal to the new node. Otherwise, we find
    the given positon in the LinkedList that the node should be enqueued.*/
    if(isEmpty()){
        head = newNode;
    } else {
        ListNode * currNode = head;
        /*We set currNode equal to head, and then we check if this is the only entry in the LinkedList. If so, we
        check whether this entry should be placed before or after the head*/
        if(currNode -> next == NULL){
            checkEnqueueOneElement(value, priority, newNode);
        }else{ /*If the array has more than zero or one entry, we traverse the array until we find the correct position to
        insert the entry in.*/
            if(!lowerPriority(currNode, value, priority)){ /*If the node sould we placed at the beginning*/
                newNode -> next = head;
                head = newNode;
            } else{
                /*We traverse the queue until we find the proper place to insert it in the LinkedList.*/
                enqueueSizeOverOne(currNode, newNode, value, priority);
                }
            }

    }
}

//This method is used to enqueue in the linkedlist if there is over one element in the queue.
void LinkedPriorityQueue::enqueueSizeOverOne(ListNode * currNode, ListNode * newNode, string value, int priority){
    while(currNode -> next != NULL){
        if(lowerPriority(currNode->next, value, priority)){
            currNode = currNode -> next;
            if(currNode -> next == NULL){
                currNode -> next = newNode;
                break;
            }
        } else {
           /*Last case --> we would insert the node at the very end of the linked list.*/
           newNode -> next = currNode -> next;
           currNode -> next = newNode;
           break;
        }
    }
}


/*This is a helper method. It takes a node, a string, and an integer as parameters. If the node is a lower
priority, we return true. Otherwise, false.*/
bool LinkedPriorityQueue::lowerPriority(ListNode * node, string value, int priority){
    if(node ->priority < priority || (node ->priority == priority && (value > node ->value))) return true;
    return false;
}

/*We simply return if the head is NULL, as this means the queue is empty.*/
bool LinkedPriorityQueue::isEmpty() const {
    return (head == NULL);
}

/*If the queue is empty, we throw an error; otherwise, we return the value of the head Node*/
string LinkedPriorityQueue::peek() const {
    if(isEmpty()) {
        throw "The Priority Queue is empty.";
    }
    return (head->value);
}

/*If the queue is empty, we throw an error; otherwise, we return the priority of the head Node*/
int LinkedPriorityQueue::peekPriority() const {
    if(isEmpty()) {
        throw "The Priority Queue is empty.";
    }
    return (head->priority);
}

/*Traverses the LinkedList to find the size, incrementing the count with each new node we reach.*/
int LinkedPriorityQueue::size() const {
    int count = 2; /*We start with count = 2, as this makes the math work out*/
    ListNode * curr = head;
    if(curr == NULL) return 0;
    if(curr-> next == NULL) return 1;
    curr = curr -> next;
    while(curr -> next != NULL){
        curr = curr->next;
        count++;
    }
    return count;
}

/*Printing out the LinkedListQueue. We go to each node and print out its value and priority.
If the next node is NULL, we do not print out a comma*/
ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    out << "{";
    ListNode * currEntry = queue.head;
    if(currEntry == NULL){
        out << "}";
    } else if (currEntry->next == NULL){
        out << "\"" << currEntry->value << "\":" << currEntry->priority;
        out << "}";
    } else {
        out << "\"" << currEntry->value << "\":" << currEntry->priority;
        out << ",";
        /*Print the rest of the entries in the array*/
        while(currEntry->next != NULL){
            out << "\"" << currEntry->next->value << "\":" << currEntry->next->priority;
            if(currEntry->next->next != NULL) out << ",";
            currEntry = currEntry -> next;
        }
        out << "}";
    }
    return out;
}
