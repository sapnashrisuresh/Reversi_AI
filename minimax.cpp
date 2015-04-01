/*
 * secret.cpp
 *
 *  Created on: Oct 18, 2014
 *      Author: Sapna
 */

#include<iostream>
#include<stdio.h>
#include "game.hpp"
#include "board.hpp"
#include "minimax.hpp"
#include<fstream>

using namespace std;

int eval2[8][8]={{99,-8,8,6,6,8,-8,99},
					{-8,-24,-4,-3,-3,-4,-24,-8},
					{8,-4,7,4,4,7,-4,8},
					{6,-3,4,0,0,4,-3,6},
					{6,-3,4,0,0,4,-3,6},
					{8,-4,7,4,4,7,-4,8},
					{-8,-24,-4,-3,-3,-4,-24,-8},
					{99,-8,8,6,6,8,-8,99}};
int flipreturn2[10][10];
int d2;
int depth1=0;
char winner4;
static char mmBoard2[61][64];
void countPieces2(char *a, int *nb, int *nw);
void copyBoardArray2(char *to, char *from);
std::ofstream out1("output.txt",ios::app);

void getminimax2(int serial,int flipr[10][10],Board *board, bool *legalMoves,int pass,int parentchild1[10][10],bool p,char winners,int dep)
{

	out1<<"Node,Depth,Value\n";
	out1<<"root,0,-Infinity\n";
	winner4=winners;
	d2=dep;
	Board *z = makeBoard(0);
	depth1=0;
	char n = z->n;
	//cout1<<"getminimax called...\n";
	int currValue1;
	char currValue2;
	int bestValue=-1000;
	const char* leastValue="Infinity";
	char bestMove = -1;
	char worstMove=1;
	char flipcolors[10];
	char color = board->wt;
	  char *a = board->a[0];
	  char *b = mmBoard2[depth1];
	  char *b1 = mmBoard2[depth1+1];
	  z=board;
	  copyBoardArray2(b, a);
	  char place;
	  int selfPieces, oppPieces, nFlips;
	  if(winner4=='X')
	  	  {
	   if(color == X)
	     countPieces2(b, &selfPieces, &oppPieces);
	   else
	     countPieces2(b, &oppPieces, &selfPieces);}
	  else{
		  if(color == O)
		  		  	     countPieces2(b, &selfPieces, &oppPieces);
		  else
			  countPieces2(b, &oppPieces, &selfPieces);}

	   depth1++;
		if(p)
			{
			out1<<"pass,1,Infinity\n";
			bool nLegalMoves = findLegalMoves(board,legalMoves);
				currValue1 = getMax2(board,legalMoves,0, 0, color, depth1+1, 0,
					                             selfPieces, oppPieces,flipr,parentchild1,serial);

			}
	   if(depth1==d2)
	   {
		   bool nLegalMoves = findLegalMoves(board,legalMoves);
		   int x=evaluate2(board,color,legalMoves,flipr,parentchild1,serial,0,0);
		   return;
	   }
	   for(char y=0; y<8; y++) {
	       for(char x=0; x<8; x++) {

	    	 bool nLegalMoves = findLegalMoves(board,legalMoves);
	         place = CONV_21(x, y);
	         if(legalMoves[place]){
	         copyBoardArray(b1, b);
	         nFlips = tryMove(b1, color, x, y);
	         //printf("%c%c,%d,%s\n",x+'a',y+'1',depth1,leastValue);
	         int k=x+97;
	         	         char l=k;
	         	         out1<<l<<y+1<<","<<depth1<<","<<leastValue<<"\n";
	         makeMove(z,x,y,flipcolors);
	         parentchild1[serial][0]=x;
	         parentchild1[serial][1]=y;
	         currValue1 = getMin2(z,legalMoves,x, y, OTHER(color), depth1+1, 0,
	                             selfPieces+nFlips+eval2[x][y], oppPieces-nFlips,flipr,parentchild1,serial);
	           //cout1<<"curr value is "<<currValue1<<"\n";
	         if(currValue1 > bestValue) {
	        	 //cout1<<"come here!!";

	        	 flipr[2][0]=x;
	        	 flipr[2][1]=y;
	           bestMove = place;
	           bestValue = currValue1;
	         }
	         //printf("%s,%d,%d\n","root",0,bestValue);
	         out1<<"root"<<","<<0<<","<<bestValue<<"\n";
	         serial++;
	         a[CONV_21(x, y)]=EMPTY;
	         undoMove(z,flipcolors);

	     }
	    }
	   }
	 return;
}

int getMin2(Board *b,bool *legalMoves,char lastx, char lasty, char color, char depth1, char passes,
             char selfPieces, char oppPieces,int flipr[10][10],int parentchild1[10][10],int i) {
	//cout<<"getmin called..."<<lastx<<"  "<<lasty<<"\n";
	Board *z2 = makeBoard(0);
	char n = z2->n;
		  z2=b;
		  char flipcolors[10];

	char *a = mmBoard2[depth1];
	  char *c = mmBoard2[depth1+1]; // a1 is the board config. after (depth1+1)th search move.
	  char nFlips;
	  char x, y, place;
	  int minValue = 1000;
	  int min=1000;
	   int currValue;
	   b->wt=color;
	   //cout<<"makemove added..\n";
	   if(depth1==d2)
	   	               			   {
	   	               				currValue=evaluate2(b,color,legalMoves,flipr,parentchild1,depth1,lastx,lasty);

	   	               				if(currValue < minValue)
	   	               				{
	   	               					minValue = currValue;
	   	               				}
	   	               				//printf("%c%c,%d,%d\n",lastx+'a',lasty+'1',depth1,currValue);

	   	               			   }
	   else {
	   for(y=0; y<8; y++)
	       for(x=0; x<8; x++) {
	    	   bool nLegalMoves = findLegalMoves(b,legalMoves);
	         place = CONV_21(x, y);
	               if(legalMoves[place]) { // move is legal
	                 copyBoardArray(c, a);
	                 nFlips = tryMove(c, color, x, y);
	                 makeMove(z2,x,y,flipcolors);
	                	// printf("%c%c,%d,%d\n",x+'a',y+'1',depth1,min);
	                	 int k=x+97;
	                	 	         	         char l=k;
	                	 	         	         out1<<l<<y+1<<","<<depth1<<","<<min<<"\n";
	                 currValue = getMax2(b,legalMoves,x, y, OTHER(color), depth1+1, passes,
	                                    selfPieces-nFlips, oppPieces+nFlips+eval2[x][y],flipr,parentchild1,i);


	                 if(currValue < minValue)
	                 {
	                	 minValue = currValue;
	                 }
	                 //printf("%c%c,%d,%d\n",lastx+'a',lasty+'1',depth1-1,minValue);
	                 k=lastx+97;
	                 	         	          l=k;
	                 	         	         out1<<l<<lasty+1<<","<<depth1-1<<","<<minValue<<"\n";
	                 a[CONV_21(x, y)]=EMPTY;
	                 	         undoMove(z2,flipcolors);
	                 }
	                 }
	       }
	   return minValue;
}

int getMax2(Board *b,bool *legalMoves,char lastx, char lasty, char color, char depth1, char passes,
             char selfPieces, char oppPieces,int flipr[10][10],int parentchild1[10][10],int i) {
	//cout1<<"getmax called..."<<lastx<<"  "<<lasty<<"\n";
	Board *z3 = makeBoard(0);
		char n = z3->n;
			  z3=b;
			  char flipcolors[10];

	char *a = mmBoard2[depth1];
		  char *a1 = mmBoard2[depth1+1]; // a1 is the board config. after (depth1+1)th search move.
		  	  //copyBoardArray(, a);
		  char nFlips;
		  char x, y, place;
		  int maxValue = -1000;
		  int max=-1000;
		   int currValue;
		   bool nLegalMoves = findLegalMoves(b,legalMoves);
		   if(depth1==d2)
		   		                			   {
		   		                	 	 	 	 	 	currValue=evaluate2(b,color,legalMoves,flipr,parentchild1,depth1,lastx,lasty);
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
		                 //printf("%c%c,%d,%d\n",x+'a',y+'1',depth1,max);
		                 int k=x+97;
		                 	                 	         	         char l=k;
		                 	                 	         	         out1<<l<<y+1<<","<<depth1<<","<<max<<"\n";
		                 currValue = getMin2(b,legalMoves,x, y, OTHER(color), depth1+1, passes,
		                                    selfPieces-nFlips, oppPieces+nFlips+eval2[x][y],flipr,parentchild1,i);
		                 if(currValue > maxValue)
		                           maxValue = currValue;
		                 }
		               int k=lastx+97;
		               	char l=k;
		               out1<<l<<lasty+1<<","<<depth1-1<<","<<maxValue<<"\n";
		              // printf("%c%c,%d,%d\n",lastx+'a',lasty+'1',depth1-1,maxValue);
		               a[CONV_21(x, y)]=EMPTY;
		               	                 	         undoMove(z3,flipcolors);
		                 }

		       }
		   //cout1<<"maxavlue = "<<maxValue<<"\n";
		   return maxValue;
}

void countPieces2(char *a, int *nb, int *nw) {
	//cout1<<"getcount called...\n";
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
       		      	 *nb=*nb+eval2[i][j];
       		      }
       		      else if(a[place] == O)
       		      {
       		      	*nw=*nw+eval2[i][j];
       		       }
       		      //sum+=eval[i][j];
       		    }
       	}
return;
}

void copyBoardArray2(char *to, char *from) {
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

int evaluate2(Board *b,char color,bool *lm,int flipr[10][10],int parentchild1[10][10],int depth1,char lastx,char lasty)
{
	//cout1<<"evaluate called...";
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
	         	         		      {  bwt=bwt+eval2[i][j];
	         	         		      }
	         	         		      else if(a[place] == O)
	         	         		      { wwt=wwt+eval2[i][j];
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
	         	        //cout1<<"flip eval vlaue: "<<flip<<"\n";
	         	  	 if(winner4=='X')
	         	  	 {
	         	        if(color==X)
	         	        {
	         	        	int k=x+97;
	         	        			               	char l=k;
	         	        			               out1<<l<<y+1<<","<<depth1-1<<","<<(flip+bwt+eval2[y][x])-(wwt)<<"\n";
	         	        	//printf("%c%c,%d,%d\n",x+'a',y+'1',depth1,(flip+bwt+eval2[y][x])-(wwt));
	         	        	//printf("%c%c\n",x+'a',y+'1');
	         	        	//cout1<<"color X\n";
	         	        if(((flip+bwt+eval2[y][x])-(wwt))>maxflip)
	         	        {
	         	        	maxflip=(flip+bwt+eval2[y][x])-(wwt);
	         	        	x1=y;
	         	        	y1=x;
	         	        }
	         	        if(((flip+bwt+eval2[y][x])-(wwt))==maxflip)
	         	        {
	         	        	if(y<x1)
	         	        	{	x1=y;
	         	        		y1=x;}
	         	        	else if(y==x1 and x<y1)
	         	        	{	x1=y;
	         	        		y1=x;}

	         	        }
	         	        flipreturn2[0][0]=maxflip;
	         	        flipreturn2[0][1]=y1;
	         	        flipreturn2[0][2]=x1;
	         	       //printf("%c%c,%d,%d\n",lastx+'a',lasty+'1',depth1-1,maxflip);
	         	       k=lastx+97;
	         	      	         	        			               	 l=k;
	         	      	         	        			               out1<<l<<lasty+1<<","<<depth1-1<<","<<maxflip<<"\n";
	         	       	   	//cout1<<color<<" maxflip:"<<maxflip<<"  x and y : "<<x1<<"  "<<y1<<"\n";
	         	        }
	         	        if(color==O)
	         	        {
	         	        	//printf("%c%c,%d,%d\n",x+'a',y+'1',depth1,(bwt-(flip+wwt+eval2[y][x])));
	         	        	int k=x+97;
	         	        		   char l=k;
	         	        		   out1<<l<<y+1<<","<<depth1<<","<<(bwt-(flip+wwt+eval2[y][x]))<<"\n";
	         	        	//printf("%c%c\n",x+'a',y+'1');
	         	        	//cout1<<"color O\n";
	         	        	if((bwt-(flip+wwt+eval2[y][x]))<minflip)
	         	        	        {
	         	        	        	minflip=bwt-(flip+wwt+eval2[y][x]);
	         	        	        	x1=y;
	         	        	        	y1=x;
	         	        	        }
	         	        	        if((bwt-(flip+wwt+eval2[y][x]))==minflip)
	         	        	        {
	         	        	        	if(y<x1)
	         	        	        	{	x1=y;
	         	        	        		y1=x;}
	         	        	        	else if(y==x1 and x<y1)
	         	        	        	{	x1=y;
	         	        	        		y1=x;}
	         	        	        }
	         	        	        flipreturn2[1][0]=minflip;
	         	        	        flipreturn2[1][1]=y1;
	         	        	        flipreturn2[1][2]=x1;
	         	        	      // printf("%c%c,%d,%d\n",lastx+'a',lasty+'1',depth1-1,minflip);
	         	        	       k=lastx+97;
	         	        	      	         	        		    l=k;
	         	        	      	         	        		   out1<<l<<lasty+1<<","<<depth1-1<<","<<minflip<<"\n";
	         	        	     //cout1<<color<<" minflip:"<<minflip<<"  x and y : "<<x1<<"  "<<y1<<"\n";
	         	        }
	         	  	 }
	         	  	if(winner4=='O')
	         	  		         	  		         	        {
	         	  		         	  		         	        	if(color==O)

	         	  		         	  		         	        {
	         	  		         	  		         	        	//cout1<<"Calledddd\n";
	         	  		         	  		         	        	int k=x+97;
	         	  		         	  		         	        		         	        	      char l=k;
	         	  		         	  		         	        		         	        	      out1<<l<<y+1<<","<<depth1<<","<<(flip+wwt+eval2[y][x])-(bwt)<<"\n";
	         	  		         	  		         	        	//printf("%c%c,%d,%d\n",x+'a',y+'1',depth1,(flip+wwt+eval2[y][x])-(bwt));
	         	  		         	  		         	        	//printf("%c%c\n",x+'a',y+'1');
	         	  		         	  		         	        	//cout1<<"color X\n";
	         	  		         	  		         	        if(((flip+wwt+eval2[y][x])-(bwt))>maxflip)
	         	  		         	  		         	        {
	         	  		         	  		         	        	maxflip=(flip+wwt+eval2[y][x])-(bwt);
	         	  		         	  		         	        	x1=y;
	         	  		         	  		         	        	y1=x;
	         	  		         	  		         	        }
	         	  		         	  		         	        if(((flip+wwt+eval2[y][x])-(bwt))==maxflip)
	         	  		         	  		         	        {
	         	  		         	  		         	        	if(y<x1)
	         	  		         	  		         	        	{	x1=y;
	         	  		         	  		         	        		y1=x;}
	         	  		         	  		         	        	else if(y==x1 and x<y1)
	         	  		         	  		         	        	{	x1=y;
	         	  		         	  		         	        		y1=x;}

	         	  		         	  		         	        }
	         	  		         	  		         	        flipreturn2[0][0]=maxflip;
	         	  		         	  		         	        flipreturn2[0][1]=y1;
	         	  		         	  		         	        flipreturn2[0][2]=x1;
	         	  		         	  		         	  k=lastx+97;
	         	  		         	  		         	 l=k;
	         	  		         	  		         	 out1<<l<<lasty+1<<","<<depth1-1<<","<<maxflip<<"\n";
	         	  		         	  		         	     //  printf("%c%c,%d,%d\n",lastx+'a',lasty+'1',depth1-1,maxflip);
	         	  		         	  		         	       	   	//cout1<<color<<" maxflip:"<<maxflip<<"  x and y : "<<x1<<"  "<<y1<<"\n";
	         	  		         	  		         	        }
	         	  		         	  		         	        if(color==X)
	         	  		         	  		         	        {
	         	  		         	  		         	        	//cout1<<"Calledddd\n";
	         	  		         	  		         	        	//printf("%c%c,%d,%d\n",x+'a',y+'1',depth1,(wwt-(flip+bwt+eval2[y][x])));
	         	  		         	  		         	     int k=x+97;
	         	  		         	  		         	    	         	  		         	  		         	char l=k;
	         	  		         	  		         	    	         	  		         	  		         	 out1<<l<<y+1<<","<<depth1<<","<<(wwt-(flip+bwt+eval2[y][x]))<<"\n";
	         	  		         	  		         	        	//printf("%c%c\n",x+'a',y+'1');
	         	  		         	  		         	        	//cout1<<"color O\n";
	         	  		         	  		         	        	if((wwt-(flip+bwt+eval2[y][x]))<minflip)
	         	  		         	  		         	        	        {
	         	  		         	  		         	        	        	minflip=wwt-(flip+bwt+eval2[y][x]);
	         	  		         	  		         	        	        	x1=y;
	         	  		         	  		         	        	        	y1=x;
	         	  		         	  		         	        	        }
	         	  		         	  		         	        	        if((wwt-(flip+bwt+eval2[y][x]))==minflip)
	         	  		         	  		         	        	        {
	         	  		         	  		         	        	        	if(y<x1)
	         	  		         	  		         	        	        	{	x1=y;
	         	  		         	  		         	        	        		y1=x;}
	         	  		         	  		         	        	        	else if(y==x1 and x<y1)
	         	  		         	  		         	        	        	{	x1=y;
	         	  		         	  		         	        	        		y1=x;}
	         	  		         	  		         	        	        }
	         	  		         	  		         	        	        flipreturn2[1][0]=minflip;
	         	  		         	  		         	        	        flipreturn2[1][1]=y1;
	         	  		         	  		         	        	        flipreturn2[1][2]=x1;


	         	  		         	  		         	        	      // printf("%c%c,%d,%d\n",lastx+'a',lasty+'1',depth1-1,minflip);
	         	  		         	  		         	    	      k=lastx+97;
	         	  		         	  		         	   	          l=k;
	         	  		         	  		         	   	        out1<<l<<lasty+1<<","<<depth1-1<<","<<minflip<<"\n";
	         	  		         	  		         	        	     //cout1<<color<<" minflip:"<<minflip<<"  x and y : "<<x1<<"  "<<y1<<"\n";
	         	  		         	  		         	        }
	         	  		         	  		         	  	 }
	         	  		         	  		         	  }

	         	  }
	         }
	         	  	if(winner4=='X'){
	         	  	if(color==X)
	         	    {
	         	  			   	flipr[0][0]=flipreturn2[0][0];
	         	  			   //	parentchild1[serial][2]=flipr[3][1];
	         	  			 	//parentchild1[serial][3]=flipr[3][2];
	         	  			   	flipr[0][1]=flipreturn2[0][1];
	         	  			   	flipr[0][2]=flipreturn2[0][2];
	         	  			   	return maxflip;
	         	    }
	         	  	else
	         	  	{
	         	  		//cout1<<"comes hereeeee\n";

	         	  			   	flipr[1][0]=flipreturn2[1][0];
	         	  			   	//parentchild1[serial][2]=flipr[4][1];
	         	  			   	//parentchild1[serial][3]=flipr[4][2];
	         	  			   	flipr[1][1]=flipreturn2[1][1];
	         	  			   	flipr[1][2]=flipreturn2[1][2];
	         	  			   	return minflip;
	         	    }}
	         	  	else
	         	  		         	  	{
	         	  		         	  		if(color==O)
	         	  		         	  			         	    {
	         	  		         	  			         	  			   	flipr[0][0]=flipreturn2[0][0];
	         	  		         	  			         	  			   	flipr[0][1]=flipreturn2[0][1];
	         	  		         	  			         	  			   	flipr[0][2]=flipreturn2[0][2];
	         	  		         	  			         	  			   	return maxflip;
	         	  		         	  			         	    }
	         	  		         	  			         	  	else
	         	  		         	  			         	  	{
	         	  		         	  			         	  			   	flipr[1][0]=flipreturn2[1][0];
	         	  		         	  			         	  			   	flipr[1][1]=flipreturn2[1][1];
	         	  		         	  			         	  			   	flipr[1][2]=flipreturn2[1][2];
	         	  		         	  			         	  			   	return minflip;
	         	  		         	  			         	    }
	         	  		         	  	}

	         }
