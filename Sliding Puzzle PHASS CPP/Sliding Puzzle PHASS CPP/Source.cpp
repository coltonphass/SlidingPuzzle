/*
 * Sliding Puzzle Game
 * Author: Colton Phass
 *
 * Description:
 * This program implements a classic 3x3 sliding puzzle game. The game initializes
 * with a solved state, then shuffles the board. The player uses WASD keys to slide
 * tiles into the empty space, aiming to restore the original ordered configuration.
 * The game features a color-coded display and continues until the puzzle is solved.
 */

#pragma warning(disable:6031)  // Disables warning about ignoring return value of _getch()

 // LIBRARIES & NAMESPACE
#include <iostream>     // For input/output operations
#include <iomanip>      // For setw() function
#include <conio.h>      // For _getch() function
#include <windows.h>    // For Windows console functions (color)
#include <cstdlib>      // For rand() and srand() functions
#include <ctime>        // For time() function

using namespace std;

// CONSTANTS
#define BOARD_ROWS             3       // Number of rows in the game board
#define BOARD_COLS             3       // Number of columns in the game board
#define SPACE_EMPTY           -1       // Value representing the empty tile
#define EMPTY_TILE_SYMBOL     "*"      // Symbol to display for the empty tile

// DIRECTION CODES
#define MOVE_TILE_UP           1       // Code for moving a tile up
#define MOVE_TILE_DOWN         2       // Code for moving a tile down
#define MOVE_TILE_LEFT         3       // Code for moving a tile left
#define MOVE_TILE_RIGHT        4       // Code for moving a tile right
#define DIRECTION_UNDEFINED   -1       // Code for an undefined direction

// COLORS
#define COLOR_DEFAULT         7        // Default console text color
#define COLOR_RED             12       // Red color for misplaced tiles
#define COLOR_GREEN           10       // Green color for correctly placed tiles

// PROTOTYPES
void InitializeBoard (int board[BOARD_ROWS][BOARD_COLS]);
void PrintBoard      (int board[BOARD_ROWS][BOARD_COLS]);
bool moveTile        (int board[BOARD_ROWS][BOARD_COLS], int direction);
void shuffleBoard    (int board[BOARD_ROWS][BOARD_COLS]);
bool checkIfSolved   (int board[BOARD_ROWS][BOARD_COLS]);

// MAIN FUNCTION
int main() {
    int slidingBoard[BOARD_ROWS][BOARD_COLS];

    char keyStroke = ' ';
    int directionCode = DIRECTION_UNDEFINED;

    // Seed the random number generator
    srand(time(0));

    // Initialize and display the solved board
    InitializeBoard(slidingBoard);
    PrintBoard(slidingBoard);

    cout << "Press any key to shuffle the board." << endl;
    _getch();

    cout << "Shuffling board..." << endl;
    shuffleBoard(slidingBoard);

    cout << "Shuffling complete. Press any key to begin game" << endl;
    _getch();

    // Main game loop
    while (!checkIfSolved(slidingBoard)) {
        system("cls");  // Clear the console
        PrintBoard(slidingBoard);

        cout << "Move with WASD (W = Up ^, A = Left <, S = Down v, D = Right >): ";
        keyStroke = _getche();  // Get a single character input without pressing Enter

        // Convert keystrokes to direction codes
        switch (keyStroke) {
        case 'w': directionCode = MOVE_TILE_UP; break;
        case 's': directionCode = MOVE_TILE_DOWN; break;
        case 'a': directionCode = MOVE_TILE_LEFT; break;
        case 'd': directionCode = MOVE_TILE_RIGHT; break;
        default: directionCode = DIRECTION_UNDEFINED; break;
        }

        if (directionCode != DIRECTION_UNDEFINED) {
            bool moveSuccessful = moveTile(slidingBoard, directionCode);
            if (!moveSuccessful) {
                cout << endl << "OUT OF BOUNDS! TRY AGAIN." << endl;
                _getch();
            }
        }
    }

    // Game completed
    system("cls");
    PrintBoard(slidingBoard);
    cout << "Congratulations! You solved the puzzle!" << endl;

    _getch();

    return 0;
}

// FUNCTIONS

// Function to initialize the game board in solved state
void InitializeBoard(int board[BOARD_ROWS][BOARD_COLS]) {
    int currentTile = 1;

    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLS; j++) {
            if (i == BOARD_ROWS - 1 && j == BOARD_COLS - 1) {
                board[i][j] = SPACE_EMPTY;  // Last tile is empty
            }
            else {
                board[i][j] = currentTile++;
            }
        }
    }
}

// Function to print the current state of the game board
void PrintBoard(int board[BOARD_ROWS][BOARD_COLS]) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    cout << "+---+---+---+" << endl;

    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLS; j++) {
            cout << "|";

            if (board[i][j] == SPACE_EMPTY) {
                SetConsoleTextAttribute(hConsole, COLOR_RED);
                cout << " " << EMPTY_TILE_SYMBOL << " ";
            }
            else {
                int expectedValue = i * BOARD_COLS + j + 1;
                if (board[i][j] == expectedValue) {
                    SetConsoleTextAttribute(hConsole, COLOR_GREEN);  // Correct position
                }
                else {
                    SetConsoleTextAttribute(hConsole, COLOR_RED);    // Incorrect position
                }
                cout << setw(3) << board[i][j];
            }

            SetConsoleTextAttribute(hConsole, COLOR_DEFAULT);
        }

        cout << "|" << endl;

        cout << "+---+---+---+" << endl;
    }
}

// Function to move a tile in the specified direction
bool moveTile(int board[BOARD_ROWS][BOARD_COLS], int direction) {
    int emptyRow = -1, emptyCol = -1;

    // Find the empty tile
    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLS; j++) {
            if (board[i][j] == SPACE_EMPTY) {
                emptyRow = i;
                emptyCol = j;
                break;
            }
        }
        if (emptyRow != -1) break;
    }

    if (emptyRow == -1 || emptyCol == -1) {
        return false;  // Empty tile not found (should never happen)
    }

    // Move tile based on direction
    switch (direction) {
    case MOVE_TILE_UP:
        if (emptyRow > 0) {
            board[emptyRow][emptyCol] = board[emptyRow - 1][emptyCol];
            board[emptyRow - 1][emptyCol] = SPACE_EMPTY;
            return true;
        }
        break;
    case MOVE_TILE_DOWN:
        if (emptyRow < BOARD_ROWS - 1) {
            board[emptyRow][emptyCol] = board[emptyRow + 1][emptyCol];
            board[emptyRow + 1][emptyCol] = SPACE_EMPTY;
            return true;
        }
        break;
    case MOVE_TILE_LEFT:
        if (emptyCol > 0) {
            board[emptyRow][emptyCol] = board[emptyRow][emptyCol - 1];
            board[emptyRow][emptyCol - 1] = SPACE_EMPTY;
            return true;
        }
        break;
    case MOVE_TILE_RIGHT:
        if (emptyCol < BOARD_COLS - 1) {
            board[emptyRow][emptyCol] = board[emptyRow][emptyCol + 1];
            board[emptyRow][emptyCol + 1] = SPACE_EMPTY;
            return true;
        }
        break;
    }
    return false;  // Move not possible
}

// Function to shuffle the board by making random moves
void shuffleBoard(int board[BOARD_ROWS][BOARD_COLS]) {
    int moves = 100;  // Number of random moves to make
    for (int i = 0; i < moves; i++) {
        int randomDirection = rand() % 4 + 1;  // Random direction (1-4)
        moveTile(board, randomDirection);
    }
}

// Function to check if the puzzle is solved
bool checkIfSolved(int board[BOARD_ROWS][BOARD_COLS]) {
    int expectedValue = 1;

    for (int i = 0; i < BOARD_ROWS; i++) {
        for (int j = 0; j < BOARD_COLS; j++) {
            if (i == BOARD_ROWS - 1 && j == BOARD_COLS - 1) {
                return board[i][j] == SPACE_EMPTY;  // Last tile should be empty
            }
            else if (board[i][j] != expectedValue) {
                return false;  // Tile is not in the correct position
            }
            expectedValue++;
        }
    }
    return true;  // All tiles are in correct positions
}