/**
*
@file othello.c
*
@brief Funções auxiliares
*
* 
*
*
@author Pedro,
pedro.s.conceicao@ua.pt
*
*
@internal
*
Created 31-Out-2017
*
Company University of Aveiro
*
Copyright Copyright (c) 2017, Pedro
*
*
=====================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "pcolor.h"
#include "othello.h"


/**
* @brief InitGame
* @param Pointer to game char array (*jg)
* @return void
*/
void InitGame( othello *jg){ // todas as céluas a VAZIO exceto as 4 centrais
	
	//Inicializar todas as células com vazio ' '
	for(int l=0;l<8;l++)
	{
		for(int c=0;c<8;c++)
		{
			jg -> T[l][c] = VAZIO;
		}
	}

	//Inicializar células do jogador 1 com 'O'
	jg -> T[3][3] = JOGADOR1;
	jg -> T[4][4] = JOGADOR1;
	
	//Inicializar células do jogador 2 com 'X'
	jg -> T[3][4] = JOGADOR2;
	jg -> T[4][3] = JOGADOR2;
}

/**
* @brief Prints game board
* @param Pointer to game char array (*jg)
* @return void
*/
void PrintGameBoard(othello *jg)
{
	int l, c;
	printf("\n\n");
	printf("     a   b   c   d   e   f   g   h\n");
	for(l=0;l<8;l++)
	{
		printf("   +-------------------------------+\n");
		printf(" %d ",l+1);
		for(c=0;c<8;c++)
		{
			printf("|");
			if(jg->T[l][c]==JOGADOR1){textcolor(RESET,BLUE,WHITE);}
			if(jg->T[l][c]==JOGADOR2){textcolor(RESET,RED,WHITE);}
			printf(" %c ",jg -> T[l][c]);
			ResetTextColors();
		}
		printf("| %d \n",l+1);
	}
	printf("   +-------------------------------+\n");
	printf("     a   b   c   d   e   f   g   h\n");
}


/**
* @brief Saves game board
* @param Pointer to game char array (*jg) and pointer to filename
* @return 1 if succesful ; 0 if unsuccesful
*/
int SaveGame(othello *jg, char *filename)
{	
	FILE *fp=fopen(filename,"w");
	if(fp == NULL) return 0;
	for(int l=0;l<8;l++)
	{
		for(int c=0;c<7;c++)
		{
			fprintf(fp,"%c,",jg -> T[l][c]);
		}
		fprintf(fp,"%c\n",jg -> T[l][7]);
	}
		
	fclose(fp);
	return 1;
}


/**
* @brief Loads game board
* @param Pointer to game char array (*jg) and pointer to filename
* @return 1 if succesful ; 0 if unsuccesful
*/
int LoadGame(othello *jg, char *filename)
{
	FILE *fp=fopen(filename,"rb");
	if(fp == NULL) return 0;
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0, SEEK_SET);  //same as rewind(f);
	char *str = malloc(size + 1);
	fread(str, size, 1, fp);
	fclose(fp);

	char *token;
	for(int l=0;l<8;l++) //for each line
	{	
		int c=0;
		token=strtok(str,",\n");
		while(token!=NULL)
		{
			jg -> T[l][c]= * token;
			token=strtok(NULL,",\n");
			c++;//next column
		}
	}

	free(str);
	return 1;	
}


/**
* @brief Reads keyboard and computes player move
* @param void
* @return player move from 11 to 88; 11 to quit; 10 to load game; 9 to save game
*/
int GetPlayerMove(othello *jg)
{
	char c_cor, l, aux;
	int c=0;
	do{
		printf("\n### COMMANDS : q/Q - quit ; s/S - Save ; l/L - Load ###\n\n");
		printf("Next move coordinates or command: ");
		scanf(" %c",&c_cor);
		scanf("%c",&l);
		if(isalpha(l)){
			aux=l;
			l=c_cor;
			c_cor=aux;
		}
		if(c_cor=='q' || c_cor=='Q'){
			exit(0);
		}
		if(c_cor=='s' || c_cor =='S'){
			int save_game=SaveGame(jg,"othello.txt");
			printf("\nSaving game...\n");
			if(save_game) printf("Game saved!\n");
			else printf("Game not saved.\n");
			c=90;
			break;
		}
		if(c_cor=='l' || c_cor =='L'){
			int load_game=LoadGame(jg,"othello.txt");
			printf("\nLoading game...\n");
			if(load_game){printf("Game loaded!\n");PrintGameBoard( jg );}
			else{printf("Game not found.\nExiting\n");break;}
			c=91;
			break;
		}
		if(atoi(&l)<1 || atoi(&l)>8){
			c_cor='0';
		}
		if(c_cor!='l' || c_cor!='L' || c_cor!='s' ||  c_cor!='S'){
			switch(c_cor){
				case'a':
				case'A':
					c=1;break;
				case'b':
				case'B':
					c=2;break;
				case'c':
				case'C':
					c=3;break;
				case'd':
				case'D':
					c=4;break;
				case'e':
				case'E':
					c=5;break;
				case'f':
				case'F':
					c=6;break;
				case'g':
				case'G':
					c=7;break;
				case'h':
				case'H':
					c=8;break;
				default:
					printf("Opção Inválida. Tente novamente! \n");
			}
		}
	}while(c==0);
	if(c==90 || c==91){return 9999;}
	else{return c*10+atoi(&l);}
}


int CheckMove( othello *jg , int move , int player )
{
	for(int l=0;l<8;l++)
	{
		for(int c=0;c<8;c++)
		{
			if(jg -> T[l][c] == player)
			{
				int check_right=CheckRight(jg,l,c,player);
				if(check_right==move){ReversePieces(jg,move,player);return 1;}

				int check_left=CheckLeft(jg,l,c,player);
				if(check_left==move){ReversePieces(jg,move,player);return 1;}

				int check_up=CheckUp(jg,l,c,player);
				if(check_up==move){ReversePieces(jg,move,player);return 1;}

				int check_down=CheckDown(jg,l,c,player);
				if(check_down==move){ReversePieces(jg,move,player);return 1;}
			
				int check_upright=CheckUpRight(jg,l,c,player);
				if(check_upright==move){ReversePieces(jg,move,player);return 1;}

				int check_upleft=CheckUpLeft(jg,l,c,player);
				if(check_upleft==move){ReversePieces(jg,move,player);return 1;}

				int check_downright=CheckDownRight(jg,l,c,player);
				if(check_downright==move){ReversePieces(jg,move,player);return 1;}
				
				int check_downleft=CheckDownLeft(jg,l,c,player);
				if(check_downleft==move){ReversePieces(jg,move,player);return 1;}
			}	
		}					
	}
	return 0;
}


int CheckRight (othello *jg , int l , int c , int player)
{
	int opponent=0;
	if(player==JOGADOR1){opponent=JOGADOR2;}
	if(player==JOGADOR2){opponent=JOGADOR1;}
	if(jg -> T[l][c+1] == opponent)
	{
		do{c++;}while(jg -> T[l][c] == opponent && c<8);
		if(jg-> T[l][c] == player){return 1;}
		if(jg-> T[l][c] == VAZIO){return (c+1)*10+l+1;}
	}
	return 0;
}

int CheckLeft (othello *jg , int l , int c , int player)
{
	int opponent=0;
	if(player==JOGADOR1){opponent=JOGADOR2;}
	if(player==JOGADOR2){opponent=JOGADOR1;}
	if(jg -> T[l][c-1] == opponent)
	{
		do{c--;}while(jg -> T[l][c] == opponent && c>-1);
		if(jg-> T[l][c] == player){return 1;}
		if(jg-> T[l][c] == VAZIO){return (c+1)*10+l+1;}
	}
	return 0;
}

int CheckUp (othello *jg , int l , int c , int player)
{
	int opponent=0;
	if(player==JOGADOR1){opponent=JOGADOR2;}
	if(player==JOGADOR2){opponent=JOGADOR1;}
	if(jg -> T[l-1][c] == opponent)
	{
		do{l--;}while(jg -> T[l][c] == opponent && l>-1);
		if(jg-> T[l][c] == player){return 1;}
		if(jg-> T[l][c] == VAZIO){return (c+1)*10+l+1;}
	}
	return 0;
}

int CheckDown (othello *jg , int l , int c , int player)
{
	int opponent=0;
	if(player==JOGADOR1){opponent=JOGADOR2;}
	if(player==JOGADOR2){opponent=JOGADOR1;}
	if(jg -> T[l+1][c] == opponent)
	{
		do{l++;}while(jg -> T[l][c] == opponent && l<8);
		if(jg-> T[l][c] == player){return 1;}
		if(jg-> T[l][c] == VAZIO){return (c+1)*10+l+1;}
	}
	return 0;
}

int CheckUpRight (othello *jg , int l , int c , int player)
{
	int opponent=0;
	if(player==JOGADOR1){opponent=JOGADOR2;}
	if(player==JOGADOR2){opponent=JOGADOR1;}
	if(jg -> T[l-1][c+1] == opponent)
	{
		do{l--;c++;}while(jg -> T[l][c] == opponent && l>-1 && c<8);
		if(jg-> T[l][c] == player){return 1;}
		if(jg-> T[l][c] == VAZIO){return (c+1)*10+l+1;}
	}
	return 0;
}

int CheckUpLeft (othello *jg , int l , int c , int player)
{
	int opponent=0;
	if(player==JOGADOR1){opponent=JOGADOR2;}
	if(player==JOGADOR2){opponent=JOGADOR1;}
	if(jg -> T[l-1][c-1] == opponent)
	{
		do{l--;c--;}while(jg -> T[l][c] == opponent && l>-1 && c>-1);
		if(jg-> T[l][c] == player){return 1;}
		if(jg-> T[l][c] == VAZIO){return (c+1)*10+l+1;}
	}
	return 0;
}

int CheckDownRight (othello *jg , int l , int c , int player)
{
	int opponent=0;
	if(player==JOGADOR1){opponent=JOGADOR2;}
	if(player==JOGADOR2){opponent=JOGADOR1;}
	if(jg -> T[l+1][c+1] == opponent)
	{
		do{l++;c++;}while(jg -> T[l][c] == opponent && l<8 && c<8);
		if(jg-> T[l][c] == player){return 1;}
		if(jg-> T[l][c] == VAZIO){return (c+1)*10+l+1;}
	}
	return 0;
}

int CheckDownLeft (othello *jg , int l , int c , int player)
{
	int opponent=0;
	if(player==JOGADOR1){opponent=JOGADOR2;}
	if(player==JOGADOR2){opponent=JOGADOR1;}
	if(jg -> T[l+1][c-1] == opponent)
	{
		do{l++;c--;}while(jg -> T[l][c] == opponent && l<8 && c>-1);
		if(jg-> T[l][c] == player){return 1;}
		if(jg-> T[l][c] == VAZIO){return (c+1)*10+l+1;}
	}
	return 0;
}


void ReversePieces(othello *jg , int move , int player)
{
	int l=move%10-1;
	int c=move/10-1;
	int cc=10, ll=10;
	int flag=0;

	jg -> T[l][c]=player;
	//right
	for(cc=c+1 ; cc<8 ; cc++){
		if(jg -> T[l][cc] == player){flag=1;break;}
		if(jg -> T[l][cc] == VAZIO) break;
	}
	if(flag){
		for(int n=c+1 ; n<cc ; n++){
			jg -> T[l][n]=player;		
		}
	}
	flag=0;

	//left
	for(cc=c-1 ; cc>-1 ; cc--){
		if(jg -> T[l][cc] == player){flag=1;break;}
		if(jg -> T[l][cc] == VAZIO) break;
	}
	if(flag){
		for(int n=c-1 ; n>cc ; n--){
			jg -> T[l][n]=player;		
		}
	}
	flag=0;

	//up
	for(ll=l+1 ; ll<8 ; ll++){
		if(jg -> T[ll][c] == player){flag=1;break;}
		if(jg -> T[ll][c] == VAZIO) break;
	}
	if(flag){
		for(int n=l+1 ; n<ll ; n++){
			jg -> T[n][c]=player;		
		}
	}
	flag=0;

	//down
	for(ll=l-1 ; ll>-1 ; ll--){
		if(jg -> T[ll][c] == player){flag=1;break;}
		if(jg -> T[ll][c] == VAZIO) break;
	}
	if(flag){
		for(int n=l-1 ; n>ll ; n--){
			jg -> T[n][c]=player;		
		}
	}
	flag=0;
	
	//upright
	for(ll=l-1 ; ll>-1 ; ll--){
		if(c-(ll-l)>8) break;
		if(jg -> T[ll][c-(ll-l)] == player){flag=1;break;}
		if(jg -> T[ll][c-(ll-l)] == VAZIO) break;
	}
	if(flag){
		for(int n=l-1 ; n>ll ; n--){
			jg -> T[n][c-(n-l)]=player;		
		}
	}
	flag=0;

	//downright
	for(ll=l+1 ; ll<8 ; ll++){
		if(c+(ll-l)>8) break;
		if(jg -> T[ll][c+(ll-l)] == player){flag=1;break;}
		if(jg -> T[ll][c+(ll-l)] == VAZIO) break;
	}
	if(flag){
		for(int n=l+1 ; n<ll ; n++){
			jg -> T[n][c+(n-l)]=player;		
		}
	}
	flag=0;

	//upleft
	for(ll=l-1 ; ll>-1 ; ll--){
		if(c+(ll-l)<0) break;
		if(jg -> T[ll][c+(ll-l)] == player){flag=1;break;}
		if(jg -> T[ll][c+(ll-l)] == VAZIO) break;
	}
	if(flag){
		for(int n=l-1 ; n>ll ; n--){
			jg -> T[n][c+(n-l)]=player;		
		}
	}
	flag=0;

	//downleft
	for(ll=l+1 ; ll<8 ; ll++){
		if(c-(ll-l)<0) break;
		if(jg -> T[ll][c-(ll-l)] == player){flag=1;break;}
		if(jg -> T[ll][c-(ll-l)] == VAZIO) break;
	}
	if(flag){
		for(int n=l+1 ; n<ll ; n++){
			jg -> T[n][c-(n-l)]=player;		
		}
	}
	flag=0;

}


int WinGame(othello *jg,int player)
{
	int score1=0, score2=0, flag=0, l, c;
	for( l=0;l<8;l++)
	{
		for( c=0;c<8;c++)
		{
			if(jg->T[l][c]==VAZIO){
				if(CheckRight(jg,l,c,player)){flag=1;break;}
				if(CheckLeft(jg,l,c,player)){flag=1;break;}
				if(CheckUp(jg,l,c,player)){flag=1;break;}
				if(CheckDown(jg,l,c,player)){flag=1;break;}
				if(CheckUpLeft(jg,l,c,player)){flag=1;break;}
				if(CheckUpRight(jg,l,c,player)){flag=1;break;}
				if(CheckDownLeft(jg,l,c,player)){flag=1;break;}
				if(CheckDownRight(jg,l,c,player)){flag=1;break;}
			}
		}
		if(flag) break;
	}

	if(! flag){
		for(int l=0; l<8; l++){
			for(int c=0; c<8; c++){
				if(jg -> T[l][c]==JOGADOR1) score1++;
				if(jg -> T[l][c]==JOGADOR2) score2++;
			}
		}
		if(score1>score2) printf("\n\nPlayer 1 wins! Player 1 %d - %d Player2\n\n", score1,score2);
		else if(score1<score2) printf("\n\nPlayer 2 wins! Player 2 %d - %d Player 1\n\n", score2,score1);
		else printf("\n\nDraw! Player 1 %d - %d Player 2\n\n",score1,score2);
		return 1;
	}
	return 0;
}

int GenerateMove(othello *jg)
{
	int moves[100]={0};
	int moves_count=0;

	for(int l=0;l<8;l++)
	{
		for(int c=0;c<8;c++)
		{
			if(jg -> T[l][c] == JOGADOR2)
			{
				int check_right=CheckRight(jg,l,c,JOGADOR2);
				if(check_right>10 && check_right<100){
					moves[moves_count]=check_right;
					moves_count++;
				}		
				int check_left=CheckLeft(jg,l,c,JOGADOR2);
				if(check_left>10 && check_left<100){
					moves[moves_count]=check_left;
					moves_count++;
				}
				int check_up=CheckUp(jg,l,c,JOGADOR2);
				if(check_up>10 && check_up<100){
					moves[moves_count]=check_up;
					moves_count++;
				}
				int check_down=CheckDown(jg,l,c,JOGADOR2);
				if(check_down>10 && check_down<100){
					moves[moves_count]=check_down;
					moves_count++;
				}		
				int check_upright=CheckUpRight(jg,l,c,JOGADOR2);
				if(check_upright>10 && check_upright<100){
					moves[moves_count]=check_upright;
					moves_count++;
				}
				int check_upleft=CheckUpLeft(jg,l,c,JOGADOR2);
				if(check_upleft>10 && check_upleft<100){
					moves[moves_count]=check_upleft;
					moves_count++;
				}				
				int check_downright=CheckDownRight(jg,l,c,JOGADOR2);
				if(check_downright>10 && check_downright<100){
					moves[moves_count]=check_downright;
					moves_count++;
				}				
				int check_downleft=CheckDownLeft(jg,l,c,JOGADOR2);
				if(check_downleft>10 && check_downleft<100){
					moves[moves_count]=check_downleft;
					moves_count++;
				}
			}
		}
	}
	int i=0;
	while(moves[i]!=0)
	{
		i++;
	}
	i--;
	srand(time(NULL));
	if(i>0){i=rand()%i;}
	return moves[i];	
}

