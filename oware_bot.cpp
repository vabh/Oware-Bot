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

	int computer_play = 0;

	Position c = init;
	int user_move;	

	Position n;
	// cout << "Enter move Human: ";
	// 	cin >> user_move;
	// 	cout << endl;
	// play_move(&n, &c, computer_play, user_move);
	// c = n;
	// print_board(&n, 1);
	for (int i = 0; ; ++i)
	{		
		cout << "Enter move Human: ";
		cin >> user_move;
		cout << endl;
		//check for valid move
		play_move(&n, &c, computer_play, user_move);		
		c = n;
		computer_play = !computer_play;
		print_board(&n, 1);

		Move play = minimax(&c, computer_play, user_move, 1);
		play_move(&n, &c, computer_play, play.column);
		computer_play = !computer_play;
				
		c = n;
		print_board(&n, 1);
		cout << endl << i << "  Play: " << play.column << endl;
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
	Move m;
	m.computer_play = computer_play;
	m.column = column;
	if (final_position(pos_current, computer_play, current_depth)){
	// returns VALMAX (=48) if the computer wins, -48 if it loses; 0 if draw
		if(computer_play){
			m.score = 48;
			return m;
		}
		else{
			m.score = -48;
			return m;
		}
	}

	if (current_depth == MAX_DEPTH) {
		m.score = evaluation(pos_current, computer_play, current_depth);
		return m;
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

	for (int i = 0; i < COLUMNS; ++i)
	{
		if (tab_values[i] != 100 && tab_values[i] > max){
			max = tab_values[i];
			colummn_to_play_max = i;
		}
		
		if (tab_values[i] != -100 && tab_values[i] < min){
			min = tab_values[i];
			colummn_to_play_min = i;
		}
		
	}
	
	m.computer_play = computer_play;
	if (computer_play){
		m.score = min;
		m.column = colummn_to_play_min;
	}
	else{
		m.score = max;
		m.column = colummn_to_play_max;
	}

	return m;
}

void play_move(Position *next, const Position *current, int computer_play, int start)
{
	
	for (int i = 0; i < COLUMNS; ++i) // improve this?
	{
		next->cells_player[i] = current->cells_player[i];
		next->cells_computer[i] = current->cells_computer[i];
	}

	if(!valid_move(current, computer_play, start)){
		next->computer_play = current->computer_play;
		next->seeds_computer = current->seeds_computer;
		next->seeds_player = current->seeds_player;
		return; 
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

	// cout << endl << "seeds: " << seeds_to_place << " start_col: " << current_cell << endl;

	while((seeds_to_place--) != 0){
		
		current_cell = (current_cell + 1) % 12; //2 * COLUMNS

		if(current_cell >= 0  && current_cell <= 5){
			int pos = COLUMNS - 1 - current_cell;
			next->cells_computer[pos] = current->cells_computer[pos] + 1;

			//seed capture
			if(next->cells_computer[pos] == 2 || next->cells_computer[pos] == 3){
				next->seeds_computer += next->cells_computer[pos];		
				next->cells_computer[pos] = 0;
			}
		}
		else{
			int pos = current_cell - COLUMNS;
			next->cells_player[pos] = current->cells_player[pos] + 1;
			//seed capture
			if(next->cells_player[pos] == 2 || next->cells_player[pos] == 3){
				next->seeds_player += next->cells_player[pos];		
				next->cells_player[pos] = 0;
			}
		}

		// cout << "cur: " << current_cell << endl;
	}
}

int final_position(const Position *current, int computer_play, int current_depth){

	int final = 0;
	for (int i = 0; i < COLUMNS; ++i)
	{
		if(computer_play)
			final += current->cells_player[i];
		else
			final += current->cells_computer[i];

		if(final != 0)
			return 0;
	}
	return 1;
}

int evaluation(const Position *p, int computer_play, int current_depth){

	int capture = 0;
	for (int i = 0; i < COLUMNS; ++i)
	{
		if(computer_play){
			if(p->cells_computer[i] == 3 || p->cells_computer[i] == 2)
				capture += p->cells_computer[i];
		}
		else{
			if(p->cells_player[i] == 3 || p->cells_player[i] == 2)				
				capture += p->cells_player[i];
		}
	}
	if (computer_play)
		return -capture;
	else
		return capture;
}

int valid_move(const Position *p, int computer_play, int column){
	
	if(column < 0 || column >= COLUMNS){ // invalid column number
		cout << "invalid column number" << endl;
		return 0;
	}

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
	cout << endl;
}