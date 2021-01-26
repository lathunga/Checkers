#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Part3.h"
#include "Part2.h"

int* findPiece(char** board, int turn, int findPieceCount)
{
  int count = 0;
  int* indexes = malloc(2*sizeof(char));
  int r;
  int c;
  for(c=0; c<8; c++)
  {
    for(r=0; r<8; r++)
    {
      if((board[r][c]=='r' || board[r][c]=='R') && turn==1)
      {
        if(count==findPieceCount)
        {
          indexes[0] = r;
          indexes[1] = c;
          return indexes;
        }
        count++;
      }
      else if((board[r][c]=='b' || board[r][c]=='B') && turn==-1)
      {
        if(count==findPieceCount)
        {
          indexes[0] = r;
          indexes[1] = c;
          return indexes;
        }
        count++;
      }
    }
  }
  indexes[0] = -1;
  return indexes;
}

int isValidCapture(char capturee1, char capturee2, char inbetween, char destination)
{
  if(destination=='.' && (inbetween==capturee1 || inbetween==capturee2))
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

void printQuestionLine(int m, int n, int file, int rank, char* turn, int fileAdj, int rankAdj, int verbose)
{
  if(verbose==1)
  {
    if(m!=n-1)
    {
      int j;
      for(j=m; j>0; j--)
      {
        printf("    ");
      }
      printf("? ");
      printf("%c%d->%c%d for %s:\n", (char)(file)+97, (rank)+1, (char)(file+fileAdj)+97, (rank+rankAdj)+1, turn);
    }
  }
}

void printDotLine(int m, int n, int file, int rank, char* turn, int fileAdj, int rankAdj, int score, int verbose)
{
  if(verbose==1)
  {
    int j;
    for(j=m; j>0; j--)
    {
      printf("    ");
    }
    printf(".");
    printf(" %c%d->%c%d for %s: score %d\n", (char)(file)+97, (rank)+1, (char)(file+fileAdj)+97, (rank+rankAdj)+1, turn, score);
  }
  else
  {
    if(m==0)
    {
      printf("%c%d->%c%d for %s: score %d\n", (char)(file)+97, (rank)+1, (char)(file+fileAdj)+97, (rank+rankAdj)+1, turn, score);
    }
  }
}

int printPossibleMovesRedPawn(char** board, char capturee1, char capturee2, int m, int n, int findPieceCount, int score, int verbose)
{
  int j;
  int* indexes = malloc(2*sizeof(char));
  indexes = findPiece(board, 1, findPieceCount);
  int rank = indexes[0];
  int file = indexes[1];
  if(m==n)
  {
    return score;
  }
  else if(indexes[0]==-1 && findPieceCount==0)
  {
    return 99;
  }
  else if(indexes[0]==-1 && findPieceCount>0)
  {
    return score;
  }
  int moveUpRight = 0;
  int captureUpRight = 0;
  int moveUpLeft = 0;
  int captureUpLeft = 0;
  int moveDownRight = 0;
  int captureDownRight = 0;
  int moveDownLeft = 0;
  int captureDownLeft = 0;
  char up_right_single;
  char up_right_double;
  char up_left_single;
  char up_left_double;
  char down_right_single;
  char down_right_double;
  char down_left_single;
  char down_left_double;
  char** progressedBoard = malloc(8*sizeof(char*));
  int i;
  for(i=0; i<8; i++)
  {
    progressedBoard[i] = malloc(8*sizeof(char));
  }
  copyBoard(progressedBoard, board);
  if(rank+1<8 && file+1<8)
  {
    moveUpRight = 1;
    up_right_single = board[rank+1][file+1];
  }
  if(rank+2<8 && file+2<8)
  {
    captureUpRight = 1;
    up_right_double = board[rank+2][file+2];
  }
  if(rank+1<8 && file-1>-1)
  {
    moveUpLeft = 1;
    up_left_single = board[rank+1][file-1];
  }
  if(rank+2<8 && file-2>0)
  {
    captureUpLeft = 1;
    up_left_double = board[rank+2][file-2];
  }
  if(rank-1>-1 && file+1<8)
  {
    moveDownRight = 1;
    down_right_single = board[rank-1][file+1];
  }
  if(rank-2>-1 && file+2<8)
  {
    captureDownRight = 1;
    down_right_double = board[rank-2][file+2];
  }
  if(rank-1>-1 && file-1>-1)
  {
    moveDownLeft = 1;
    down_left_single = board[rank-1][file-1];
  }
  if(rank-2>-1 && file-2>-1)
  {
    captureDownLeft = 1;
    down_left_double = board[rank-2][file-2];
  }
  int score1;
  int highest = score;
  if(board[rank][file]=='r')
  {
    if(captureUpLeft==1 && isValidCapture(capturee1, capturee2, up_left_single, up_left_double)==1)
    {
      capture_up_left(progressedBoard, rank, file);
      printQuestionLine(m, n, file, rank, "red", -2, 2, verbose);
      if(m==n-1)
      {
        score = calculateScore(progressedBoard, 1);
        if(score>highest)
        {
          highest = score;
        }
        printDotLine(m, n, file, rank, "red", -2, 2, score, verbose);
      }
      else
      {
        score1 = printPossibleMovesBlackPawn(progressedBoard, 'r', 'R', m+1, n, 0, -99, verbose);
        if(score1>score)
        {
          score = score1;
        }
        printDotLine(m, n, file, rank, "red", -2, 2, score1, verbose);
      }
      copyBoard(progressedBoard, board);
    }
    if(moveUpLeft==1 && up_left_single=='.')
    {
      move_up_left(progressedBoard, rank, file);
      printQuestionLine(m, n, file, rank, "red", -1, 1, verbose);
      if(m==n-1)
      {
        score = calculateScore(progressedBoard, 1);
        if(score>highest)
        {
          highest = score;
        }
        printDotLine(m, n, file, rank, "red", -1, 1, score, verbose);
      }
      else
      {
        score1 = printPossibleMovesBlackPawn(progressedBoard, 'r', 'R', m+1, n, 0, -99, verbose);
        if(score1>score)
        {
          score = score1;
        }
        printDotLine(m, n, file, rank, "red", -1, 1, score1, verbose);
      }
      copyBoard(progressedBoard, board);
    }
    if(moveUpRight==1 && up_right_single=='.')
    {
      move_up_right(progressedBoard, rank, file);
      printQuestionLine(m, n, file, rank, "red", 1, 1, verbose);
      if(m==n-1)
      {
        score = calculateScore(progressedBoard, 1);
        if(score>highest)
        {
          highest = score;
        }
        printDotLine(m, n, file, rank, "red", 1, 1, score, verbose);
      }
      else
      {
        score1 = printPossibleMovesBlackPawn(progressedBoard, 'r', 'R', m+1, n, 0, -99, verbose);
        if(score1>score)
        {
          score = score1;
        }
        printDotLine(m, n, file, rank, "red", 1, 1, score1, verbose);
      }
      copyBoard(progressedBoard, board);
    }
    if(captureUpRight==1 && isValidCapture(capturee1, capturee2, up_right_single, up_right_double)==1)
    {
      capture_up_right(progressedBoard, rank, file);
      printQuestionLine(m, n, file, rank, "red", 2, 2, verbose);
      if(m==n-1)
      {
        score = calculateScore(progressedBoard, 1);
        if(score>highest)
        {
          highest = score;
        }
        printDotLine(m, n, file, rank, "red", 2, 2, score, verbose);
      }
      else
      {
        score1 = printPossibleMovesBlackPawn(progressedBoard, 'r', 'R', m+1, n, 0, -99, verbose);
        if(score1>score)
        {
          score = score1;
        }
        printDotLine(m, n, file, rank, "red", 2, 2, score1, verbose);
      }
      copyBoard(progressedBoard, board);
    }
  }
  else
  {
    if(captureDownLeft==1 && isValidCapture(capturee1, capturee2, down_left_single, down_left_double))
    {
      capture_down_left(progressedBoard, rank, file);
      printQuestionLine(m, n, file, rank, "red", -2, -2, verbose);
      if(m==n-1)
      {
        score = calculateScore(progressedBoard, 1);
        if(score>highest)
        {
          highest = score;
        }
        printDotLine(m, n, file, rank, "red", -2, -2, score, verbose);
      }
      else
      {
        score1 = printPossibleMovesBlackPawn(progressedBoard, 'r', 'R', m+1, n, 0, -99, verbose);
        if(score1>score)
        {
          score = score1;
        }
        printDotLine(m, n, file, rank, "red", -2, -2, score1, verbose);
      }
      copyBoard(progressedBoard, board);
    }
    if(captureUpLeft==1 && isValidCapture(capturee1, capturee2, up_left_single, up_left_double))
    {
      capture_up_left(progressedBoard, rank, file);
      printQuestionLine(m, n, file, rank, "red", -2, 2, verbose);
      if(m==n-1)
      {
        score = calculateScore(progressedBoard, 1);
        if(score>highest)
        {
          highest = score;
        }
        printDotLine(m, n, file, rank, "red", -2, 2, score, verbose);
      }
      else
      {
        score1 = printPossibleMovesBlackPawn(progressedBoard, 'r', 'R', m+1, n, 0, -99, verbose);
        if(score1>score)
        {
          score = score1;
        }
        printDotLine(m, n, file, rank, "red", -2, 2, score1, verbose);
      }
      copyBoard(progressedBoard, board);
    }
    if(moveDownLeft==1 && down_left_single=='.')
    {
      move_down_left(progressedBoard, rank, file);
      printQuestionLine(m, n, file, rank, "red", -1, -1, verbose);
      if(m==n-1)
      {
        score = calculateScore(progressedBoard, 1);
        if(score>highest)
        {
          highest = score;
        }
        printDotLine(m, n, file, rank, "red", -1, -1, score, verbose);
      }
      else
      {
        score1 = printPossibleMovesBlackPawn(progressedBoard, 'r', 'R', m+1, n, 0, -99, verbose);
        if(score1>score)
        {
          score = score1;
        }
        printDotLine(m, n, file, rank, "red", -1, -1, score1, verbose);
      }
      copyBoard(progressedBoard, board);
    }
    if(moveUpLeft==1 && up_left_single=='.')
    {
      move_up_left(progressedBoard, rank, file);
      printQuestionLine(m, n, file, rank, "red", -1, 1, verbose);
      if(m==n-1)
      {
        score = calculateScore(progressedBoard, 1);
        if(score>highest)
        {
          highest = score;
        }
        printDotLine(m, n, file, rank, "red", -1, 1, score, verbose);
      }
      else
      {
        score1 = printPossibleMovesBlackPawn(progressedBoard, 'r', 'R', m+1, n, 0, -99, verbose);
        if(score1>score)
        {
          score = score1;
        }
        printDotLine(m, n, file, rank, "red", -1, 1, score1, verbose);
      }
      copyBoard(progressedBoard, board);
    }
    if(moveDownRight==1 && down_right_single=='.')
    {
      move_down_right(progressedBoard, rank, file);
      printQuestionLine(m, n, file, rank, "red", 1, -1, verbose);
      if(m==n-1)
      {
        score = calculateScore(progressedBoard, 1);
        if(score>highest)
        {
          highest = score;
        }
        printDotLine(m, n, file, rank, "red", 1, -1, score, verbose);
      }
      else
      {
        score1 = printPossibleMovesBlackPawn(progressedBoard, 'r', 'R', m+1, n, 0, -99, verbose);
        if(score1>score)
        {
          score = score1;
        }
        printDotLine(m, n, file, rank, "red", 1, -1, score1, verbose);
      }
      copyBoard(progressedBoard, board);
    }
    if(moveUpRight==1 && up_right_single=='.')
    {
      move_up_right(progressedBoard, rank, file);
      printQuestionLine(m, n, file, rank, "red", 1, 1, verbose);
      if(m==n-1)
      {
        score = calculateScore(progressedBoard, 1);
        if(score>highest)
        {
          highest = score;
        }
        printDotLine(m, n, file, rank, "red", 1, 1, score, verbose);
      }
      else
      {
        score1 = printPossibleMovesBlackPawn(progressedBoard, 'r', 'R', m+1, n, 0, -99, verbose);
        if(score1>score)
        {
          score = score1;
        }
        printDotLine(m, n, file, rank, "red", 1, 1, score1, verbose);
      }
      copyBoard(progressedBoard, board);
    }
    if(captureDownRight==1 && isValidCapture(capturee1, capturee2, down_right_single, down_right_double))
    {
      capture_down_right(progressedBoard, rank, file);
      printQuestionLine(m, n, file, rank, "red", 2, -2, verbose);
      if(m==n-1)
      {
        score = calculateScore(progressedBoard, 1);
        if(score>highest)
        {
          highest = score;
        }
        printDotLine(m, n, file, rank, "red", 2, -2, score, verbose);
      }
      else
      {
        score1 = printPossibleMovesBlackPawn(progressedBoard, 'r', 'R', m+1, n, 0, -99, verbose);
        if(score1>score)
        {
          score = score1;
        }
        printDotLine(m, n, file, rank, "red", 2, -2, score1, verbose);
      }
      copyBoard(progressedBoard, board);
    }
    if(captureUpRight==1 && isValidCapture(capturee1, capturee2, up_right_single, up_right_double))
    {
      capture_up_right(progressedBoard, rank, file);
      printQuestionLine(m, n, file, rank, "red", 2, 2, verbose);
      if(m==n-1)
      {
        score = calculateScore(progressedBoard, 1);
        if(score>highest)
        {
          highest = score;
        }
        printDotLine(m, n, file, rank, "red", 2, 2, score, verbose);
      }
      else
      {
        score1 = printPossibleMovesBlackPawn(progressedBoard, 'r', 'R', m+1, n, 0, -99, verbose);
        if(score1>score)
        {
          score = score1;
        }
        printDotLine(m, n, file, rank, "red", 2, 2, score1, verbose);
      }
      copyBoard(progressedBoard, board);
    }
  }
  if(m==n-1)
  {
    score = highest;
  }
  findPieceCount++;
  score = printPossibleMovesRedPawn(progressedBoard, 'b', 'B', m, n, findPieceCount, score, verbose);
  if(findPieceCount>1)
  {
    if(m==n-1)
    {
      score = highest;
    }
    return score;
  }
  return score*-1;
}

int printPossibleMovesBlackPawn(char** board, char capturee1, char capturee2, int m, int n, int findPieceCount, int score, int verbose)
{
  int j;
  int* indexes = malloc(2*sizeof(char));
  indexes = findPiece(board, -1, findPieceCount);
  int rank = indexes[0];
  int file = indexes[1];
  if(m==n)
  {
    return score;
  }
  else if(indexes[0]==-1 && findPieceCount==0)
  {
    return 99;
  }
  else if(indexes[0]==-1 && findPieceCount>0)
  {
    return score;
  }
  int moveUpRight = 0;
  int captureUpRight = 0;
  int moveUpLeft = 0;
  int captureUpLeft = 0;
  int moveDownRight = 0;
  int captureDownRight = 0;
  int moveDownLeft = 0;
  int captureDownLeft = 0;
  char up_right_single;
  char up_right_double;
  char up_left_single;
  char up_left_double;
  char down_right_single;
  char down_right_double;
  char down_left_single;
  char down_left_double;
  char** progressedBoard = malloc(8*sizeof(char*));
  int i;
  for(i=0; i<8; i++)
  {
    progressedBoard[i] = malloc(8*sizeof(char));
  }
  copyBoard(progressedBoard, board);
  if(rank+1<8 && file+1<8)
  {
    moveUpRight = 1;
    up_right_single = board[rank+1][file+1];
  }
  if(rank+2<8 && file+2<8)
  {
    captureUpRight = 1;
    up_right_double = board[rank+2][file+2];
  }
  if(rank+1<8 && file-1>-1)
  {
    moveUpLeft = 1;
    up_left_single = board[rank+1][file-1];
  }
  if(rank+2<8 && file-2>0)
  {
    captureUpLeft = 1;
    up_left_double = board[rank+2][file-2];
  }
  if(rank-1>-1 && file+1<8)
  {
    moveDownRight = 1;
    down_right_single = board[rank-1][file+1];
  }
  if(rank-2>-1 && file+2<8)
  {
    captureDownRight = 1;
    down_right_double = board[rank-2][file+2];
  }
  if(rank-1>-1 && file-1>-1)
  {
    moveDownLeft = 1;
    down_left_single = board[rank-1][file-1];
  }
  if(rank-2>-1 && file-2>-1)
  {
    captureDownLeft = 1;
    down_left_double = board[rank-2][file-2];
  }
  int score1;
  int highest = score;
  if(board[rank][file]=='b')
  {
    if(captureDownLeft==1 && isValidCapture(capturee1, capturee2, down_left_single, down_left_double))
    {
      capture_down_left(progressedBoard, rank, file);
      printQuestionLine(m, n, file, rank, "black", -2, -2, verbose);
      if(m==n-1)
      {
        score = calculateScore(progressedBoard, -1);
        if(score>highest)
        {
          highest = score;
        }
        printDotLine(m, n, file, rank, "black", -2, -2, score, verbose);
      }
      else
      {
        score1 = printPossibleMovesRedPawn(progressedBoard, 'b', 'B', m+1, n, 0, -99, verbose);
        if(score1>score)
        {
          score = score1;
        }
        printDotLine(m, n, file, rank, "black", -2, -2, score1, verbose);
      }
      copyBoard(progressedBoard, board);
    }
    if(moveDownLeft==1 && down_left_single=='.')
    {
      move_down_left(progressedBoard, rank, file);
      printQuestionLine(m, n, file, rank, "black", -1, -1, verbose);
      if(m==n-1)
      {
        score = calculateScore(progressedBoard, -1);
        if(score>highest)
        {
          highest = score;
        }
        printDotLine(m, n, file, rank, "black", -1, -1, score, verbose);
      }
      else
      {
        score1 = printPossibleMovesRedPawn(progressedBoard, 'b', 'B', m+1, n, 0, -99, verbose);
        if(score1>score)
        {
          score = score1;
        }
        printDotLine(m, n, file, rank, "black", -1, -1, score1, verbose);
      }
      copyBoard(progressedBoard, board);
    }
    if(moveDownRight==1 && down_right_single=='.')
    {
      move_down_right(progressedBoard, rank, file);
      printQuestionLine(m, n, file, rank, "black", 1, -1, verbose);
      if(m==n-1)
      {
        score = calculateScore(progressedBoard, -1);
        if(score>highest)
        {
          highest = score;
        }
        printDotLine(m, n, file, rank, "black", 1, -1, score, verbose);
      }
      else
      {
        score1 = printPossibleMovesRedPawn(progressedBoard, 'b', 'B', m+1, n, 0, -99, verbose);
        if(score1>score)
        {
          score = score1;
        }
        printDotLine(m, n, file, rank, "black", 1, -1, score1, verbose);
      }
      copyBoard(progressedBoard, board);
    }
    if(captureDownRight==1 && isValidCapture(capturee1, capturee2, down_right_single, down_right_double))
    {
      capture_down_right(progressedBoard, rank, file);
      printQuestionLine(m, n, file, rank, "black", 2, -2, verbose);
      if(m==n-1)
      {
        score = calculateScore(progressedBoard, -1);
        if(score>highest)
        {
          highest = score;
        }
        printDotLine(m, n, file, rank, "black", 2, -2, score, verbose);
      }
      else
      {
        score1 = printPossibleMovesRedPawn(progressedBoard, 'b', 'B', m+1, n, 0, -99, verbose);
        if(score1>score)
        {
          score = score1;
        }
        printDotLine(m, n, file, rank, "black", 2, -2, score1, verbose);
      }
      copyBoard(progressedBoard, board);
    }
  }
  else
  {
    if(captureDownLeft==1 && isValidCapture(capturee1, capturee2, down_left_single, down_left_double))
    {
      capture_down_left(progressedBoard, rank, file);
      printQuestionLine(m, n, file, rank, "black", -2, -2, verbose);
      if(m==n-1)
      {
        score = calculateScore(progressedBoard, -1);
        if(score>highest)
        {
          highest = score;
        }
        printDotLine(m, n, file, rank, "black", -2, -2, score, verbose);
      }
      else
      {
        score1 = printPossibleMovesRedPawn(progressedBoard, 'b', 'B', m+1, n, 0, -99, verbose);
        if(score1>score)
        {
          score = score1;
        }
        printDotLine(m, n, file, rank, "black", -2, -2, score1, verbose);
      }
      copyBoard(progressedBoard, board);
    }
    if(captureUpLeft==1 && isValidCapture(capturee1, capturee2, up_left_single, up_left_double))
    {
      capture_up_left(progressedBoard, rank, file);
      printQuestionLine(m, n, file, rank, "black", -2, 2, verbose);
      if(m==n-1)
      {
        score = calculateScore(progressedBoard, -1);
        if(score>highest)
        {
          highest = score;
        }
        printDotLine(m, n, file, rank, "black", -2, 2, score, verbose);
      }
      else
      {
        score1 = printPossibleMovesRedPawn(progressedBoard, 'b', 'B', m+1, n, 0, -99, verbose);
        if(score1>score)
        {
          score = score1;
        }
        printDotLine(m, n, file, rank, "black", -2, 2, score1, verbose);
      }
      copyBoard(progressedBoard, board);
    }
    if(moveDownLeft==1 && down_left_single=='.')
    {
      move_down_left(progressedBoard, rank, file);
      printQuestionLine(m, n, file, rank, "black", -1, -1, verbose);
      if(m==n-1)
      {
        score = calculateScore(progressedBoard, -1);
        if(score>highest)
        {
          highest = score;
        }
        printDotLine(m, n, file, rank, "black", -1, -1, score, verbose);
      }
      else
      {
        score1 = printPossibleMovesRedPawn(progressedBoard, 'b', 'B', m+1, n, 0, -99, verbose);
        if(score1>score)
        {
          score = score1;
        }
        printDotLine(m, n, file, rank, "black", -1, -1, score1, verbose);
      }
      copyBoard(progressedBoard, board);
    }
    if(moveUpLeft==1 && up_left_single=='.')
    {
      move_up_left(progressedBoard, rank, file);
      printQuestionLine(m, n, file, rank, "black", -1, 1, verbose);
      if(m==n-1)
      {
        score = calculateScore(progressedBoard, -1);
        if(score>highest)
        {
          highest = score;
        }
        printDotLine(m, n, file, rank, "black", -1, 1, score, verbose);
      }
      else
      {
        score1 = printPossibleMovesRedPawn(progressedBoard, 'b', 'B', m+1, n, 0, -99, verbose);
        if(score1>score)
        {
          score = score1;
          if(score>highest)
          {
            highest = score;
          }
        }
        printDotLine(m, n, file, rank, "black", -1, 1, score1, verbose);
      }
      copyBoard(progressedBoard, board);
    }
    if(moveDownRight==1 && down_right_single=='.')
    {
      move_down_right(progressedBoard, rank, file);
      printQuestionLine(m, n, file, rank, "black", 1, -1, verbose);
      if(m==n-1)
      {
        score = calculateScore(progressedBoard, -1);
        if(score>highest)
        {
          highest = score;
        }
        printDotLine(m, n, file, rank, "black", 1, -1, score, verbose);
      }
      else
      {
        score1 = printPossibleMovesRedPawn(progressedBoard, 'b', 'B', m+1, n, 0, -99, verbose);
        if(score1>score)
        {
          score = score1;
        }
        printDotLine(m, n, file, rank, "black", 1, -1, score1, verbose);
      }
      copyBoard(progressedBoard, board);
    }
    if(moveUpRight==1 && up_right_single=='.')
    {
      move_up_right(progressedBoard, rank, file);
      printQuestionLine(m, n, file, rank, "black", 1, 1, verbose);
      if(m==n-1)
      {
        score = calculateScore(progressedBoard, -1);
        if(score>highest)
        {
          highest = score;
        }
        printDotLine(m, n, file, rank, "black", 1, 1, score, verbose);
      }
      else
      {
        score1 = printPossibleMovesRedPawn(progressedBoard, 'b', 'B', m+1, n, 0, -99, verbose);
        if(score1>score)
        {
          score = score1;
        }
        printDotLine(m, n, file, rank, "black", 1, 1, score1, verbose);
      }
      copyBoard(progressedBoard, board);
    }
    if(captureDownRight==1 && isValidCapture(capturee1, capturee2, down_right_single, down_right_double))
    {
      capture_down_right(progressedBoard, rank, file);
      printQuestionLine(m, n, file, rank, "black", 2, -2, verbose);
      if(m==n-1)
      {
        score = calculateScore(progressedBoard, -1);
        if(score>highest)
        {
          highest = score;
        }
        printDotLine(m, n, file, rank, "black", 2, -2, score, verbose);
      }
      else
      {
        score1 = printPossibleMovesRedPawn(progressedBoard, 'b', 'B', m+1, n, 0, -99, verbose);
        if(score1>score)
        {
          score = score1;
        }
        printDotLine(m, n, file, rank, "black", 2, -2, score1, verbose);
      }
      copyBoard(progressedBoard, board);
    }
    if(captureUpRight==1 && isValidCapture(capturee1, capturee2, up_right_single, up_right_double))
    {
      capture_up_right(progressedBoard, rank, file);
      printQuestionLine(m, n, file, rank, "black", 2, 2, verbose);
      if(m==n-1)
      {
        score = calculateScore(progressedBoard, -1);
        if(score>highest)
        {
          highest = score;
        }
        printDotLine(m, n, file, rank, "black", 2, 2, score, verbose);
      }
      else
      {
        score1 = printPossibleMovesRedPawn(progressedBoard, 'b', 'B', m+1, n, 0, -99, verbose);
        if(score1>score)
        {
          score = score1;
        }
        printDotLine(m, n, file, rank, "black", 2, 2, score1, verbose);
      }
      copyBoard(progressedBoard, board);
    }
  }
  if(m==n-1)
  {
    score = highest;
  }
  findPieceCount++;
  score = printPossibleMovesBlackPawn(progressedBoard, 'r', 'R', m, n, findPieceCount, score, verbose);
  if(findPieceCount>1)
  {
    if(m==n-1)
    {
      score = highest;
    }
    return score;
  }
  return score*-1;
}

void move_up_left(char** board, int rank, int file)
{
  if(board[rank][file]=='r' && rank+1==7)
  {
    board[rank+1][file-1] = 'R';
  }
  else
  {
    board[rank+1][file-1] = board[rank][file];
  }
  board[rank][file]='.';
}

void move_up_right(char** board, int rank, int file)
{
  if(board[rank][file]=='r' && rank+1==7)
  {
    board[rank+1][file+1] = 'R';
  }
  else
  {
    board[rank+1][file+1] = board[rank][file];
  }
  board[rank][file]='.';
}

void move_down_left(char** board, int rank, int file)
{
  if(board[rank][file]=='b' && rank-1==0)
  {
    board[rank-1][file-1] = 'B';
  }
  else
  {
    board[rank-1][file-1] = board[rank][file];
  }
  board[rank][file]='.';
}

void move_down_right(char** board, int rank, int file)
{
  if(board[rank][file]=='b' && rank-1==0)
  {
    board[rank-1][file+1] = 'B';
  }
  else
  {
    board[rank-1][file+1] = board[rank][file];
  }
  board[rank][file]='.';
}

void capture_up_left(char** board, int rank, int file)
{
  if(board[rank][file]=='r' && rank+2==7)
  {
    board[rank+2][file-2] = 'R';
  }
  else
  {
    board[rank+2][file-2] = board[rank][file];
  }
  board[rank+1][file-1] = '.';
  board[rank][file]='.';
}

void capture_up_right(char** board, int rank, int file)
{
  if(board[rank][file]=='r' && rank+2==7)
  {
    board[rank+2][file+2] = 'R';
  }
  else
  {
    board[rank+2][file+2] = board[rank][file];
  }
  board[rank+1][file+1] = '.';
  board[rank][file]='.';
}

void capture_down_left(char** board, int rank, int file)
{
  if(board[rank][file]=='b' && rank-2==0)
  {
    board[rank-2][file-2] = 'B';
  }
  else
  {
    board[rank-2][file-2] = board[rank][file];
  }
  board[rank-1][file-1] = '.';
  board[rank][file]='.';
}

void capture_down_right(char** board, int rank, int file)
{
  if(board[rank][file]=='b' && rank-2==0)
  {
    board[rank-2][file+2] = 'B';
  }
  else
  {
    board[rank-2][file+2] = board[rank][file];
  }
  board[rank-1][file+1] = '.';
  board[rank][file]='.';
}

void copyBoard(char** progressedBoard, char** board)
{
  int r;
  int c;
  for(r=0; r<8; r++)
  {
    for(c=0; c<8; c++)
    {
      progressedBoard[r][c] = board[r][c];
    }
  }
}

int calculateScore(char** board, int turn)
{
  int r;
  int c;
  int redPawns = 0;
  int redKings = 0;
  int blackPawns = 0;
  int blackKings = 0;
  for(r=0; r<8; r++)
  {
    for(c=0; c<8; c++)
    {
      if(board[r][c]=='r')
      {
        redPawns++;
      }
      else if(board[r][c]=='R')
      {
        redKings++;
      }
      else if(board[r][c]=='b')
      {
        blackPawns++;
      }
      else if(board[r][c]=='B')
      {
        blackKings++;
      }
    }
  }
  if(turn==1)
  {
    if(blackPawns==0 && blackKings==0)
    {
      return 99;
    }
    return (redPawns + 2 * redKings) - (blackPawns + 2 * blackKings);
  }
  else
  {
    if(redPawns==0 && redKings==0)
    {
      return 99;
    }
    return (blackPawns + 2 * blackKings) - (redPawns + 2 * redKings);
  }
}
