//Alex Monahan
//Section Leader: Annie
//Please note that all the extensions work, but I could not figure out how to
//add my new files to the <T> template class. Thus, to use a different extension
//(e.g. Extra, ExtraPriorityQueue, or DoubleLinked) just paste the file into ExtraPriorityQueue
//and it will work!

#include "HeapPriorityQueue.h"

/*usedSize starts at one because we leave index 0 blank. Create the new array and set its capacity.*/
HeapPriorityQueue::HeapPriorityQueue() {
    array = new PQEntry[50];
    usedSize = 1;
    arrayCapacity = 50;
}

/*We free dynamic memory in the destructor*/
HeapPriorityQueue::~HeapPriorityQueue() {
    delete[] array;
}

/*Exactly like the changePriority method for ArrayPriorityQueue. As long as the value can be found and with a more
urgent priority, we make the change in the queue.*/
void HeapPriorityQueue::changePriority(string value, int newPriority) {
    int indexChange = -1;

    for(int i = 1; i <= size(); i++){
        if(value == array[i].value && newPriority < array[i].priority){
            indexChange = i;
            break;
        }
    }

    if(indexChange == -1){
        throw "This value is NOT in your priority queue or the new priority is NOT more urgent.";
    } else {
        PQEntry entry (value, newPriority);
        array[indexChange] = entry;
    }
}

/*Similar to the clear() method for ArrayPriorityQueue. We need to delete the dynamically created memory and reset
the array to its initial size and capacity.*/
void HeapPriorityQueue::clear() {
    delete[] array;
    array = new PQEntry[50];
    usedSize = 1;
    arrayCapacity = 50;

}

/*If the array is empty, we throw and error. Otherwise, we find the first entry and bubble down the heap
 until we find the proper position for the element. We return the value of the entry in the very first index
of the array*/
string HeapPriorityQueue::dequeue() {
    if (isEmpty()) throw "There is nothing in the queue right now";
    PQEntry dequeuedEntry = array[1];
    if(usedSize == 2){ //if usedSize == 2, there is only one element in the array
        clear();
    } else {
        array[1] = array[usedSize-1];
        usedSize--;
        dequeueBubbleDown(1);
    }
    /*Return the value of array[i]*/
    return dequeuedEntry.value;
}

/*The bubble down method for the heap. We find each of the children indexes for the current position,
and then, as long as these children exist, we check if they have a lower priority than the parent. If so,
we then "bubble" this element down by switching the parent and this child.*/
void HeapPriorityQueue::dequeueBubbleDown(int bubblingThisDown){
    /*The indexes of the two children*/
    int firstToCheck = 2*bubblingThisDown;
    int secondToCheck = 2*bubblingThisDown + 1;
    PQEntry parent = array[bubblingThisDown];

    if(!(firstToCheck < usedSize)){
        //do nothing
    } else if (!(secondToCheck < usedSize)) {
        PQEntry firstChild = array[firstToCheck];
        if(firstChild.priority < parent.priority || (firstChild.priority == parent.priority && firstChild.value < parent.value)){
            swap(firstToCheck, bubblingThisDown, parent, firstChild);
        }
    } else {
        PQEntry secondChild = array[secondToCheck];
        bool firstSmaller = true; /*A boolean that checks whether the first or second child has a lower priority*/
        if(secondChild.priority < array[firstToCheck].priority || (array[firstToCheck].priority == secondChild.priority && secondChild.value < array[firstToCheck].value)) firstSmaller = false;
        if(firstSmaller){ /*If the first child has a lower priority*/
            if(array[firstToCheck].priority < parent.priority || (array[firstToCheck].priority == parent.priority && array[firstToCheck].value < parent.value)){
                swap(firstToCheck, bubblingThisDown, parent, array[firstToCheck]);
            }
        } else { /*If the second child has a lower priority*/
            if(array[secondToCheck].priority < parent.priority || (array[secondToCheck].priority == parent.priority && array[secondToCheck].value < parent.value)){
                swap(secondToCheck, bubblingThisDown, parent, array[secondToCheck]);
            }
        }
    }
}


void HeapPriorityQueue::swap(int check, int bubblingThisDown, PQEntry parent, PQEntry child) {
    array[check] = parent;
    array[bubblingThisDown] = child;
    dequeueBubbleDown(check);
}

/*When we enqueue, we add in the new entry to the end of the array. We increment the usedSize, too, and then
if the usedSize is equal to the array capacity, we double allocation. Then, as long as there is not ONLY one entry
in the array, we bubble up the entry.*/
void HeapPriorityQueue::enqueue(string value, int priority) {
    PQEntry entry(value, priority);
    array[usedSize] = entry;
    usedSize++;
    if(usedSize == arrayCapacity) {
        doubleAllocation();
    }
    if(usedSize != 2) sortParentsAndChildren(usedSize-1);
}

/*Recursive -- if pos <=1, then we hit the base case and are done bubbling up. Otherwise, we find
the position of the parent (i/2), and, if the priority of the parent is greater than the priority of the child,
we bubble up the child. We keep doing this as long as necessary.*/
void HeapPriorityQueue::sortParentsAndChildren(int pos) {
    if(pos <= 1){ //do nothing, base case
    } else {
        int parent = (pos)/2;
        PQEntry parEntry = array[parent];
        PQEntry currChildEntry = array[pos];
        //compare priorities
        if((parEntry.priority > currChildEntry.priority) || ((parEntry.priority == currChildEntry.priority) && (parEntry.value > currChildEntry.value))){
            array[parent] = currChildEntry;
            array[pos] = parEntry;
            sortParentsAndChildren(parent); /*Recursive call*/
        }
    }
}

/*Exact same as doubleAllocation() for the ArrayPriorityQueue. Note, we start at
i=1 because we do NOT use index zero.*/
void HeapPriorityQueue::doubleAllocation(){
    arrayCapacity = arrayCapacity*2;
    PQEntry * priorityQueueTemp = new PQEntry[arrayCapacity];
    for(int i = 1; i <= (usedSize - 1); i++) {
        priorityQueueTemp[i] = array[i];
    }
    delete[] array; /*Delete the pointer before reassigning*/
    array = priorityQueueTemp;
}

/*Note, the array is empty if the usedSize is equal to one, as we skip index zero.*/
bool HeapPriorityQueue::isEmpty() const {
    return usedSize == 1;
}

/*Simply return the first entry's value in the array, unless there is nothing in the array, when
the array isEmpty*/
string HeapPriorityQueue::peek() const {
        if (isEmpty()) throw "There is nothing in the queue right now";
        PQEntry first = array[1];
        string val = first.value;
        return val;
}

/*Simply return the first entry's priority in the array, unless there is nothing in the array, when
the array isEmpty*/
int HeapPriorityQueue::peekPriority() const {
    if (isEmpty()) throw "There is nothing in the queue right now";
    PQEntry first = array[1];
    int priorityToReturn = first.priority;
    return priorityToReturn;
}

/*Note, usedSize is always one larger than the actual size because we start at usedSize = 1*/
int HeapPriorityQueue::size() const {
    return usedSize-1;
}

/*Prints out the HeapPriorityQueue. Note, we start at i=1*/
ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    out << "{";
    for(int i = 1; i < queue.usedSize; i++){
        PQEntry entry = queue.array[i];
        out << entry;
        if(i != (queue.usedSize -1)) out << ",";
    }
    out << "}";
    return out;
}
