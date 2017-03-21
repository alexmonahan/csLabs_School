//Alex Monahan
//Section Leader: Annie

#include "lexicon.h"
#include "Boggle.h"
#include "console.h"
#include "simpio.h"

//Function Prototypes
Boggle setBoardUp(Lexicon & dictionary);
void computerTurn(Boggle & boggle);
void printWordsAndScore(Boggle & boggle);
void initialPrint(Boggle & boggle);
void playEnteredText(string line, Boggle & boggle, Lexicon & dictionary);
void initializeGui();

/*In playOneGame, we initalize the GUI (or reset it), create a new boggle object, and begin
to take the word inputs from the user. This function basically runs each individual boggle game.
When the user enters '1', all highlighting is cleared and the computer turn is played.*/
void playOneGame(Lexicon& dictionary) {
    initializeGui();
    Boggle boggle = setBoardUp(dictionary);

    initialPrint(boggle);
    string line = getLine("Type a word (or '1' to stop): ");
    while(toUpperCase(line) != "1"){
        clearConsole();
        playEnteredText(line, boggle, dictionary);
        line = getLine("Type a word (or '1' to stop): ");
    }

    BoggleGUI::clearHighlighting(); /*removes any remaining highlighting*/
    computerTurn(boggle); /*concludes the individual game*/
}

/*This method initially initalizes the GUI to be reset for each game.*/
void initializeGui(){
    if (BoggleGUI::isInitialized()) {
        BoggleGUI::reset();
        BoggleGUI::setStatusMessage("");
    } else{
        BoggleGUI::initialize(4,4);
    }
}

/*We find all the possible words on the board that the user did not find and print them out.
We also calculate the score of the computer and display this information with the GUI. Additionally,
we use this method to calculate the winner, which is almost always the computer, and we print out
the winning message to the screen, too. This method involves a lot of GUI interaction.*/
void computerTurn(Boggle & boggle){
    cout << " " << endl;
    cout << "It's my turn! " << endl;

    Set<string> computerSet = boggle.computerWordSearch();
    int computerScore = boggle.getScoreComputer();
    BoggleGUI::setScore(computerScore, BoggleGUI::COMPUTER);

    /*We display all the unfound words by the user for the Boggle board on the GUI.*/
    for (string word : computerSet){
        BoggleGUI::recordWord(word, BoggleGUI::COMPUTER);
    }

    cout << "My words (" << computerSet.size() << "): " << computerSet << endl;
    cout << "My score: " << computerScore << endl;

    int humanScore = boggle.getScoreHuman();

    /*Find the winner*/
    if(computerScore > humanScore){
        BoggleGUI::setStatusMessage("Ha ha ha, I destroyed you, puny human!");
        cout << "Ha ha ha, I destroyed you, puny human!" << endl;
    } else {
        cout << "Wow! You won! Great job!" << endl;
        BoggleGUI::setStatusMessage("Wow! You won! Great job!");

    }
    cout << " " << endl;

}

/*This method takes a word from the user and checks if the word can be formed on the
  boggle board. If they do not enter a valid word, we print out messages to the
 console and GUI. If they do enter a valid word, we also print out a message and update
  their score. This method involves a lot of interaction with the GUI.*/
void playEnteredText(string line, Boggle & boggle, Lexicon & dictionary){
    BoggleGUI::clearHighlighting(); /*we want to make sure the GUI is clear first*/

    /*User enters an invalid word.*/
    if(!boggle.checkWord(toUpperCase(line))){
        cout << "You must enter an unfound 4+ letter word from the dictionary." << endl;
        BoggleGUI::setStatusMessage("You must enter an unfound 4+ letter word from the dictionary.");
    }

    else{
        bool b = boggle.humanWordSearch(toUpperCase(line)); /*Check if the word is a valid search by the user*/
        if(b){
            cout << "You found a new word! '" << toUpperCase(line) << "'" <<endl;
            BoggleGUI::setStatusMessage("You found a new word! '" + toUpperCase(line) + "'");
            BoggleGUI::recordWord(line, BoggleGUI::HUMAN);
        } else {
            BoggleGUI::setStatusMessage("That word can't be formed on this board.");
            cout << "That word can't be formed on this board." << endl;
        }
    }
    cout << boggle << endl;
    printWordsAndScore(boggle);
    BoggleGUI::setScore(boggle.getScoreHuman(), BoggleGUI::HUMAN);
}

/*This method prints the human and computer score, as we well as the words found.*/
void printWordsAndScore(Boggle & boggle){
    int score = boggle.getScoreHuman();
    Vector<string> words = boggle.getAllHumanWords();
    cout << "Your words (" << words.size() << "): ";
    cout << words << endl;
    cout << "Your score: " << score << endl;
}

/*This method prints the initial output to the screen when the user begins the game.*/
void initialPrint(Boggle & boggle){
    clearConsole();
    cout << "It's your turn!" << endl;
    cout << boggle;
    printWordsAndScore(boggle);

}


/*This function is used to set up Boggle board. We allow the user to decide if they want to enter
16 letters and make a board or whether they want a random board. We make a random board for them if they
choose this option. Otherwise, we make sure they enter a valid 16 character string. We also print out
the initial board with the GUI set up in this method.*/
Boggle setBoardUp(Lexicon & dictionary){
    bool ownBoard = getYesOrNo("Do you want to generate a random board? ", "Try again. Do you want to generate a random board? ");
    if(ownBoard){
        Boggle boggle (dictionary, "");
        return boggle; /*If the user decides to use a random board*/

    } else{
        string line = getLine("Type the 16 letters to apepar on the board: ");
        /*We ensure the user enters a valid string*/
        while(true){
            bool alphaTest = true;
            for(int i = 0; i < line.length(); i++){
                if(!isalpha(line[i])) alphaTest = false;
            }
            if(line.length() == 16 && alphaTest) break;
            cout << "That is not a valid 16-letter board string. Try again." << endl;
            line = getLine("Type the 16 letters to apepar on the board: ");
        }

        /*Set up the necessary GUI*/
        BoggleGUI::labelAllCubes(line);
        Boggle boggle(dictionary, toUpperCase(line));
        return boggle;

    }

  }

