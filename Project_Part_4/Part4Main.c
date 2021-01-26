#include <stdio.h>
#include <termbox.h>
#include "Part1.h"
#include "Part2.c"
int getInitialMoves(FILE* input, char** moves);
void clearhighlited(int currHighlited, struct tb_cell* cells);
void highlite(int toHighlight, struct tb_cell* cells);
void writeInstructions();
void writeHeaders(int start);
void writeMoves(struct data* state, int numMoves, char** moves, int start, int invalid, int currMoveElement);
void createBoard(char** board);
void movesToChar(int** newMoves, char** moves, int numMoves);
int capture_up_left(char** board, int** newMoves, int curr);
int capture_up_right(char** board, int** newMoves, int curr);
int capture_down_left(char** board, int** newMoves, int curr);
int capture_down_right(char** board, int** newMoves, int curr);
int move_up_left(char** board, int** newMoves, int curr, int invalid, struct data* state, int currTurn);
int move_up_right(char** board, int** newMoves, int curr, int invalid, struct data* state, int currTurn);
int move_down_left(char** board, int** newMoves, int curr, int invalid, struct data* state, int currTurn);
int move_down_right(char** board, int** newMoves, int curr, int invalid, struct data* state, int currTurn);
void setDisplay(struct tb_cell* currentBuffer);
void writeInstructionsDisplay();
int isValidCapture(char** board, int r, int c, struct data* state, int currTurn);
int getNumMoves(FILE* input);
struct node
{
  char** board;
  struct node* next;
  struct node* prev;
};
struct moveNode
{
  char** moves;
  int numMoves;
  struct moveNode* next;
  struct moveNode* prev;
};
void createMovesList(char** moves, int numMoves, struct moveNode* moveList);

int main(int argc, char* argv[])
{
  FILE* temp = fopen(argv[1], "r");
  struct data stateTemp = {0, 0, 0, 0, 0, 0, 0};
  struct data* state = &stateTemp;
  checkValid(temp, state);
  fclose(temp);
  FILE* tempInput = fopen(argv[1], "r");
  scanUntilBoard(tempInput);
  char** tempBoard = getInitialBoard(tempInput);
  scanIgnored(tempInput, 0);
  int numMoves = getNumMoves(tempInput);
  fclose(tempInput);

  FILE* input = fopen(argv[1], "r");
  scanUntilBoard(input);
  char** board = getInitialBoard(input);
  scanIgnored(input, 0);
  char** moves = malloc(numMoves*sizeof(char*));
  if(numMoves!=0)
  {
    getInitialMoves(input, moves);
  }
  fclose(input);
  int** newMoves = malloc(numMoves*sizeof(int*));
  movesToChar(newMoves, moves, numMoves);
  int invalid = -1;
  int start = state->turn;
  struct node* head = malloc(sizeof(struct node));
  head->board = board;
  struct node* curr = head;
  if(numMoves!=0)
  {
  int currTurn = start;
  int i;
  for(i=0; i<numMoves; i++)
  {
    char** newBoard = malloc(8*sizeof(char*));
    int r;
    int c;
    for(r=0; r<8; r++)
    {
      newBoard[r] = malloc(8*sizeof(char));
      for(c=0; c<8; c++)
      {
        newBoard[r][c] = curr->board[r][c];
      }
    }

    //move up
    if(newBoard[newMoves[i][0]][newMoves[i][1]]!='r' && newBoard[newMoves[i][0]][newMoves[i][1]]!='R' && currTurn==1 && invalid==-1)
    {
      invalid = i;
    }
    else if(newBoard[newMoves[i][0]][newMoves[i][1]]!='b' && newBoard[newMoves[i][0]][newMoves[i][1]]!='B' && currTurn==0 && invalid==-1)
    {
      invalid = i;
    }
    else if(moves[i][1]+1==moves[i][3])
    {
      //move right
      if(moves[i][0]+1==moves[i][2])
      {
        invalid = move_up_right(newBoard, newMoves, i, invalid, state, currTurn);
      }
      //move left
      else if(moves[i][0]-1==moves[i][2])
      {
        invalid = move_up_left(newBoard, newMoves, i, invalid, state, currTurn);
      }
    }
    //move down
    else if(moves[i][1]-1==moves[i][3])
    {
      //move right
      if(moves[i][0]+1==moves[i][2])
      {
        invalid = move_down_right(newBoard, newMoves, i, invalid, state, currTurn);
      }
      //move left
      else if(moves[i][0]-1==moves[i][2])
      {
        invalid = move_down_left(newBoard, newMoves, i, invalid, state, currTurn);
      }
    }
    //capture up
    else if(moves[i][1]+2==moves[i][3])
    {
      //move right
      if(moves[i][0]+2==moves[i][2])
      {
        if(capture_up_right(newBoard, newMoves, i)==-1 && invalid==-1)
        {
          invalid = i;
        }
      }
      //move left
      else if(moves[i][0]-2==moves[i][2])
      {
        if(capture_up_left(newBoard, newMoves, i)==-1 && invalid==-1)
        {
          invalid = i;
        }
      }
    }
    //capture down
    else if(moves[i][1]-2==moves[i][3])
    {
      //move right
      if(moves[i][0]+2==moves[i][2])
      {
        if(capture_down_right(newBoard, newMoves, i)==-1 && invalid==-1)
        {
          invalid = i;
        }
      }
      //move left
      else if(moves[i][0]-2==moves[i][2])
      {
        if(capture_down_left(newBoard, newMoves, i)==-1 && invalid==-1)
        {
          invalid = i;
        }
      }
    }
    else if(invalid==-1)
    {
      invalid = i;
    }

    struct node* newNode = malloc(sizeof(struct node));
    struct node* temp = curr;
    newNode->board = newBoard;
    curr->next = newNode;
    curr = curr->next;
    curr->prev = temp;
    if(currTurn==0)
    {
      currTurn=1;
    }
    else
    {
      currTurn=0;
    }
  }
}
  struct moveNode* moveHead = malloc(sizeof(struct moveNode));
  struct moveNode* moveList = moveHead;
tb_init();
if(numMoves!=0)
{
  // createMovesList(moves, numMoves, currMove);
  int i;
  int numElem = numMoves/((tb_height()-2)*2);
  if(numMoves%((tb_height()-2)*2)!=0)
  {
    numElem++;
  }

  int j;

  int count = 0;
  for(j=0; j<numElem; j++)
  {
    char** newMoves = malloc(numMoves*sizeof(char*));
    for(i=0; i<(tb_height()-2)*2 && count<numMoves; i++)
    {
      newMoves[i] = malloc(4*sizeof(char));
      newMoves[i][0] = moves[count][0];
      newMoves[i][1] = moves[count][1];
      newMoves[i][2] = moves[count][2];
      newMoves[i][3] = moves[count][3];
      count++;
    }
    if(j==0)
    {
      moveList->moves = newMoves;
      moveList->numMoves = i;
      moveList->prev = NULL;
      moveList->next = NULL;
    }
    else
    {
      struct moveNode* newNode = malloc(sizeof(struct moveNode));
      moveList->next = newNode;
      newNode->moves = newMoves;
      newNode->numMoves = i;
      moveList->next->prev = moveList;
      moveList = newNode;
      moveList->next = NULL;
    }
  }
}
  writeInstructions();
  writeHeaders(start);
  if(numMoves!=0)
  {
    writeMoves(state, moveHead->numMoves, moveHead->moves, start, invalid, 1);
  }
  createBoard(board);
  tb_present();















  //tb_set_cursor(0, 2);
  struct tb_cell* cells = tb_cell_buffer();
  int currHighlited = 2*tb_width();
  highlite(2*tb_width(), cells);
  tb_present();
  int x = 0;
  int y = 2;
  int counter = 0;
  int currPage = 1;
  struct tb_event ev;
	while (tb_poll_event(&ev)) {
		switch (ev.type) {
		case TB_EVENT_KEY:
    if(ev.key==TB_KEY_ARROW_DOWN && counter+2<=numMoves)
    {
      if(y==tb_height()-1 && moveHead->next!=NULL)
      {
        counter+=2;
        tb_clear();
        writeInstructions();
        writeHeaders(start);
        currPage++;
        writeMoves(state, moveHead->next->numMoves, moveHead->next->moves, start, invalid, currPage);
        createBoard(head->next->next->board);
        head = head->next->next;
        moveHead = moveHead->next;
        if(x!=0)
        {
          x=tb_width()/4;
          y=2;
          //tb_set_cursor(x, y);
          struct tb_cell* currCells = tb_cell_buffer();
          currHighlited = tb_width()*2+tb_width()/4;
          highlite(currHighlited, currCells);
        }
        else
        {
          x=0;
          y=2;
          //tb_set_cursor(x, y);
          struct tb_cell* currCells = tb_cell_buffer();
          currHighlited = tb_width()*2;
          highlite(currHighlited, currCells);
        }
      }
      else if(y<numMoves/2+1+1)
      {
        counter+=2;
        tb_clear();
        writeInstructions();
        writeHeaders(start);
        createBoard(head->next->next->board);
        head = head->next->next;
        struct tb_cell* currCells = tb_cell_buffer();
        clearhighlited(currHighlited, currCells);
        y++;
        //tb_set_cursor(x, ++y);
        currHighlited = currHighlited+tb_width();
        writeMoves(state, moveHead->numMoves, moveHead->moves, start, invalid, currPage);
        highlite(currHighlited, currCells);
      }
      tb_present();
    }
    else if(ev.key==TB_KEY_ARROW_UP)
    {
      if(y>2)
      {
        counter-=2;
        tb_clear();
        writeInstructions();
        writeHeaders(start);
        writeMoves(state, moveHead->numMoves, moveHead->moves, start, invalid, currPage);
        createBoard(head->prev->prev->board);
        head = head->prev->prev;
        struct tb_cell* currCells = tb_cell_buffer();
        clearhighlited(currHighlited, currCells);
        y--;
        //tb_set_cursor(x, --y);
        currHighlited = currHighlited-tb_width();
        highlite(currHighlited, currCells);
      }
      else if(moveHead->prev!=NULL)
      {
        counter-=2;
        tb_clear();
        writeInstructions();
        writeHeaders(start);
        currPage--;
        writeMoves(state, moveHead->prev->numMoves, moveHead->prev->moves, start, invalid, currPage);
        moveHead = moveHead->prev;
        createBoard(head->prev->prev->board);
        head = head->prev->prev;
        if(x==0)
        {
          x=0;
          y=tb_height()-1;
          //tb_set_cursor(x, y);
          struct tb_cell* currCells = tb_cell_buffer();
          currHighlited = tb_height()*tb_width()-tb_width();
          highlite(currHighlited, currCells);
        }
        else
        {
          x=tb_width()/4;
          y=tb_height()-1;
          //tb_set_cursor(x, y);
          struct tb_cell* currCells = tb_cell_buffer();
          currHighlited = tb_height()*tb_width()-(tb_width()/2)-(tb_width()/4);
          highlite(currHighlited, currCells);
        }
      }
      tb_present();
    }
    else if(ev.key==TB_KEY_ARROW_LEFT)
    {
      if(x!=0)
      {
        counter--;
        tb_clear();
        writeInstructions();
        writeHeaders(start);
        writeMoves(state, moveHead->numMoves, moveHead->moves, start, invalid, currPage);
        createBoard(head->prev->board);
        head = head->prev;
        struct tb_cell* currCells = tb_cell_buffer();
        clearhighlited(currHighlited, currCells);
        x=0;
        //tb_set_cursor(0, y);
        currHighlited = currHighlited-tb_width()/4;
        highlite(currHighlited, currCells);
      }
      else if(y!=2)
      {
        counter--;
        tb_clear();
        writeInstructions();
        writeHeaders(start);
        writeMoves(state, moveHead->numMoves, moveHead->moves, start, invalid, currPage);
        createBoard(head->prev->board);
        head = head->prev;
        struct tb_cell* currCells = tb_cell_buffer();
        clearhighlited(currHighlited, currCells);
        x=tb_width()/4;
        y--;
        //tb_set_cursor(x, --y);
        currHighlited = currHighlited-tb_width()+tb_width()/4;
        highlite(currHighlited, currCells);
      }
      else if(moveHead->prev!=NULL)
      {
        counter--;
        tb_clear();
        writeInstructions();
        writeHeaders(start);
        currPage--;
        writeMoves(state, moveHead->prev->numMoves, moveHead->prev->moves, start, invalid, currPage);
        moveHead = moveHead->prev;
        createBoard(head->prev->board);
        head = head->prev;
        x=tb_width()/4;
        y=tb_height()-1;
        struct tb_cell* currCells = tb_cell_buffer();
        currHighlited = tb_height()*tb_width()-(tb_width()/2)-(tb_width()/4);
        highlite(currHighlited, currCells);
      }
      tb_present();
    }
    else if(ev.key==TB_KEY_ARROW_RIGHT && counter+1<=numMoves)
    {
      struct tb_cell* testBuff = tb_cell_buffer();
      testBuff+=currHighlited;
      if(x!=tb_width()/4 && testBuff->ch!=0x20)
      {
        counter++;
        tb_clear();
        writeInstructions();
        writeHeaders(start);
        writeMoves(state, moveHead->numMoves, moveHead->moves, start, invalid, currPage);
        createBoard(head->next->board);
        head = head->next;
        struct tb_cell* currCells = tb_cell_buffer();
        clearhighlited(currHighlited, currCells);
        x=tb_width()/4;
        currHighlited = currHighlited+tb_width()/4;
        highlite(currHighlited, currCells);
      }
      else if(x==tb_width()/4 && testBuff->ch!=0x20)
      {
        counter++;
        tb_clear();
        writeInstructions();
        writeHeaders(start);
        createBoard(head->next->board);
        head = head->next;
        struct tb_cell* currCells = tb_cell_buffer();
        clearhighlited(currHighlited, currCells);
        x=0;
        y++;
        if(y==tb_height() && moveHead->next!=NULL)
        {
          moveHead = moveHead->next;
          currPage++;
          x=0;
          y=2;
          currHighlited = tb_width()*2;
        }
        else
        {
          currHighlited = currHighlited+tb_width()-tb_width()/4;
        }
        writeMoves(state, moveHead->numMoves, moveHead->moves, start, invalid, currPage);
        highlite(currHighlited, currCells);
      }
      tb_present();
    }
    else if(ev.key==TB_KEY_ESC)
    {
      tb_shutdown();
      exit(1);
    }
    else if(ev.key==TB_KEY_F2)
    {
      tb_clear();
      writeInstructionsDisplay();
      tb_present();
    }
    else if(ev.key==TB_KEY_F3)
    {
      tb_clear();
      writeInstructions();
      writeHeaders(start);
      writeMoves(state, moveHead->numMoves, moveHead->moves, start, invalid, currPage);
      createBoard(head->board);
      struct tb_cell* currCells = tb_cell_buffer();
      highlite(currHighlited, currCells);
      tb_present();
    }
    else if(ev.key==TB_KEY_INSERT)
    {
      if(counter==numMoves)
      {
        numMoves++;
        moves = realloc(moves, numMoves*sizeof(char*));
        moves[numMoves-1] = malloc(4*sizeof(char));
      }
      tb_set_cursor(x,y);
      tb_present();
      tb_poll_event(&ev);
      if(ev.type==TB_EVENT_KEY)
      {
        moves[counter][0] = ev.ch;
      	tb_change_cell(x, y, ev.ch, TB_WHITE, TB_DEFAULT);
        tb_set_cursor(x+1,y);
        tb_present();
      }
      tb_poll_event(&ev);
      if(ev.type==TB_EVENT_KEY)
      {
        moves[counter][1] = ev.ch;
      	tb_change_cell(x+1, y, ev.ch, TB_WHITE, TB_DEFAULT);
        tb_set_cursor(x+4,y);
        tb_present();
      }
      tb_poll_event(&ev);
      if(ev.type==TB_EVENT_KEY)
      {
        moves[counter][2] = ev.ch;
      	tb_change_cell(x+4, y, ev.ch, TB_WHITE, TB_DEFAULT);
        tb_set_cursor(x+5,y);
        tb_present();
      }
      tb_poll_event(&ev);
      if(ev.type==TB_EVENT_KEY)
      {
        moves[counter][3] = ev.ch;
      	tb_change_cell(x+5, y, ev.ch, TB_WHITE, TB_DEFAULT);
        tb_set_cursor(x+5,y);
        tb_present();
      }

      head->board = board;
      int currTurn = start;
      curr = head;
      int i;
      newMoves[numMoves-1] = malloc(4*sizeof(int));
      invalid = -1;
      movesToChar(newMoves, moves, numMoves);
      for(i=0; i<numMoves; i++)
      {
        char** newBoard = malloc(8*sizeof(char*));
        int r;
        int c;
        for(r=0; r<8; r++)
        {
          newBoard[r] = malloc(8*sizeof(char));
          for(c=0; c<8; c++)
          {
            newBoard[r][c] = curr->board[r][c];
          }
        }

        //move up
        if(newBoard[newMoves[i][0]][newMoves[i][1]]!='r' && newBoard[newMoves[i][0]][newMoves[i][1]]!='R' && currTurn==1 && invalid==-1)
        {
          invalid = i;
        }
        else if(newBoard[newMoves[i][0]][newMoves[i][1]]!='b' && newBoard[newMoves[i][0]][newMoves[i][1]]!='B' && currTurn==0 && invalid==-1)
        {
          invalid = i;
        }
        else if(moves[i][1]+1==moves[i][3])
        {
          //move right
          if(moves[i][0]+1==moves[i][2])
          {
            invalid = move_up_right(newBoard, newMoves, i, invalid, state, currTurn);
          }
          //move left
          else if(moves[i][0]-1==moves[i][2])
          {
            invalid = move_up_left(newBoard, newMoves, i, invalid, state, currTurn);
          }
        }
        //move down
        else if(moves[i][1]-1==moves[i][3])
        {
          //move right
          if(moves[i][0]+1==moves[i][2])
          {
            invalid = move_down_right(newBoard, newMoves, i, invalid, state, currTurn);
          }
          //move left
          else if(moves[i][0]-1==moves[i][2])
          {
            invalid = move_down_left(newBoard, newMoves, i, invalid, state, currTurn);
          }
        }
        //capture up
        else if(moves[i][1]+2==moves[i][3])
        {
          //move right
          if(moves[i][0]+2==moves[i][2])
          {
            if(capture_up_right(newBoard, newMoves, i)==-1 && invalid==-1)
            {
              invalid = i;
            }
          }
          //move left
          else if(moves[i][0]-2==moves[i][2])
          {
            if(capture_up_left(newBoard, newMoves, i)==-1 && invalid==-1)
            {
              invalid = i;
            }
          }
        }
        //capture down
        else if(moves[i][1]-2==moves[i][3])
        {
          //move right
          if(moves[i][0]+2==moves[i][2])
          {
            if(capture_down_right(newBoard, newMoves, i)==-1 && invalid==-1)
            {
              invalid = i;
            }
          }
          //move left
          else if(moves[i][0]-2==moves[i][2])
          {
            if(capture_down_left(newBoard, newMoves, i)==-1 && invalid==-1)
            {
              invalid = i;
            }
          }
        }
        else if(invalid==-1)
        {
          invalid = i;
        }

        struct node* newNode = malloc(sizeof(struct node));
        struct node* temp = curr;
        newNode->board = newBoard;
        curr->next = newNode;
        curr = curr->next;
        curr->prev = temp;
        if(currTurn==0)
        {
          currTurn=1;
        }
        else
        {
          currTurn=0;
        }
      }

      moveHead = malloc(sizeof(struct moveNode));
      moveList = moveHead;

      // createMovesList(moves, numMoves, currMove);
      int numElem = numMoves/((tb_height()-2)*2);
      if(numMoves%((tb_height()-2)*2)!=0)
      {
        numElem++;
      }

      int j;

      int count = 0;
      for(j=0; j<numElem; j++)
      {
        char** newMoves = malloc(numMoves*sizeof(char*));
        for(i=0; i<(tb_height()-2)*2 && count<numMoves; i++)
        {
          newMoves[i] = malloc(4*sizeof(char));
          newMoves[i][0] = moves[count][0];
          newMoves[i][1] = moves[count][1];
          newMoves[i][2] = moves[count][2];
          newMoves[i][3] = moves[count][3];
          count++;
        }
        if(j==0)
        {
          moveList->moves = newMoves;
          moveList->numMoves = i;
          moveList->prev = NULL;
          moveList->next = NULL;
        }
        else
        {
          struct moveNode* newNode = malloc(sizeof(struct moveNode));
          moveList->next = newNode;
          newNode->moves = newMoves;
          newNode->numMoves = i;
          moveList->next->prev = moveList;
          moveList = newNode;
          moveList->next = NULL;
        }
      }


      tb_clear();
      writeInstructions();
      writeHeaders(start);
      int q;
      // for(q=0; q<count; q++)
      // {
      //   head = head->ne
      // }
      currPage = 1;
      writeMoves(state, moveHead->numMoves, moveHead->moves, start, invalid, 1);
      createBoard(head->board);
      struct tb_cell* currCells = tb_cell_buffer();
      currHighlited=tb_width()*2;
      highlite(currHighlited, currCells);
      tb_set_cursor(TB_HIDE_CURSOR, TB_HIDE_CURSOR);
      x = 0;
      y = 2;
      counter = 0;
      tb_present();
    }
    default:
			break;
    }
  }
}

int getNumMoves(FILE* input)
{
  int i;
  for(i=0; i<6; i++)
  {
    fgetc(input);
  }
  scanIgnored(input, 0);
  int currChar = fgetc(input);
  int numMoves = 0;
  int currCol = 0;
  while(currChar!=EOF)
  {
    if(currChar=='-')
    {
      currChar = fgetc(input);
    }
    else if(currChar=='>')
    {
      currChar = fgetc(input);
    }
    else if((currChar>=97 && currChar<=104) || (currChar>=49 && currChar<=56))
    {
        currChar = fgetc(input);
        currCol++;
    }
    else
    {
      numMoves++;
      scanIgnored(input, 0);
      currCol = 0;
      currChar = fgetc(input);
    }
  }
  return numMoves;
}

int getInitialMoves(FILE* input, char** moves)
{
  int i;
  for(i=0; i<6; i++)
  {
    fgetc(input);
  }
  scanIgnored(input, 0);
  int currChar = fgetc(input);
  int numMoves = 0;
  int currCol = 0;
  while(currChar!=EOF)
  {
    if(currChar=='-')
    {
      currChar = fgetc(input);
    }
    else if(currChar=='>')
    {
      currChar = fgetc(input);
    }
    else if((currChar>=97 && currChar<=104) || (currChar>=49 && currChar<=56))
    {
        if(currCol==0)
        {
          moves[numMoves] = malloc(4*sizeof(char));
        }
        moves[numMoves][currCol] = currChar;
        currChar = fgetc(input);
        currCol++;
    }
    else
    {
      numMoves++;
      scanIgnored(input, 0);
      currCol = 0;
      currChar = fgetc(input);
    }
  }
  return numMoves;
}

void clearhighlited(int currHighlited, struct tb_cell* cells)
{
  int i;
  cells+=currHighlited;
  for(i=0; i<6; i++)
  {
    cells->bg = TB_DEFAULT;
    cells++;
  }
}

void highlite(int toHighlight, struct tb_cell* cells)
{
  int i;
  cells+=toHighlight;
  for(i=0; i<6; i++)
  {
    cells->bg = TB_MAGENTA;
    cells++;
  }
}

void writeInstructions()
{
  tb_change_cell(0, 0, 	0x70, TB_WHITE, TB_DEFAULT); //p
  tb_change_cell(1, 0, 0x72	, TB_WHITE, TB_DEFAULT); //r
  tb_change_cell(2, 0, 0x65	, TB_WHITE, TB_DEFAULT); //e
  tb_change_cell(3, 0, 0x73, TB_WHITE, TB_DEFAULT); //s
  tb_change_cell(4, 0, 0x73, TB_WHITE, TB_DEFAULT); //s
  tb_change_cell(5, 0, 	0x20, TB_WHITE, TB_DEFAULT); //space
  tb_change_cell(6, 0, 		0x66, TB_WHITE, TB_DEFAULT); //f
  tb_change_cell(7, 0, 	0x32, TB_WHITE, TB_DEFAULT); //"1
  tb_change_cell(8, 0, 0x20	, TB_WHITE, TB_DEFAULT); //space
  tb_change_cell(9, 0, 	0x66, TB_WHITE, TB_DEFAULT); //f
  tb_change_cell(10, 0, 0x6F, TB_WHITE, TB_DEFAULT); //o
  tb_change_cell(11, 0, 0x72, TB_WHITE, TB_DEFAULT); //r
  tb_change_cell(12, 0, 0x20, TB_WHITE, TB_DEFAULT); //space
  tb_change_cell(13, 0, 	0x68, TB_WHITE, TB_DEFAULT); //h
  tb_change_cell(14, 0, 0x6F, TB_WHITE, TB_DEFAULT); //o
  tb_change_cell(15, 0, 0x74, TB_WHITE, TB_DEFAULT); //t
  tb_change_cell(16, 0, 0x6B, TB_WHITE, TB_DEFAULT); //k
  tb_change_cell(17, 0, 0x65, TB_WHITE, TB_DEFAULT); //e
  tb_change_cell(18, 0, 0x79, TB_WHITE, TB_DEFAULT); //y
  tb_change_cell(19, 0, 0x20, TB_WHITE, TB_DEFAULT); //space
  tb_change_cell(20, 0, 0x64, TB_WHITE, TB_DEFAULT); //d
  tb_change_cell(21, 0, 0x69, TB_WHITE, TB_DEFAULT); //i
  tb_change_cell(22, 0, 0x73, TB_WHITE, TB_DEFAULT); //s
  tb_change_cell(23, 0, 0x70, TB_WHITE, TB_DEFAULT); //p
  tb_change_cell(24, 0, 0x6C	, TB_WHITE, TB_DEFAULT); //l
  tb_change_cell(25, 0, 0x61, TB_WHITE, TB_DEFAULT); //a
  tb_change_cell(26, 0, 0x79, TB_WHITE, TB_DEFAULT); //y
}

void writeHeaders(int start)
{
  if(start==1)
  {
    tb_change_cell(0, 1, 0x0052, TB_WHITE, TB_RED);
    tb_change_cell(1, 1, 0x0045, TB_WHITE, TB_RED);
    tb_change_cell(2, 1, 0x0044, TB_WHITE, TB_RED);

    tb_change_cell(tb_width()/4, 1, 0x0042, TB_WHITE, TB_BLACK);
    tb_change_cell(tb_width()/4+1, 1, 0x004C, TB_WHITE, TB_BLACK);
    tb_change_cell(tb_width()/4+2, 1, 0x0041, TB_WHITE, TB_BLACK);
    tb_change_cell(tb_width()/4+3, 1, 0x0043, TB_WHITE, TB_BLACK);
    tb_change_cell(tb_width()/4+4, 1, 0x004B, TB_WHITE, TB_BLACK);
  }
  else
  {
    tb_change_cell(tb_width()/4, 1, 0x0052, TB_WHITE, TB_RED);
    tb_change_cell(tb_width()/4+1, 1, 0x0045, TB_WHITE, TB_RED);
    tb_change_cell(tb_width()/4+2, 1, 0x0044, TB_WHITE, TB_RED);

    tb_change_cell(0, 1, 0x0042, TB_WHITE, TB_BLACK);
    tb_change_cell(1, 1, 0x004C, TB_WHITE, TB_BLACK);
    tb_change_cell(2, 1, 0x0041, TB_WHITE, TB_BLACK);
    tb_change_cell(3, 1, 0x0043, TB_WHITE, TB_BLACK);
    tb_change_cell(4, 1, 0x004B, TB_WHITE, TB_BLACK);
  }
}

void writeMoves(struct data* state, int numMoves, char** moves, int start, int invalid, int currMoveElement)
{
  uint16_t currColor;
  int i;
  int movesOnPage = (tb_height()-2)*2;
  int realMovesIndex = 0;
  if(currMoveElement!=1)
  {
    for(i=0; i<currMoveElement-1; i++)
    {
      realMovesIndex += movesOnPage;
    }
  }

  if(start==1)
  {
    for(i=0, realMovesIndex; i<numMoves; i++, realMovesIndex++)
    {
      if(state->turn==1)
      {
        if(realMovesIndex==invalid)
        {
          currColor = TB_RED;
        }
        else if(realMovesIndex>invalid && invalid!=-1)
        {
          currColor = TB_WHITE;
        }
        else
        {
          currColor = TB_GREEN;
        }
        int j;
        for(j=0; j<4; j++)
        {
          if(j==2)
          {
            tb_change_cell(j, 2+i/2, 0x002D, currColor, TB_DEFAULT);
            tb_change_cell(j+1, 2+i/2, 0x003E, currColor, TB_DEFAULT);
          }
          struct tb_cell cell;
          cell.fg = currColor;
          cell.bg = TB_DEFAULT;
          char* str = malloc(2*sizeof(char));
          str[0] = moves[i][j];
          str[1] = '\0';
          tb_utf8_char_to_unicode(&cell.ch, str);
          if(j<2)
          {
            tb_put_cell(j, 2+i/2, &cell);
          }
          else
          {
            tb_put_cell(j+2, 2+i/2, &cell);
          }
        }
        state->turn=0;
      }
      else
      {
        if(realMovesIndex==invalid)
        {
          currColor = TB_RED;
        }
        else if(realMovesIndex>invalid && invalid!=-1)
        {
          currColor = TB_WHITE;
        }
        else
        {
          currColor = TB_GREEN;
        }
        int j;
        for(j=0; j<4; j++)
        {
          if(j==2)
          {
            tb_change_cell(j+tb_width()/4, 2+i/2, 0x002D, currColor, TB_DEFAULT);
            tb_change_cell(j+1+tb_width()/4, 2+i/2, 0x003E, currColor, TB_DEFAULT);
          }
          struct tb_cell cell;
          cell.fg = currColor;
          cell.bg = TB_DEFAULT;
          char* str = malloc(2*sizeof(char));
          str[0] = moves[i][j];
          str[1] = '\0';
          tb_utf8_char_to_unicode(&cell.ch, str);
          if(j<2)
          {
            tb_put_cell(j+tb_width()/4, 2+i/2, &cell);
          }
          else
          {
            tb_put_cell(j+2+tb_width()/4, 2+i/2, &cell);
          }
        }
        state->turn=1;
      }
    }
  }
  else
  {
    for(i=0, realMovesIndex; i<numMoves; i++, realMovesIndex++)
    {
      if(realMovesIndex==invalid)
      {
        currColor = TB_RED;
      }
      else if(realMovesIndex>invalid && invalid!=-1)
      {
        currColor = TB_WHITE;
      }
      else
      {
        currColor = TB_GREEN;
      }
      if(state->turn==0)
      {
        int j;
        for(j=0; j<4; j++)
        {
          if(j==2)
          {
            tb_change_cell(j, 2+i/2, 0x002D, currColor, TB_DEFAULT);
            tb_change_cell(j+1, 2+i/2, 0x003E, currColor, TB_DEFAULT);
          }
          struct tb_cell cell;
          cell.fg = currColor;
          cell.bg = TB_DEFAULT;
          char* str = malloc(2*sizeof(char));
          str[0] = moves[i][j];
          str[1] = '\0';
          tb_utf8_char_to_unicode(&cell.ch, str);
          if(j<2)
          {
            tb_put_cell(j, 2+i/2, &cell);
          }
          else
          {
            tb_put_cell(j+2, 2+i/2, &cell);
          }
        }
        state->turn=1;
      }
      else
      {
        if(realMovesIndex==invalid)
        {
          currColor = TB_RED;
        }
        else if(realMovesIndex>invalid && invalid!=-1)
        {
          currColor = TB_WHITE;
        }
        else
        {
          currColor = TB_GREEN;
        }
        int j;
        for(j=0; j<4; j++)
        {
          if(j==2)
          {
            tb_change_cell(j+tb_width()/4, 2+i/2, 0x002D, currColor, TB_DEFAULT);
            tb_change_cell(j+1+tb_width()/4, 2+i/2, 0x003E, currColor, TB_DEFAULT);
          }
          struct tb_cell cell;
          cell.fg = currColor;
          cell.bg = TB_DEFAULT;
          char* str = malloc(2*sizeof(char));
          str[0] = moves[i][j];
          str[1] = '\0';
          tb_utf8_char_to_unicode(&cell.ch, str);
          if(j<2)
          {
            tb_put_cell(j+tb_width()/4, 2+i/2, &cell);
          }
          else
          {
            tb_put_cell(j+2+tb_width()/4, 2+i/2, &cell);
          }
        }
        state->turn=0;
      }
    }
  }
  if(numMoves%2==1)
  {
    if(state->turn==0)
    {
      state->turn = 1;
    }
    else
    {
      state->turn = 0;
    }
  }
}

void createBoard(char** board)
{
  struct tb_cell* buff = tb_cell_buffer();
  uint16_t currColor = TB_RED;
  buff+=tb_width()/2;
  int c;
  int row = 7;
  int col = 0;
  int i;
  for(c=0; c<8; c++)
  {
    int r;
    for(r=0; r<tb_height()/8; r++)
    {
      int j;
      for(j=0; j<8; j++)
      {
        for(i=tb_width()/2; i<tb_width()/2+tb_width()/16; i++)
        {
          if(board[row][col]=='r')
          {
            buff->ch = 0x72;
            buff->fg = TB_BLACK;
          }
          if(board[row][col]=='b')
          {
            buff->ch = 0x62;
            buff->fg = TB_BLACK;
          }
          if(board[row][col]=='R')
          {
            buff->ch = 	0x52;
            buff->fg = TB_BLACK;
          }
          if(board[row][col]=='B')
          {
            buff->ch = 0x42;
            buff->fg = TB_BLACK;
          }
          struct tb_cell cell;
          buff->bg = currColor;
          buff++;
        }
        if(currColor==TB_RED)
        {
          col++;
          currColor=TB_WHITE;
        }
        else
        {
          col++;
          currColor=TB_RED;
        }
      }
      col=0;
      if(tb_width()%2==1)
      {
        buff+=(tb_width()/2)+((tb_width()/2)%8)+1;
      }
      else
      {
        buff+=(tb_width()/2)+((tb_width()/2)%8);
      }
    }
    row--;
    if(currColor==TB_RED)
    {
      currColor=TB_WHITE;
    }
    else
    {
      currColor=TB_RED;
    }
  }
}

void movesToChar(int** newMoves, char** moves, int numMoves)
{
  int r;
  int c;
  for(r=0; r<numMoves; r++)
  {
    newMoves[r] = malloc(4*sizeof(int));
    for(c=0; c<4; c++)
    {
      if((c+1)%2==1)
      {
        newMoves[r][c] = (int) moves[r][c]-97;
      }
      else
      {
        newMoves[r][c] = (int) moves[r][c]-49;
      }
    }
    int temp = newMoves[r][0];
    newMoves[r][0] = newMoves[r][1];
    newMoves[r][1] = temp;
    temp = newMoves[r][2];
    newMoves[r][2] = newMoves[r][3];
    newMoves[r][3] = temp;
  }
}

int capture_up_left(char** board, int** newMoves, int curr)
{
  if((board[newMoves[curr][0]][newMoves[curr][1]]=='r' || board[newMoves[curr][0]][newMoves[curr][1]] == 'R') && ((board[newMoves[curr][0]+1][newMoves[curr][1]-1]!='b' && board[newMoves[curr][0]+1][newMoves[curr][1]-1]!='B') || (board[newMoves[curr][0]+2][newMoves[curr][1]-2]!='.')))
  {
    return -1;
  }
  if((board[newMoves[curr][0]][newMoves[curr][1]]=='b' || board[newMoves[curr][0]][newMoves[curr][1]] == 'B') && ((board[newMoves[curr][0]+1][newMoves[curr][1]-1]!='r' && board[newMoves[curr][0]+1][newMoves[curr][1]-1]!='R') || (board[newMoves[curr][0]+2][newMoves[curr][1]-2]!='.')))
  {
    return -1;
  }
  if(newMoves[curr][0]+2==7 && board[newMoves[curr][0]][newMoves[curr][1]]=='r')
  {
    board[newMoves[curr][0]][newMoves[curr][1]]=='R';
  }
  board[newMoves[curr][0]+2][newMoves[curr][1]-2] = board[newMoves[curr][0]][newMoves[curr][1]];
  board[newMoves[curr][0]+1][newMoves[curr][1]-1] = '.';
  board[newMoves[curr][0]][newMoves[curr][1]] = '.';
}

int capture_up_right(char** board, int** newMoves, int curr)
{
  if((board[newMoves[curr][0]][newMoves[curr][1]]=='r' || board[newMoves[curr][0]][newMoves[curr][1]] == 'R') && ((board[newMoves[curr][0]+1][newMoves[curr][1]+1]!='b' && board[newMoves[curr][0]+1][newMoves[curr][1]+1]!='B') || (board[newMoves[curr][0]+2][newMoves[curr][1]+2]!='.')))
  {
    return -1;
  }
  if((board[newMoves[curr][0]][newMoves[curr][1]]=='b' || board[newMoves[curr][0]][newMoves[curr][1]] == 'B') && ((board[newMoves[curr][0]+1][newMoves[curr][1]+1]!='r' && board[newMoves[curr][0]+1][newMoves[curr][1]+1]!='R') || (board[newMoves[curr][0]+2][newMoves[curr][1]+2]!='.')))
  {
    return -1;
  }
  if(newMoves[curr][0]+2==7 && board[newMoves[curr][0]][newMoves[curr][1]]=='r')
  {
    board[newMoves[curr][0]][newMoves[curr][1]] = 'R';
  }
  board[newMoves[curr][0]+2][newMoves[curr][1]+2] = board[newMoves[curr][0]][newMoves[curr][1]];
  board[newMoves[curr][0]+1][newMoves[curr][1]+1] = '.';
  board[newMoves[curr][0]][newMoves[curr][1]] = '.';
}

int capture_down_left(char** board, int** newMoves, int curr)
{
  if((board[newMoves[curr][0]][newMoves[curr][1]]=='r' || board[newMoves[curr][0]][newMoves[curr][1]] == 'R') && ((board[newMoves[curr][0]-1][newMoves[curr][1]-1]!='b' && board[newMoves[curr][0]-1][newMoves[curr][1]-1]!='B') || (board[newMoves[curr][0]-2][newMoves[curr][1]-2]!='.')))
  {
    return -1;
  }
  if((board[newMoves[curr][0]][newMoves[curr][1]]=='b' || board[newMoves[curr][0]][newMoves[curr][1]] == 'B') && ((board[newMoves[curr][0]-1][newMoves[curr][1]-1]!='r' && board[newMoves[curr][0]-1][newMoves[curr][1]-1]!='R') || (board[newMoves[curr][0]-2][newMoves[curr][1]-2]!='.')))
  {
    return -1;
  }
  if(newMoves[curr][0]-2==0 && board[newMoves[curr][0]][newMoves[curr][1]]=='b')
  {
    board[newMoves[curr][0]][newMoves[curr][1]] = 'B';
  }
  board[newMoves[curr][0]-2][newMoves[curr][1]-2] = board[newMoves[curr][0]][newMoves[curr][1]];
  board[newMoves[curr][0]-1][newMoves[curr][1]-1] = '.';
  board[newMoves[curr][0]][newMoves[curr][1]] = '.';
}

int capture_down_right(char** board, int** newMoves, int curr)
{
  if((board[newMoves[curr][0]][newMoves[curr][1]]=='r' || board[newMoves[curr][0]][newMoves[curr][1]] == 'R') && ((board[newMoves[curr][0]-1][newMoves[curr][1]+1]!='b' && board[newMoves[curr][0]+1][newMoves[curr][1]+1]!='B') || (board[newMoves[curr][0]-2][newMoves[curr][1]+2]!='.')))
  {
    return -1;
  }
  if((board[newMoves[curr][0]][newMoves[curr][1]]=='b' || board[newMoves[curr][0]][newMoves[curr][1]] == 'B') && ((board[newMoves[curr][0]-1][newMoves[curr][1]+1]!='r' && board[newMoves[curr][0]-1][newMoves[curr][1]+1]!='R') || (board[newMoves[curr][0]-2][newMoves[curr][1]+2]!='.')))
  {
    return -1;
  }
  if(newMoves[curr][0]-2==0 && board[newMoves[curr][0]][newMoves[curr][1]]=='b')
  {
    board[newMoves[curr][0]][newMoves[curr][1]] = 'B';
  }
  board[newMoves[curr][0]-2][newMoves[curr][1]+2] = board[newMoves[curr][0]][newMoves[curr][1]];
  board[newMoves[curr][0]-1][newMoves[curr][1]+1] = '.';
  board[newMoves[curr][0]][newMoves[curr][1]] = '.';
}

int move_up_left(char** board, int** newMoves, int curr, int invalid, struct data* state, int currTurn)
{
  if(board[newMoves[curr][0]+1][newMoves[curr][1]-1]!='.' && invalid==-1)
  {
    return curr;
  }
  if(state->capture==1)
  {
    if(isValidCapture(board, newMoves[curr][0], newMoves[curr][1], state, currTurn) == 0)
    {
      if(invalid==-1)
      {
        invalid = curr;
      }
    }
  }
  if(newMoves[curr][0]+1==7 && board[newMoves[curr][0]][newMoves[curr][1]]=='r')
  {
    board[newMoves[curr][0]][newMoves[curr][1]] = 'R';
  }
  board[newMoves[curr][0]+1][newMoves[curr][1]-1] = board[newMoves[curr][0]][newMoves[curr][1]];
  board[newMoves[curr][0]][newMoves[curr][1]] = '.';
  return invalid;
}

int move_up_right(char** board, int** newMoves, int curr, int invalid, struct data* state, int currTurn)
{
  if(board[newMoves[curr][0]+1][newMoves[curr][1]+1]!='.' && invalid==-1)
  {
    return curr;
  }
  if(state->capture==1)
  {
    if(isValidCapture(board, newMoves[curr][0], newMoves[curr][1], state, currTurn) == 0)
    {
      if(invalid==-1)
      {
        invalid = curr;
      }
    }
  }
  if(newMoves[curr][0]+1==7 && board[newMoves[curr][0]][newMoves[curr][1]]=='r')
  {
    board[newMoves[curr][0]][newMoves[curr][1]] = 'R';
  }
  board[newMoves[curr][0]+1][newMoves[curr][1]+1] = board[newMoves[curr][0]][newMoves[curr][1]];
  board[newMoves[curr][0]][newMoves[curr][1]] = '.';
  return invalid;
}

int move_down_left(char** board, int** newMoves, int curr, int invalid, struct data* state, int currTurn)
{
  if(board[newMoves[curr][0]-1][newMoves[curr][1]-1]!='.' && invalid==-1)
  {
    return curr;
  }
  if(state->capture==1)
  {
    if(isValidCapture(board, newMoves[curr][0], newMoves[curr][1], state, currTurn) == 0)
    {
      if(invalid==-1)
      {
        invalid = curr;
      }
    }
  }
  if(newMoves[curr][0]-1==0 && board[newMoves[curr][0]][newMoves[curr][1]]=='b')
  {
    board[newMoves[curr][0]][newMoves[curr][1]] = 'B';
  }
  board[newMoves[curr][0]-1][newMoves[curr][1]-1] = board[newMoves[curr][0]][newMoves[curr][1]];
  board[newMoves[curr][0]][newMoves[curr][1]] = '.';
  return invalid;
}

int move_down_right(char** board, int** newMoves, int curr, int invalid, struct data* state, int currTurn)
{
  if(board[newMoves[curr][0]-1][newMoves[curr][1]+1]!='.' && invalid==-1)
  {
    return curr;
  }
  if(state->capture==1)
  {
    if(isValidCapture(board, newMoves[curr][0], newMoves[curr][1], state, currTurn) == 0)
    {
      if(invalid==-1)
      {
        invalid = curr;
      }
    }
  }
  if(newMoves[curr][0]-1==0 && board[newMoves[curr][0]][newMoves[curr][1]]=='b')
  {
    board[newMoves[curr][0]][newMoves[curr][1]] = 'B';
  }
  board[newMoves[curr][0]-1][newMoves[curr][1]+1] = board[newMoves[curr][0]][newMoves[curr][1]];
  board[newMoves[curr][0]][newMoves[curr][1]] = '.';
  return invalid;
}

void setDisplay(struct tb_cell* currentBuffer)
{
  int i;
  struct tb_cell* buffer = tb_cell_buffer();
  buffer = currentBuffer;
}

void createMovesList(char** moves, int numMoves, struct moveNode* moveList)
{
  int numElem = numMoves/((tb_height()-2)*2);
  if(numMoves/((tb_height()-2)*2))
  {
    numElem++;
  }
  int i;
  int j;



  int count = 0;
  for(j=0; j<numElem; j++)
  {
    char** newMoves = malloc(numMoves*sizeof(char*));
    for(i=0; i<(tb_height()-2)*2 && count<numMoves; i++)
    {
      newMoves[i] = malloc(4*sizeof(char));
      newMoves[i][0] = moves[i][0];
      newMoves[i][1] = moves[i][1];
      newMoves[i][2] = moves[i][2];
      newMoves[i][3] = moves[i][3];
      count++;
    }
    if(j==0)
    {
      moveList->moves = newMoves;
    }
    else
    {
      struct moveNode* newNode = malloc(sizeof(struct moveNode));
      moveList->next = newNode;
      newNode->moves = newMoves;
      moveList->next->prev = moveList;
      moveList = newNode;
    }
  }
}

void writeInstructionsDisplay()
{
  int y=0;
  char* str = "TO EXIT THE PROGRAM -> ESC key";
  int x=0;
  while (*str) {
		uint32_t uni;
		str += tb_utf8_char_to_unicode(&uni, str);
		tb_change_cell(x, y, uni, TB_WHITE, TB_DEFAULT);
		x++;
	}

  y=1;
  str = "GO BACK TO MOVES -> f3";
  x=0;
  while (*str) {
		uint32_t uni;
		str += tb_utf8_char_to_unicode(&uni, str);
		tb_change_cell(x, y, uni, TB_WHITE, TB_DEFAULT);
		x++;
	}

  y=3;
  str = "move cursor left -> left arrow key";
  x=0;
  while (*str) {
		uint32_t uni;
		str += tb_utf8_char_to_unicode(&uni, str);
		tb_change_cell(x, y, uni, TB_WHITE, TB_DEFAULT);
		x++;
	}

  y=4;
  str = "move cursor right -> right arrow key";
  x=0;
  while (*str) {
		uint32_t uni;
		str += tb_utf8_char_to_unicode(&uni, str);
		tb_change_cell(x, y, uni, TB_WHITE, TB_DEFAULT);
		x++;
	}

  y=5;
  str = "move cursor up -> up arrow key";
  x=0;
  while (*str) {
		uint32_t uni;
		str += tb_utf8_char_to_unicode(&uni, str);
		tb_change_cell(x, y, uni, TB_WHITE, TB_DEFAULT);
		x++;
	}

  y=6;
  str = "move cursor down -> down arrow key";
  x=0;
  while (*str) {
		uint32_t uni;
		str += tb_utf8_char_to_unicode(&uni, str);
		tb_change_cell(x, y, uni, TB_WHITE, TB_DEFAULT);
		x++;
	}

  y=8;
  str = "In order to move to the next page go to the last move and go right or go to the";
  x=0;
  while (*str) {
		uint32_t uni;
		str += tb_utf8_char_to_unicode(&uni, str);
		tb_change_cell(x, y, uni, TB_WHITE, TB_DEFAULT);
		x++;
	}

  y=9;
  str = "last row of moves and go down.";
  x=0;
  while (*str) {
		uint32_t uni;
		str += tb_utf8_char_to_unicode(&uni, str);
		tb_change_cell(x, y, uni, TB_WHITE, TB_DEFAULT);
		x++;
	}

  y=11;
  str = "In order to move to the previous page go to the first move and go left or go to";
  x=0;
  while (*str) {
		uint32_t uni;
		str += tb_utf8_char_to_unicode(&uni, str);
		tb_change_cell(x, y, uni, TB_WHITE, TB_DEFAULT);
		x++;
	}

  y=12;
  str = "the first row of moves and go up.";
  x=0;
  while (*str) {
		uint32_t uni;
		str += tb_utf8_char_to_unicode(&uni, str);
		tb_change_cell(x, y, uni, TB_WHITE, TB_DEFAULT);
		x++;
	}

  y=14;
  str = "In order to change/add a move:";
  x=0;
  while (*str) {
		uint32_t uni;
		str += tb_utf8_char_to_unicode(&uni, str);
		tb_change_cell(x, y, uni, TB_WHITE, TB_DEFAULT);
		x++;
	}

  y=15;
  str = " 1. Highlight the move you want to change.";
  x=0;
  while (*str) {
		uint32_t uni;
		str += tb_utf8_char_to_unicode(&uni, str);
		tb_change_cell(x, y, uni, TB_WHITE, TB_DEFAULT);
		x++;
	}

  y=16;
  str = " 2. Press the insert key.";
  x=0;
  while (*str) {
		uint32_t uni;
		str += tb_utf8_char_to_unicode(&uni, str);
		tb_change_cell(x, y, uni, TB_WHITE, TB_DEFAULT);
		x++;
	}

  y=17;
  str = " 3. Type the four coordinates that you want to input.";
  x=0;
  while (*str) {
		uint32_t uni;
		str += tb_utf8_char_to_unicode(&uni, str);
		tb_change_cell(x, y, uni, TB_WHITE, TB_DEFAULT);
		x++;
	}

  y=18;
  str = "**To add a move go to the last move (empty space) and insert there**";
  x=0;
  while (*str) {
		uint32_t uni;
		str += tb_utf8_char_to_unicode(&uni, str);
		tb_change_cell(x, y, uni, TB_WHITE, TB_DEFAULT);
		x++;
	}
}

int isValidCapture(char** board, int r, int c, struct data* state, int currTurn)
{
  int val = 1;
  //capture_up_left
  r = 0;
  c = 0;
  // if(currTurn==0)
  // {
  //   currTurn=1;
  // }
  // else
  // {
  //   currTurn=0;
  // }

  for(r=0; r<8; r++)
  {

    for(c=0; c<8; c++)
    {

      if(board[r][c]=='r' || board[r][c]=='R' || board[r][c]=='B')
      {
        if(r<=5 && c>=2)
        {
          if((board[r][c]=='r' || board[r][c]=='R') && currTurn==1)
          {
            if((board[r+1][c-1]=='b' || board[r+1][c-1]=='B') && board[r+2][c-2]=='.')
            {
              printf("%d %d\n", r, c);
              printf("%d", currTurn);
              printf("here");
              return 0;
            }
          }
          else if(board[r][c]=='B' && currTurn==0)
          {
            if((board[r+1][c-1]=='r' || board[r+1][c-1]=='R') && board[r+2][c-2]=='.')
            {
              printf("%d %d\n", r, c);
              return 0;
            }
          }
        }
      }

      //capture_up_right
      if(board[r][c]=='r' || board[r][c]=='R' || board[r][c]=='B')
      {
        if(r<=5 && c<=5)
        {
          if((board[r][c]=='r' || board[r][c]=='R') && currTurn==1)
          {
            if((board[r+1][c+1]=='b' || board[r+1][c+1]=='B') && board[r+2][c+2]=='.')
            {
              printf("%d %d\n", r, c);
              return 0;
            }
          }
          else if(board[r][c]=='B' && currTurn==0)
          {
            if((board[r+1][c+1]=='r' || board[r+1][c+1]=='R') && board[r+2][c+2]=='.')
            {
              printf("%d %d\n", r, c);
              return 0;
            }
          }
        }
      }

      //capture_down_left
      if(board[r][c]=='b' || board[r][c]=='B' || board[r][c]=='R')
      {
        if(r>=2 && c>=2)
        {
          if((board[r][c]=='b' || board[r][c]=='B') && currTurn==0)
          {
            if((board[r-1][c-1]=='r' || board[r-1][c-1]=='R') && board[r-2][c-2]=='.')
            {
              printf("%d %d\n", r, c);
              return 0;
            }
          }
          else if(board[r][c]=='R' && currTurn==1)
          {
            if((board[r-1][c-1]=='b' || board[r-1][c-1]=='B') && board[r-2][c-2]=='.')
            {
              printf("%d %d\n", r, c);
              return 0;
            }
          }
        }
      }

      //capture_down_right
      if(board[r][c]=='b' || board[r][c]=='B' || board[r][c]=='R')
      {
        if(r>=2 && c<=5)
        {
          if((board[r][c]=='b' || board[r][c]=='B') && currTurn==0)
          {
            if((board[r-1][c+1]=='r' || board[r-1][c+1]=='R') && board[r-2][c+2]=='.')
            {
              printf("%d %d\n", r, c);
              return 0;
            }
          }
          else if(board[r][c]=='R' && currTurn==1)
          {
            if((board[r-1][c+1]=='b' || board[r-1][c+1]=='B') && board[r-2][c+2]=='.')
            {
              printf("%d %d\n", r, c);
              return 0;
            }
          }
        }
      }

    }

  }
  return val;
}

