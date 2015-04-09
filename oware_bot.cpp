#include <iostream>
using namespace std;

#define ROWS 2
#define COLUMNS 6
#define MAX_DEPTH 6
#define INIT_SEEDS_IN_PIT 4

struct Position {
   int cells_player[6];
   int cells_computer[6];
   bool computer_play;
   int seeds_player;
   int seeds_computer;
 };

int minimax(Position *p, int, int);
int evaluation(Position *p, int, int);
int final_position(Position *p, int, int);
int valid_move(const Position *p, int, int);
void play_move(Position *p,const Position *p1, int c_play, int start);
void print_board(const Position *p, int);

int main(){
	
	Position init, next1, next2, next3;
	for (int i = 0; i < COLUMNS; ++i)
	{
		init.cells_player[i] = INIT_SEEDS_IN_PIT;
		init.cells_computer[i] = INIT_SEEDS_IN_PIT;
	}
	init.computer_play = 1;

	print_board(&init, 1);
	play_move(&next1,  &init, 0, 3);
	print_board(&next1, 2);
	play_move(&next2,  &next1, 1, 3);
	print_board(&next2, 3);

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

	for(int i = 0;i < COLUMNS; i++){
	
		if (valid_move(pos_current, computer_play, i)){

			play_move(&pos_next, pos_current, computer_play, i);
			// pos_next is the new current poisition and we change the player
			tab_values[i]= minimax(&pos_next, !computer_play, current_depth + 1);
		}
		else {
			if (computer_play) 
				tab_values[i]= -100;
			else
				tab_values[i]= +100;			
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

void play_move(Position *next, const Position *current, int computer_play, int start)
{

	for (int i = 0; i < COLUMNS; ++i) // improve this?
	{
		next->cells_player[i] = current->cells_player[i];
		next->cells_computer[i] = current->cells_computer[i];
	}

	next->computer_play = !computer_play;

	int current_cell;
	int seeds_to_place;

	//abstract the positions as a ring with 12 cells
	//translate start position from respective boards to the ring
	//easier to do the anti clockwise movement (I think)
	if(computer_play){

		current_cell = COLUMNS - 1 - start;
		next->cells_computer[start] = 0;
		seeds_to_place = current->cells_computer[start];					
	}
	else{

		current_cell = start + COLUMNS;
		next->cells_player[start] = 0;
		seeds_to_place = current->cells_player[start];					
	}

	cout << endl << "seeds: " << seeds_to_place << " start_col: " << current_cell << endl;

	while((seeds_to_place--) != 0){
		
		current_cell = (current_cell + 1) % 12;

		if(current_cell >= 0  && current_cell <= 5){
			int pos = COLUMNS - 1 - current_cell;
			next->cells_computer[pos] = current->cells_computer[pos] + 1;
		}
		else{
			int pos = current_cell - COLUMNS;
			next->cells_player[pos] = current->cells_player[pos] + 1;	
		}

		cout << "cur: " << current_cell << endl;
	}
}

int final_position(Position *current, int computer_play, int current_depth){

	int value = -1;
	return value;
}

int evaluation(Position *p, int computer_play, int current_depth){
	return -1;
}

int valid_move(const Position *p, int computer_play, int column){
	
	if(column < 0 || column >= COLUMNS) // invalid column number
		return 0;

	if(computer_play){
		if(p->cells_computer[column] != 0) //cell has seeds, valid move
			return 1;
		else
			return 0;
	}
	else{
		if(p->cells_player[column] != 0) //cell has seeds, valid move
			return 1;
		else
			return 0;	
	}
}

void print_board(const Position *p, int current_depth){

	cout << endl << "--" << current_depth << "--";
	cout << endl << "------Computer------" << endl;
	for (int i = 0; i < 6; ++i)
	{
		cout << p->cells_computer[i] << ", ";
	}
	cout << endl;
	cout << endl << "------Human------" << endl;
	for (int i = 0; i < 6; ++i)
	{
		cout << p->cells_player[i] << ", ";
	}
	cout << endl;
}