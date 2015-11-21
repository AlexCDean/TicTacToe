#include <stdio.h>
#include <stdlib.h>
#define TOGGLE(a) ((a) == ('X')) ? ('O') : ('X') 
// Everything but AI finished. 

// Forward Declarations.
void map();
int move();
void errorprint();
void clear();
void check();
void win();
void play();
void set();
int checkRow();
int checkColumn();
void checkFull();
char turn = 'X';

typedef struct{
				char board[9];
				STATE *next;
			  } STATE;
// So this struct should hold current state and point to next state, like creating node trees. 
// We should learn a bit more about pointers in this state, hey there's an exercies on C the hardway that uses structs like this. CHECK IT OUT

char board[9];

int main(int argc, char *argv[])
{
	set();
	return 0;
}

void play()
{
	while(1)
	{
		map();
		move();
		turn = TOGGLE(turn);
	}
}

void map()
{
	int i;
	int j = 1;
	for(i = 0; i < 7; i+=3)
	{					
		printf("%d %c|%c|%c\n", j,board[i], board[i+1], board[i+2]);
		j++;
		if(i == 6) 
		{
			printf("  1 2 3\n");
			return; // To avoid a third _ _ _ showing up. 
		}
		printf("  _ _ _ \n");
	}
} 

void clear ()
{    
 	 while ( getchar() != '\n' );
}

void set()
{
	int i;
	for(i = 0; i < 9; i++)
	{
		board[i] = ' ';
	}
	play();
}

void errorprint()
{
	printf("That's not a valid move!\n");
	clear();
	move(turn);
}

void win()
{
	char a;
	map(); // Refresh map with winning move.
	printf("Would you like to play again? Y/N\n");
	scanf("%c",&a);
	if(a == 'Y' || a == 'y')
	{
		printf("I challenge you to another duel!\n");
		set();
	}
	else if(a == 'N' || a == 'n')
	{
		printf("Goodbye!\n");
		exit(EXIT_SUCCESS);
	}
}

int checkRow()
{
	int i, sum;
	for(i = 0; i < 7; i+=3)
 	{
		// 0 1 2, 3 4 5, 6 7 8. 
		sum = board[i] + board[i+1] +  board[i+2]; 
		if(sum == 264 || sum == 237)
		{
			return 1;
		}
		sum = 0;
	 }
	return 0;
}

int checkColumn()
{
	int i, sum;
	// For a column checker, we need to do 0 3 6, 1 4 7, 2 5 8, obviously. 
	for(i = 0; i < 3; i++)
	{
		sum = board[i] + board[i+3] + board[i+6];
		if(sum == 264 || sum == 237)
		{
			return 1;
		}
		sum = 0; 
	}
	return 0;
}
int checkDiag()
{
	int sum;
 	if(( sum = board[0] + board[4] +  board[8]) == 237 || (sum = board[0] + board[4] +  board[8]) == 264)
 	{
		return 1;
	 }	
	 // Diagonal case 2
	 if(( sum = board[2] + board[4] +  board[6]) == 237 || (sum = board[2] + board[4] +  board[6]) == 264)
	 {
		return 1;
	 }
	return 0;
}
// This function can be made void
void checkFull()
{
	int i;
	int sum = 9;
	for(i = 0; i < 9; i++)
	{
		if(!(board[i] == ' '))
		{
			sum--;
			if(!sum)
			{
				printf("Oh you got a draw! How cute!\n");
				win();
			}
		}
	}
}

void check() 
{

		if(checkRow() || checkColumn() || checkDiag())
		{
			printf("Wow! Looks like someone won!! I bet it was that sneaky %c!\n", turn);
			win();
		}
		checkFull(); // Should draw when board is full AND no winning moves. 
}

int move()
{
	int a,b;
	// Take in co-ordinates, change board. 
	printf("It's your move, %c! What move will you make? Row then Column separated by space\n", turn);
	scanf("%d%d", &a, &b);
	// Test for boundary (1-3 only for each) and test for legal move
	if((a < 4 && a > 0) && (b < 4 && b > 0) && (board[((a-1)*3)+(b-1)]== ' '))
	{
			board[((a-1)*3)+(b-1)]=turn;
			clear(); // Needed to clear stdin. 
			check();
			return 0;
	}		
	errorprint(); // This also acts to clear stdin buffer.
	return 0;
}
