/*
 * secret.cpp
 *
 *  Created on: Oct 18, 2014
 *      Author: Sapna
 */

#include<iostream>
#include "game.hpp"
#include "board.hpp"
#include "minimax.hpp"
#include<stdio.h>
#include<fstream>


using namespace std;

int eval1[8][8]={{99,-8,8,6,6,8,-8,99},
					{-8,-24,-4,-3,-3,-4,-24,-8},
					{8,-4,7,4,4,7,-4,8},
					{6,-3,4,0,0,4,-3,6},
					{6,-3,4,0,0,4,-3,6},
					{8,-4,7,4,4,7,-4,8},
					{-8,-24,-4,-3,-3,-4,-24,-8},
					{99,-8,8,6,6,8,-8,99}};
int flipreturn1[10][10];
int depth=0;
int countPruning=0;
int countSearching=0;
static char mmBoard[61][64];
int alpha=-1000;
int beta=1000;
char winner2;
void countPieces(char *a, int *nb, int *nw);
void copyBoardArray(char *to, char *from);
int d;
std::ofstream out("output.txt",ios::app);
string inf="Infinity";
string inf2="-Infinity";
string al,be;
void getminimax(int serial,int flipr[10][10],Board *board, bool *legalMoves,int pass,int parentchild1[10][10],bool p,char winners,int dep)
{

	winner2=winners;
	d=dep;

	out<<"Node,Depth,Value,Alpha,Beta\n";
	out<<"root,0,-Infinity,-Infinity,Infinity\n";
	//winner=board->wt;
	Board *z = makeBoard(0);
	depth=0;
	char n = z->n;
	//cout<<"getminimax called...\n";
	int currValue1;
	char currValue2;
	int bestValue=-1000;
	const char* leastValue="Infinity";
	char bestMove = -1;
	char worstMove=1;
	char flipcolors[10];
	char color = board->wt;
	  char *a = board->a[0];
	  char *b = mmBoard[depth];
	  char *b1 = mmBoard[depth+1];
	  z=board;
	  copyBoardArray(b, a);
	  char place;

	  int selfPieces, oppPieces, nFlips;
	  if(winner2=='X')
	  { if(color == X)
	     countPieces(b, &selfPieces, &oppPieces);
	   else
	     countPieces(b, &oppPieces, &selfPieces);}
	  else
	  {
		  if(color == O)
		  	     countPieces(b, &selfPieces, &oppPieces);
		  	   else
		  	     countPieces(b, &oppPieces, &selfPieces);
	  }
	   depth++;
		if(p)
			{
			out<<"pass,1,Infinity,-Infinity,Infinity\n";
			bool nLegalMoves = findLegalMoves(board,legalMoves);
				currValue1 = getMax(board,legalMoves,0, 0, color, depth+2, 0,
					                             selfPieces, oppPieces,flipr,parentchild1,serial,alpha,beta);

			}
	   if(depth==d)
	   {
		   bool nLegalMoves = findLegalMoves(board,legalMoves);
		   int x=evaluate(board,color,legalMoves,flipr,parentchild1,serial,0,0,alpha,beta);
		   return;
	   }
	   for(char y=0; y<8; y++) {
	       for(char x=0; x<8; x++) {

	    	 bool nLegalMoves = findLegalMoves(board,legalMoves);
	    	 if(nLegalMoves)
	    	 {
	         place = CONV_21(x, y);
	         if(legalMoves[place]){
	         copyBoardArray(b1, b);
	         nFlips = tryMove(b1, color, x, y);
	         int k=x+97;
	         char l=k;

	         //cout<<l<<"\n";

	        	 out<<l<<y+1<<","<<depth<<","<<leastValue<<","<<alpha<<","<<beta<<"\n";


	         makeMove(z,x,y,flipcolors);
	        // printBoard(z,legalMoves);
	         parentchild1[serial][0]=x;
	         parentchild1[serial][1]=y;
	         currValue1 = getMin(z,legalMoves,x, y, OTHER(color), depth+1, 0,
	                             selfPieces+nFlips+eval1[x][y], oppPieces-nFlips,flipr,parentchild1,serial,alpha,beta);
	           //cout<<"curr value is "<<currValue1<<"\n";
	         if(currValue1 > bestValue) {
	        	 //cout<<"come here!!";

	        	 flipr[2][0]=x;
	        	 flipr[2][1]=y;
	           bestMove = place;
	           bestValue = currValue1;
	         }
	         if(bestValue > alpha)
	         {       alpha = bestValue;}
	         if(currValue1==-1)
	         {
	        	 bool nLegalMoves = findLegalMoves(board,legalMoves);
	        	 		   int bestValue=evaluate(board,color,legalMoves,flipr,parentchild1,serial,0,0,alpha,beta);

	         }
	         //printf("%c%c,%d,%s,%d,%d\n",x+'a',y+'1',depth,bestValue,alpha,beta);


	         	        	 out<<"root"<<","<<0<<","<<bestValue<<","<<alpha<<","<<beta<<"\n";


	         serial++;
	         a[CONV_21(x, y)]=EMPTY;
	         undoMove(z,flipcolors);

	     }
	    }
	    	 else
	    	 {

	    	 }
	   }
	   }
	 return;
}

int getMin(Board *b,bool *legalMoves,char lastx, char lasty, char color, char depth, char passes,
             char selfPieces, char oppPieces,int flipr[10][10],int parentchild1[10][10],int i,int alpha,int beta) {
	//cout<<"getmin called..."<<lastx<<"  "<<lasty<<"\n";
	if(COUNT_PRUNING)
	    countSearching++;
	Board *z2 = makeBoard(0);
	char n = z2->n;
		  z2=b;
		  char flipcolors[10];

	char *a = mmBoard[depth];
	  char *c = mmBoard[depth+1]; // a1 is the board config. after (depth+1)th search move.
	  char nFlips;
	  char x, y, place;
	  int minValue = 1000;
	  int min=1000;
	   int currValue;
	   b->wt=color;
	   //cout<<"makemove added..\n";
	   if(depth==d)
	   	               			   {
	   	               				currValue=evaluate(b,color,legalMoves,flipr,parentchild1,depth,lastx,lasty,alpha,beta);

	   	               				if(currValue < minValue)
	   	               				{
	   	               					minValue = currValue;
	   	               				}

	   	               				//printf("%c%c,%d,%d\n",lastx+'a',lasty+'1',depth,currValue);

	   	               			   }
	   else {
		   bool nLegalMoves = findLegalMoves(b,legalMoves);
		   	    	   if(nLegalMoves){
	   for(y=0; y<8; y++)
	       for(x=0; x<8; x++) {

	         place = CONV_21(x, y);
	               if(legalMoves[place]) { // move is legal
	                 copyBoardArray(c, a);
	                 nFlips = tryMove(c, color, x, y);
	                 makeMove(z2,x,y,flipcolors);
	                 //printBoard(z2,legalMoves);
	                 int k=x+97;
	                 char l=k;

	                 	         	        	out<<l<<y+1<<","<<depth<<","<<min<<","<<alpha<<","<<beta<<"\n";

	                	 //printf("%c%c,%d,%d,%d,%d\n",x+'a',y+'1',depth,min,alpha,beta);
	                 currValue = getMax(b,legalMoves,x, y, OTHER(color), depth+1, passes,
	                                    selfPieces-nFlips, oppPieces+nFlips+eval1[x][y],flipr,parentchild1,i,alpha,beta);


	                 if(currValue < minValue)
	                          minValue = currValue;
	                        if(minValue < beta)
	                          beta = minValue;
	                        if(alpha >= beta) { // prun brunch as soon as alpha and beta crosses.
	                          if(COUNT_PRUNING)
	                            countPruning++;
	                          return beta;
	               }
	                        //printf("%c%c,%d,%d,%d,%d\n",lastx+'a',lasty+'1',depth-1,minValue,alpha,beta);
	                        k=lastx+97;
	                        	                  l=k;

	                        	                  	                 	         	         out<<l<<lasty+1<<","<<depth-1<<","<<minValue<<","<<alpha<<","<<beta<<"\n";

	                        	                // out<<l<<lasty+1<<","<<depth-1<<","<<minValue<<","<<alpha<<","<<beta<<"\n";
	                   	                 a[CONV_21(x, y)]=EMPTY;
	                   	                 	         undoMove(z2,flipcolors);

	                 }
	       }
	   }
	   else
	   {
		  return -1;

	   }
	   }
	   return minValue;
}

int getMax(Board *b,bool *legalMoves,char lastx, char lasty, char color, char depth, char passes,
             char selfPieces, char oppPieces,int flipr[10][10],int parentchild1[10][10],int i,int alpha,int beta) {
	//cout<<"getmax called..."<<lastx<<"  "<<lasty<<"\n";
	if(COUNT_PRUNING)
	    countSearching++;
	Board *z3 = makeBoard(0);
		char n = z3->n;
			  z3=b;
			  char flipcolors[10];

	char *a = mmBoard[depth];
		  char *a1 = mmBoard[depth+1]; // a1 is the board config. after (depth+1)th search move.
		  	  //copyBoardArray(, a);
		  char nFlips;
		  char x, y, place;
		  int maxValue = -1000;
		  int max=-1000;
		   int currValue;
		   bool nLegalMoves = findLegalMoves(b,legalMoves);
		   if(depth==d)
		   		                			   {
		   		                	 	 	 	 	 	currValue=evaluate(b,color,legalMoves,flipr,parentchild1,depth,lastx,lasty,alpha,beta);
		   		                	 	               	if(currValue > maxValue)
		   		                	 	               		maxValue = currValue;
		   		                			   }

		   else{
		   for(y=0; y<8; y++)
		       for(x=0; x<8; x++) {
		         place = CONV_21(x, y);
		               if(legalMoves[place]) { // move is legal
		                 copyBoardArray(a1, a);
		                 nFlips = tryMove(a1, color, x, y);
		                 makeMove(z3,x,y,flipcolors);
		                 //printf("%c%c,%d,%d,%d,%d\n",x+'a',y+'1',depth,max,alpha,beta);
		                 int k=x+97;
		                 	                 char l=k;

		                 	                 out<<l<<y+1<<","<<depth<<","<<max<<","<<alpha<<","<<beta<<"\n";
		                 currValue = getMin(b,legalMoves,x, y, OTHER(color), depth+1, passes,
		                                    selfPieces-nFlips, oppPieces+nFlips+eval1[x][y],flipr,parentchild1,i,alpha,beta);
		                 if(currValue > maxValue)
		                           maxValue = currValue;
		                 if(maxValue > alpha)
		                           alpha = maxValue;
		                         if(alpha >= beta) { // prun brunch as soon as alpha and beta crosses.
		                           if(COUNT_PRUNING)
		                             countPruning++;
		                           return alpha;
		                         }

		                 }
		               int k=lastx+97;
		              char l=k;
		              if(alpha==-1000)

		              	out<<l<<lasty+1<<","<<depth-1<<","<<maxValue<<","<<alpha<<","<<beta<<"\n";
		              // printf("%c%c,%d,%d,%d,%d\n",lastx+'a',lasty+'1',depth-1,maxValue,alpha,beta);
		               a[CONV_21(x, y)]=EMPTY;
		               	                 	         undoMove(z3,flipcolors);
		                 }

		       }
		   //cout<<"maxavlue = "<<maxValue<<"\n";
		   return maxValue;
}

void countPieces(char *a, int *nb, int *nw) {
	//cout<<"getcount called...\n";
char place;
*nb=0;
*nw=0;
for(char i=0; i<8; i++)
       	{
       		    for(char j=0; j<8; j++)
       		    {
       		      place = CONV_21(j, i);
       		      if(a[place] == X)
       		      {
       		      	 *nb=*nb+eval1[i][j];
       		      }
       		      else if(a[place] == O)
       		      {
       		      	*nw=*nw+eval1[i][j];
       		       }
       		      //sum+=eval[i][j];
       		    }
       	}
return;
}

void copyBoardArray(char *to, char *from) {
  for(int i=0; i<60; i+=6) { // some loop unrolling
    to[i] = from[i];
    to[i+1] = from[i+1];
    to[i+2] = from[i+2];
    to[i+3] = from[i+3];
    to[i+4] = from[i+4];
    to[i+5] = from[i+5];
  }
  to[60] = from[60];
  to[61] = from[61];
  to[62] = from[62];
  to[63] = from[63];
}

int evaluate(Board *b,char color,bool *lm,int flipr[10][10],int parentchild1[10][10],int depth,char lastx,char lasty,int alpha,int beta)
{
	//cout<<"evaluate called...";
	         		  char n = b->n;
	         		  char place;
	         		  char *a = b->a[n];
	         		  int flip=0;
	         		  int bwt=0;
	         		  int wwt=0;
	         		  bool result=false;
	         		  int x2=flipr[3][1];
	         		  int y2=flipr[3][2];
	         		 int y1=flipr[4][1];
	         		 int x1=flipr[4][2];
	         	  	for(char i=0; i<8; i++)
	         	         	{
	         	         		    for(char j=0; j<8; j++)
	         	         		    {
	         	         		      place = CONV_21(j, i);
	         	         		      if(a[place] == X)
	         	         		      {  bwt=bwt+eval1[i][j];
	         	         		      }
	         	         		      else if(a[place] == O)
	         	         		      { wwt=wwt+eval1[i][j];
	         	         		       }
	         	         		    }
	         	         	}
	         	  	int maxflip=-1000;
	         	  	int minflip=1000;
	         	  	for(int y=0; y<8; y++) {
	         	  	    for(int x=0; x<8; x++) {
	         	  	      lm[CONV_21(x, y)] = legalMove(b, x, y);
	         	  	      if(lm[CONV_21(x, y)])
	         	  	      {
	         	  	    	  result=true;
	         	  	    	//printf("%c%c\n",x+'a',y+'1');
	         	  		flip=tryMove(a,color,x,y);
	         	        //cout<<"flip eval vlaue: "<<flip<<"\n";
	         	        if(winner2=='X')
	         	        {
	         	        	//cout<<"here...";
	         	        	if(color==X)

	         	        {
	         	        		int k=x+97;
	         	        				              char l=k;

	         	        				              	out<<l<<y+1<<","<<depth<<","<<(flip+bwt+eval1[y][x])-(wwt)<<","<<alpha<<","<<beta<<"\n";
	         	        	//printf("%c%c,%d,%d,%d,%d\n",x+'a',y+'1',depth,(flip+bwt+eval1[y][x])-(wwt),alpha,beta);
	         	        	//printf("%c%c\n",x+'a',y+'1');
	         	        	//cout<<"color X\n";
	         	        if(((flip+bwt+eval1[y][x])-(wwt))>maxflip)
	         	        {
	         	        	maxflip=(flip+bwt+eval1[y][x])-(wwt);
	         	        	x1=y;
	         	        	y1=x;
	         	        }
	         	        if(((flip+bwt+eval1[y][x])-(wwt))==maxflip)
	         	        {
	         	        	if(y<x1)
	         	        	{	x1=y;
	         	        		y1=x;}
	         	        	else if(y==x1 and x<y1)
	         	        	{	x1=y;
	         	        		y1=x;}

	         	        }
	         	        flipreturn1[0][0]=maxflip;
	         	        flipreturn1[0][1]=y1;
	         	        flipreturn1[0][2]=x1;
	         	       k=lastx+97;
	         	       l=k;

	         	       out<<l<<lasty+1<<","<<depth-1<<","<<maxflip<<","<<alpha<<","<<beta<<"\n";
	         	       //printf("%c%c,%d,%d,%d,%d\n",lastx+'a',lasty+'1',depth-1,maxflip,alpha,beta);
	         	       	   	//cout<<color<<" maxflip:"<<maxflip<<"  x and y : "<<x1<<"  "<<y1<<"\n";
	         	        }
	         	        if(color==O)
	         	        {
	         	        	int k=x+97;
	         	        	char l=k;

	         	        	out<<l<<y+1<<","<<depth<<","<<(bwt-(flip+wwt+eval1[y][x]))<<","<<alpha<<","<<beta<<"\n";
	         	        	//printf("%c%c,%d,%d,%d,%d\n",x+'a',y+'1',depth,(bwt-(flip+wwt+eval1[y][x])),alpha,beta);

	         	        	//printf("%c%c\n",x+'a',y+'1');
	         	        	//cout<<"color O\n";
	         	        	if((bwt-(flip+wwt+eval1[y][x]))<minflip)
	         	        	        {
	         	        	        	minflip=bwt-(flip+wwt+eval1[y][x]);
	         	        	        	x1=y;
	         	        	        	y1=x;
	         	        	        }
	         	        	        if((bwt-(flip+wwt+eval1[y][x]))==minflip)
	         	        	        {
	         	        	        	if(y<x1)
	         	        	        	{	x1=y;
	         	        	        		y1=x;}
	         	        	        	else if(y==x1 and x<y1)
	         	        	        	{	x1=y;
	         	        	        		y1=x;}
	         	        	        }
	         	        	        flipreturn1[1][0]=minflip;
	         	        	        flipreturn1[1][1]=y1;
	         	        	        flipreturn1[1][2]=x1;

	         	        	       	   	               				if(minflip < beta)
	         	        	       	   	               				{
	         	        	       	   	               					//cout<<"here";
	         	        	       	   	               				      beta = minflip;
	         	        	       	   	               				      //cout<<"beta is "<<beta<<"\n";
	         	        	       	   	               				}
	         	        	       	   	               				if(alpha >= beta) { // prun brunch as soon as alpha and beta crosses.
	         	        	       	   	               				                          if(COUNT_PRUNING)
	         	        	       	   	               				                            countPruning++;
	         	        	       	   	               				                int k=lastx+97;
	         	        	       	   	               				               char l=k;

	         	        	       	   	               				              out<<l<<lasty+1<<","<<depth-1<<","<<minflip<<","<<alpha<<","<<beta<<"\n";
	         	        	       	   	               				                //printf("%c%c,%d,%d,%d,%d\n",lastx+'a',lasty+'1',depth-1,minflip,alpha,beta);
	         	        	       	   	               				                          return beta;
	         	        	       	   	               				}
	         	        	       	   	               		k=lastx+97;
	         	        	       	   	              l=k;

	         	        	       	   	              out<<l<<lasty+1<<","<<depth-1<<","<<minflip<<","<<alpha<<","<<beta<<"\n";
	         	        	       //printf("%c%c,%d,%d,%d,%d\n",lastx+'a',lasty+'1',depth-1,minflip,alpha,beta);
	         	        	     //cout<<color<<" minflip:"<<minflip<<"  x and y : "<<x1<<"  "<<y1<<"\n";
	         	        }
	         	  	 }

	         	  	if(winner2=='O')
	         	  		         	        {
	         	  		         	        	if(color==O)

	         	  		         	        {
	         	  		         	        	//cout<<"Calledddd\n";
	         	  		         	        	int k=x+97;
	         	  		         	        	char l=k;

	         	  		         	        	out<<l<<y+1<<","<<depth<<","<<(flip+wwt+eval1[y][x])-(bwt)<<","<<alpha<<","<<beta<<"\n";
	         	  		         	        	//printf("%c%c,%d,%d,%d,%d\n",x+'a',y+'1',depth,(flip+wwt+eval1[y][x])-(bwt),alpha,beta);
	         	  		         	        	//printf("%c%c\n",x+'a',y+'1');
	         	  		         	        	//cout<<"color X\n";
	         	  		         	        if(((flip+wwt+eval1[y][x])-(bwt))>maxflip)
	         	  		         	        {
	         	  		         	        	maxflip=(flip+wwt+eval1[y][x])-(bwt);
	         	  		         	        	x1=y;
	         	  		         	        	y1=x;
	         	  		         	        }
	         	  		         	        if(((flip+wwt+eval1[y][x])-(bwt))==maxflip)
	         	  		         	        {
	         	  		         	        	if(y<x1)
	         	  		         	        	{	x1=y;
	         	  		         	        		y1=x;}
	         	  		         	        	else if(y==x1 and x<y1)
	         	  		         	        	{	x1=y;
	         	  		         	        		y1=x;}

	         	  		         	        }
	         	  		         	        flipreturn1[0][0]=maxflip;
	         	  		         	        flipreturn1[0][1]=y1;
	         	  		         	        flipreturn1[0][2]=x1;
	         	  		         	    k=lastx+97;
	         	  		         	   l=k;

	         	  		         	    out<<l<<lasty+1<<","<<depth-1<<","<<maxflip<<","<<alpha<<","<<beta<<"\n";
	         	  		         	       //printf("%c%c,%d,%d,%d,%d\n",lastx+'a',lasty+'1',depth-1,maxflip,alpha,beta);
	         	  		         	       	   	//cout<<color<<" maxflip:"<<maxflip<<"  x and y : "<<x1<<"  "<<y1<<"\n";
	         	  		         	        }
	         	  		         	        if(color==X)
	         	  		         	        {
	         	  		         	        	//cout<<"Calledddd\n";
	         	  		         	        int k=x+97;
	         	  		         	        	         	  		         	   char l=k;

	         	  		         	        	         	  		         	    out<<l<<y+1<<","<<depth<<","<<(wwt-(flip+bwt+eval1[y][x]))<<","<<alpha<<","<<beta<<"\n";
	         	  		         	        	//printf("%c%c,%d,%d,%d,%d\n",x+'a',y+'1',depth,(wwt-(flip+bwt+eval1[y][x])),alpha,beta);

	         	  		         	        	//printf("%c%c\n",x+'a',y+'1');
	         	  		         	        	//cout<<"color O\n";
	         	  		         	        	if((wwt-(flip+bwt+eval1[y][x]))<minflip)
	         	  		         	        	        {
	         	  		         	        	        	minflip=wwt-(flip+bwt+eval1[y][x]);
	         	  		         	        	        	x1=y;
	         	  		         	        	        	y1=x;
	         	  		         	        	        }
	         	  		         	        	        if((wwt-(flip+bwt+eval1[y][x]))==minflip)
	         	  		         	        	        {
	         	  		         	        	        	if(y<x1)
	         	  		         	        	        	{	x1=y;
	         	  		         	        	        		y1=x;}
	         	  		         	        	        	else if(y==x1 and x<y1)
	         	  		         	        	        	{	x1=y;
	         	  		         	        	        		y1=x;}
	         	  		         	        	        }
	         	  		         	        	        flipreturn1[1][0]=minflip;
	         	  		         	        	        flipreturn1[1][1]=y1;
	         	  		         	        	        flipreturn1[1][2]=x1;

	         	  		         	        	       	   	               				if(minflip < beta)
	         	  		         	        	       	   	               				{
	         	  		         	        	       	   	               					//cout<<"here";
	         	  		         	        	       	   	               				      beta = minflip;
	         	  		         	        	       	   	               				      //cout<<"beta is "<<beta<<"\n";
	         	  		         	        	       	   	               				}
	         	  		         	        	       	   	               				if(alpha >= beta) { // prun brunch as soon as alpha and beta crosses.
	         	  		         	        	       	   	               				                          if(COUNT_PRUNING)
	         	  		         	        	       	   	               				                            countPruning++;
	         	  		         	        	       	   	               				             int k=lastx+97;
	         	  		         	        	       	   	               				             char l=k;

	         	  		         	        	       	   	               				             out<<l<<lasty+1<<","<<depth-1<<","<<minflip<<","<<alpha<<","<<beta<<"\n";
	         	  		         	        	       	   	               				                //printf("%c%c,%d,%d,%d,%d\n",lastx+'a',lasty+'1',depth-1,minflip,alpha,beta);
	         	  		         	        	       	   	               				                          return beta;
	         	  		         	        	       	   	               				}
	         	  		         	        	       	   	               	k=lastx+97;
	         	  		         	        	       	   	               	l=k;

	         	  		         	        	       	   	               	out<<k<<lasty+1<<","<<depth-1<<","<<minflip<<","<<alpha<<","<<beta<<"\n";
	         	  		         	        	      // printf("%c%c,%d,%d,%d,%d\n",lastx+'a',lasty+'1',depth-1,minflip,alpha,beta);
	         	  		         	        	     //cout<<color<<" minflip:"<<minflip<<"  x and y : "<<x1<<"  "<<y1<<"\n";
	         	  		         	        }
	         	  		         	  	 }
	         	  		         	  }
	         	  		         }
	         	  	}
	         	  	if(winner2=='X'){
	         	  	if(color==X)
	         	    {
	         	  			   	flipr[0][0]=flipreturn1[0][0];
	         	  			   	flipr[0][1]=flipreturn1[0][1];
	         	  			   	flipr[0][2]=flipreturn1[0][2];
	         	  			   	return maxflip;
	         	    }
	         	  	else
	         	  	{
	         	  			   	flipr[1][0]=flipreturn1[1][0];
	         	  			   	flipr[1][1]=flipreturn1[1][1];
	         	  			   	flipr[1][2]=flipreturn1[1][2];
	         	  			   	return minflip;
	         	    }}
	         	  	else
	         	  	{
	         	  		if(color==O)
	         	  			         	    {
	         	  			         	  			   	flipr[0][0]=flipreturn1[0][0];
	         	  			         	  			   	flipr[0][1]=flipreturn1[0][1];
	         	  			         	  			   	flipr[0][2]=flipreturn1[0][2];
	         	  			         	  			   	return maxflip;
	         	  			         	    }
	         	  			         	  	else
	         	  			         	  	{
	         	  			         	  			   	flipr[1][0]=flipreturn1[1][0];
	         	  			         	  			   	flipr[1][1]=flipreturn1[1][1];
	         	  			         	  			   	flipr[1][2]=flipreturn1[1][2];
	         	  			         	  			   	return minflip;
	         	  			         	    }
	         	  	}

	         }
