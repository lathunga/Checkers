#include <stdio.h>
#include <stdlib.h>
#include "Part2.c"
#include "Part3.h"


int main(int argc, char *argv[])
{
  int p = 1;
  int in = 1;
  int depth = 0;
  int verbose = 0;
  int numMoves;
  int m = 0;
  int countMoves = 1;
  int isIllegal = 0;
  while(p<argc)
  {
    if(argv[p][0]=='-')
    {
      if(argv[p][1]=='d')
      {
        depth = (int) strtol(argv[++p], (char **)NULL, 10);
        p++;
      }
      else if(argv[p][1]=='v')
      {
        verbose = 1;
        p++;
      }
      else
      {
        p = p+2;
      }
    }
    else
    {
      in = p;
      p++;
    }
  }

  FILE* temp;
  if(in == -1)
  {
    temp = stdin;
  }
  else
  {
    temp = fopen(argv[in], "r");
  }
  struct data stateTemp = {0, 0, 0, 0, 0, 0, 0};
  struct data* state = &stateTemp;
  checkValid(temp, state);
  fclose(temp);
  if(state->turn==0)
  {
    state->turn = -1;
  }
  else
  {
    state->turn = 1;
  }

  FILE* input;
  if(in == -1)
  {
    input = stdin;
  }
  else
  {
    input = fopen(argv[in], "r");
  }
  scanUntilBoard(input);
  char** board = getInitialBoard(input);
  int counter = 0;
  int i;
  if(state->turn==1)
  {
    printPossibleMovesRedPawn(board, 'b', 'B', counter, ++depth, 0, -99, verbose);
  }
  else
  {
    printPossibleMovesBlackPawn(board, 'r', 'R', counter, ++depth, 0, -99, verbose);
  }
  fclose(input);
}
