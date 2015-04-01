#include<iostream>
#include<fstream>
#include "board.hpp"
#include "game.hpp"
#include<stdio.h>
using namespace std;

//bool showLegalMoves3;
int bwt3=0,wwt3=0;
int flipreturn3[10][10];
char start3;
char flipCoins3[10];
char winner3;
int eval3[10][10]={{99,-8,8,6,6,8,-8,99},
					{-8,-24,-4,-3,-3,-4,-24,-8},
					{8,-4,7,4,4,7,-4,8},
					{6,-3,4,0,0,4,-3,6},
					{6,-3,4,0,0,4,-3,6},
					{8,-4,7,4,4,7,-4,8},
					{-8,-24,-4,-3,-3,-4,-24,-8},
					{99,-8,8,6,6,8,-8,99}};
char input3[10][10];
//char player13, player23;
/* initialize a given board to start-of-game configuration. */
extern char player1, player2;
extern bool showLegalMoves;
int flipr3[10][10];
bool legalMoves3[64];

/* initialize a given board to start-of-game configuration. */

void initBoard3(Board *board, char isFlipped) {
  char *a = board->a[0];
  for(int i=0; i<64; i++)
    a[i] = EMPTY;
  for(int y=0; y<8; y++) {
      for(int x=0; x<8; x++) {
    	if(input3[y][x]=='X')
    		a[CONV_21(x, y)] = X;
    	if(input3[y][x]=='O')
    	    a[CONV_21(x, y)] = O;
      }
  }
  board->m = 0;
  board->top = 0;
  if(start3=='X')
  {	  board->wt = X;
  	  winner3='X';}
  else
  {	  board->wt=O;
  	  winner3='O';}
  board->moves[0] = PASS; // "zeroth" move actually doesn't exist
}

Board* makeBoard3(char isFlipped) {
  Board *b = (Board*)malloc(sizeof(Board)); // free if no longer used!
  initBoard3(b, isFlipped);
  return b;
}

bool legalMove3(Board *b, char x, char y) {
  char n = b->n;
  char *a = b->a[n];
  char wt=b->wt;
  char place = CONV_21(x, y);
  if(x < 0 || x > 7 || y < 0 || y > 7)
    return false;
  if(a[place] != EMPTY)
    return false;
  /* test left for possible flips */
  //char wt = b->wt;
  bool result = false;
  for(char dir=0; dir<8; dir++) {
    char dx = DIRECTION[dir][0];
    char dy = DIRECTION[dir][1];
    char tx = x+2*dx;
    char ty = y+2*dy;
    /* need to be at least 2 grids away from the edge and a oppenent piece
      adjacent in the direction */
    if(!ON_BOARD(tx, ty) || a[CONV_21(x+dx, y+dy)] != OTHER(wt))
      continue;
    while(ON_BOARD(tx, ty) && a[CONV_21(tx, ty)] == OTHER(wt)) {
      tx += dx;
      ty += dy;
    }
    if(ON_BOARD(tx, ty) && a[CONV_21(tx, ty)] == wt) {
      result = true;
      break;
    }
  }
  //cout<<result;
  return result;
}

/* get all legal moves */

/* get all legal moves */
bool findLegalMoves3(Board *b, bool *lm) {
	char n = b->n;
	  char place;
	  char *a = b->a[n];
	  char color=b->wt;
	  int flip=0;
	  bwt3=0;
	  wwt3=0;
  	int maxflip=0,minflip=100;
  	int x1=0,y1=0;
  bool result = false;
  for(char i=0; i<8; i++)
         	{
         		    for(char j=0; j<8; j++)
         		    {
         		      place = CONV_21(j, i);
         		      if(a[place] == X)
         		      {  bwt3=bwt3+eval3[i][j];
         		      }
         		      else if(a[place] == O)
         		      { wwt3=wwt3+eval3[i][j];
         		       }
         		    }
         	}
 // cout<<"Current black n white count: "<<bwt3<<" "<<wwt3<<"\n";
  for(int y=0; y<8; y++) {
    for(int x=0; x<8; x++) {
      lm[CONV_21(x, y)] = legalMove3(b, x, y);
      if(lm[CONV_21(x, y)])
      {
        result = true;
        //cout<<"trymove clled for "<<x<<" "<<y<<"\t";
        flip=tryMove3(a,color,x,y);
        //cout<<"flip eval3 vlaue: "<<flip<<"\n";
        if(winner3=='X'){
        if(color==X)
        {
        if(((flip+bwt3+eval3[y][x])-wwt3)>maxflip)
        {
        	maxflip=flip+bwt3+eval3[y][x];
        	x1=y;
        	y1=x;
        }
        if(((flip+bwt3+eval3[y][x])-wwt3)==maxflip)
        {
        	if(y<x1)
        	{	x1=y;
        		y1=x;}
        	else if(y==x1 and x<y1)
        	{	x1=y;
        		y1=x;}

        }
        flipreturn3[0][0]=maxflip;
        flipreturn3[0][1]=y1;
        flipreturn3[0][2]=x1;
        //cout<<color<<" maxflip:"<<maxflip<<"  x and y : "<<x1<<"  "<<y1<<"\n";
        }
        if(color==O)
        {
        	if((bwt3-(flip+wwt3+eval3[y][x]))<minflip)
        	        {
        	        	minflip=flip+wwt3+eval3[y][x];
        	        	x1=y;
        	        	y1=x;
        	        }
        	        if((bwt3-(flip+wwt3+eval3[y][x]))==minflip)
        	        {
        	        	if(y<x1)
        	        	{	x1=y;
        	        		y1=x;}
        	        	else if(y==x1 and x<y1)
        	        	{	x1=y;
        	        		y1=x;}

        	        }
        	        flipreturn3[1][0]=minflip;
        	        flipreturn3[1][1]=y1;
        	        flipreturn3[1][2]=x1;
        	      //  cout<<color<<" minflip:"<<minflip<<"  x and y : "<<x1<<"  "<<y1<<"\n";
        }
        }
        else
        {
        	if(color==O)
        	        {
        	        if(((flip+wwt3+eval3[y][x])-bwt3)>maxflip)
        	        {
        	        	maxflip=flip+wwt3+eval3[y][x];
        	        	x1=y;
        	        	y1=x;
        	        }
        	        if(((flip+wwt3+eval3[y][x])-bwt3)==maxflip)
        	        {
        	        	if(y<x1)
        	        	{	x1=y;
        	        		y1=x;}
        	        	else if(y==x1 and x<y1)
        	        	{	x1=y;
        	        		y1=x;}

        	        }
        	        flipreturn3[0][0]=maxflip;
        	        flipreturn3[0][1]=y1;
        	        flipreturn3[0][2]=x1;
        	        //cout<<color<<" maxflip:"<<maxflip<<"  x and y : "<<x1<<"  "<<y1<<"\n";
        	        }
        	        if(color==X)
        	        {
        	        	if((wwt3-(flip+bwt3+eval3[y][x]))<minflip)
        	        	        {
        	        	        	minflip=flip+bwt3+eval3[y][x];
        	        	        	x1=y;
        	        	        	y1=x;
        	        	        }
        	        	        if((wwt3-(flip+bwt3+eval3[y][x]))==minflip)
        	        	        {
        	        	        	if(y<x1)
        	        	        	{	x1=y;
        	        	        		y1=x;}
        	        	        	else if(y==x1 and x<y1)
        	        	        	{	x1=y;
        	        	        		y1=x;}

        	        	        }
        	        	        flipreturn3[1][0]=minflip;
        	        	        flipreturn3[1][1]=y1;
        	        	        flipreturn3[1][2]=x1;
        	        	      //  cout<<color<<" minflip:"<<minflip<<"  x and y : "<<x1<<"  "<<y1<<"\n";
        	        }

        }
      }
    }
  }

  return result;
}
/* print out the board */
void printBoard3(Board *b, bool *legalMoves) {
	std::ofstream out("output.txt");
  char *a = b->a[b->n];
  char lastMove = b->moves[b->m];
  char place;
  char s;
  char nb, nw;
	char n = b->n;
	char *c = b->a[n];
	bwt3=0;
		         		wwt3=0;
		         	  	for(char i=0; i<8; i++)
		         	         	{
		         	         		    for(char j=0; j<8; j++)
		         	         		    {
		         	         		      place = CONV_21(j, i);
		         	         		      if(a[place] == X)
		         	         		      {  bwt3=bwt3+eval3[i][j];
		         	         		      }
		         	         		      else if(a[place] == O)
		         	         		      { wwt3=wwt3+eval3[i][j];
		         	         		       }
		         	         		    }
		         	         	}
  //countPieces(b, &nb, &nw);
  //cout<<"\n     A B C D E F G H\n";
  for(char y=0; y<8; y++) {
    /*if(lastMove == 8*y)
      //printf("  %d (", y+1);
    else
      //printf("  %d  ", y+1);*/
    for(char x=0; x<8; x++) {
      place = CONV_21(x, y);
      if(a[place] == X)
        s = 'X';
      else if(a[place] == O)
        s = 'O';
      //else if(legalMoves[place] && showLegalMoves)
        //s = "+";
      else
        s = '*';
      if(place+1 == lastMove && x != 7)
        out<<s;
      else if(place == lastMove)
    	  out<<s;
      else
    	  out<<s;
    }
    // printf(" %d", y+1);
   /* if(y == 0)
      printf("\tBlack: %d", bwt3);
    if(y == 1)
      printf("\tWhite: %d", wwt3);*/
    out<<"\n";
  }
  // printf("   A B C D E F G H\n");
 // printf("\n");
}

/* try a move on the minimax's board. Return the number of pieces flipped.
(also slow) */
char tryMove3(char *a, char color, char x, char y) {
	//cout<<"trymove clled for\t"<<x<<" "<<y;
char flipCount = 0;
char dx, dy, tx, ty;
for(char dir=0; dir<8; dir++) {
  dx = DIRECTION[dir][0];
  dy = DIRECTION[dir][1];
  tx = x+2*dx;
  ty = y+2*dy;
  /* need to be at least 2 grids away from the edge and a oppenent piece
    adjacent in the direction to make flips in this direction. */
  if(!ON_BOARD(tx, ty) || a[CONV_21(x+dx, y+dy)] != OTHER(color))
    continue;
  while(ON_BOARD(tx, ty) && a[CONV_21(tx, ty)] == OTHER(color)) {
    tx += dx;
    ty += dy;
  }
  /* go back and flip the pieces if it should happen */
  if(ON_BOARD(tx, ty) && a[CONV_21(tx, ty)] == color) {
    tx -= dx;
    ty -= dy;
    while(a[CONV_21(tx, ty)] == OTHER(color)) {
      //a[CONV_21(tx, ty)] = color;
      flipCount=flipCount+eval3[ty][tx];
      tx -= dx;
      ty -= dy;
    }
  }
}
/* the new piece is added if it caused any flips
if(flipCount)
  a[CONV_21(x, y)] = color
    cout<<"Flipcount eval3:"<<flipCount<<"\n";*/
return flipCount;
}

/* make a move, (x, y) must be a legal move */
void makeMove3(Board *b, char x, char y,char flipCoins3[10]) {
  char n = b->n;
  char wt = b->wt;
  char *a = b->a[n];
  char *a1 = b->a[n+1];
  char place = CONV_21(x, y);
  int k=0;
  for(int i=0; i<64; i++)
    a1[i] = a[i];
  for(char dir=0; dir<8; dir++) {
    char dx = DIRECTION[dir][0];
    char dy = DIRECTION[dir][1];
    char tx = x+2*dx;
    char ty = y+2*dy;
    /* need to be at least 2 grids away from the edge and a oppenent piece
      adjacent in the direction to make flips in this direction. */
    if(!ON_BOARD(tx, ty) || a[CONV_21(x+dx, y+dy)] != OTHER(wt))
      continue;
    while(ON_BOARD(tx, ty) && a[CONV_21(tx, ty)] == OTHER(wt)) {
      tx += dx;
      ty += dy;
    }
    /* go back and flip the pieces if it should happen */
    if(ON_BOARD(tx, ty) && a[CONV_21(tx, ty)] == wt) {
      tx -= dx;
      ty -= dy;
      while(a[CONV_21(tx, ty)] == OTHER(wt)) {
        a1[CONV_21(tx, ty)] = wt;
        flipCoins3[k]=CONV_21(tx, ty);
        k++;
        tx -= dx;
        ty -= dy;
      }
    }
  }
  /* update the board to the next step */
  a1[place] = wt;
  b->wt = OTHER(wt);
  b->n += 1;  // actual move pointer
  b->m += 1;  // total move pointer
  b->top = b->m;  // whenever a move is made, no 'redo' possible
  b->moves[b->m] = place;  // record this move
}

void makePass3(Board *b,bool *legalMoves) {
	//cout<<"Pass!!";
	printBoard3(b,legalMoves3);
  b->m += 1;
  b->wt = OTHER(b->wt);
  b->top = b->m;
  b->moves[b->m] = PASS;
}

void nextmove(int flipr[10][10],const char *color)
{
	if(winner3=='X'){
	if(color=="X")
	{
	flipr[0][0]=flipreturn3[0][0];
	flipr[0][1]=flipreturn3[0][1];
	flipr[0][2]=flipreturn3[0][2];
	}
	else
	{
		flipr[0][0]=flipreturn3[1][0];
		flipr[0][1]=flipreturn3[1][1];
		flipr[0][2]=flipreturn3[1][2];
	}}
	else
	{
		if(color=="O")
			{
			flipr[0][0]=flipreturn3[0][0];
			flipr[0][1]=flipreturn3[0][1];
			flipr[0][2]=flipreturn3[0][2];
			}
			else
			{
				flipr[0][0]=flipreturn3[1][0];
				flipr[0][1]=flipreturn3[1][1];
				flipr[0][2]=flipreturn3[1][2];
			}
	}
	return;
}
/* the main loop of the game. player1 is black, player2 is white */
void playGame3(Board *board,char winner,int depth) {
  /* declare some variables */
  char currentPlayer;
  char countBlack, countWhite;  // # of black and white pieces

  bool hasLegalMove;
  char buf[1024], bufCopied[1024];
  char nextMove;
  int x, y;
  const char *color;
  int nUR;  // how many moves to undo/redo
  int countUR;  // how many moves can be actually undo/redo
  char *token, *token2;
  bool hasSpecialCommand;
  bool firstInput;
  bool skip = false;
  player1=COMPUTER;
  player2=COMPUTER;
  /* Timing -- none essential variables */
 // time_t tm1, tm2, steptm1, steptm2;
  //double steptime;
  //time(&tm1);
  //srand(time(NULL));
  // seed the random generator

    hasLegalMove = findLegalMoves3(board, legalMoves3);
    currentPlayer = board->wt == X? player1 : player2;
    color = board->wt == X? "X" : "O";

    // Hand game control over to human after the specified # of self-played moves.

     // printBoard(board, legalMoves);
      if(hasLegalMove) {
    	  //cout<<"here";
    	  nextmove(flipr3,color);
    	  x=flipr3[0][1];
    	  y=flipr3[0][2];

       // cout<<"values of x,y "<<x<<" "<<y;
        if(legalMove(board, x, y)) {
          makeMove3(board, x, y,flipCoins3);
          printBoard3(board, legalMoves3);
          /*printf("Computer (%s, # %d) played at %c%c.", color,
                board->m, x+'a', y+'1');
          //if(showTime)
            //printf("\t(%.3f seconds)", steptime);
          printf("\n");*/
        }
        else {
          //printf("Computer (%s, # %d) returned ILLEGAL MOVE: %c%c !! Please debug!\n",      color, board->m, x+'a', y+'1');
          exit(1);
        }
      }
      else {
        makePass3(board,legalMoves3);

      }
}
int main4()
{
	int a,depth;
	char isFlipped=0;
	//bool legalMoves[64];
	  player1 = COMPUTER;
	  player2 = COMPUTER;
	  showLegalMoves = true;
	  ifstream in;
	  in.open("input.txt");
	  in>>a;
	  in>>start3;
	  in>>depth;
	  for(int i=0;i<8;i++)
	  {
		  for(int j=0;j<8;j++)
		  {
			  in>>input3[i][j];
		  }
	  }
	Board *gb = makeBoard3(isFlipped);
	//printBoard3(gb,legalMoves3);
	playGame3(gb,winner3,depth);
	//printBoard3(gb,legalMoves3);

	return 0;
}


bool undoMove3(Board *b,char flipcolors[10]) {
	char n = b->n;
	  char *a = b->a[n];
  char m = b->m;
  if(m == 0)
    return false;
  if(b->moves[m] == PASS) {
    b->m -= 1;
    b->wt = OTHER(b->wt);
  }
  else {
    b->m -= 1;
    b->n -= 1;
    b->wt = OTHER(b->wt);
    for(char y=0;y<8;y++)
    	for(char x=0;x<8;x++)
    		a[CONV_21(x, y)]=OTHER(b->wt);
  }
  return true;
}



