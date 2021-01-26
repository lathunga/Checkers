Author: Luke Thunga

Part1:

Data Structures-

The keywords are read from the input file and stored in strings.

Source Files -

main.c: Takes a file as standard input and calls other functions to check for validity.

ReadInput.c: Contains the implementation of the function headers in ReadInput.h

ReadInput.h: Contains the scanIgnored, scanBoardIgnored, oneWords, twoWords, checkBoard, and checkMoves functions.

Functions -

scanIgnored: This scans past whitespace (spaces, tabs, carriage returns, newlines). It returns the current linenumber.

scanBoardIgnored: This is used when reading the board. It scans past certain characters (’|’, ’-’, ’+’) and whitespace. It returns the current line number.

oneWord: This is used to read an expected single keyword. It calls scanBoard to get the first character and reads for the length of the word. Then, it compares the read word to the expected word. It sends an error to standard error if they don't match. Otherwise, It returns the current line number. 

twoWord: This is used to read two expected keywords in succession. It calls scanBoard to get the first character and reads for the length of the words. Then, it compares the read words to the expected words. It sends an error to standad error if they don't match. Otherwise, it returns the current line number.

checkBoard: This is used to check the validity of the checker board. It checks for both a flipped and a standard board. If the board is invalid it sends an error to standard error, otherwise it returns the current line number.

checkMoves: This is used to check the validity of the moves. If the move is invalid, it sends an error to standard error, otherwise it returns the current line number.

Part3: 

Unimplemented - capture and multiple jumps are unimplemented

Data Structures -

A 2D char array is used to track the state of the board.

Source files -

Part3Main.c: Checks to see if valid file and finds all possible moves/scores up to a certain depth.

Part3.c: functions to find scores/moves.

Part3.h: function prototypes for part3.c

Functions -

int* findPiece: finds a valid piece to be moved
int isValidCapture: finds if a certain capture a valid move
int printPossibleMovesRedPawn: find and print possible red moves recursively
int printPossibleMovesBlackPawn: find and print possible black moves recursively
void move_up_left: bounds check
void move_up_right: bounds check
void move_down_left: bounds check
void move_down_right: bounds check
void capture_up_left: bounds check
void capture_up_right: bounds check
void capture_down_left: bounds check
void capture_down_right: bounds check
void copyBoard: Reverts board back to previous state
void printQuestionLine: prints line starting with question mark
void printDotLine: prints line starting with dot.
int calculateScore: calculates the score of the current board.
