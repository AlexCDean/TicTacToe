#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#define TOGGLE(a) ((a) == ('X')) ? ('O') : ('X') 
// Global variables
GtkBuilder *builder;


// Forward declarations
void check(char *board);
void set(char *board);
int checkRow(char *board);
int checkColumn(char *board);
int checkFull(char *board);
int minmax(char *board, char move);
int EndState(char *board);
int isLegal(char *board, int a);
void AI(char *board);
int Score(char *board);
int getNextState(char *board, char move, int depth);
static void ButtonSet(char *board);
static void ButtonMove(GtkButton *button, char *board);
void quick_message(GtkWindow *parent, gchar *message, char *board);
static void ButtonChange(GtkButton *button, char *M);

int main (void)
{
  	GObject *window;
  	GObject *button;
  	int i;
  	char board[10];
 
  	gtk_init (NULL, NULL);
  	char buff[8]; // Array to hold button names
  	/* Construct a GtkBuilder instance and load our UI description */
  	builder = gtk_builder_new ();
  	gtk_builder_add_from_file(builder, "builder.ui", NULL);
  	/* Connect signal handlers to the constructed widgets. */
  	window = gtk_builder_get_object(builder, "window");
  	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
  	ButtonSet(board);
  	for(i = 0; i < 9; i++)
  	{
		sprintf(buff, "button%d", i);
		button = gtk_builder_get_object(builder, buff);
		g_signal_connect(button, "clicked", G_CALLBACK(ButtonMove), (board+i));
 	}
  
 	button = gtk_builder_get_object (builder, "quit");
 	g_signal_connect (button, "clicked", G_CALLBACK (gtk_main_quit), NULL);

 	gtk_main();

  return 0;
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
	char buff[8];
	int i, index = 0;
	int j = -10000;
	int score[10]= {0};

	for(i = 0; i < 9; i++)
	{
		if(isLegal(board, i))
		{
			board[i] = 'O'; // That's us
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
			sprintf(buff, "button%d", index);
			GObject *button = gtk_builder_get_object(builder, buff);
			board[index] = 'O';
			ButtonChange(GTK_BUTTON(button), "O");
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
			move = TOGGLE(move); // Change it, back to us or opponent. 
			score += getNextState(temp, move, 2); // What is the tree like?
			move = TOGGLE(move);
			temp[i] = ' ';
		}
	}
	return score;
}


int getNextState(char *board, char move, int depth)
{
	int i, score = -500;
	char temp[10]; // See minmax
	strcpy(temp, board);
	if(EndState(board)) return (Score(board)-depth);
	for(i = 0; i < 9; i++)
	{
		if(isLegal(board, i))
		{	
			temp[i] = move; // Play the next move
			move = TOGGLE(move); // Change turn
			score = getNextState(temp, move, ++depth); // Recursive yet? 
			move = TOGGLE(move);
			temp[i] = ' '; // Remove our play, go to the next position
			if(score > -100) return score; // Prunes branches. 
		}	
	}
	return score; 
}


int Score(char *temp)
{
	if(((checkRow(temp) == 1)  || (checkColumn(temp) == 1) || (checkDiag(temp)) == 1)) return -200;
	if(((checkRow(temp) == 2)  || (checkColumn(temp) == 2) || (checkDiag(temp)) == 2)) return 200;
	if(checkFull(temp)) return 100;
}

void check(char *board) 
{
	GObject *window = gtk_builder_get_object(builder, "window");

	if(checkRow(board) == 1 || checkColumn(board) == 1|| checkDiag(board) == 1 )
	{
		quick_message(GTK_WINDOW(window), "You won! Play again?", board);
	}
	else if(checkRow(board) == 2 || checkColumn(board) == 2|| checkDiag(board) == 2)
	{
		quick_message(GTK_WINDOW(window), "You lose! Play again?", board);
	}
	else if(checkFull(board))
   	{
		quick_message(GTK_WINDOW(window), "Draw! Play again?", board);
	} 
}


static void ButtonChange(GtkButton *button, char *M)
{
	gtk_button_set_label(button, M);
}

void quick_message(GtkWindow *parent, gchar *message, char *board)
{
	GtkWidget *dialog, *label, *content_area;
	GtkDialogFlags flags;
 
	// Create the widgets
	flags = GTK_DIALOG_DESTROY_WITH_PARENT;
	dialog = gtk_dialog_new_with_buttons ("Message", parent, flags, ("Yes!"), GTK_RESPONSE_ACCEPT, ("No!"), GTK_RESPONSE_REJECT, NULL);
	content_area = gtk_dialog_get_content_area (GTK_DIALOG(dialog));
	label = gtk_label_new (message);
 
	// Add the label, and show everything we’ve added
	gtk_container_add(GTK_CONTAINER(content_area), label);
	gtk_widget_show_all(dialog);

	gint result = gtk_dialog_run (GTK_DIALOG(dialog));
	switch (result)
	{
		case GTK_RESPONSE_ACCEPT:
			ButtonSet(board);
			gtk_widget_destroy(dialog);
	        	break;
		default:
			gtk_widget_destroy (dialog);
			gtk_main_quit();
		       break;
	}
}
                          

static void ButtonMove(GtkButton *button, char *board)
{
	int n = 9 - strlen(board);
	if(*board == ' ')
	{
		*board = 'X';
		ButtonChange(button, "X");
		check(board-n);
		AI(board-n);	
	}
}


void set(char *board)
{
	int i;
	for(i = 0; i < 9; i++)
	{
		board[i] = ' ';
	}
}

static void ButtonSet(char *board)
{
	int i;
	char buff[8];
	GObject *button;
	for(i = 0; i < 9; i++)
	{
		sprintf(buff, "button%d", i);
		button = gtk_builder_get_object(builder, buff);
		gtk_button_set_label(GTK_BUTTON(button), " ");
	}
	set(board);
}

