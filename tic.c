#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TOGGLE(a) ((a) == ('X')) ? ('O') : ('X') 
#define LEGAL ((a < 4 && a > 0) && (b < 4 && b > 0) && (board[((a-1)*3)+(b-1)] == ' '))
// Everything but AI finished. 

// Forward Declarations.
void map(char *board);
int move(char *board, char turn);
void errorprint(char *board, char turn);
void clear();
void check(char *board, char turn);
void win(char *board);
void play(char *board, char turn);
void set(char *board, char turn);
int checkRow(char *board);
int checkColumn(char *board);
int checkFull(char *board);
int minmax(char *board, int a, char move);
int EndState(char *board);
int isLegal(char *board, int a);
int AI(char *board, char turn);



int main()
{
	char turn = 'X';
    char board[10];
    set(board, turn);
    return 0;
}

void play(char *board, char turn)
{
    while(1)
	{
        map(board);
        move(board, turn);
		turn = TOGGLE(turn);
		AI(board, turn);
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
 	 while (getchar() != '\n' );
}

void set(char *board, char turn)
{
	int i;
	for(i = 0; i < 9; i++)
	{
		board[i] = ' ';
	}
	play(board, turn);
}

void errorprint(char *board, char turn)
{
	printf("That's not a valid move!\n");
	clear();
    move(board, turn);
}

void win(char *board)
{
	char a;
	printf("Would you like to play again? Y/N\n");
	scanf("%c",&a);
	if(a == 'Y' || a == 'y')
	{
		printf("I challenge you to another duel!\n");
		main();
	}
	else if(a == 'N' || a == 'n')
	{
		printf("Goodbye!\n");
		exit(EXIT_SUCCESS);
	}
    win(board);
}

int checkRow(char *board)
{
	int i, sum;
	for(i = 0; i < 7; i+=3)
 	{
		// 0 1 2, 3 4 5, 6 7 8. 
		sum = board[i] + board[i+1] +  board[i+2]; 
		if(sum == 264)
		{
			return 1;
		}
		if(sum == 237)
		{
			return 2;
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
		if(sum == 264)
		{
			return 1;
		}
		if(sum == 237)
		{
			return 2;
		}
		sum = 0; 
	}
	return 0;
}
int checkDiag(char *board)
{
	int sum;
	if( (sum = board[2] + board[4] +  board[6]) == 264 || (sum = board[0] + board[4] +  board[8]) == 264)
	{
		return 1;
	}
 	if(( sum = board[0] + board[4] +  board[8]) == 237 || (sum = board[2] + board[4] +  board[6]) == 237)
 	{
		return 2;
	}	
	return 0;
}

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
// Okay our AI needs to know if we can place a move, so this works. 
int isLegal(char *board, int a)
{
	if(board[a] == ' ')
	{
		return 1; 
	}
	return 0;
}
	

// This will allow the AI to stop going once an endstate is reached in our minimax. 
int EndState(char *board)
{
	if( checkRow(board) || checkColumn(board) || checkDiag(board) || checkFull(board))
	{
		return 1;
	}
	return 0;
}

// Okay so this is where we use the minmax function to determine scores of each node. 
int AI(char *board, char turn)
{
    int i = 0;
	int score[10];
	char move = turn;

	for(i = 0; i < 9; i++)
	{
		if(isLegal(board,i))
		{	
			score[i] = minmax(board, i, move);
 			printf("%d  %d\n", i, score[i]);
		}
		else if(!isLegal(board,i))
		{
			score[i] = -1000;
		}
	}
	return 1;
}

// We have to sum up all the trees. 
int minmax(char *board, int a, char move)
{
	int i, j;
	char temp[10];
	strcpy(temp, board);
	if(EndState(temp))
	{
		if(((checkRow(temp) == 1)  || (checkColumn(temp) == 1) || (checkDiag(temp)) == 1))
		{
			return -100;
		}
		if(((checkRow(temp) == 2)  || (checkColumn(temp) == 2) || (checkDiag(temp)) == 2) )
		{	
			return 100;
		}
		if(checkFull(temp))
		{
			return 0;
		}
	}
	for(i = a; i < 9 && !EndState(temp); i++)
	{
		if(isLegal(temp,i))
		{	
			temp[i] = move;
			move = TOGGLE(move);
			map(temp);
			minmax(temp, i, move);
			i = 0;
		}
	}
	/*for(i = a; i < 9 && !EndState(temp); i++)
	{
		temp[i] = move;
		for(j = 0; j < 9 && !EndState(temp); j++)
		{
			if((isLegal(temp, j)))
			{
				temp[j] = move;
				move = TOGGLE(move);
				map(temp);
				minmax(temp, i, move);
			}
		}
	}*/
	minmax(temp, 0, move);
}


void check(char *board, char turn) 
{

		if(checkRow(board) || checkColumn(board) || checkDiag(board))
		{
			printf("%c Won!\n", turn);
			map(board);
			win(board);
		}
		if(checkFull(board))
        {
            printf("\nOh a draw, how cute\n");
            map(board);
            win(board);
        } 
}


int move(char *board, char turn)
{
	int a, b;
	// Take in co-ordinates, change board. 
	printf("It's your move, %c! What move will you make? Row then Column separated by space\n", turn);
	scanf("%d%d", &a, &b);
	// Test for boundary (1-3 only for each) and test for legal move
	if(LEGAL) 
	{
        // Maths to map player input to actual board position, accounting for zero index
        board[((a-1)*3)+(b-1)] = turn; 
		clear(); // Needed to clear stdin. 
		check(board, turn);
		return 0;
	}		
	errorprint(board, turn); // This also acts to clear stdin buffer.
	return 0;
}
