/* Author: Luke Thunga */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Part1.h"

int main()
{
  FILE* input = stdin;

  int currLevel = 1;

  currLevel = oneWord("RULES:", 7, input, currLevel);

  char cap[11];
  currLevel = twoWords("capture", "no capture", 8, 11, cap, input, currLevel);

  char jumps[15];
  currLevel = twoWords("single jumps", "multiple jumps", 13, 15, jumps, input, currLevel);

  currLevel = oneWord("TURN:", 6, input, currLevel);

  char turn[6];
  currLevel = twoWords("red", "black", 4, 6, turn, input, currLevel);

  currLevel = oneWord("BOARD:", 7, input, currLevel);

  int blackPawns = 0;
  int blackKings = 0;
  int redPawns = 0;
  int redKings = 0;

  currLevel = checkBoard(input, &blackPawns, &blackKings, &redPawns, &redKings, currLevel);

  currLevel = oneWord("MOVES:", 7, input, currLevel);

  currLevel = checkMoves(input, currLevel);

  printf("%s%s%s%s%i %s%i%s%s%i %s%i%s","VALID INPUT\nInitial configuration:\nTurn: ", turn,
  "\n","Red: ", redKings, "kings, ", redPawns, " pawns\n",
  "Black: ", blackKings, "kings, ", blackPawns, " pawns\n");
}
