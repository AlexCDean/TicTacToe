#include <stdio.h>
#include <stdlib.h>
#define TOGGLE(a) ((a) == ('X')) ? ('O') : ('X') 
// Everything but AI finished. 

// Forward Declarations.
void map(char *board);
int move(char *board);
void errorprint(char *board);
void clear();
void check(char *board);
void win(char *board);
void play(char *board);
void set(char *board);
int checkRow(char *board);
int checkColumn(char *board);
int checkFull(char *board);
char turn = 'X'; // Let's get this global var off into pipelines instead. 




int main(int argc, char *argv[])
{
    char board[9];
	set(board);
	return 0;
}

void play(char *board)
{
	while(1)
	{
		map(board);
		move(board);
		turn = TOGGLE(turn);
	}
}

void map(char *board)
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

void set(char *board)
{
	int i;
	for(i = 0; i < 9; i++)
	{
		board[i] = ' ';
	}
	play(board);
}

void errorprint(char *board)
{
	printf("That's not a valid move!\n");
	clear();
    move(board);
}

void win(char *board)
{
	char a;
	map(board); // Refresh map with winning move.
	printf("Would you like to play again? Y/N\n");
	scanf("%c",&a);
	if(a == 'Y' || a == 'y')
	{
		printf("I challenge you to another duel!\n");
		set(board);
	}
	else if(a == 'N' || a == 'n')
	{
		printf("Goodbye!\n");
		exit(EXIT_SUCCESS);
	}
}

int checkRow(char *board)
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

int checkColumn(char *board)
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
int checkDiag(char *board)
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
int checkFull(char *board)
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
				return 1;
			}
		}
	}
	return 0;
}

// Okay this will allow the AI to stop going once an endstate is reached in our minimax. 
int EndState(char *board)
{
	if( checkRow(board) || checkColumn(board) || checkDiag(board) || checkFull(board))
	{
		return 1;
	}
	return 0;
}

void check(char *board) 
{

		if(checkRow(board) || checkColumn(board) || checkDiag(board))
		{
			printf("Wow! Looks like someone won!! I bet it was that sneaky %c!\n", turn);
			win(board);
		}
		if(checkFull(board))
        {
            printf("\nOh a draw, how cute\n");
            win(board);
        } // Should draw when board is full AND no winning moves. 
}


// Okay our AI needs to know if we can place a move, so this works. 
int isLegal(char *board, int a)
{
	if(board[a] == ' ')
	{
		return 1; 
	}
	return 0;
}
	

int move(char *board)
{
	int a, b;
	// Take in co-ordinates, change board. 
	printf("It's your move, %c! What move will you make? Row then Column separated by space\n", turn);
	scanf("%d%d", &a, &b);
	// Test for boundary (1-3 only for each) and test for legal move
	if((a < 4 && a > 0) && (b < 4 && b > 0) && (board[((a-1)*3)+(b-1)] == ' ')) 
	{
			board[((a-1)*3)+(b-1)]=turn;
			clear(); // Needed to clear stdin. 
			check(board);
			return 0;
	}		
	errorprint(board); // This also acts to clear stdin buffer.
	return 0;
}
