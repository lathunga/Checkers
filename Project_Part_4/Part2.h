#include <stdio.h>



void scanUntilMoves(FILE* input);

void scanUntilBoard(FILE* input);

char** getInitialBoard(FILE* input);

char* getCoords(FILE* input);

struct data

{
  int blackPawns;

  int blackKings;

  int redPawns;

  int redKings;

  int capture;

  int multipleJumps;

  int turn;

  int wasCapture;

  int actualNumDoubleMoves;
};

int isValidRedPawnMove(char* coords, char** board, int capture, struct data* init);

int isValidBlackPawnMove(char* coords, char** board, int capture, struct data* init);

int isValidRedKingMove(char* coords, char** board, int capture, struct data* init);

int isValidBlackKingMove(char* coords, char** board, int capture, struct data* init);

int numberOfMoves(FILE* input);

char* getCoordsMultiple(FILE* input, char coord1, char coord2);

char* formatBoard(char** board, char* newBoard);

void checkValid(FILE* input, struct data* initData);

void createExchange(char* name, struct data* final, char** board, char* coords, int numMoves, int isIllegal, int actualNumMoves);

void reformatBoard(char** board);

void createHumanReadable(char** board, char* name, char* coords, int actualNumMoves, int numMoves, int isIllegal);
