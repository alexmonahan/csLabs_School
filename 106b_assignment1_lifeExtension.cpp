// Everything included in this program

/*For extension -- see my Main() method and the bool checkSpecificLocationForNeighborExtension(Grid<int> & initGrid, int row, int col) method
I created a "random" world, as seen in the code, and completed the extension for the wrap around*/

/*My extension also adds the GUI reference! I pass the LifeGUI by reference
in my code!*/

/*My extension also lets the user enter the pause time. I make sure the pause time is a valid,
positive integer with my code in the Main function.*/

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "lifegui.h"
#include "random.h" //for the extension
using namespace std;

//function prototypes below

void readInData(ifstream & infile, int rows, int cols, Grid<int> & gridCopy);

void printGrid(Grid<int> & gridCopy, LifeGUI & gui);

void checkNeighbors(Grid<int> & gridCopy, Grid<int> & initGrid, int rowNum, int colNum);

void advanceGeneration(Grid<int> & gridCopy);

void animate(Grid<int> & gridCopy, int frames, LifeGUI & gui, int timeInt);

/*bool checkSpecificLocationForNeighbor(Grid<int> & initGrid, int row, int col);*/

bool checkSpecificLocationForNeighborExtension(Grid<int> & initGrid, int row, int col);

bool checkIfInteger(string str);

/*the main function initializes the grid,
prints out the initial text to the screen,
inputs the filename from the user, checks
if this filename is valid, and if it is valid the filename
is opened. Then, the user is prompted to tick, animate, or quit,
and the game continues until the user quits the game.
*/
int main() {
    ifstream infile;
    int nRows;
    int nCols;
    LifeGUI name; //declare my LifeGUI
    Grid<int> grid; //the main grid for the game is initialized

    //the initial text is printed out
    cout<< "Welcome to the CS106B Game of Life," << endl;
    cout << "A simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells live (X) and die by the following rules:" << endl;
    cout << " - A cell with one or fewer neighbors dies." << endl;
    cout << " - Locations with two neighbors remain stable." << endl;
    cout << " - Locations with three neighbors will create life." << endl;
    cout << " - A cell with four or more neighbors dies." << endl;
    cout << " " << endl;
    cout << "Grid input file name? ";

    //open the grid file from the user
    string filename;
    cin >> filename;
    if (!fileExists(filename) && filename != "random"){
        cout << "Unable to open that file.  Try again." << endl;
        cout << "Grid input file name? ";
        cin >> filename;
    }
    //if the user enters random
    if (filename == "random"){
        nRows = randomInteger(1, 50);
        nCols = randomInteger(1, 50);
        grid.resize(nRows, nCols);
        name.resize(nRows, nCols);
        for(int i = 0; i < nRows; i++){
            for(int j = 0; j < nCols; j++){
                int n = randomInteger(1,2);
                if(n == 1){
                    grid.set(i, j, 0); //dead cell
                } else {
                    grid.set(i, j, 1); //living cell
                }
            }
        }
    } else{  //read the file if the user enters the filename
        openFile(infile, filename);
        //reads in the number of rows and columns in the grid
        string rowString;
        string colString;
        getline(infile, rowString);
        getline(infile, colString);
        nRows = stringToInteger(rowString);
        nCols = stringToInteger(colString);

        grid.resize(nRows, nCols); //resize the grid to that of the current gameboard
        name.resize(nRows, nCols);

        //reads in the data from the text file and prints the initial grid out
        readInData(infile, nRows, nCols, grid);
    }

    //to let the user determine pause time

    string time;
    cout << "Please enter the pause time for animation: ";
    cin >> time;
    bool k = checkIfInteger(time);
    while(!k){ //while the user has not entered an integer
        cin.clear();
        cout<< "Enter a valid pause time. Try again ";
        cout<< "How many seconds would you like to pause? ";
        cin >> time;
        k = checkIfInteger(time);
    }
    int timeInt = stringToInteger(time);

    printGrid(grid, name);

    //lets the user decide whether to animate, tick, or quit
    char decision = 'a';
    while(true){
        string decision;
        cout << "a)nimate, t)ick, q)uit? ";
        cin >> decision;
        if(decision == "q") break; //exits the game

        //if the user decides to begin the animation
        else if(decision == "a"){
            string frames;
            cout<< "How many frames? ";
            cin >> frames;
            //I check to make sure the user enters a valid integer with the method below
            bool p = checkIfInteger(frames);
            while(!p){ //while the user has not entered an integer
                cin.clear();
                cout<< "Enter a valid integer. Try again ";
                cout<< "How many frames? ";
                cin >> frames;
                p = checkIfInteger(frames);
            }
            int numFrames = stringToInteger(frames);
            animate(grid, numFrames, name, timeInt); //animate the grid with the given number of frames
        }

        //if the user ticks, simply advance the grid and then print it out!
        else if(decision == "t"){
            advanceGeneration(grid);
            printGrid(grid, name);

        //if the user does not enter 'a', 't', or 'q', we prompt the user to re-enter
        } else {
            cout << "Invalid choice; please try again.";
            cout << "a)nimate, t)ick, q)uit? ";
        }
    }

    cout << "Have a nice Life!";
    return 0;
}


/*I use a for-loop to complete the given number of frame
cycles required. To animate, for each frame, I just advance
each generation, print the new grid, pause, and then clear
the screen. Note that LifeGUI is passed by reference!*/
void animate(Grid<int> & gridCopy, int frames, LifeGUI & gui, int timeInt){
    for(int i = 0; i < frames - 1; i++){
        advanceGeneration(gridCopy);
        printGrid(gridCopy, gui);
        pause(timeInt);
        clearConsole();
    }
    if(frames != 0) advanceGeneration(gridCopy); //advances to the last frame as long as the number of frames is not zero
    printGrid(gridCopy, gui);
}

/*to check if the user entered an integer when animating,
I simply check that each character in their entered text
is equal to a whole number between 0 and 9. For example, if a letter, decimal, etc.
is part of the entered text, then I know an integer was not entered and can return false*/
bool checkIfInteger(string str){
    for(int i = 0; i < str.length(); i++){
        if(!isdigit(str[i])) return false;
    }
    return true;
}

/* this method reads in each row of the infile. Then, each column of
 * each line (or row) is checked to see if a dead or alive cell is in the
given space. My grid is made of integers, so 0 represents a dead cell
and 1 represents an alive cell.*/
void readInData(ifstream & infile, int rows, int cols, Grid<int> & gridCopy){
    for(int i = 0; i < rows; i++){
        string line;
        getline(infile, line);
        for(int j = 0; j < cols; j++){
           if(line[j] == '-'){
                gridCopy.set(i, j, 0); //dead cell
           }else{
                gridCopy.set(i, j, 1); //alive cell
           }
        }
    }
    infile.close(); //close the infile after reading
}

/*This method is frequently called to print the grid. I loop
over each row and column, and if my grid contains a zero (dead cell),
I print out a '-'. If my grid contains a one, an alive cell
is printed out 'X'.*/
void printGrid(Grid<int> & gridCopy, LifeGUI & gui){
    for (int i = 0; i < gridCopy.numRows(); i++){
        for(int j = 0; j < gridCopy.numCols(); j++){
            if(gridCopy.get(i,j) == 0){
                cout << '-';
                gui.drawCell(i,j, false);
            } else {
                cout << 'X';
                gui.drawCell(i,j, true);
            }
        }
        cout << "" << endl;
    }
}

/*To advance the generation, I first make a copy of the initial grid, as
I do not want to run into issues with generation 'k+1.' For each cell in
 the grid, I check to see how many neighbors the given cell has, which can
 then be used to see if the cell will live or die.*/
void advanceGeneration(Grid<int> & gridCopy){
    Grid<int> initGrid;
    initGrid = gridCopy;
    for(int i = 0; i < gridCopy.numRows(); i++){
        for(int j = 0; j < gridCopy.numCols(); j++){
            checkNeighbors(gridCopy, initGrid, i, j);
        }
    }
}

/*This method counts the number of neighbors for a given cell. It is similar to the example
of the Raptor from class during the first week. Each neighboring cell is checked to see if
the neighbor is alive or dead, and a count of the number of neighbors is found for the
specific cell. Then, based on the rules from the syllabus, we determine whether the cell
should live or die.*/
void checkNeighbors(Grid<int> & gridCopy, Grid<int> & initGrid, int rowNum, int colNum){
    int numNeighbors = 0;

    for(int i = rowNum - 1; i <= rowNum + 1; i++){
        for(int j = colNum - 1; j <= colNum + 1; j++){
            if(!(i == rowNum && j == colNum)) {
                bool p = checkSpecificLocationForNeighborExtension(initGrid, i, j);
                if(p) numNeighbors++;
            }
        }
    }

    if(numNeighbors == 0 || numNeighbors == 1){
        gridCopy.set(rowNum, colNum, 0); //dead cell
    } else if(numNeighbors == 2){
    } else if(numNeighbors == 3){
        gridCopy.set(rowNum, colNum, 1); //live cell
    } else {
        gridCopy.set(rowNum, colNum, 0); //dead cell
    }
}

/*This method checks a specific location of the grid from generation 'k'
to see if a cell exists. It returns false if the location is out of bounds and also
returns false if the cell is dead. It returns true if the cell is alive.*/
/*bool checkSpecificLocationForNeighbor(Grid<int> & initGrid, int row, int col){
    if(row < 0 || col < 0 || col >= initGrid.numCols() || row >= initGrid.numRows()){
        return false;
    } else if (initGrid.get(row,col) == 0){
        return false;
    }
    return true;
}*/

//if wrap-around is used!
bool checkSpecificLocationForNeighborExtension(Grid<int> & initGrid, int row, int col){
    if(row < 0){
        row = initGrid.numRows();
    }
    if (col < 0){
        col = initGrid.numCols();
    }
    if (col >= initGrid.numCols()){
        col = 0;
    }
    if (row >= initGrid.numRows()){
        row = 0;
    }

    if (initGrid.get(row,col) == 0){
        return false;
    }
    return true;
}

