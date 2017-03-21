//Alex Monahan
//extension: adds in an encryption/decryption method

//imported inputs
#include "encoding.h"
#include "bitstream.h"
#include "filelib.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "random.h"
#include "simpio.h"
#include "strlib.h"
#include "HuffmanNode.h"
#include "encoding.h"
#include "huffmanutil.h"
#include "pqueue.h"
#include "map.h"
#include "filelib.h"

/* This function takes in an ifstream input and return a map.
 The first thing I do is put PSEUDO_EOF in the map, as this value has to be in the map
 with a result of one. Then, I would take in a character. If the character is NOT_A_CHAR
 then the loop will exit, otherwise, I convert the character to an integer and add this character
 into the map with a value of one greater. */
Map<int, int> buildFrequencyTable(istream& input) {

    int choice = (getInteger("Would you like to encrypt the data: '1' = yes, '0' = no"));
    while(true){
        if(choice == 1) break;
        if(choice == 0) break;
        choice = (getInteger("Try again. Would you like to encrypt the data: '1' = yes, '0' = no"));
    }

    int num;
    if(choice == 1){
        num = encrypt();
    } else {
        num = 0;
        cout << "We will not use encryption here." << endl;
    }

    Map<int, int> map;
    char ch;
    map.put(PSEUDO_EOF, 1);
    while(input.get(ch)){
        if(ch == NOT_A_CHAR){
            break;
        } else {
            int ch_int = (int)ch;
            if(!map.containsKey(ch_int)){
                map.put(ch_int, 1);
            } else {
                int currentCount = map.get(ch_int);
                map.put(ch_int, currentCount++);
            }
        }

    }

    return map; //we return the map in the end
}

/* This function takes in a map with keys and values that are integers and returns a pointer
 to the root HuffmanNode. The first thing we do in this method is create a PriorityQueue
of HuffmanNodes. Then, for each key in the map, we find the priority and create a new HuffmanNode
with this key and value and put this node into the priority queue. We then call a method called build
 which builds the proper queue. We return the first HuffmanNode in the queue.*/
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {

    PriorityQueue<HuffmanNode*> queue;

    for(int charKey : freqTable){
        int charKeyPriority = freqTable.get(charKey);
        HuffmanNode* node = new HuffmanNode(charKey, charKeyPriority, NULL, NULL); //start with children as NULL
        queue.enqueue(node, charKeyPriority);
    }

    build(queue);
    HuffmanNode* returnThisNode = queue.dequeue();
    return returnThisNode;
}

/*A helper method used in my lab. We take in the priority queue by passing it by reference, and then
we combine the first two nodes out of the queue following the format stated in the lab.*/
void build(PriorityQueue<HuffmanNode*> & queue) {
    while(queue.size() >= 2){
        //the two first items of the queue
        HuffmanNode* left = queue.dequeue();
        HuffmanNode* right = queue.dequeue();

        //the priorities combined of the two nodes
        int prioritySum = left->count + right->count;

        //the parent node
        HuffmanNode* parent = new HuffmanNode(NOT_A_CHAR, prioritySum, left, right);
        queue.enqueue(parent, prioritySum); //we enqueue this parent back into the queue
    }
}

/*This method takes a node to the start of the tree and returns a map with integers as keys
and strings as values. If the tree is empty, we return a blank encoding map. Otherwise, we call
a traverseTree helper method and then return the encoiding map.*/
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    if(encodingTree == NULL) return encodingMap;
    traverseTree(encodingTree, encodingMap, "");
    return encodingMap;
}


/* A helper method for the above reason. If both children of the tree are equal to null, we have
 reached a character and we put this string in the map. Otherwise, if not both of the children are equal
to NULL, we recursively call traverseTree and add either a "0" or "1" to the string depending on the specific child.*/
void traverseTree(HuffmanNode* root, Map<int, string> & encodingMap, string str){
    if(root->zero == NULL && root->one == NULL){
        encodingMap.put(root->character, str);
    } else {
        if(root->zero!=NULL){
            traverseTree(root->zero, encodingMap, (str + "0"));
        }
        if(root->one!=NULL){
            traverseTree(root->one, encodingMap, (str + "1"));
        }
    }
}

/*This function takes in the input, encodingMap, and output. For each key in map, we find
the necessaru string value and call the helper method decompose on this string. Finally, we call the
 decompose method on PSEUDO_EOF, too, adding this to the end of the string.*/
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    char ch;
    while(input.get(ch)){
        int ch_int = (int)ch;
        if(encodingMap.containsKey(ch_int)){
            string prefix = encodingMap.get(ch_int);
            decompose(prefix, output); //helper method
        }
    }
    decompose(encodingMap.get(PSEUDO_EOF), output);

}

/* My helper method that takes the string value from the map and prints the necessary
 bits to the output using the method writeBit for each character in the spring. */
void decompose(string prefix, obitstream& output) {
    for(int i = 0; i < prefix.size(); i++){
        char myBit =  prefix[i];
        if(myBit == '0'){
            output.writeBit(0);
        } else {
            output.writeBit(1);
        }
    }
}

int encrypt(){
    cout << "Encryption will be used. Please select a key below and remember the key for decoding." << endl;
    cout << "1" << endl;
    cout << "2" << endl;
    cout << "3" << endl;
    cout << "4" << endl;
    cout << "5" << endl;

    int choice = (getInteger("Please enter an integer for the encryption vlaue you would like"));
    while(true){
        if(choice == 1 || choice == 2 || choice == 3 || choice == 4 || choice == 5) break;
        choice = (getInteger("Try again. Please enter an integer for the encryption vlaue you would like"));
    }

    return choice;
}

/* This function decodes the data from the input. While we can still read another bit, we
 find the next bit and move down our Huffman tree depending on the bit value. If we reach
an actual character, we place the character in the output and reset our current node back to the
start node. We then move on to find the next bit and move down the tree accordingly.*/
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    HuffmanNode* curr = encodingTree;
    while(true){
        int bit = input.readBit(); //bit
        if(curr->character != NOT_A_CHAR){
            if(curr->character != PSEUDO_EOF) output.put(curr->character);
            curr = encodingTree;
        }

        if(bit == 1){
            curr = curr->one;
         } else if (bit == 0){
             curr = curr->zero;
        } else {
            break; //when bit == -1
        }

    }
}

/* This function compresses the input and outputs it. First, we build the map with key-value
 pairs as integers and print this map to the output. Then, we find the necessary HuffmanNode
root from the map, and, from this root, we then find build the encoding map. We rewind the stream and then
encode the data. Finally, we free the tree with our method freeTree.*/
void compress(istream& input, obitstream& output) {
    Map<int, int> myMap;
    myMap = buildFrequencyTable(input);
    output << myMap;

    HuffmanNode* root = buildEncodingTree(myMap);
    Map<int, string> encodingMap;
    encodingMap = buildEncodingMap(root);
    rewindStream(input);
    encodeData(input, encodingMap, output);
    freeTree(root);

}

/* We decompress inn this method. First, we find the map with key-value pairs as integers, and then we
 build the encodingTree from the root, finding the starting HuffmanNode. We then decode the data and free
the tree.*/
void decompress(ibitstream& input, ostream& output) {
    HuffmanNode* root;
    Map<int, int> myMap;
    input >> myMap;

    root = buildEncodingTree(myMap);

    decodeData(input, root, output);

    freeTree(root);

}

/* We delete each node in the tree. This method is called recursively. */
void freeTree(HuffmanNode* node) {
    if(node == NULL){ //base case; do nothing
    } else{
        HuffmanNode* left = node->zero; /* first child */
        HuffmanNode* right = node -> one; /* second child */
        if(left!=NULL){
            freeTree(left); /* Recursive call */
        }
        if(right!=NULL){
            freeTree(right); /* Recursive call */
        }
        delete node;
    }
}
/*
 * //Alex Monahan
//extension: adds in an encryption method -- gives the user five possible options for encryption that they can pick from
//The user then has the ability to decode their data if they remember their key

//imported inputs
#include "encoding.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "random.h"
#include "simpio.h"
#include "strlib.h"
#include "HuffmanNode.h"
#include "huffmanutil.h"
#include "bitstream.h"
#include "pqueue.h"
#include "map.h"
#include "filelib.h"

/* This function takes in an ifstream input and return a map.
 The first thing I do is put PSEUDO_EOF in the map, as this value has to be in the map
 with a result of one. Then, I would take in a character. If the character is NOT_A_CHAR
 then the loop will exit, otherwise, I convert the character to an integer and add this character
 into the map with a value of one greater.
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> map;
    char ch;
    map.put(PSEUDO_EOF, 1);
    while(input.get(ch)){
        if(ch == NOT_A_CHAR){
            break;
        } else {
            int ch_int = (int)ch;
            if(!map.containsKey(ch_int)){
                map.put(ch_int, 1);
            } else {
                int currentCount = map.get(ch_int);
                map.put(ch_int, currentCount++);
            }
        }

    }

    return map; //we return the map in the end
}

/* This function takes in a map with keys and values that are integers and returns a pointer
 to the root HuffmanNode. The first thing we do in this method is create a PriorityQueue
of HuffmanNodes. Then, for each key in the map, we find the priority and create a new HuffmanNode
with this key and value and put this node into the priority queue. We then call a method called build
 which builds the proper queue. We return the first HuffmanNode in the queue.
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {

    PriorityQueue<HuffmanNode*> queue;

    for(int charKey : freqTable){
        int charKeyPriority = freqTable.get(charKey);
        HuffmanNode* node = new HuffmanNode(charKey, charKeyPriority, NULL, NULL); //start with children as NULL
        queue.enqueue(node, charKeyPriority);
    }

    build(queue);
    HuffmanNode* returnThisNode = queue.dequeue();
    return returnThisNode;
}

/*A helper method used in my lab. We take in the priority queue by passing it by reference, and then
we combine the first two nodes out of the queue following the format stated in the lab.
void build(PriorityQueue<HuffmanNode*> & queue) {
    while(queue.size() >= 2){
        //the two first items of the queue
        HuffmanNode* left = queue.dequeue();
        HuffmanNode* right = queue.dequeue();

        //the priorities combined of the two nodes
        int prioritySum = left->count + right->count;

        //the parent node
        HuffmanNode* parent = new HuffmanNode(NOT_A_CHAR, prioritySum, left, right);
        queue.enqueue(parent, prioritySum); //we enqueue this parent back into the queue
    }
}

/*This method takes a node to the start of the tree and returns a map with integers as keys
and strings as values. If the tree is empty, we return a blank encoding map. Otherwise, we call
a traverseTree helper method and then return the encoiding map./
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    if(encodingTree == NULL) return encodingMap;
    traverseTree(encodingTree, encodingMap, "");

    return encodingMap;
}




/* A helper method for the above reason. If both children of the tree are equal to null, we have
 reached a character and we put this string in the map. Otherwise, if not both of the children are equal
to NULL, we recursively call traverseTree and add either a "0" or "1" to the string depending on the specific child.*]
void traverseTree(HuffmanNode* root, Map<int, string> & encodingMap, string str){
    if(root->zero == NULL && root->one == NULL){
        encodingMap.put(root->character, str);
    } else {
        if(root->zero!=NULL){
            traverseTree(root->zero, encodingMap, (str + "0"));
        }
        if(root->one!=NULL){
            traverseTree(root->one, encodingMap, (str + "1"));
        }
    }
}

/*This function takes in the input, encodingMap, and output. For each key in map, we find
the necessaru string value and call the helper method decompose on this string. Finally, we call the
 decompose method on PSEUDO_EOF, too, adding this to the end of the string/
void encodeData(istream& input, const Map<int, string>& encodingMap, obitsteam& output) {
    char ch;



    Vector<int> bitsWithEncryption;

    while(input.get(ch)){
        int ch_int = (int)ch;
        if(encodingMap.containsKey(ch_int)){
            string prefix = encodingMap.get(ch_int);
            decompose(prefix, output, num, bitsWithEncryption); //helper method
        }
    }
    writeOutMap(bitsWithEncryption, num, output);
    /*string prefix = encodingMap.get(PSEUDO_EOF);

    for(int i = 0; i < prefix.size(); i++){
        char myBit =  prefix[i];
        if(myBit == '0'){
           output.writeBit(0);
        } else {
            output.writeBit(1);
        }
     }
}

void writeOutMap(Vector<int> & bitsWithEncryption, int choice, obitstream& output){
    if(choice == 0){
        printOutput(bitsWithEncryption, output, 1);
    }
    else if(choice == 1){
        for(int i = 0; i < bitsWithEncryption.size(); i++){
            if(bitsWithEncryption[i] == 0){
                bitsWithEncryption[i] = 1;
            } else if(bitsWithEncryption[i] == 1){
                bitsWithEncryption[i] = 0;
            }
        }

        printOutput(bitsWithEncryption, output, 1);
    }
    else if (choice == 2){
        printOutput(bitsWithEncryption, output, 2);

    } else if (choice == 3){
        printOutput(bitsWithEncryption, output, 3);

    }
    else if (choice == 4){
        printOutput(bitsWithEncryption, output, 4);

    } else {
        printOutput(bitsWithEncryption, output, 5);

    }

}

void printOutput(Vector<int> & bitsWithEncryption, obitstream& output, int num){
     for(int j = 0; j < num; j++){
        for(int i = 0; i < bitsWithEncryption.size(); i++){
            if(bitsWithEncryption[i] == 0){
                output.writeBit(0);
            } else {
                output.writeBit(1);
            }
     }

        }
}






/* My helper method that takes the string value from the map and prints the necessary
 bits to the output using the method writeBit for each character in the spring. *
void decompose(string prefix, obitstream& output, int encryptionKey,   Vector<int> & bitsWithEncryption) {
    for(int i = 0; i < prefix.size(); i++){
        char myBit =  prefix[i];
        if(myBit == '0'){
           bitsWithEncryption.add(0);
        } else {
            bitsWithEncryption.add(1);
        }
    }
}

/* This function decodes the data from the input. While we can still read another bit, we
 find the next bit and move down our Huffman tree depending on the bit value. If we reach
an actual character, we place the character in the output and reset our current node back to the
start node. We then move on to find the next bit and move down the tree accordingly./
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {



    HuffmanNode* curr = encodingTree;
    while(true){
        int bit = input.readBit(); //bit
        if(curr->character != NOT_A_CHAR){
            if(curr->character != PSEUDO_EOF) output.put(curr->character);
            curr = encodingTree;
        }

        if(bit == 1){
            if(num == 1){
                curr = curr->zero;
            } else {
                curr = curr->one;
            }

         } else if (bit == 0){
            if(num == 1){
                curr = curr->one;
            } else {
                curr = curr->zero;
            }
        } else {
            break; //when bit == -1
        }

    }
}

int decrypt(){
    cout << "Decryption will be used" << endl;
    cout << "1: to decrypt reverse bits encrypt" << endl;
    cout << "2: to decrypt double bits encrypt" << endl;
    cout << "3: to decrypt triple bits encrypt" << endl;
    cout << "4: to decrypt quadruple bits encrypt" << endl;
    cout << "5: 5x bits encrypt" << endl;

    int choice = (getInteger("Please enter an integer for the decryption vlaue you would like"));
    while(true){
        if(choice == 1 || choice == 2 || choice == 3 || choice == 4 || choice == 5) break;
        choice = (getInteger("Try again. Please enter an integer for the decryption vlaue you would like"));
    }

    return choice;
}

/* This function compresses the input and outputs it. First, we build the map with key-value
 pairs as integers and print this map to the output. Then, we find the necessary HuffmanNode
root from the map, and, from this root, we then find build the encoding m
    Map<int, int> myMap;
    myMap = buildFrequencyTable(input);
    output << myMap;

    HuffmanNode* root = buildEncodingTree(myMap);
    Map<int, string> encodingMap;
    encodingMap = buildEncodingMap(root);
    rewindStream(input);
    encodeData(input, encodingMap, output);
    freeTree(root);

}

/* We decompress inn this method. First, we find the map with key-value pairs as integers, and then we
 build the encodingTree from the root, finding the starting HuffmanNode. We then decode the data and free
the tree.
void decompress(ibitstream& input, ostream& output) {
    HuffmanNode* root;
    Map<int, int> myMap;
    input >> myMap;

    root = buildEncodingTree(myMap);

    decodeData(input, root, output);

    freeTree(root);

}

/* We delete each node in the tree. This method is called recursively.
void
        delete node;
    }
}

   */
