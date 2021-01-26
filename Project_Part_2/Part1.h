/* Author: Luke Thunga */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int scanIgnored(FILE* in, int level);

int scanBoardIgnored(FILE* in, int level);

int oneWord(char word[], int wordLen, FILE* input, int level);

int twoWords(char word1[], char word2[], int word1Len, int word2Len, char output[], FILE* input, int level);

int checkBoard(FILE* input, int* blackPawns, int* blackKings, int* redPawns, int* redKings, int currLevel);

int checkMoves(FILE* input, int currLevel);
