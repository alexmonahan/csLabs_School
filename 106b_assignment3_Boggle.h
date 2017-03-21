//Alex Monahan
//Section Leader: Annie

//The interface for this lab
#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "grid.h"
#include "lexicon.h"
#include "bogglegui.h"
using namespace std;

class Boggle {
/*The following functions were made public because they will be called from boggleplay.
Thus, these functions cannot be private!*/
public:
    Boggle(Lexicon& dictionary, string boardText = ""); /*creates a new Boggle*/
    bool checkWord(string word); /*checks if the word is in the board*/
    bool humanWordSearch(string word); /*checks whether the user searched a valid word*/
    Set<string> computerWordSearch(); /*all the possibilities of the computer*/
     int getScoreHuman(); /*returns the score of the human*/
     int getScoreComputer(); /*returns the computer score*/
     const Vector<string> getAllHumanWords(); /*returns all of the words found*/
     friend ostream& operator<<(ostream& out, Boggle& boggle); /*we overlaod the operator method*/

/*private functions and variables are only called from within Boggle.h*/
private:
    void setAllFalse(); /*for setting all of GridMarked back to false*/
    char getLetter(int row, int col); /*returns the letter at the given position*/
    bool helperComp(int row, int col, Set<string> & compWords, string current); /*helper method for the computer search*/
    bool humanSearchHelper(string word); /*helper method for human search*/
    bool checkIndivid(string word, int row, int col); /*checks each individual Boggle square*/

    /*all variables used in this interface are private*/
    Grid<string> grid;
    Lexicon dict;
    Lexicon beenFound;
    Vector<string> humanFound;
    Grid <bool> gridMarked;
};

#endif // _boggle_h
