//Alex Monahan
//Array PQ from lecture is used as a source

#include "ArrayPriorityQueue.h"
#include "PQEntry.h"

/*Initially when the array is created, we want an initial capacity of 50, which I made up, and then
a usedSize of zero and the PQEntry should contain space for 50 values.*/
ArrayPriorityQueue::ArrayPriorityQueue() {
    priorityQueue = new PQEntry[50];
    usedSize = 0;
    arrayCapacity = 50;
}

/*In the destructor we want to delete the entire private member variable for the priority queue*/
ArrayPriorityQueue::~ArrayPriorityQueue() {
    delete[] priorityQueue;
}

/*This method finds the minimum index. For arrays, the searching comes in during dequeue, so we
must find the index with the lowest priority. This method does that.*/
int ArrayPriorityQueue::bulkOfCheck() const{
    if(isEmpty()){
        throw "There is nothing to dequeue.";
    } /*If there is nothing in the array, we throw an error*/

    /*Local variables*/
    int priorityMin = priorityQueue[0].priority;
    string valueMin = priorityQueue[0].value;
    int indexMin = 0;

    /*We check each position in the array, and if the given entry has a lower priority, then we set minimum
    index equal to this index and move on.*/
    for(int i = 0; i < usedSize; i++){
        if(priorityQueue[i].priority < priorityMin || (priorityQueue[i].priority == priorityMin && (priorityQueue[i].value < valueMin))){
            priorityMin = priorityQueue[i].priority;
            valueMin = priorityQueue[i].value;
            indexMin = i;
        }
    }

    /*Return the minimum index*/
    return indexMin;
}

/*We can the queue for the first entry with the given value and a less urgent priority and then we move on
and replace this entry with the new one.*/
void ArrayPriorityQueue::changePriority(string value, int newPriority) {
    int indexChange = -1; //initially set indexChange = -1

    for(int i = 0; i < size(); i++){
        if(value == priorityQueue[i].value && newPriority < priorityQueue[i].priority){
            indexChange = i;
            break;
        }
    }

    /*If indexChange is still -1, then the value is not in the priority or urgent enough so we
    throw an error. Otherwise, we change the given entry in the priorityQueue*/
    if(indexChange == -1){
        throw "This value is NOT in your priority queue or the new priority is NOT more urgent.";
    } else {
        PQEntry entry (value, newPriority);
        priorityQueue[indexChange] = entry;
    }
}

/*To clear the array, reset values to the initial amount (e.g. usedSize) and free dynamic memory.*/
void ArrayPriorityQueue::clear() {
  delete[] priorityQueue;
  priorityQueue = new PQEntry[50];
  usedSize = 0;
  arrayCapacity = 50;

}

/*Removes the given array position when dequeueing*/
void ArrayPriorityQueue::remove(int indexMin){
    for(int i = indexMin; i < usedSize - 1; i++) {
        priorityQueue[i] = priorityQueue[i + 1];
    }
    usedSize--;
}

/*If the array is empty, throw an error. Otherwise, find the minimum value with bulkOfCheck and return it after
removing the associated index and decreasing usedSize*/
string ArrayPriorityQueue::dequeue() {
    if(usedSize == 0){
        throw "There is nothing to dequeue. Add items and then try to dequeue";
    } else {
        int indexMin = bulkOfCheck();
        PQEntry currEntry = priorityQueue[indexMin];
        string returnVal = currEntry.value;
        remove(indexMin);
        return returnVal;
    }
}

/*Simply increased usedSize and place the new entry at the end of the array. Double allocation if necessary.*/
void ArrayPriorityQueue::enqueue(string value, int priority) {
    PQEntry entry(value, priority);
    priorityQueue[usedSize] = entry;
    usedSize++;
    if(usedSize == arrayCapacity) {
        doubleAllocation();
    }
}

/*Increase the storage space of the array. Similar to the lecture in class I utilized while creating this code.*/
void ArrayPriorityQueue::doubleAllocation() {
    arrayCapacity = arrayCapacity*2;
    PQEntry * priorityQueueTemp = new PQEntry[arrayCapacity];
    for(int i = 0; i < size(); i++) {
        priorityQueueTemp[i] = priorityQueue[i];
    }
    delete[] priorityQueue;
    priorityQueue = priorityQueueTemp;
}

/*The array is empty if usedSize = 0*/
bool ArrayPriorityQueue::isEmpty() const {
    return (usedSize == 0);
}

/*If empty, we throw an error. Otherwise, we find the minimum index of the entry and find its assocaited value.*/
string ArrayPriorityQueue::peek() const {
    if(isEmpty()){
        throw "There is nothing to 'peek' at, as the Priority Queue is empty.";
    } else {
        int indexMin = bulkOfCheck();
        PQEntry currEntry = priorityQueue[indexMin];
        return currEntry.value;
    }
}

/*If the array is empty, throw an error. Otherwise, find the minimum index and return this priority.*/
int ArrayPriorityQueue::peekPriority() const {
    if(isEmpty()){
        throw "There is nothing to check the priority of, as the Priority Queue is empty.";
    }
    int indexMin = bulkOfCheck();
    PQEntry currEntry = priorityQueue[indexMin];
    return currEntry.priority;
}

/*Simply return usedSize to find the size*/
int ArrayPriorityQueue::size() const {
    return usedSize;
}

/*We want to print out the values of the queue with this method. */
ostream& operator<<(ostream& out, const ArrayPriorityQueue& queue) {
    out << "{";
    for(int i = 0; i < queue.size(); i++){
        PQEntry entry = queue.priorityQueue[i];
        out << entry;
        if(i != (queue.size() - 1)) out << ","; /*Out a comma unless it is the last value in the queue*/
    }
    out << "}";
    return out;
}
