#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TOGGLE(a) ((a) == ('X')) ? ('O') : ('X') 
#define LEGAL ((a < 4 && a > 0) && (b < 4 && b > 0) && (board[((a-1)*3)+(b-1)] == ' '))
// Everything is implemented, next up is a GUI. 
// Note:Minmax has alpha-beta esque pruning, but needs to keep track of scores 
//	and select min/max as appropiate. So keep track of node type, and score.
// funcs to imp.: Min(), Max(). 
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
int minmax(char *board, char move);
int EndState(char *board);
int isLegal(char *board, int a);
void AI(char *board);
int Score(char *board);
int getNextState(char *board, char move);

int main()
{
	// Board has been fed a pre-set map for test. 
	// Decomment set() if a clean one is needed.
	char board[10] = { ' ', ' ', 'O', 'O', 'X', 'X', ' ', ' ', ' '};
    //	set(board);
    	play(board);
	return 0;
}

void play(char *board)
{
    while(1)
	{
        map(board);
        move(board);
		AI(board);
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

void set(char *board)
{
	int i;
	for(i = 0; i < 9; i++)
	{
		board[i] = ' ';
	}
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
	clear();
    win(board);
}

int checkRow(char *board)
{
	int i, sum;
	for(i = 0; i < 7; i+=3)
 	{
		// 0 1 2, 3 4 5, 6 7 8. 
		sum = board[i] + board[i+1] +  board[i+2]; 
		if(sum == 264) return 1;
		if(sum == 237) return 2;
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
		if(sum == 264) return 1;
		if(sum == 237) return 2;
		sum = 0; 
	}
	return 0;
}
int checkDiag(char *board)
{
	int sum;
	if( (sum = board[2] + board[4] +  board[6]) == 264 || (sum = board[0] + board[4] +  board[8]) == 264) return 1;
 	if(( sum = board[0] + board[4] +  board[8]) == 237 || (sum = board[2] + board[4] +  board[6]) == 237) return 2;
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
			if(!sum) return 1;
		}
	}
	return 0;
}
// Okay our AI needs to know if we can place a move, so this works. 
int isLegal(char *board, int a)
{
	if(board[a] == ' ') return 1;
	return 0;
}
	

// This will allow the AI to stop going once an endstate is reached in our minimax. 
int EndState(char *board)
{
	if( checkRow(board) || checkColumn(board) || checkDiag(board) || checkFull(board)) return 1;
	return 0;
}

// Okay so this is where we use the minmax function to determine scores of each node. 
void AI(char *board)
{
    int i, index = 0;
	int j = -10000;
	int score[10]= {0};

		for(i = 0; i < 9; i++)
		{
			if(isLegal(board, i))
			{
				board[i] = 'O'; // That's us
				map(board);
				score[i] = minmax(board, 'X'); // What will the opponent do? 
				board[i] = ' ';
			}
		}	
		for(i = 0; i < 9; i++)
		{
			if(score[i] > j && isLegal(board, i))
			{
				j = score[i];
				index = i;
			}
		}
		if(isLegal(board, index))
		{
			board[index] = 'O';
			check(board);
		}
	else if(board[4] == ' ')
	{
		board[4] = 'O';
		check(board);
	}
	
}

int minmax(char *board, char move)
{
	int i;
	char temp[10];
	strcpy(temp, board); // We don't want to affect the actual board.
	int score = -500;; // Again, our no action score. 
	// if game is over, return score. 
	if(EndState(temp)) return Score(temp); // Catch condition.
	// Else play for first position, then that tree. 
	for(i = 0; i < 9; i++)
	{
		if(isLegal(temp, i))
		{
			temp[i] = move; // Play this move, opponent or us. 
			map(temp);
			move = TOGGLE(move); // Change it, back to us or opponent. 
			score += getNextState(temp, move); // What is the tree like?
			move = TOGGLE(move);
			temp[i] = ' ';
		}
	}
	return score;
}

// This getNextState function 

int getNextState(char *board, char move)
{
	int i, score = -500;
	char temp[10]; // See minmax
	strcpy(temp, board);
	if(EndState(board)) return Score(board);
	for(i = 0; i < 9; i++)
	{
		if(isLegal(board, i))
		{	
			temp[i] = move; // Play the next move
			map(temp);
			move = TOGGLE(move); // Change turn
			score = getNextState(temp, move); // Recursive yet? 
			move = TOGGLE(move);
			temp[i] = ' '; // Remove our play, go to the next position
			if(score > -100) return score;
		}	
	}
	return score; 
}


int Score(char *temp)
{
	if(((checkRow(temp) == 1)  || (checkColumn(temp) == 1) || (checkDiag(temp)) == 1)) return -100;
	if(((checkRow(temp) == 2)  || (checkColumn(temp) == 2) || (checkDiag(temp)) == 2)) return 200;
	if(checkFull(temp)) return 100;
}

void check(char *board) 
{

		if(checkRow(board) == 1 || checkColumn(board) == 1|| checkDiag(board) == 1 )
		{
			printf("\nYou won!\n");
			map(board);
			win(board);
		}
		else if(checkRow(board) == 2 || checkColumn(board) == 2|| checkDiag(board) == 2)
		{
			printf("\nYou lost! To me! A computer! Mwuahahaha!\n");
			map(board);
			win(board);
		}
		else if(checkFull(board))
        {
            printf("\nOh a draw, how cute\n");
            map(board);
            win(board);
        } 
}

int move(char *board)
{
	int a, b;
	// Take in co-ordinates, change board. 
	printf("It's your move! What move will you make? Row then Column separated by space\n");
	scanf("%d%d", &a, &b); 
	// Test for boundary (1-3 only for each) and test for legal move
	if(LEGAL) 
	{
        // Maths to map player input to actual board position, accounting for zero index
        board[((a-1)*3)+(b-1)] = 'X'; 
		clear(); // Needed to clear stdin. 
		check(board);
		return 0;
	}		
	errorprint(board); // This also acts to clear stdin buffer.
	return 0;
}
