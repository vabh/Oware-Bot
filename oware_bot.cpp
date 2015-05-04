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

struct Move {
	int score;
	int column;
	bool computer_play;
};

Move minimax(const Position *p, int, int, int);
int evaluation(const Position *p, int, int);
int final_position(const Position *p, int, int);
int valid_move(const Position *p, int, int);
void play_move(Position *p, const Position *p1, int c_play, int start);
void print_board(const Position *p, int);

int main(){
	
	Position init, next1, next2, next3;
	for (int i = 0; i < COLUMNS; ++i)
	{
		init.cells_player[i] = INIT_SEEDS_IN_PIT;
		init.cells_computer[i] = INIT_SEEDS_IN_PIT;
	}

	init.computer_play = 0;
	init.seeds_computer = 0;
	init.seeds_player = 0;

	int computer_play = 0;

	Position current = init;	
	Position next;

	int user_move;	
	int computer_move;
	
	for (int i = 0; ; ++i)
	{		
		cout << "Enter move HUMAN: ";
		cin >> user_move;
		cout << endl;
		//check for valid move

		if (valid_move(&current, computer_play, user_move)){		
			play_move(&next, &current, computer_play, user_move);		
			current = next;
			computer_play = !computer_play;
			print_board(&next, 1);
		}
		else{
			cout << "invalid move"  << endl;
			continue;
		}

		cout << "Enter move COMPUTER: ";
		cin >> computer_move;
		cout << endl;
		if (valid_move(&current, computer_play, computer_move)){		
			play_move(&next, &current, computer_play, computer_move);		
			current = next;
			computer_play = !computer_play;
			print_board(&next, 1);
		}
		else{
			cout << "invalid move"  << endl;
			continue;
		}

		// cout << "Computer Move";
		// Move play = minimax(&current, computer_play, user_move, 1);
		//play_move(&next, &current, computer_play, play.column);
		// computer_play = !computer_play;
				
		// current = next;
		//print_board(&current, 1);
		// cout << endl << i << "  Play: " << play.column << endl;
	}

	return 0;
}

Move minimax(const Position* pos_current, int computer_play, int column, int current_depth){
	// computer_play is true if the computer has to play and false otherwise

	// print_board(pos_current, current_depth);
	int tab_values[6];
	Position pos_next; // In C : created on the stack: = very fast
	int max, min;

	int colummn_to_play_max, colummn_to_play_min;
	Move move;
	move.computer_play = computer_play;
	move.column = column;

	if (final_position(pos_current, computer_play, current_depth)){
	// returns VALMAX (=48) if the computer wins, -48 if it loses; 0 if draw
		if(computer_play){
			move.score = 48;
			return move;
		}
		else{
			move.score = -48;
			return move;
		}
	}

	if (current_depth == MAX_DEPTH) {
		move.score = evaluation(pos_current, computer_play, current_depth);
		return move;
		// the simplest evealution fucntion is the difference of the taken seeds
	}

	//each possible move
	for(int i = 0;i < COLUMNS; i++){
	
		if (valid_move(pos_current, computer_play, i)){

			play_move(&pos_next, pos_current, computer_play, i);
			// pos_next is the new current poisition and we change the player
			Move c = minimax(&pos_next, !computer_play, i, current_depth + 1);
			tab_values[i] = c.score;
		}
		else {
			if (computer_play) 
				tab_values[i]= -100;
			else
				tab_values[i]= +100;			
		}		
	}
	
	// colummn_to_play_min = 0;
	// colummn_to_play_max = 0;

	
	//player should return min value
	if (!computer_play){

		min = tab_values[0];
		colummn_to_play_min = 0;
		for (int i = 1; i < 6; ++i)
		{
			if(tab_values[i] < min){
				min = tab_values[i];
				colummn_to_play_min = i;
			}
		}

		move.score = min;
		move.column = colummn_to_play_min;
	}
	//computer should return max value
	else{

		max = tab_values[0];
		colummn_to_play_max = 0;
		for (int i = 1; i < 6; ++i)
		{
			if (tab_values[i] > max)
			{
				max = tab_values[i];
				colummn_to_play_max = i;
			}
		}
		move.score = max;
		move.column = colummn_to_play_max;
	}

	return move;
}

void play_move(Position *next, const Position *current, int computer_play, int start)
{
	//copying current state to new state
	for (int i = 0; i < COLUMNS; ++i) // improve this?
	{
		next->cells_player[i] = current->cells_player[i];
		next->cells_computer[i] = current->cells_computer[i];
	}
	//in move is invalid, do not change anything
	if(!valid_move(current, computer_play, start)){
		next->computer_play = current->computer_play;
		next->seeds_computer = current->seeds_computer;
		next->seeds_player = current->seeds_player;
		return; 
	}

	//change player turn
	next->computer_play = !computer_play;
	next->seeds_computer = current->seeds_computer;
	next->seeds_player = current->seeds_player;

	int current_cell;
	int seeds_to_place;
	//abstract the positions as a ring with 12 cells
	//translate start position from respective boards to the ring
	//easier to do the anti clockwise movement (I think)
	if(computer_play){

		current_cell = COLUMNS - 1 - start;
		seeds_to_place = next->cells_computer[start];
		next->cells_computer[start] = 0;		
	}
	else{

		current_cell = start + COLUMNS;		
		seeds_to_place = next->cells_player[start];
		next->cells_player[start] = 0;
	}

	// cout << endl << "seeds: " << seeds_to_place << " start_col: " << current_cell << endl;

	int start_cell = current_cell;
	while((seeds_to_place--) != 0){
		
		current_cell = (current_cell + 1) % 12; //2 * COLUMNS
		//skip the cell from which seeds are initially drawn
		if(current_cell == start_cell){
			seeds_to_place++; //seed has not been placed
			continue;
		}

		if(current_cell >= 0  && current_cell <= 5){
			int pos = COLUMNS - 1 - current_cell;
			next->cells_computer[pos] = next->cells_computer[pos] + 1;			
		}
		else{
			int pos = current_cell - COLUMNS;
			next->cells_player[pos] = next->cells_player[pos] + 1;
		}
		cout << "current cell:" << current_cell << endl;
	}

	//seed capture from computer cells [player turn]
	if(!computer_play && (current_cell >= 0 && current_cell <= 5)){
		cout << endl <<  "player captures seeds";
		for (int i = current_cell; i >= 0; --i)
		{			
			int pos = COLUMNS - 1 - i;
			int seeds = next->cells_computer[pos];
			if(seeds == 2 || seeds == 3){
				cout << endl << "capture from: " << pos;
				next->seeds_player += seeds;
				next->cells_computer[pos] = 0;
				continue;
			}
			else{
				break;
			}
		}
	}
	//seed capture from player cells [computer turn]
	else if(computer_play && (current_cell >= 6 && current_cell <= 11)){
		cout << endl <<  "computer captures seeds";
		for (int i = current_cell; i >= 6; --i)
		{			
			int pos = i - COLUMNS;
			int seeds = next->cells_player[pos];
			if(seeds == 2 || seeds == 3){
				cout << endl << "capture from: " << pos;
				next->seeds_computer += seeds;
				next->cells_player[pos] = 0;
				continue;
			}
			else{
				break;
			}
		}			
	}
}

int final_position(const Position *current, int computer_play, int current_depth){

	int final = 0;
	
	//winning condition
	if(computer_play && current->seeds_computer > 24){
		cout << endl << "Computer wins";
		return 1;
	}
	else if(!computer_play && current->seeds_player > 24){
		cout << endl << "Player wins";
		return 1;
	}

	//if no move to play
	for (int i = 0; i < COLUMNS; ++i)
	{
		if(computer_play)
			final += current->cells_computer[i];
		else
			final += current->cells_player[i];

		if(final != 0){
			cout << endl << "no move to play";
			return 0;
		}
	}
	return 1;
}

int evaluation(const Position *p, int computer_play, int current_depth){

	//AM I SURE ?!?!?!
	int difference = p->seeds_player - p->seeds_computer;
	if(computer_play){
		return difference;
	}
	else{
		return -difference;
	}
}

int valid_move(const Position *p, int computer_play, int column){
	
	if(column < 0 || column >= COLUMNS){ // invalid column number
		cout << "invalid column number: " << column << endl;
		return 0;
	}

	if(computer_play){
		if(p->cells_computer[column] != 0) //cell has seeds, valid move
			return 1;
		else{
			cout << "invalid move computer " << column << endl;
			return 0;
		}
	}
	else{
		if(p->cells_player[column] != 0) //cell has seeds, valid move
			return 1;
		else{
			cout << "invalid move player " << column << endl;
			return 0;	
		}
	}
}

void print_board(const Position *p, int current_depth){

	cout << endl << "--" << current_depth << "--";
	cout << endl << "------Computer------" << endl;
	for (int i = 0; i < COLUMNS; ++i)
	{
		cout << p->cells_computer[i] << ", ";
	}
	cout << endl;
	cout << endl << "------Human------" << endl;
	for (int i = 0; i < COLUMNS; ++i)
	{
		cout << p->cells_player[i] << ", ";
	}
	cout << endl << "Seeds Computer: " << p->seeds_computer;
	cout << endl << "Seeds Player: " << p->seeds_player;
	cout << endl;
}