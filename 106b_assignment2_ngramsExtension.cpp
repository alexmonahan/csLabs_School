// ALEX MONAHAN
// CS106B
// Section Leader: Annie!
//EXTENSION
/* In this extension, following the instructions, I make sure each entry begins with
 an uppercase character, and I make sure that each entry ends with a period, question mark,
or exlamation mark.*/

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"

/*I include filelib.h to use the fileExists method from the first lab.
I include random.h to find a random key in determining what to print.
In my program, I use vectors, queues, and maps, explaining why these libraries
are implemented.*/
#include "filelib.h"
#include "random.h"
#include "queue.h"
#include "map.h"
#include "vector.h"

using namespace std;

//function prototypes below

/*In this method, I read the data from the given file into my map. My map's
keys (prefixes) and values (suffixes) are both vectors of strings, and each prefix
contains numGramMinusOne (N-1) words from the original text, where N = the N-gram
entered by the user.*/
void readInData(int numGramMinusOne, string filename, Map<Vector<string>, Vector<string> > & map);

/*This method determines the output that will be printed to the console. The user inputs
the number of random words they want printed out, and the program selects a random key
from the map to begin the word chain at.*/
string whatToPrint(int numRandomWords, Map<Vector<string>, Vector<string> > & map);

/*My main function prints out the initial console output, opens the given file from the user, prompts
the user for valid inputs for both the N-gram and number of random words
(according to the instructions). This information is then passed to the readInData()
and whatToPrint() methods.*/

int main() {
    /*The initial output to the console*/
    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;
    cout << endl;

    //ensures the user enters a valid file
    string filename;
    cout << "Input File Name? ";
    cin >> filename;
    while (!fileExists(filename)){
        cout << "Unable to open that file.  Try again." << endl;
        cout << "Input file name? ";
        cin >> filename;
    }

    /*The code below prompts the user to enter a valid number for 'N-gram.' The user
    is forced to enter a valid integer greater than or equal to 2, otherwise they
    are re-prompted for this input.*/
    string numStr;
    int numGram;
    while(true){
        cout << "Value of N? ";
        cin >> numStr;
        if (stringIsInteger(numStr)){
            numGram = stringToInteger(numStr);
            if(numGram >= 2) break;
            cout << "Enter a value greater than or equal to two." << endl;
        }
        else {
          cout << "Enter a valid Integer!" << endl;
        }
    }

    /*The map with Vectors of strings as both the keys and values is created, and this map
    is passed to the readInData method along with the filename and N-1 as an integer.*/
    Map<Vector<string>, Vector<string> > map;
    readInData(numGram - 1, filename, map);

    /*The code below prompts the user to enter how many random words they want to appear.
    The user is forced to enter a valid integer greater than or equal to the value of N.
    If the user enters 0, the program exits. This part of the code also prints out the
    random text to the screen using the whatToPrint method for the given amount of random
    words entered by the user.*/
    while(true){
        string numRandomWordsStr;
        int numRandomWords;
        while(true){
            cout << "# of random words to generate (0 to quit)? ";
            cin >> numRandomWordsStr;
            if (stringIsInteger(numRandomWordsStr)){
                numRandomWords = stringToInteger(numRandomWordsStr);
                if(numRandomWords >= numGram) break;
                if(numRandomWords == 0) break;
                cout << "Enter a value greater than or equal to N-grams!" << endl;
            }
            else {
                cout << "Enter a valid Integer!" << endl;
            }
        }

        if(numRandomWords == 0) break;

        string textToPrint = whatToPrint(numRandomWords, map);
        cout << "" + textToPrint + " " << endl;
        cout << endl;
    }

    cout << "Exiting." << endl; //Output when the user finally exits the program
    return 0;
}

/*This method reads the words from the file into the map. Each key is a vector of N-1 strings, and each
value of the map are the suffixes for the given vector of prefixes.*/
void readInData(int numGramMinusOne, string filename, Map<Vector<string>, Vector<string> > & map){
    //the file is opened
    ifstream infile;
    openFile(infile, filename);

    string word;
    Queue<string> queue;
    Queue<string> initialQueue;

    /*The queue is filled with strings until its size reaches the number of random words.
    We assume the number of random words is less than the number of words in the file.
    To complete the wrap-around portion of this lab, these same initial words are also stored in
    an initialQueue used closer to the end of this method.*/
    while (queue.size() < numGramMinusOne){
        infile >> word;
        queue.enqueue(word);
        initialQueue.enqueue(word);
    }

    /*we input every word from the given file and create two vectors of strings that will serve
    as the map's key and value entries.*/
    while(infile >> word){
        string nextWord = word;
        Vector<string> prefix;
        Vector<string> suffix;

        /*The prefix vector is filled with entries from the queue, and then these
        same strings are added back into the queue.*/
        for(int i = 0; i < numGramMinusOne; i++){
            string wordToAdd = queue.dequeue();
            queue.enqueue(wordToAdd);
            prefix.add(wordToAdd);
        }

        /*We get rid of the oldest word in the queue (dequeue) and then enqueue the
        new word that we read in from the file*/
        queue.dequeue();
        queue.enqueue(nextWord);

        /*If the map already contains the prefix, we retrieve this suffix and add
        the given word to the vector. We then place this key-value pair back into the
        map with the suffix updated. Otherwise, we simply add the word the suffix vector
        and place this key-value pair in the map.*/
        if(map.containsKey(prefix)){
            Vector<string> currentSuff = map.get(prefix);
            currentSuff.add(nextWord);
            map.put(prefix, currentSuff);
        } else {
            suffix.add(nextWord);
            map.put(prefix, suffix);
        }
    }

    /*This portion of the code is to handle the wrap-around portion of the program.
    After we have read every word into the file, we still have to create N-1 more
    key-value pairs for our map. The methodology is very similar to the code used above.
    The words from the queue are placed in the prefix vector. However, this time, the
    word to be added to the suffix vector comes from our initialQueue, and we retrieve
    this string using dequeue(). Then, the key-value pair is placed in the map.*/
    for(int j = 0; j < numGramMinusOne; j++){
        Vector<string> prefix;
        Vector<string> suffix;
        for(int i = 0; i < numGramMinusOne; i++){
            string wordToAdd = queue.dequeue();
            queue.enqueue(wordToAdd);
            prefix.add(wordToAdd);
        }

        queue.dequeue();
        string fromInitial = initialQueue.dequeue();
        queue.enqueue(fromInitial);

        if(map.containsKey(prefix)){
            Vector<string> currentSuff = map.get(prefix);
            currentSuff.add(fromInitial);
            map.put(prefix, currentSuff);
        } else {
            suffix.add(fromInitial);
            map.put(prefix, suffix);
        }
    }

    infile.close(); //close the infile after reading
}

/*This method prints out the random text to the console. We print out the number of
random words inputted by the user, and we create our random text by starting at a random
key in our map. We use the variable numWords to ensure our returned string has the given number
of random words entered by the user.*/
string whatToPrint(int numRandomWords, Map<Vector<string>, Vector<string> > & map){

    /*Here, we pick a random starting position for our random text using the "random.h"
    library.*/
    string returnStr = "";
    Vector <Vector<string> > vec = map.keys();
    int startingPoint = randomInteger(0, vec.size() - 1);
    Vector<string> startVector = vec.get(startingPoint);
    string str = startVector[0];

    /*I add in this code to ensure the first letter of the first string of startVector is
    uppercase for the extension!*/
    while (!isupper(str[0])){
        startingPoint = randomInteger(0, vec.size() - 1);
        startVector = vec.get(startingPoint);
        str = startVector[0];
    }

    /*Our first 'N-1' random words are all the strings in startVector separated by a space.
    Punctuation and capitalization is not altered.*/
    int numWords = 0;
    for(int i = 0; i < startVector.size(); i++){
        returnStr += startVector[i] + " ";
        numWords++;
    }

    /*While our string to return (returnStr) does not have enough random words, we choose
    a random string from the suffix vector for the given prefix vector. This word is added
    to returnStr and then our "startVector" or prefix vector is shifted over by one. We eliminate
    the first entry of startVector and add the random suffix as a new entry to startVector. This is done
    by creating a copy of startVector (called newVector).*/
    while(numWords < numRandomWords){
        Vector<string> suffixes = map.get(startVector);
        int randomChoice = randomInteger(0, suffixes.size()-1);
        string randomWord = suffixes[randomChoice];
        returnStr += randomWord + " ";

        Vector<string> newVector(startVector.size());
        for(int i = 1; i < startVector.size(); i++){
            newVector[i-1] = startVector[i];
        }
        newVector[startVector.size()-1] = randomWord;
        startVector = newVector;
        numWords++;
    }

    /*This part of the code below is used by the extension to continue to look for new words
    until we need a word that ends in a question, exlamation mark, or a period!*/
    bool notEnded = true;
    if(returnStr[returnStr.length() - 2] == '?') notEnded = false;
    if(returnStr[returnStr.length() - 2] == '!') notEnded = false;
    if(returnStr[returnStr.length() - 2] == '.') notEnded = false;

    while(notEnded){
        Vector<string> suffixes = map.get(startVector);
        int randomChoice = randomInteger(0, suffixes.size()-1);
        string randomWord = suffixes[randomChoice];
        returnStr += randomWord + " ";

        Vector<string> newVector(startVector.size());
        for(int i = 1; i < startVector.size(); i++){
            newVector[i-1] = startVector[i];
        }
        newVector[startVector.size()-1] = randomWord;
        startVector = newVector;

        if(returnStr[returnStr.length() - 2] == '?') break;
        if(returnStr[returnStr.length() - 2] == '!') break;
        if(returnStr[returnStr.length() - 2] == '.') break;
    }


    return returnStr; //This is the random text that will be outputted to the console
}
