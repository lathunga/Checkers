Author: Luke Thunga

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

    
