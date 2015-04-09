#include <iostream>
using namespace std;

#define rows 2
#define columns 6
#define MAX_DEPTH 6

struct Position {
   int cells_player[6];
   int cells_computer[6];
   bool computer_play;
   int seeds_player;
   int seeds_computer;
 };

int minimax(Position *, int, int);
int evaluation(Position *p, int, int);
int final_position(Position *p, int, int);
int valid_move(Position *p, int, int);
void play_move(Position *p, Position *p1, int, int);
void print_board(Position *p, int);

int main(){
	
	Position init;
	for (int i = 0; i < 6; ++i)
	{
		init.cells_player[i] = 6;
		init.cells_computer[i] = 6;
	}

	print_board(&init, 1);
	return 0;
}

int minimax(Position* pos_current, int computer_play, int current_depth){
	// computer_play is true if the computer has to play and false otherwise
	int tab_values[6];
	Position pos_next; // In C : created on the stack: = very fast
	int max, min;

	if (final_position(pos_current, computer_play, current_depth)){
	// returns VALMAX (=48) if the computer wins, -48 if it loses; 0 if draw
	}

	if (current_depth == MAX_DEPTH) {
		return evaluation(pos_current, computer_play, current_depth);
		// the simplest evealution fucntion is the difference of the taken seeds
	}

	for(int i = 0;i < columns; i++){
	
		if (valid_move(pos_current, computer_play, i)){
	       
			play_move(&pos_next, pos_current, computer_play,i);
			// pos_next is the new current poisition and we change the player
			tab_values[i]= minimax(&pos_next, !computer_play, current_depth + 1);
		}
		else {
			if (computer_play) 
				tab_values[i]=-100;
			else
				tab_values[i]=+100;			
		}

		if(i == 0){
			max = tab_values[i];
			min = tab_values[i];
		}
		else {
			max = tab_values[i] > max ? tab_values[i] : max;
			min = tab_values[i] < min ? tab_values[i] : min;
		}
	}
	int res;
	if (computer_play)	   
		res = max;	
	else 	   
		res = min;

	return res;
}

void play_move(Position *next, Position *current, int computer_play, int i)
{}

int final_position(Position *current, int computer_play, int current_depth){

	int value = -1;
	return value;
}

int evaluation(Position *p, int computer_play, int current_depth){
	return -1;
}

int valid_move(Position *p, int computer_play, int current_depth){
	return -1;
}

void print_board(Position *p, int current_depth){

	cout << endl << "--" << current_depth << "--";
	cout << endl << "------Human------" << endl;
	for (int i = 0; i < 6; ++i)
	{
		cout << p->cells_player[i] << ", ";
	}
	cout << endl;
	cout << endl << "------Computer------" << endl;
	for (int i = 0; i < 6; ++i)
	{
		cout << p->cells_computer[i] << ", ";
	}
	cout << endl;

}