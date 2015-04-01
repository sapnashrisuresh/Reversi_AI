#include<iostream>
#include<fstream>
#include<stdio.h>
#include "board.hpp"
#include "game.hpp"
#include "minimax.hpp"
using namespace std;

extern char player1, player2;

bool legalMoves[64];
bool showLegalMoves;
int bwt=0,wwt=0;
int flipreturn[10][10];
char start;
char flipCoins[10];

char winner;
int eval[10][10]={{99,-8,8,6,6,8,-8,99},
					{-8,-24,-4,-3,-3,-4,-24,-8},
					{8,-4,7,4,4,7,-4,8},
					{6,-3,4,0,0,4,-3,6},
					{6,-3,4,0,0,4,-3,6},
					{8,-4,7,4,4,7,-4,8},
					{-8,-24,-4,-3,-3,-4,-24,-8},
					{99,-8,8,6,6,8,-8,99}};
char input[10][10];
char player1, player2;
/* initialize a given board to start-of-game configuration. */
extern char player1, player2;
extern bool showLegalMoves;
static bool temp[64];
int flipr[10][10];


void initBoard(Board *board, char isFlipped) {
  char *a = board->a[0];
  for(int i=0; i<64; i++)
    a[i] = EMPTY;
  for(int y=0; y<8; y++) {
      for(int x=0; x<8; x++) {
    	if(input[y][x]=='X')
    		a[CONV_21(x, y)] = X;
    	if(input[y][x]=='O')
    	    a[CONV_21(x, y)] = O;
      }
  }
  board->m = 0;
  board->top = 0;
  if(start=='X')
  {	  board->wt = X;
  	  winner='X';}
  else
  {	  board->wt=O;
  	  winner='O';}
  board->moves[0] = PASS; // "zeroth" move actually doesn't exist
}

Board* makeBoard(char isFlipped) {
  Board *b = (Board*)malloc(sizeof(Board)); // free if no longer used!
  initBoard(b, isFlipped);
  return b;
}

bool legalMove(Board *b, char x, char y) {
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
bool findLegalMoves(Board *b, bool *lm) {
	char n = b->n;
	  char place;
	  char *a = b->a[n];
	  char color=b->wt;
	  int flip=0;
	  bwt=0;
	  wwt=0;
  	int maxflip=0,minflip=100;
  	int x1=0,y1=0;
  bool result = false;
 // cout<<"Current black n white count: "<<bwt<<" "<<wwt<<"\n";
  for(int y=0; y<8; y++) {
    for(int x=0; x<8; x++) {
      lm[CONV_21(x, y)] = legalMove(b, x, y);
      if(lm[CONV_21(x, y)])
      {
        result = true;
      }
    }
  }

  return result;
}
/* print out the board */
void printBoard(Board *b, bool *legalMoves) {
	std::ofstream out("output.txt");
  char *a = b->a[b->n];
  char lastMove = b->moves[b->m];
  char place;
  char s;
  char nb, nw;
	char n = b->n;
	char *c = b->a[n];
	bwt=0;
		         		wwt=0;
		         	  	for(char i=0; i<8; i++)
		         	         	{
		         	         		    for(char j=0; j<8; j++)
		         	         		    {
		         	         		      place = CONV_21(j, i);
		         	         		      if(a[place] == X)
		         	         		      {  bwt=bwt+eval[i][j];
		         	         		      }
		         	         		      else if(a[place] == O)
		         	         		      { wwt=wwt+eval[i][j];
		         	         		       }
		         	         		    }
		         	         	}
  //countPieces(b, &nb, &nw);

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
}

/* try a move on the minimax's board. Return the number of pieces flipped.
(also slow) */
char tryMove(char *a, char color, char x, char y) {
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
      flipCount=flipCount+eval[ty][tx];
      tx -= dx;
      ty -= dy;
    }
  }
}
/* the new piece is added if it caused any flips
if(flipCount)
  a[CONV_21(x, y)] = color
    cout<<"Flipcount eval:"<<flipCount<<"\n";*/
return flipCount;
}

/* make a move, (x, y) must be a legal move */
void makeMove(Board *b, char x, char y,char flipcoins[10]) {
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
        flipcoins[k]=CONV_21(tx, ty);
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

void makePass(Board *b,bool *legalMoves) {
	//cout<<"Pass!!";
	//printBoard(b,legalMoves);
  b->m += 1;
  b->wt = OTHER(b->wt);
  b->top = b->m;
  b->moves[b->m] = PASS;
}

void declarewinner()
{
	if(bwt > wwt) { // use the winner-gets-empties convention.
	    cout<<"Black wins! "<<bwt;
	  }
	  else if(bwt<wwt) {
	    cout<<"White wins! "<<wwt;
	  }
	  else {
	    cout<<"Game is drawn! ";
	  }
}


bool undoMove(Board *b,char flipcolors[10]) {
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


/* the main loop of the game. player1 is black, player2 is white */
void playGame1(Board *board,char winner,int dep) {
  /* declare some variables */
  char currentPlayer;
  char countBlack, countWhite;  // # of black and white pieces

  bool hasLegalMove;
  char buf[1024], bufCopied[1024];
  char nextMove;
  int x1, y1,x2,y2;
  const char *color;
  int nUR;  // how many moves to undo/redo
  int countUR;  // how many moves can be actually undo/redo
  char *token, *token2;
  bool hasSpecialCommand;
  bool firstInput;
  bool skip = false;
  char n = board->n;
  char *a = board->a[n];
  char color2;
  player1=COMPUTER;
  player2=COMPUTER;
  int serial=0;
  int pass=0;
  char flipcolors[10];
  int flipr[10][10];

  /* main loop */

	  int parentchild1[10][10]={{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0},
	  			{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0},
	  			{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0}};


    hasLegalMove = findLegalMoves(board, legalMoves);
    currentPlayer = board->wt == X? player1 : player2;
    color = board->wt == X? "X" : "O";
    //char winner=board->wt;
    bool p=false;
      printBoard(board, legalMoves);
      if(hasLegalMove) {
    	  getminimax(serial,flipr,board,legalMoves,pass,parentchild1,p,winner,dep);

    	  //cout<<"values of x,y "<<flipr[2][3]<<" "<<flipr[2][2]<<" "<<flipr[2][1]<<" "<<flipr[2][0];
    	 // serial++;
    	 x1=flipr[2][0];
    	 y1=flipr[2][1];

    	          if(legalMove(board, x1, y1)) {
    	            makeMove(board, x1, y1,flipcolors);
    	            printBoard(board, legalMoves);
    	            printf("Computer (%s, # %d) played at %c%c.", color,
    	                  board->m, x1+'a', y1+'1');

    	            printf("\n");
    	          }
    	          else {
    	                    printf("Computer (%s, # %d) returned ILLEGAL MOVE: %c%c !! Please debug!\n",
    	                            color, board->m, x1+'a', y1+'1');
    	                    exit(1);
    	          }
      }
    	          else {
    	        	  bool p=true;
    	                  makePass(board,legalMoves);
    	                  pass++;
    	                  getminimax(serial,flipr,board,legalMoves,pass,parentchild1,p,winner,dep);
    	                }

     // cout<<"x1 y1"<<x1<<"  "<<y1<<"\n";


            	  for(int i=0;i<8;i++)
            	        	  //for(int j=0;j<8;j++)
            	        	  {
            		  	  	  	  //int j=0;
            	        		  if(parentchild1[i][0]==x1 and parentchild1[i][1]==y1)
            	        		   { x2=parentchild1[i][2];
            	        			  y2=parentchild1[i][3];
            	        			  //cout<<parentchild1[i][0]<<"  "<<parentchild1[i][1]<<"  "<<parentchild1[i][2]<<"  "<<parentchild1[i][3]<<"  ";
            	        			 // cout<<"\n";
            	        			  //cout<<parentchild1[i+1][0]<<"  "<<parentchild1[i+1][1]<<"  "<<parentchild1[i+1][2]<<"  "<<parentchild1[i+1][3]<<"  ";
            	        			  break;
            	        		   }
            	        	  }
            	  /*ofstream out;
            	  	out.open ("output.txt", ios::out | ios::app | ios::binary);
            	  	            	  string line;
            	    ifstream myfile ("output1.txt");
            	    if (myfile.is_open())
            	    {
            	      while ( getline (myfile,line) )
            	      {
            	        out << line << '\n';
            	      }
            	      myfile.close();
            	    }
            	  std::ifstream    inFile("output1.txt");
            	      std::ofstream    outFile("output.txt",ios::app);

            	      outFile << inFile.rdbuf();
            	    writetofile();*/

}


void playGame2(Board *board,char winner,int depth) {
  /* declare some variables */
	//cout<<"here1\n";
  char currentPlayer;
  char countBlack, countWhite;  // # of black and white pieces

  bool hasLegalMove;
  char buf[1024], bufCopied[1024];
  char nextMove;
  int x1, y1,x2,y2;
  const char *color;
  int nUR;  // how many moves to undo/redo
  int countUR;  // how many moves can be actually undo/redo
  char *token, *token2;
  bool hasSpecialCommand;
  bool firstInput;
  bool skip = false;
  char n = board->n;
  char *a = board->a[n];
  char color2;
  player1=COMPUTER;
  player2=COMPUTER;
  int serial=0;
  int pass=0;
  char flipcolors[10];
  int flipr[10][10];

  /* main loop */

	  int parentchild1[10][10]={{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0},
	  			{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0},
	  			{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0}};


    hasLegalMove = findLegalMoves(board, legalMoves);
    currentPlayer = board->wt == X? player1 : player2;
    color = board->wt == X? "X" : "O";
    bool p=false;
      printBoard(board, legalMoves);
      if(hasLegalMove) {
    	  getminimax2(serial,flipr,board,legalMoves,pass,parentchild1,p,winner,depth);

    	  //cout<<"values of x,y "<<flipr[2][3]<<" "<<flipr[2][2]<<" "<<flipr[2][1]<<" "<<flipr[2][0];
    	 // serial++;
    	 x1=flipr[2][0];
    	 y1=flipr[2][1];

    	          if(legalMove(board, x1, y1)) {
    	            makeMove(board, x1, y1,flipcolors);
    	            printBoard(board, legalMoves);
    	            printf("Computer (%s, # %d) played at %c%c.", color,
    	                  board->m, x1+'a', y1+'1');

    	            printf("\n");
    	          }
    	          else {
    	                    printf("Computer (%s, # %d) returned ILLEGAL MOVE: %c%c !! Please debug!\n",
    	                            color, board->m, x1+'a', y1+'1');
    	                    exit(1);
    	          }
      }
    	          else {
    	        	  bool p=true;
    	                  makePass(board,legalMoves);
    	                  pass++;
    	                  getminimax2(serial,flipr,board,legalMoves,pass,parentchild1,p,winner,depth);
    	                }



            	  for(int i=0;i<8;i++)
            	        	  //for(int j=0;j<8;j++)
            	        	  {
            		  	  	  	  //int j=0;
            	        		  if(parentchild1[i][0]==x1 and parentchild1[i][1]==y1)
            	        		   { x2=parentchild1[i][2];
            	        			  y2=parentchild1[i][3];
            	        			  //cout<<parentchild1[i][0]<<"  "<<parentchild1[i][1]<<"  "<<parentchild1[i][2]<<"  "<<parentchild1[i][3]<<"  ";
            	        			  cout<<"\n";
            	        			  //cout<<parentchild1[i+1][0]<<"  "<<parentchild1[i+1][1]<<"  "<<parentchild1[i+1][2]<<"  "<<parentchild1[i+1][3]<<"  ";
            	        			  break;
            	        		   }
            	        	  }


}

int main2()
{
	int a,depth;
	char isFlipped=0;
	bool legalMoves[64];
	fstream file1, file2,file3;
	        char ch;
	  player1 = COMPUTER;
	  player2 = COMPUTER;
	  showLegalMoves = true;
	  ifstream in;
	  in.open("input.txt");
	  in>>a;
	  in>>start;
	  in>>depth;
	  for(int i=0;i<8;i++)
	  {
		  for(int j=0;j<8;j++)
		  {
			  in>>input[i][j];
		  }
	  }
	Board *gb = makeBoard(isFlipped);
	if(a==3)
		playGame1(gb,winner,depth);
	if(a==2)
		playGame2(gb,winner,depth);
	//printBoard(gb,legalMoves);
	return 0;
}
