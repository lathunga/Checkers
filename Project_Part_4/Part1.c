/* Author: Luke Thunga */
#include "Part1.h"

/*
  scans past whitespace
  returns current line number
*/
int scanIgnored(FILE* in, int level)
{
  int currChar = fgetc(in);
  int i;
  int isIgnored = 1;
  while(currChar=='#' || currChar==' ' || currChar=='\n' || currChar==13)
  {
    if(currChar=='\n')
    {
      level++;
    }
    if(currChar=='#')
    {
      while(currChar!='\n' && currChar!=EOF)
      {
        currChar = fgetc(in);
      }
      if(currChar=='\n')
      {
        level++;
      }
    }
    currChar = fgetc(in);
  }
  ungetc(currChar, in);
  return level;
}

/*
  Scans past ignored characters and whitespace when reading the board
  returns current line number
*/
int scanBoardIgnored(FILE* in, int level)
{
  char currChar = fgetc(in);
  int i;
  while(currChar=='#' || currChar==' ' || currChar=='\n' || currChar=='|' || currChar=='-' || currChar=='+' || currChar==13)
  {
    if(currChar=='\n')
    {
      level++;
    }
    if(currChar=='#')
    {
      while(currChar!='\n' && currChar!=EOF)
      {
        currChar = fgetc(in);
      }
      if(currChar=='\n')
      {
        level++;
      }
    }
    currChar = fgetc(in);
  }
  ungetc(currChar, in);
  return level;
}

/*
  checks the validity of a single keyword
  returns the current line number
*/
int oneWord(char word[], int wordLen, FILE* input, int level)
{
  char wordActual[wordLen];
  level = scanIgnored(input, level);
  wordActual[0] = fgetc(input);
  int i;
  for(i = 1; i<wordLen-1; i++)
  {
    wordActual[i] = fgetc(input);
  }
  wordActual[i] = '\0';
  if(strcmp(wordActual, word)!=0)
  {
    fprintf(stderr, "%s%i%s%s%s", "Error near line ", level, ": expecting '", word, "'");
    exit(1);
  }
  return level;
}

/*
  checks the validity of two keywords
  stores the information in an array
  returns the current line number
*/
int twoWords(char word1[], char word2[], int word1Len, int word2Len, char output[], FILE* input, int level)
{
  int returnVal;
  int i;
  level = scanIgnored(input, level);
  char firstChar = fgetc(input);
  int size;
  if(firstChar==word1[0])
  {
    size = word1Len-1;
  }
  else if(firstChar==word2[0])
  {
    size = word2Len-1;
  }
  else
  {
    fprintf(stderr, "%s%i%s%s%s%s%s", "Error near line ", level, ": expecting '", word1, "' or '", word2, "'");
    exit(1);
  }
  output[0] = firstChar;
  for(i = 1; i<size; i++)
  {
    output[i] = fgetc(input);
  }
  output[i] = '\0';
  if(strcmp(output, word1)!=0 && strcmp(output, word2)!=0)
  {
    fprintf(stderr, "%s%i%s%s%s%s", "Error near line ", level, ": expecting ", word1, " or ", word2);
    exit(1);
  }
  return level;
}

/*
  checks the validity of the board
  stores the information regarding the numeber of pieces
  returns the current line number
*/
int checkBoard(FILE* input, int* blackPawns, int* blackKings, int* redPawns, int* redKings, int currLevel)
{
  int first;
  currLevel = scanBoardIgnored(input, currLevel);
  char firstChar = fgetc(input);
  if(firstChar == '.' || firstChar == 'b' || firstChar == 'B')
  {
    first = 0;
  }
  else
  {
    first = 1;
  }
  ungetc(firstChar, input);
  int i;
  char boardChar;
  int squareColor = 0;
  int j = 1;
  i=1;
  while(i<65)
  {
    if(j%9==0)
    {
      if(first==1)
      {
        j=1;
        first=0;
      }
      else
      {
        j=1;
        first=1;
      }
    }
    currLevel = scanBoardIgnored(input, currLevel);
    boardChar = fgetc(input);
    if(i%2==first)
    {
      if(boardChar=='b' || boardChar=='B' || boardChar=='r' || boardChar=='R')
      {
        fprintf(stderr, "%s%i%s", "Error near line ", currLevel, ": piece is on a red square");
        exit(1);
      }
      else if(boardChar!='"')
      {
        if(boardChar=='.')
        {
          fprintf(stderr, "%s%i%s", "Error near line ", currLevel, ": board square color is incorrect h");
          exit(1);
        }
        else if(boardChar==EOF)
        {
          fprintf(stderr, "%s%i%s", "Error near line ", currLevel, ": unexpected end of file");
          exit(1);
        }
        else
        {
          fprintf(stderr, "%s%i%s", "Error near line ", currLevel, ": unexpected character");
          exit(1);
        }
      }
      i++;
    }
    else
    {
      if(boardChar=='b')
      {
        *blackPawns+=1;
      }
      else if(boardChar=='B')
      {
        *blackKings+=1;
      }
      else if(boardChar=='r')
      {
        *redPawns+=1;
      }
      else if(boardChar=='R')
      {
        *redKings+=1;
      }
      else if(boardChar!='.')
      {
        if(boardChar=='"')
        {
          fprintf(stderr, "%s%i%s", "Error near line ", currLevel, ": board square color is incorrect");
          exit(1);
        }
        else if(boardChar==EOF)
        {
          fprintf(stderr, "%s%i%s", "Error near line ", currLevel, ": unexpected end of file");
          exit(1);
        }
        else
        {
          fprintf(stderr, "%s%i%s", "Error near Line ", currLevel, ": unexpected character");
          exit(1);
        }
      }
      i++;
    }
    j++;
  }
  scanBoardIgnored(input, currLevel);
  return currLevel;
}

/*
  checks the validity of the moves
  returns the current line number
*/
int checkMoves(FILE* input, int currLevel)
{
  currLevel = scanIgnored(input, currLevel);
  char currChar = fgetc(input);
  int i=1;
  int count = 1;
  int mult = 0;
  while((currChar>=49 && currChar<=56) || (currChar>=97 && currChar<=104))
  {
    if(i%4==1)
    {
      if(currChar!='a' && currChar!='b' && currChar!='c' && currChar!='d' && currChar!='e' && currChar!='f' && currChar!='g' && currChar!='h')
      {
        fprintf(stderr, "%s%i%s", "Error near line ", currLevel, ": invalid coordinate");
        exit(1);
      }
      i++;
    }
    else if(i%4==2)
    {
      if(currChar!='1' && currChar!='2' && currChar!='3' && currChar!='4' && currChar!='5' && currChar!='6' && currChar!='7' && currChar!='8')
      {
        fprintf(stderr, "%s%i%s", "Error near line ", currLevel, ": invalid coordinate");
        exit(1);
      }
      i++;
    }
    else if(i%4==3)
    {
      if(mult==1 && currChar!='-')
      {
        count = 6;
      }
      else if(currChar!='-')
      {
printf("%c", 'K');
        fprintf(stderr, "%s%i%s", "Error near line ", currLevel, ": invalid coordinate");
        exit(1);
      }
      i++;
    }
    else if(i%4==0)
    {
      if(currChar!='>')
      {
        fprintf(stderr, "%s%i%s", "Error near line ", currLevel, ": invalid coordinate");
        exit(1);
      }
      i=1;
    }
    if(count==6)
    {
      if(currChar=='-')
      {
        i=4;
        currChar = fgetc(input);
        count=0;
        mult=1;
      }
      else
      {
        currLevel = scanIgnored(input, currLevel);
        currChar = fgetc(input);
        i=1;
        count=0;
        mult=0;
      }
    }
    else
    {
      currChar = fgetc(input);
    }
    count++;
  }
  return currLevel;
}
