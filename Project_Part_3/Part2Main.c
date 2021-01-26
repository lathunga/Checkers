#include "Part2.h"
#include "Part1.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void freeBoard(char** board);
void createFiles(FILE* input, char* coords, int numMoves, char** board, struct data* init, int turn, int argc, char* argv[], int actualNumMoves, int m, int count, int isIllegal);

int main(int argc, char *argv[])
{
  int p = 1;
  int numMoves;
  int m = 0;
  int countMoves = 1;
  int in = 1;
  int isIllegal = 0;

  /*
    Parses through the command line arguments
    Interprets m switch and input file
  */
  while(p<argc)
  {
    if(argv[p][0]=='-')
    {
      if(argv[p][1]=='m')
      {
        numMoves = (int) strtol(argv[++p], (char **)NULL, 10);
        m = 1;
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

  FILE* firstTemp = fopen(argv[in], "r");
  int actualNumMoves = numberOfMoves(firstTemp);
  fclose(firstTemp);

  if(m==0)
  {
    numMoves=actualNumMoves;
  }

  if(numMoves>actualNumMoves)
  {
	   numMoves=actualNumMoves;
  }

  printf("%s %d %s %d %s", "Input file has", actualNumMoves, "moves\nProcessing", numMoves, "moves\n");

  FILE* temp = fopen(argv[in], "r");
  struct data tempor = {0, 0, 0, 0, 0, 0, 0};
  struct data* init = &tempor;
  checkValid(temp, init);
  int capture = init->capture;
  int turn = init->turn;
  fclose(temp);
  FILE* input = fopen(argv[in], "r");
  scanUntilBoard(input);
  char** board = getInitialBoard(input);
  scanUntilMoves(input);
  char* coords = getCoords(input);
  int count = 0;
  int startFile;
  int startRank;
  int destFile;
  int destRank;
  int num = 2;
  int multiple = 0;
  char current;

  if(numMoves==0)
  {
    createFiles(input, coords, numMoves, board, init, turn, argc, argv, actualNumMoves, m, countMoves, 0);
    printf("All moves are legal\n");
    free(coords);
    fclose(input);
    return 1;
  }

  /*
    Parses through moves in the input file
    If a move is legal, the move is executed and saved
    If is a move is illegal, the necessary files are created, outputs stdout information, and returns 0
  */
  while(countMoves-1<=numMoves)
  {
    startFile = coords[0]-97;
    startRank = coords[1]-48-1;
    destFile = coords[2]-97;
    destRank = coords[3]-48-1;
    current = fgetc(input);
    if( (current=='>' && init->multipleJumps==0) || (current=='>' && (startRank+2!=destRank && startRank-2!=destRank) ))
    {
      char coord1 = coords[2];
      char coord2 = coords[3];
      coords = realloc(coords, 8*sizeof(char));
      coords[2] = '-';
      coords[3] = '>';
      coords[4] = coord1;
      coords[5] = coord2;
      coords[6] = '-';
      coords[7] = '>';
      current=fgetc(input);
      int c = 8;
      int z = 1;
      while( ((current>=49 && current<=56) || (current>=97 && current<=104) || current=='-' || current=='>') && current!=' ' && current!='#' && current!=13)
      {
        coords = realloc(coords, (z*2)+8*sizeof(char));
        coords[c] = current;
        current=fgetc(input);
        coords[++c] = current;
        current=fgetc(input);
        c++;
        z++;
      }
      reformatBoard(board);
      int k = 0;
      if(turn ==0)
      {
        turn = 1;
      }
      else
      {
        turn = 0;
      }
      while(k<argc)
      {
        if(argv[k][0]=='-')
        {
          if(argv[k][1]=='e')
          {
            k++;
            createExchange(argv[k], init, board, coords, numMoves, 1, actualNumMoves);
          }
          else if(argv[k][1]=='h')
          {
            k++;
            createHumanReadable(board, argv[k], coords, actualNumMoves, numMoves, 1);
          }
        }
        k++;
      }
      printf("%s %d %s %s\n","Move", countMoves, "is illegal:", coords);
      free(coords);
      freeBoard(board);
      fclose(input);
      return 0;
    }
    countMoves++;
    if(turn==1 && (board[startRank][startFile]=='r' || board[startRank][startFile]=='R'))
    {
      if(board[startRank][startFile]=='r')
      {
        if(isValidRedPawnMove(coords, board, capture, init)==0)
        {
          createFiles(input, coords, numMoves, board, init, turn, argc, argv, actualNumMoves, m, countMoves, 1);
          coords = realloc(coords, 7*sizeof(char));
          char coord2_1 = coords[2];
          char coord2_2 = coords[3];
          coords[2] = '-';
          coords[3] = '>';
          coords[4] = coord2_1;
          coords[5] = coord2_2;
          coords[6] = '\0';
          printf("%s %d %s %s\n","Move", --countMoves, "is illegal:", coords);
          free(coords);
          fclose(input);
          return 0;
        }
      }
      else
      {
        if(isValidRedKingMove(coords, board, capture, init)==0)
        {
          createFiles(input, coords, numMoves, board, init, turn, argc, argv, actualNumMoves, m, countMoves, 1);
          coords = realloc(coords, 7*sizeof(char));
          char coord2_1 = coords[2];
          char coord2_2 = coords[3];
          coords[2] = '-';
          coords[3] = '>';
          coords[4] = coord2_1;
          coords[5] = coord2_2;
          coords[6] = '\0';
          printf("%s %d %s %s\n","Move", --countMoves, "is illegal:", coords);
          free(coords);
          fclose(input);
          return 0;
        }
      }
    }
    else if(turn==0 && board[startRank][startFile]=='b' || board[startRank][startFile]=='B')
    {
      if(board[startRank][startFile]=='b')
      {
        if(isValidBlackPawnMove(coords, board, capture, init)==0)
        {
          createFiles(input, coords, numMoves, board, init, turn, argc, argv, actualNumMoves, m, countMoves, 1);
          coords = realloc(coords, 7*sizeof(char));
          char coord2_1 = coords[2];
          char coord2_2 = coords[3];
          coords[2] = '-';
          coords[3] = '>';
          coords[4] = coord2_1;
          coords[5] = coord2_2;
          coords[6] = '\0';
          printf("%s %d %s %s\n","Move", --countMoves, "is illegal:", coords);
          free(coords);
          fclose(input);
          return 0;
        }
      }
      else
      {
        if(isValidBlackKingMove(coords, board, capture, init)==0)
        {
          createFiles(input, coords, numMoves, board, init, turn, argc, argv, actualNumMoves, m, countMoves, 1);
          coords = realloc(coords, 7*sizeof(char));
          char coord2_1 = coords[2];
          char coord2_2 = coords[3];
          coords[2] = '-';
          coords[3] = '>';
          coords[4] = coord2_1;
          coords[5] = coord2_2;
          coords[6] = '\0';
          printf("%s %d %s %s\n","Move", --countMoves, "is illegal:", coords);
          free(coords);
          fclose(input);
          return 0;
        }
      }
    }
    else
    {
      createFiles(input, coords, numMoves, board, init, turn, argc, argv, actualNumMoves, m, countMoves, 1);
      coords = realloc(coords, 7*sizeof(char));
      char coord2_1 = coords[2];
      char coord2_2 = coords[3];
      coords[2] = '-';
      coords[3] = '>';
      coords[4] = coord2_1;
      coords[5] = coord2_2;
      coords[6] = '\0';
      printf("%s %d %s %s\n","Move", --countMoves, "is illegal:", coords);
      free(coords);
      fclose(input);
      return 0;
    }
    if(current!='>')
    {
      if(turn==0)
      {
        turn = 1;
      }
      else
      {
        turn = 0;
      }
    }
    if(countMoves<=numMoves && current!='>')
    {
      ungetc(current, input);
      scanIgnored(input, 0);
      coords = getCoords(input);
      if(coords==NULL)
      {
        break;
      }
    }
    else if(current=='>')
    {
      countMoves--;
      coords[0] = coords[2];
      coords[1] = coords[3];
      coords[2] = fgetc(input);
      coords[3] = fgetc(input);
      fgetc(input);
    }
    else
    {
      break;
    }
    num++;
  }

  ungetc(current, input);
  scanIgnored(input, 0);
  coords = getCoords(input);
  createFiles(input, coords, numMoves, board, init, turn, argc, argv, actualNumMoves, m, --countMoves, 0);
  free(coords);
  printf("All moves are legal\n");
  fclose(input);
  return 1;
}

/*
  Creates the neccarry files based on the switches
*/
void createFiles(FILE* input, char* coords, int numMoves, char** board, struct data* init, int turn, int argc, char* argv[], int actualNumMoves, int m, int count, int isIllegal)
{
  char* remainingCoords = malloc(6*sizeof(char));
  int counter = numMoves;
  int i = 0;
  int j = 1;
  while(coords!=NULL || (m==1 && counter<actualNumMoves && numMoves!=0) || (numMoves==0 && coords!=NULL))
  {
    remainingCoords = realloc(remainingCoords, (j*6)*sizeof(char));
    remainingCoords[i] = coords[0];
    remainingCoords[++i] = coords[1];
    remainingCoords[++i] = '-';
    remainingCoords[++i] = '>';
    remainingCoords[++i] = coords[2];
    remainingCoords[++i] = coords[3];
    i++;
    scanIgnored(input, 0);
    coords = getCoords(input);
    counter++;
    j++;
  }
  remainingCoords[(j-1)*6] = '\0';
  reformatBoard(board);
  int k = 0;
  init->turn = turn;
  while(k<argc)
  {
    if(argv[k][0]=='-')
    {
      if(argv[k][1]=='e')
      {
        k++;
        createExchange(argv[k], init, board, remainingCoords, numMoves, isIllegal, actualNumMoves);
      }
      else if(argv[k][1]=='h')
      {
        k++;
        createHumanReadable(board, argv[k], remainingCoords, actualNumMoves, numMoves, isIllegal);
      }
    }
    k++;
  }
  free(remainingCoords);
  freeBoard(board);
}

void freeBoard(char** board)
{
  int r;
  for(r=0; r<8; r++)
  {
    free(board[r]);
  }
  free(board);
}
