#include "Part1.h"
#include "Part2.h"
#include <stdio.h>
#include "Part1.c"

void scanUntilBoard(FILE* input)
{
  scanIgnored(input, 0);
  int currChar = fgetc(input);
  while(currChar!='"')
  {
    scanIgnored(input, 0);
    currChar = fgetc(input);
  }
  ungetc(currChar, input);
}

char** getInitialBoard(FILE* input)
{
  char **initBoard = malloc(8 * sizeof(char*));
  int r = 7;
  int c;
  while(r>=0)
  {
    initBoard[r] = malloc(8*sizeof(char));
    c = 0;
    while(c<8)
    {
      initBoard[r][c] = fgetc(input);
      scanBoardIgnored(input, 0);
      c++;
    }
    r--;
  }
  return initBoard;
}

int isValidBlackPawnMove(char* coords, char** board, int capture, struct data* init)
{
  int i = 0;
  int startFile = coords[0]-97;
  int startRank = coords[1]-48-1;
  int destFile = coords[2]-97;
  int destRank = coords[3]-48-1;
  if(board[destRank][destFile] != '.' || board[startRank][startFile]=='R' || board[startRank][startFile]=='r')
  {
    return 0;
  }
  if( (destFile==startFile+2 || destFile==startFile-2) && (destRank==startRank-2) )
  {
    if(destFile==startFile+2)
    {
      if(board[startRank-1][startFile+1]=='r' || board[startRank-1][startFile+1]=='R')
      {
        board[destRank][destFile] = board[startRank][startFile];
        board[startRank][startFile] = '.';
        board[(startRank-1)][startFile+1]='.';
      }
      else
      {
        return 0;
      }
    }
    else if(destFile==startFile-2)
    {
      if(board[(startRank-1)][startFile-1]=='r' || board[(startRank-1)][startFile-1]=='R')
      {
        if(destRank==0)
        {
          board[destRank][destFile] == 'B';
        }
        else
        {
          board[destRank][destFile] = board[startRank][startFile];
        }
        board[startRank][startFile] = '.';
        board[(startRank-1)][startFile-1]='.';
      }
      else
      {
        return 0;
      }
    }
  }
  else if((destFile==startFile+1 || destFile==startFile-1) && (destRank==startRank-1))
  {
     if(init->capture==1)
     {
       if(startRank-2>=0)
       {
         if(startFile-1>=0)
         {
           if((board[startRank-1][startFile-1]=='r' || board[startRank-1][startFile-1]=='R') && board[startRank-2][startFile-2]=='.')
           {
             return 0;
           }
         }
         if(startFile+1<=7)
         {
           if((board[startRank-1][startFile+1]=='r' || board[startRank-1][startFile+1]=='R') && board[startRank-2][startFile+2]=='.')
           {
              return 0;
           }
         }
       }
     }
    if(destRank==0)
    {
      board[startRank][startFile]='.';
      board[destRank][destFile] = 'B';
    }
    else
    {
      board[destRank][destFile] = board[startRank][startFile];
      board[startRank][startFile] = '.';
    }
  }
  else
  {
    return 0;
  }
  return 1;
}

int isValidRedKingMove(char* coords, char** board, int capture, struct data* init)
{
  if(isValidRedPawnMove(coords, board, capture, init)==1)
  {
    return 1;
  }
  int i = 0;
  int startFile = coords[0]-97;
  int startRank = coords[1]-48-1;
  int destFile = coords[2]-97;
  int destRank = coords[3]-48-1;
  if(board[destRank][destFile] != '.')
  {
    return 0;
  }
  if( (destFile==startFile+2 || destFile==startFile-2) && (destRank==startRank-2) )
  {
    if(destFile==startFile+2)
    {
      if(board[startRank-1][startFile+1]=='b' || board[startRank-1][startFile+1]=='B')
      {
        board[destRank][destFile] = board[startRank][startFile];
        board[startRank][startFile] = '.';
        board[(startRank-1)][startFile+1]='.';
      }
    }
    else if(board[(startRank-1)][startFile-1]=='b' || board[(startRank-1)][startFile-1]=='B')
    {
      board[destRank][destFile] = board[startRank][startFile];
      board[startRank][startFile] = '.';
      board[(startRank-1)][startFile-1]='.';
    }
    else
    {
      return 0;
    }
  }
  else if((destFile==startFile+1 || destFile==startFile-1) && (destRank==startRank-1))
  {
    if(init->capture==1)
    {
      if(startRank-2>=0)
      {
        if(startFile-1>=0)
        {
          if((board[startRank-1][startFile-1]=='b' || board[startRank-1][startFile-1]=='B') && board[startRank-2][startFile-2]=='.')
          {
            return 0;
          }
        }
        if(startFile+1<=7)
        {
          if((board[startRank-1][startFile+1]=='b' || board[startRank-1][startFile+1]=='B') && board[startRank-2][startFile+2]=='.')
          {
             return 0;
          }
        }
      }
    }
    board[destRank][destFile] = board[startRank][startFile];
    board[startRank][startFile] = '.';
  }
  else
  {
    return 0;
  }
  return 1;
}

int isValidBlackKingMove(char* coords, char** board, int capture, struct data* init)
{
  if(isValidBlackPawnMove(coords, board, capture, init)==1)
  {
    return 1;
  }
  int i = 0;
  int startFile = coords[0]-97;
  int startRank = coords[1]-48-1;
  int destFile = coords[2]-97;
  int destRank = coords[3]-48-1;
  if(board[destRank][destFile] != '.')
  {
    return 0;
  }
  if( (destFile==startFile+2 || destFile==startFile-2) && (destRank==startRank+2) )
  {
    if(destFile==startFile+2)
    {
      if(board[(startRank+1)][startFile+1]=='r' || board[(startRank+1)][startFile+1]=='R')
      {
        board[destRank][destFile] = board[startRank][startFile];
        board[startRank][startFile] = '.';
        board[(startRank+1)][startFile+1]='.';
      }
    }
    else if(board[(startRank+1)][startFile-1]=='r' || board[(startRank+1)][startFile-1]=='R')
    {
      board[destRank][destFile] = board[startRank][startFile];
      board[startRank][startFile] = '.';
      board[(startRank+1)][startFile-1]='.';
    }
    else
    {
      return 0;
    }
  }
  else if((destFile==startFile+1 || destFile==startFile-1) && (destRank==startRank+1))
  {
    if(init->capture==1)
    {
      if(startRank+2<=7)
      {
        if(startFile-1>=0)
        {
          if((board[startRank+1][startFile-1]=='r' || board[startRank+1][startFile-1]=='R') && board[startRank+2][startFile-2]=='.')
          {
            return 0;
          }
        }
        if(startFile+1<=7)
        {
          if((board[startRank+1][startFile+1]=='r' || board[startRank+1][startFile+1]=='R') && board[startRank+2][startFile+2]=='.')
          {
            return 0;
          }
        }
      }
    }
    board[destRank][destFile] = board[startRank][startFile];
    board[startRank][startFile] = '.';
  }
  else
  {
    return 0;
  }
  return 1;
}

int isValidRedPawnMove(char* coords, char** board, int capture, struct data* init)
{
  int i = 0;
  int startFile = coords[0]-97;
  int startRank = coords[1]-48-1;
  int destFile = coords[2]-97;
  int destRank = coords[3]-48-1;
  if(board[destRank][destFile] != '.' || board[startRank][startFile]=='B' || board[startRank][startFile]=='b')
  {
    return 0;
  }
  if( (destFile==startFile+2 || destFile==startFile-2) && (destRank==startRank+2) )
  {
    if(destFile==startFile+2)
    {
      if(board[(startRank+1)][startFile+1]=='b' || board[(startRank+1)][startFile+1]=='B')
      {
        if(destRank==7)
        {
          board[destRank][destFile] = 'R';
        }
        else
        {
          board[destRank][destFile] = board[startRank][startFile];
        }
        board[startRank][startFile] = '.';
        board[(startRank+1)][startFile+1]='.';
      }
      else
      {
        return 0;
      }
    }
    else if(destFile==startFile-2)
    {
      if(board[(startRank+1)][startFile-1]=='b' || board[(startRank+1)][startFile-1]=='B')
      {
        if(destRank==7)
        {
          board[destRank][destFile] == 'R';
        }
        else
        {
          board[destRank][destFile] = board[startRank][startFile];
        }
        board[startRank][startFile] = '.';
        board[(startRank+1)][startFile-1]='.';
      }
      else
      {
        return 0;
      }
    }
    else if(board[(startRank+1)][startFile-1]=='b' || board[(startRank+1)][startFile-1]=='B')
    {
      if(destRank==7)
      {
        board[destRank][destFile] == 'R';
      }
      else
      {
        board[destRank][destFile] = board[startRank][startFile];
      }
      board[startRank][startFile] = '.';
      board[(startRank+1)][startFile-1]='.';
    }
    else
    {
      return 0;
    }
  }
  else if((destFile==startFile+1 || destFile==startFile-1) && (destRank==startRank+1))
  {
    if(init->capture==1)
    {
      if(startRank+2<=7)
      {
        if(startFile-1>=0)
        {
          if((board[startRank+1][startFile-1]=='b' || board[startRank+1][startFile-1]=='B') && board[startRank+2][startFile-2]=='.')
          {
            return 0;
          }
        }
        if(startFile+1<=7)
        {
          if((board[startRank+1][startFile+1]=='b' || board[startRank+1][startFile+1]=='B') && board[startRank+2][startFile+2]=='.')
          {
            return 0;
          }
        }
      }
    }
    if(destRank==7)
    {
      board[startRank][startFile]='.';
      board[destRank][destFile] = 'R';
    }
    else
    {
      board[destRank][destFile] = board[startRank][startFile];
      board[startRank][startFile] = '.';
    }
  }
  else
  {
    return 0;
  }
  return 1;
}

void scanUntilMoves(FILE* input)
{
  scanIgnored(input, 0);
  while(fgetc(input)!=':');
  scanIgnored(input, 0);
}

char* getCoords(FILE* input)
{
  char currChar = fgetc(input);
  char* coords = malloc(4 * sizeof(char));
  if(currChar==EOF)
  {
    return NULL;
  }
  int i=1;
  int count = 0;
  while(count<4)
  {
    if(i%4==1)
    {
      coords[count] = currChar;
      i++;
      count++;
    }
    else if(i%4==2)
    {
      coords[count] = currChar;
      i++;
      count++;
    }
    else if(i%4==0)
    {
      i=1;
    }
    else
    {
      i++;
    }
    currChar = fgetc(input);
  }
  return coords;
}

void checkValid(FILE* input, struct data* initData)
{
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

  initData->blackPawns = blackPawns;
  initData->blackKings = blackKings;
  initData->redPawns = redPawns;
  initData->redKings = redKings;
  if(cap[0]=='c')
  {
    initData->capture = 1;
  }
  else
  {
    initData->capture = 0;
  }
  if(jumps[0]=='m')
  {
    initData->multipleJumps = 1;
  }
  else
  {
    initData->multipleJumps= 0;
  }
  if(turn[0]=='r')
  {
    initData->turn = 1;
  }
  else
  {
    initData->turn = 0;
  }
}

void createExchange(char* name, struct data* final, char** board, char* coords, int numMoves, int isIllegal, int actualNumMoves)
{
  char* capturestr;
  char* multipleJumpsstr;
  char* turnstr;
  if(final->capture==0)
  {
    capturestr = "no capture";
  }
  else
  {
    capturestr = "capture";
  }
  if(final->multipleJumps==0)
  {
    multipleJumpsstr = "single jumps";
  }
  else
  {
    multipleJumpsstr = "multiple jumps";
  }
  if(final->turn==0)
  {
    turnstr = "black";
  }
  else
  {
    turnstr = "red";
  }
  char newBoard[65];
  formatBoard(board, newBoard);
  FILE* exchange = fopen(name, "w");
  fputs("RULES:", exchange);
  fputs(" ", exchange);
  fputs(capturestr, exchange);
  fputs(" ", exchange);
  fputs(multipleJumpsstr, exchange);
  fputs(" ", exchange);
  fputs("TURN:", exchange);
  fputs(" ", exchange);
  fputs(turnstr, exchange);
  fputs(" ", exchange);
  fputs("BOARD:", exchange);
  fputs(" ", exchange);
  fputs(newBoard, exchange);
  fputs(" ", exchange);
  fputs("MOVES:", exchange);
  fputs(" ", exchange);
  if(actualNumMoves-numMoves>=1 || isIllegal==1)
  {
    fputs(coords, exchange);
  }
}

void createHumanReadable(char** board, char* name, char* coords, int actualNumMoves, int numMoves, int isIllegal)
{
  char newBoard[65];
  formatBoard(board, newBoard);
  FILE* human = fopen(name, "w");
  fputs("    a   b   c   d   e   f   g   h\n", human);
  int r=0;
  int i;
  char rank = '8';
  while(r<64)
  {
    fputs("  +---+---+---+---+---+---+---+---+\n", human);
    i = 0;

    fputc(rank, human);
    while(i<8)
    {
      fputs(" | ", human);
      fputc(newBoard[r], human);
      i++;
      r++;
    }
    fputs(" | ", human);
    fputc(rank, human);
    fputs("\n", human);
    rank--;
  }
  fputs("  +---+---+---+---+---+---+---+---+\n", human);
  fputs("    a   b   c   d   e   f   g   h\n\n", human);
  i = 0;
  int j = 0;
  if(actualNumMoves-numMoves>=1)
  {
    while(i<actualNumMoves-numMoves)
    {
      fputc(coords[j], human);
      fputc(coords[++j], human);
      fputc(coords[++j], human);
      fputc(coords[++j], human);
      fputc(coords[++j], human);
      fputc(coords[++j], human);
      j++;
      fputc('\n', human);
      i++;
    }
  }
  else if(isIllegal==1)
  {
    fputs(coords, human);
    fputs("\n", human);
  }
}
char* formatBoard(char** board, char* newBoard)
{
  int i = 0;
  int r = 0;
  int c;
  while(r<8)
  {
    c = 0;
    while(c<8)
    {
      newBoard[i] = board[r][c];
      i++;
      c++;
    }
    r++;
  }
  newBoard[i] = '\0';
}

void reformatBoard(char** board)
{
  int r = 7;
  int r2 = 0;
  int c;
  while(r>=4)
  {
    c = 0;
    while(c<8)
    {
      char temp = board[r2][c];
      board[r2][c] = board[r][c];
      board[r][c] = temp;
      c++;
    }
    r--;
    r2++;
  }
}

int numberOfMoves(FILE* input)
{
  char curr = fgetc(input);
  while(curr!='M')
  {
    curr = fgetc(input);
  }
  int i = 0;
  while(i<6)
  {
    curr = fgetc(input);
    i++;
  }
  scanIgnored(input, 0);
  curr = fgetc(input);
  if(curr==EOF)
  {
    return 0;
  }
  int num = 0;
  while(curr!=EOF)
  {
    if(curr<45)
    {
      num++;
      scanIgnored(input, 0);
    }
    curr = fgetc(input);
  }
  return num;
}
