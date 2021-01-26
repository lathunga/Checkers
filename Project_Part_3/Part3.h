#include <stdlib.h>
int* findPiece(char** board, int turn, int findPieceCount);
int isValidCapture(char capturee1, char capturee2, char inbetween, char destination);
int printPossibleMovesRedPawn(char** board, char capturee1, char capturee2, int m, int n, int findPieceCount, int score, int verbose);
int printPossibleMovesBlackPawn(char** board, char capturee1, char capturee2, int m, int n, int findPieceCount, int score, int verbose);
void move_up_left(char** board, int rank, int file);
void move_up_right(char** board, int rank, int file);
void move_down_left(char** board, int rank, int file);
void move_down_right(char** board, int rank, int file);
void capture_up_left(char** board, int rank, int file);
void capture_up_right(char** board, int rank, int file);
void capture_down_left(char** board, int rank, int file);
void capture_down_right(char** board, int rank, int file);
char** getInitialBoard2(FILE* input);
void copyBoard(char** progressedBoard, char** board);
void printQuestionLine(int m, int n, int file, int rank, char* turn, int fileAdj, int rankAdj, int verbose);
void printDotLine(int m, int n, int file, int rank, char* turn, int fileAdj, int rankAdj, int score, int verbose);
int calculateScore(char** board, int turn);
