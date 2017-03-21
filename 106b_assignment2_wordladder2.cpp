
/*As long as the user enters two words ('0' to quit), I check if the words are legal for the rules
of the game with testQualifications() and then find the given wordLadder with wordFlow()*/
void playGame(){
    string word1;
    string word2;
    //I store all the words in a Lexicon collection, as this collection is fast and efficient for words
    Lexicon dict(filename);
    while(true){
        cout << "Word #1 (or '0' to exit): ";
        word1 = getLine();
        if(word1 == "0") break;

        cout << "Word #2 (or '0' to exit): ";
        word2 = getLine();
        if(word2 == "0") break;

        bool p = testQualifications(word1, word2, dict);
        if(p){
            string wordLadder = wordFlow(word1, word2, dict);
            cout << wordLadder << endl;
        }
        cout << endl;
    }
}

void initialPrint(){
    cout << "Welcome to CS 106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << "Dictionary file name? ";

    /*Here I read in the given filename*/
    filename = getLine();

    //I included the "filelib.h" library from the past lab to use fileExists
    //Do not want to let the user try to open "EnglishWords.dat," though, as this results in an error
    /*I repeatedly prompt the user for the filename if they do not enter a valid file*/
    while (!fileExists(filename) || filename == "EnglishWords.dat"){
        cout << "Unable to open that file.  Try again." << endl;
        cout << "Dictionary file name? ";
        filename = getLine();
    }
    cout << endl;
}

/*This checks to ensure the words are legal for the given dictionary for the rules
of the game. I pass the two words by reference because I change them to lowercase. Returns true
if both words are legal and false otherwise.*/
bool testQualifications (string & word1, string & word2, Lexicon dict){
    word1 = toLowerCase(word1);
    word2 = toLowerCase(word2);

    /*These two for loops ensure the two given strings are all alpha characters*/
    for(int i = 0; i < word1.length(); i++){
        if(!isalpha(word1[i])){
            cout << "Please enter a valid string for Word 1." << endl;
            return false;
        }
    }
    for(int i = 0; i < word2.length(); i++){
        if(!isalpha(word2[i])){
            cout << "Please enter a valid string for Word 2." << endl;
            return false;
        }
    }

    /*This if-statement ensures the two words are NOT the same*/
    if(word1 == word2){
        cout << "The two words must be different." << endl;
        return false;
    }

    /*This if-statement ensures the two words are the same length*/
    if(word1.length() != word2.length()){
        cout << "The two words must have the same length." << endl;
        return false;
    }

    /*This if-statement ensures both words are in the given dictionary*/
    if(!inDict(word1, word2, dict)){
        cout << "The two words must be found in the dictionary." << endl;
        return false;
    }
    return true;
}

/*This methods ensure the two given words are both in the given dictionary entered
by the user. We return true if both words are in the dictionary and false otherwise.*/
bool inDict(string word1, string word2, Lexicon dict){
    if(!dict.contains(word1) || !dict.contains(word2)) return false;
    return true;
}

/*This method implements the algorithm to find the solution set of all stacks
connecting the two given words from the user. It returns what we print out to the Console.
We create a new Lexicon called usedWords to ensure we do NOT have repeated stacks containing
the same connector words.*/
string wordFlow(string word1, string word2, Lexicon dict){
    Lexicon usedWords; //our new Lexicon
    string wordLadder = ""; //the solution we will return

    Queue<Stack<string> > queue; //We have a queue of stacks to store the partial ladders
    Stack<Stack<string> > solutions; //We have our solution set of stacks, and we need to find the shortest

    /*We create our first stack with word1 and enqueue this stack to the Queue*/
    Stack<string> stack1;
    stack1.push(word1);
    queue.enqueue(stack1);

    while(!queue.isEmpty()){
        Stack<string> currStack = queue.dequeue(); //We find the stack placed in queue the longest time ago
        string currString = currStack.peek(); //We get the first string in this stack

        /*Here, with these two nested for loops, we check all possible word ladders
        that are one letter away from the current string.*/
        for(int i = 0; i < currString.length(); i++){
            for(char ch = 'a'; ch <= 'z'; ch++){
               string testWord = toLowerCase(currString);

               //If testWord will be unchanged for the given character and index, we skip over this testWord
               if (testWord[i] == ch) continue;
               testWord[i] = ch; //change the character of testWord for the given index

               /*This first if-statement ensures we have not checked the testWord already (to avoid repeats) AND
               that the given testWord is valid in our dictionary*/
               if (dict.contains(testWord) && !usedWords.contains(testWord)){

                   /*This else-if-statement checks to see if the testWord is our solution word. If so,
                    the word is added to the current stack and this stack is pushed onto the stack of solutions.
                    If the testWord is NOT word2, then we add testWord to the Lexicon of used words and create a
                    new stack that is a copy of the current stack. We add the test word to this stack, and subsequently
                    add this new stack to the queue.*/
                   if(testWord == word2){
                        currStack.push(testWord);
                        solutions.push(currStack);
                   }
                   else{
                        usedWords.add(testWord);
                        Stack<string> newStack = currStack;
                        newStack.push(testWord);
                        queue.enqueue(newStack);
                   }
               }
           }
        }

    }

    /*We now find the shortest ladder using findShortestWordLadder, and then we return
    this string to print to the console.*/
    wordLadder = findShortestWordLadder(word1, word2, solutions);
    return wordLadder;
}

/*This method returns the given word ladder of the shortest length as a string, which
we then print to the console. We pass the entire Stack of solutions to this method.*/
string findShortestWordLadder(string word1, string word2, Stack<Stack<string> > & solutions){
    string printThisOutForLadder = "A ladder from " + word2 + " back to " + word1 + ": ";

    /*This if-statement is used for examples such as "azure" and "metal" that have no word ladder.
    This is the case when the solution set is empty.*/
    if(solutions.isEmpty()){
        printThisOutForLadder = "No word ladder found from " + word2 + " back to " + word1 + ".";
        return printThisOutForLadder;
    }

    /*Here we find the Stack of strings of the shortest length.*/
    Stack<string> shortest = solutions.pop();
    while (!solutions.isEmpty()){
        Stack<string> s = solutions.pop();
        if(s.size() < shortest.size()){
            shortest = s;
        }
    }

    /*We append each string in the given shortest Stack to the string we are going to return.*/
    while(!shortest.isEmpty()){
        printThisOutForLadder += shortest.pop();
        printThisOutForLadder += " ";
    }
    return printThisOutForLadder;
}
