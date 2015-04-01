/*
 * game.hpp
 *
 *  Created on: Oct 12, 2014
 *      Author: Sapna
 */

#ifndef GAME_HPP_
#define GAME_HPP_

/***
  game.hpp

  */

#ifndef _GAME_HPP_
#define _GAME_HPP_

#include <string.h>
#include <time.h>
#include "board.hpp"
//#include "minimax.hpp"

#define HUMAN 1
#define COMPUTER 2

extern double totalTimeUsed;
extern bool showDots;
extern bool showTime;

void playGame(Board *board,char winner,int depth);
void playGame1(Board *board,char winner,int dep);
void playGame3(Board *board,char winner,int dep);
static bool gameOver(Board *b);
static char parseHumanMove(char *buf);
static char getRandomMove(Board *board, bool *legalMoves);

#endif




#endif /* GAME_HPP_ */
