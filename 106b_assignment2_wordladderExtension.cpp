int main() {
    while(true){
        cout << "Word #1 (or '0' to exit): ";
        cin >> word1;
        if(word1 == "0") break;

        cout << "Word #2 (or '0' to exit): ";
        cin >> word2;
        if(word2 == "0") break;

        bool p = testQualifications(word1, word2, dict);
        if(p){
            string wordLadder = wordFlow(word1, word2, dict);
            cout << wordLadder << endl;
        }
        cout << endl;
    }

    cout << "Have a nice day." << endl;
    return 0;
}

bool testQualifications (string & word1, string & word2, Lexicon dict){
    word1 = toLowerCase(word1);
    word2 = toLowerCase(word2);

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

    if(word1 == word2){
        cout << "The two words must be different." << endl;
        return false;
    }
    return true;
}

string wordFlow(string word1, string word2, Lexicon dict){
    Lexicon usedWords;
    Lexicon extWords;
    string wordLadder = "";
    Queue<Stack<string> > queue;
    Stack<Stack<string> > solutions;
    Stack<string> stack1;

    stack1.push(word1);
    queue.enqueue(stack1);

    while(!queue.isEmpty()){
        Stack<string> currStack = queue.dequeue();
        string currString = currStack.peek();

        for(int i = 0; i <= currString.length(); i++){
            for(char ch = 'a'; ch <= 'z'; ch++){
               string testWord = toLowerCase(currString);

               //I add this line below for the extension for words of different lengths
               string extensionWord = findOther(word2, currString, i, ch);

               /*If the extensionWord is a valid word and has NOT been tested already and has length two
                *  (is not just a character), then we decrement the character and set our testWord equal
                to extensionWord.*/
               if(!extWords.contains(extensionWord) && extensionWord.length() >= 2){
                    testWord = extensionWord;
                    ch--;
                    extWords.add(extensionWord);
               } else {
                   if(i == currString.length()) continue;
                   testWord[i] = ch;
               }

               /*Shifting the 'dict.contains(testWord)' portion of the code to the second if, else-if statement
                is used to allow words at the start and end to not be valid in the dictionary.*/
               if ((testWord.length() >= 2) && !usedWords.contains(testWord)){
                   if(testWord == word2){
                        currStack.push(testWord);
                        solutions.push(currStack);
                   }
                   else if (dict.contains(testWord)){
                        usedWords.add(testWord);
                        Stack<string> newStack = currStack;
                        newStack.push(testWord);
                        queue.enqueue(newStack);
                   }

               }
           }

        }
    }
    wordLadder = findShortestWordLadder(word1, word2, solutions);
    return wordLadder;
}

string findShortestWordLadder(string word1, string word2, Stack<Stack<string> > & solutions){
    string printThisOutForLadder = "A ladder from " + word2 + " back to " + word1 + ": ";
    if(solutions.isEmpty()){
        printThisOutForLadder = "No word ladder found from " + word2 + " back to " + word1 + ".";
        return printThisOutForLadder;
    }

    Stack<string> shortest = solutions.pop();
    while (!solutions.isEmpty()){
        Stack<string> s = solutions.pop();
        if(s.size() < shortest.size()){
            shortest = s;
        }
    }

    while(!shortest.isEmpty()){
        printThisOutForLadder += shortest.pop();
        printThisOutForLadder += " ";
    }
    return printThisOutForLadder;
}

/* This method is used in my extension to check words of different lengths (e.g. 'cat'
 * and 'cheat.') If currString is shorter than word2, we insert the new random character
 to each index, whereas if currString is longer than word2, we erase the current index to
shorten the current word.*/
string findOther(string word2, string currString, int i, char ch){
    string newWord = currString;
    if(currString.length() < word2.length()){
        return newWord.insert(i, charToString(ch));
    }
    else if(currString.length() > word2.length()){
        if(currString.length() == i) return newWord;
        return newWord.erase(i, 1);
    } else {
    return newWord;
    }
}

