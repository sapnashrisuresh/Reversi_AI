/*
 * board.hpp
 *
 *  Created on: Oct 12, 2014
 *      Author: Sapna
 */

#ifndef BOARD_HPP_
#define BOARD_HPP_
/* board.hpp   Part of Othello

   */

#ifndef _BOARD_HPP_
#define _BOARD_HPP_

#include <stdlib.h>
#include <stdio.h>

#define EMPTY 0
#define X 1
#define O 2
#define OTHER(x) (3-(x))  // OTHER(BLACK) = WHITE, vice versa.

#define PASS -1
#define ILLEGAL -2

#define uchar unsigned char

#define CONV_21(x, y) (((y) << 3)+(x))
#define ON_BOARD(x, y) ((x) >= 0 && (x) < 8 && (y) >= 0 && (y) < 8)

extern bool showLegalMoves;

const char DIRECTION[8][2] = {{1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};

struct Board {
  char a[61][64];  // stack of board array
  char moves[128];  // sequence of moves include PASS until this point.
  char n;  // number of actual moves made (NOT incl. PASS).
  char m;  // number of moves incl. PASSes made so far.
  char top;  // redo possible if m < top.
  char wt;  // whose turn it is to move.
};


/*
struct HistElem {
  uchar move;  // the place where a new piece is dropped
  uchar flips[18];  // places where a stone waas flipped
} */

Board* makeBoard(char isFlipped);
void initBoard(Board *board, char isFlipped);
void printBoard(Board *b, bool *legalMoves);

Board* makeBoard3(char isFlipped);
void initBoard3(Board *board, char isFlipped);
void printBoard3(Board *b, bool *legalMoves);

bool legalMove(Board *board, char x, char y);
bool findLegalMoves(Board *board, bool *legalMoves);
void nextmove(int flipr[10][10],const char *color);
bool legalMove3(Board *board, char x, char y);
bool findLegalMoves3(Board *board, bool *legalMoves);

void makeMove(Board *board, char x, char y,char flipcolors[10]);
char tryMove(char *a, char color, char x, char y);
void makeMove3(Board *board, char x, char y,char flipcolors[10]);
char tryMove3(char *a, char color, char x, char y);

void makePass3(Board *board,bool *legalMoves);
void makePass(Board *board,bool *legalMoves);
void countPieces(char *a, char *nb, char *nw, uchar ntotal);
void countPieces(Board *b, char *nb, char *nw);
bool undoMove(Board *b,char flipcolors[10]);
bool undoMove3(Board *b,char flipcolors[10]);
#endif





#endif /* BOARD_HPP_ */
