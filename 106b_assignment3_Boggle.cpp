//Alex Monahan
//Section Leader: Annie

#include "Boggle.h"
#include "bogglegui.h"
#include "shuffle.h"
#include "grid.h"
#include <stdexcept>

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

// letters on every cube in 5x5 "Big Boggle" version (extension)
static string BIG_BOGGLE_CUBES[25] = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

/*We use this constructor to create a Boggle object. If the user enters the optional
boardText, we create a Boggle by using these sixteen letters, otherwise we use the lists above
 to create the new Boggle board.*/
Boggle::Boggle(Lexicon& dictionary, string boardText) {

    /*Some initial set up is needed for the variables*/
    dict = dictionary;
    gridMarked.resize(4,4);
    grid.resize(4,4);
    boardText = toUpperCase(boardText);
    int count = 0;

    /*If the user enters their own 16 letters*/
    if(boardText != ""){
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                string current;
                current += + boardText[count]; /*The next letter in the string*/
                grid.set(i,j,current); /*The next place in the grid receives the next letter*/
                gridMarked.set(i,j,false);
                count++;
            }
        }
    }

    /*If the user does not enter their own 16 letters. In other words, a random board!*/
    else {
        Vector<string> smallCubes;
        for(int i = 0; i < 16; i++){
            smallCubes.add(CUBES[i]);
        }
        shuffle(smallCubes); /*Using shuffle, as described in the lab instructions, we shake up the CUBES.*/
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                string current = smallCubes[count];
                string shuffled = shuffle(current); /*For each cube, we then pick a random letter from the string of six letters.*/
                string toAdd;
                toAdd += shuffled[0]; /*Essentially a random letter from the string, as the string is shuffled.*/
                BoggleGUI::labelCube(i,j, shuffled[0], false); /*We label this letter on the GUI, too.*/
                gridMarked.set(i,j,false);
                grid.set(i,j,toAdd);
                count++;
            }
        }
    }
}

/*This method returns the given letter for the position. We make sure the row and column
are in bound first.*/
 char Boggle::getLetter(int row, int col) {
    if(row < 0 || row > 4 || col < 0 || col > 4){
        throw std::invalid_argument( "out of bounds");
    }else{
        string str = grid.get(row, col);
        char ch = str[0];
        return ch;
    }
}

/*This function returns all the words we have found so far in the game.*/
const Vector<string> Boggle::getAllHumanWords(){
    return humanFound;
}



/*This method ensures that the word being searched for is valid. In other words, the word
has more than four letters, is in the dictionary and has not been found yet.*/
 bool Boggle::checkWord(string word) {
    word = toUpperCase(word);
    if(word.length() < 4) return false;
    if(!dict.contains(word)) return false;
    if(beenFound.contains(word)) return false;
    return true;
}

/*For each word that is searched, we check and return if this word is valid. If the word is a
valid search, we add it to the list for the human and update the necessary score and list
for the user, too.*/
bool Boggle::humanWordSearch(string word) {
    setAllFalse();
    word = toUpperCase(word);
    if(!checkWord(word)) return false;
    bool wordFound = humanSearchHelper(word);
    if(wordFound){
        humanFound.add(word);
        beenFound.add(word);
    }
    return wordFound;
}

/*A helper method for the recursive backtracking used to determine whether the user
input is a valid word. For each spot on the boggle board, we check whether the word
can be formed from this position. This method also interacts with the highlighting
of the GUI.*/
bool Boggle::humanSearchHelper(string word) {
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            BoggleGUI::setHighlighted(i, j, true);
            BoggleGUI::setAnimationDelay(100);
            BoggleGUI::setHighlighted(i, j, false);
            if(checkIndivid(word, i, j)) return true;
        }
    }
    return false;
}

/*A method using recursive backtracking to determine if the given position can form
the word the user searched for. First, this method by highlighting the necessary
 row and column on the GUI. For each square on the boggle board, we check each direction
to see if the word the user searched for can be created with all possibile combinations. If
we reach the end and we still have not found the word, we finally return false;*/
bool Boggle::checkIndivid(string word, int row, int col) {
    BoggleGUI::setHighlighted(row, col, true);
    BoggleGUI::setAnimationDelay(100);

    if(word == "") return true; //base case

    if(gridMarked.get(row, col)) return false; //base case

    //recursive case
    gridMarked.set(row, col, true); //"mark" the given position
    if(word[0] == getLetter(row, col)){
        word = word.erase(0,1);
        if(word == "") return true;
        for(int i = -1; i <= 1; i++){
           for(int j = -1; j <= 1; j++){
               if(i == j){
                   if(i == 0) continue; //as if i=j=0 we would just be staying in place
               }
               int newRow = row + i;
               int newCol = col + j;

               //we must checkt that the new row and column are in bounds
               if(newRow < 0 || newRow > 3 || newCol < 0 || newCol > 3) continue;
               if(checkIndivid(word, newRow, newCol)) return true; // recursive call
           }
        }
    }
    BoggleGUI::setHighlighted(row, col, false); //we unhighlight the GUI
    gridMarked.set(row, col, false); //undo the marking -- "unmark" the position
    return false;
}

/*This method computes the total score of the human using the methodology described
in the lab handout */
int Boggle::getScoreHuman() {
    int totalScore = 0;
    for(string word : beenFound){
        int length = word.length();
        int points = length - 3;
        totalScore += points;
    }
    return totalScore;
}

/*After the user is done guessing words, this method searches the boggle board
for all remaining words, and these words make up the score of the computer. We add
each of the words to a set of strings and return this set with all possible words for the boggle
board. We return this set.*/
Set<string> Boggle::computerWordSearch() {
    setAllFalse();
    Set<string> result;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            string str;
            helperComp(i, j, result, str);
        }
    }
    setAllFalse();
    return result;
}

/*We want to mark each position of gridMarked as false*/
void Boggle::setAllFalse() {
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            gridMarked.set(i, j, false);
        }
    }
}

/*A method using recursive backtracking to find each word i nthe computer set. This method is
a helper method for computerWordSearch. For each possible row and column, we check whether or not words can
be formed in every possible direction. We use the containsPrefix method in Lexicon to
cut off unnecessary searched.*/
bool Boggle::helperComp(int row, int col, Set<string> & compWords, string current){
    if(gridMarked.get(row, col)){ //base case
    } else {
        //recursive case
        gridMarked.set(row, col, true); //"mark" the given position, just like in the human search

        //we want to check whether a word can be formed in every possible direction
        for(int i = -1; i <= 1; i++){
           for(int j = -1; j <= 1; j++){
               if(i == j){
                   if(i == 0) continue; //we do not want to check the position again if we are not changing position!
               }
               int newRow = row + i;
               int newCol = col + j;
               if(newRow < 0 || newRow > 3 || newCol < 0 || newCol > 3) continue; //check if in bounds

               //we create a copy of each string before calling the recursive method on itself
               string newCopy;
               newCopy = current;
               newCopy += getLetter(newRow, newCol);
               if(!dict.containsPrefix(newCopy)) continue;

               //we add the word to the set of computer words if the word is valid and was not found previously
               if(checkWord(newCopy)) compWords.add(newCopy);

               helperComp(newRow, newCol, compWords, newCopy); //recursive call
               }
            }
        gridMarked.set(row, col, false); //"unmakr the position"
        }
    return false;

}

/*Similar to get HumanScore, this method loops over the set of computer words
and calculates the total score of the computer.*/
int Boggle::getScoreComputer() {

    int totalScore = 0;
    Set<string> compSet = computerWordSearch();

    for(string word : compSet){ //loop over the set of computer words
        int length = word.length();
        int points = length - 3;
        totalScore += points;
    }
    return totalScore;
}

/*This method overlaods an operator and allows us to print out our boggle
board by using cout. We define how the new operator will be used in our program and
return the given ostream.*/
ostream& operator<<(ostream& out, Boggle& boggle) {
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            char letter = boggle.getLetter(i, j);
            out << letter;
        }
        out << endl;
    }
    out << " " << endl;
    return out;
}
