/****
  Minimax.hpp
  
  */
  
#ifndef _MINIMAX_HPP_
#define _MINIMAX_HPP_

#include <time.h>
#include "board.hpp"

#ifndef uint
//#define uint unsigned int
#endif

#define MAX_INT 2147483647
#define MIN_INT -2147483648
#define LARGE_FLOAT 1.0e35
#define SMALL_FLOAT -1.0e35
#define HUGE_FLOAT 2.0e38
#define TINY_FLOAT -2.0e38

#define HARD_WIRED_FIRST_MOVE 37  // F-5 position
#define HARD_WIRED_FIRST_MOVE_BOARD_FLIPPED 34  // C-5
#define ESTIMATED_CORNER_WORTH 10.0  // Wild guess, don't trust!
#define DENOMINATOR_EXTRA_PREV_MIN_DOF 0.2  // for avoiding divide by 0

#define DEBUG_MINIMAX 0
#define COUNT_PRUNING 0

extern char searchDepth;
extern char originalSearchDepth;
extern char bruteForceDepth; // for approaching the end of game.
extern char mpcDepth;
extern bool winLarge;
extern char randomnessLevel;
extern bool useAndersson;  // use Andersson's sophisticated end game solver

extern bool boardFlipped;
extern bool showDots;  // simply for output

void writetofile();
/*char getMinimaxMove(Board *board, bool *legalMoves);
float getMin(char lastx, char lasty, char color, char depth, char passes,
             char prevmaxDOF, char prevminDOF, //DOF found in previous getMax and getMin;
             char selfPieces, char oppPieces, float alpha, float beta);
float getMax(char lastx, char lasty, char color, char depth, char passes,
             char prevmaxDOF, char prevminDOF, 
             char selfPieces, char oppPieces, float alpha, float beta);*/

void getminimax(int serial,int flipr[10][10],Board *board, bool *legalMoves,int pass,int parentchild[10][10],bool p,char winners,int dep);
int getMin(Board *b,bool *legalMoves,char lastx, char lasty, char color, char depth, char passes,
             char selfPieces, char oppPieces,int flipr[10][10],int parentchild1[10][10],int i,int alpha,int beta);
int getMax(Board *b,bool *legalMoves,char lastx, char lasty, char color, char depth, char passes,
             char selfPieces, char oppPieces,int flipr[10][10],int parentchild1[10][10],int i,int alpha,int beta);
int evaluate(Board *b,char color,bool *lm,int flipr[10][10],int parentchild1[10][10],int depth,char lastx,char lasty,int alpha,int beta);
void copyBoardArray(char *to, char *from);
bool legalMove(char *a, char color, char x, char y);
char findLegalMoves(char *a, char color, unsigned int *mask0, unsigned int *mask1);
char tryMove(char *a, char color, char x, char y);

float evaluateEndGame(char selfPieces, char oppPieces);
float evaluateBoard(char *a, char forWhom, char whoseTurn, char prevmaxDOF, 
                    char prevminDOF, char selfPieces, char oppPieces);

void printBoardArray(char *a);
int strongEndGameSolve(char depth, char color, char selfPieces, char oppPieces, 
                      char prevNotPass, float alpha, float beta);

void getminimax2(int serial,int flipr[10][10],Board *board, bool *legalMoves,int pass,int parentchild[10][10],bool p,char winners,int dep);
int getMin2(Board *b,bool *legalMoves,char lastx, char lasty, char color, char depth1, char passes,
             char selfPieces, char oppPieces,int flipr[10][10],int parentchild1[10][10],int i);
int getMax2(Board *b,bool *legalMoves,char lastx, char lasty, char color, char depth1, char passes,
             char selfPieces, char oppPieces,int flipr[10][10],int parentchild1[10][10],int i);
int evaluate2(Board *b,char color,bool *lm,int flipr[10][10],int parentchild1[10][10],int depth,char lastx,char lasty);
void copyBoardArray2(char *to, char *from);
void countPieces2(char *a, int *nb, int *nw);

#endif
