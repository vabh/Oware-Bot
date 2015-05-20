#include <iostream>
#include <sys/time.h>
#include <ctime>
#include <vector>
#include <unordered_map>

using namespace std;

#define ROWS 2
#define COLUMNS 6
// #define MAX_DEPTH 17
#define INIT_SEEDS_IN_PIT 4

typedef unordered_map<int, int> move_map;
typedef unordered_map<int, move_map> depth_map;

typedef long long int64; typedef unsigned long long uint64;

//credit to SO user @andreas-bonini
uint64 GetTimeMs64()
{
	#ifdef _WIN32
	 /* Windows */
	 FILETIME ft;
	 LARGE_INTEGER li;

	 /* Get the amount of 100 nano seconds intervals elapsed since January 1, 1601 (UTC) and copy it
	  * to a LARGE_INTEGER structure. */
	 GetSystemTimeAsFileTime(&ft);
	 li.LowPart = ft.dwLowDateTime;
	 li.HighPart = ft.dwHighDateTime;

	 uint64 ret = li.QuadPart;
	 ret -= 116444736000000000LL; /* Convert from file time to UNIX epoch time. */
	 ret /= 10000; /* From 100 nano seconds (10^-7) to 1 millisecond (10^-3) intervals */

	 return ret;
	#else
	 /* Linux */
	 struct timeval tv;

	 gettimeofday(&tv, NULL);

	 uint64 ret = tv.tv_usec;
	 /* Convert from micro seconds (10^-6) to milliseconds (10^-3) */
	 ret /= 1000;

	 /* Adds the seconds (10^0) after converting them to milliseconds (10^-3) */
	 ret += (tv.tv_sec * 1000);

	 return ret;
	#endif
}

struct Position {
   int cells_player[6];
   int cells_computer[6];
   int seeds_player;
   int seeds_computer;
};

struct Move {
	int score;
	int column;	
};

Move minimax(const Position *p, int, int, int, int, int);
int evaluation(const Position *p, int, int);
int final_position(const Position *p, int, int);
int valid_move(const Position *p, int, int);
void play_move(Position *p, const Position *p1, int c_play, int start);
void print_board(const Position *p, int);
void print_board_clockwise(const Position *p, int);

int MAX_DEPTH = 17;

int main(){
	
	Position init;
	for (int i = 0; i < COLUMNS; ++i)
	{
		init.cells_player[i] = INIT_SEEDS_IN_PIT;
		init.cells_computer[i] = INIT_SEEDS_IN_PIT;
	}
	
	init.seeds_computer = 0;
	init.seeds_player = 0;

	int computer_play = 1;

	Position current = init;	
	Position next;

	int user_move = 0;	
	int computer_move = 0;
	int current_depth = 0;

	int alpha = -100;
	int beta = 100;
	
	cout << "Human plays first? (1/0)";
	cin >> computer_play;
	computer_play = !computer_play;

	for (;;)
	{		
		if(!computer_play){

			MAX_DEPTH = 17 + (int)((current.seeds_computer + current.seeds_player) / 8.);
			uint64 t1 = GetTimeMs64();
			Move play = minimax(&current, computer_play, user_move, current_depth, alpha, beta);
			uint64 t2 = GetTimeMs64();

			//generate moves while waiting
			//check for validity of returned move and winning conditions
			play_move(&next, &current, computer_play, play.column);
			computer_play = !computer_play;
			cout << endl << "Human Play: " << play.column << " in: " << (t2 - t1) / 1000. << endl << endl;
			print_board(&next, 1);

			current = next;
		}
		else{
			do{
				cout << endl << "Enter move COMPUTER: ";
				cin >> computer_move;
				cout << endl;
				if (valid_move(&current, computer_play, computer_move)){

					play_move(&next, &current, computer_play, computer_move);		
					current = next;
					computer_play = !computer_play;
					print_board(&next, 1);
					break;
				}
				else{
					cout << "invalid move"  << endl;
					continue;
				}
			}while(!valid_move(&current, computer_play, computer_move));
		}
	}
	return 0;
}

Move minimax(const Position* pos_current, int computer_play, int column, int current_depth, int alpha, int beta){
	// computer_play is true if the computer has to play and false otherwise

	// print_board(pos_current, current_depth);	
	//Position pos_next;
	int max = -100, min = 100;
	int colummn_to_play_max, colummn_to_play_min;

	Move move;		

	if (final_position(pos_current, computer_play, current_depth)){
	// returns =48 if the computer wins, -48 if it loses; 0 if draw
		if(computer_play){

			//player won
			if (pos_current->seeds_player > 24){
				move.score = 48;	
				move.column = column;						
			}
			else{
				move.score = evaluation(pos_current, computer_play, current_depth);
				move.column = column;
			}						
		}
		else{
			//computer won
			if (pos_current->seeds_computer > 24){
				move.score = -48;
				move.column = column;
			}
			else{
				move.score = evaluation(pos_current, computer_play, current_depth);
				move.column = column;
			}			
		}
		return move;
	}

	if (current_depth == MAX_DEPTH) {
		move.score = evaluation(pos_current, computer_play, current_depth);
		move.column = column;
		return move;
	}

	//player turn
	if(!computer_play){
		//each possible move
		for(int i = COLUMNS - 1;i >= 0; i--){
	
			if (valid_move(pos_current, computer_play, i)){

				Position pos_next;
				play_move(&pos_next, pos_current, computer_play, i);
				// pos_next is the new current poisition and we change the player
				Move c = minimax(&pos_next, !computer_play, i, current_depth + 1, alpha, beta);
				int score = c.score;			
				// cout << "Depth " << current_depth << ":" << tab_values[i] << endl;

				if (score > max){
					max = score;
					colummn_to_play_max = i;

					move.score = max;
					move.column = colummn_to_play_max;
				}

				if(alpha < max){
					alpha = max;
				}
				//pruning
				if(beta <= alpha){
					break;
				}

			}		
		}
		return move;
	}
	//computer turn
	else{
		// each possible move
		for(int i = COLUMNS - 1;i >=0 ; i--){

			if (valid_move(pos_current, computer_play, i)){ 

				Position pos_next;
				play_move(&pos_next, pos_current, computer_play, i);
				// pos_next is the new current poisition and we change the player
				Move c = minimax(&pos_next, !computer_play, i, current_depth + 1, alpha, beta);
				int score = c.score;			
				// cout << "Depth " << current_depth << ":" << tab_values[i] << endl;

				if(score < min){
					min = score;
					colummn_to_play_min = i;

					move.score = min;
					move.column = colummn_to_play_min;
				}

				if(beta > min){
					beta = min;
				}

				//pruning
				if(beta <= alpha){
					break;
				}
			}
		}
		return move;
	}		
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
		// next->computer_play = current->computer_play;
		next->seeds_computer = current->seeds_computer;
		next->seeds_player = current->seeds_player;
		return; 
	}

	//change player turn
	// next->computer_play = !computer_play;
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
		//cout << "current cell:" << current_cell << endl;
	}

	//seed capture from computer cells [player turn]
	if(!computer_play && (current_cell >= 0 && current_cell <= 5)){
		//cout << endl <<  "player captures seeds";
		for (int i = current_cell; i >= 0; --i)
		{			
			int pos = COLUMNS - 1 - i;
			int seeds = next->cells_computer[pos];
			if(seeds == 2 || seeds == 3){
				//cout << endl << "capture from: " << pos;
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
		//cout << endl <<  "computer captures seeds";
		for (int i = current_cell; i >= 6; --i)
		{			
			int pos = i - COLUMNS;
			int seeds = next->cells_player[pos];
			if(seeds == 2 || seeds == 3){
				//cout << endl << "capture from: " << pos;
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
		//cout << endl << "Computer wins";
		return 1;
	}
	else if(!computer_play && current->seeds_player > 24){
		//cout << endl << "Player wins";
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
			//cout << endl << "there is move to play";
			return 0;
		}
	}
	return 1;
}

int evaluation(const Position *p, int computer_play, int current_depth){

	//AM I SURE ?!?!?!
	int difference = p->seeds_player - p->seeds_computer;
	return difference;
}

int valid_move(const Position *p, int computer_play, int column){
	
	if(column < 0 || column >= COLUMNS){ // invalid column number
		//cout << "invalid column number: " << column << endl;
		return 0;
	}

	if(computer_play){
		if(p->cells_computer[column] != 0) //cell has seeds, valid move
			return 1;
		else{
			//cout << "invalid move computer " << column << endl;
			return 0;
		}
	}
	else{
		if(p->cells_player[column] != 0) //cell has seeds, valid move
			return 1;
		else{
			//cout << "invalid move player " << column << endl;
			return 0;	
		}
	}

	//move should leave atleast one seed for opponent
	Position pos_next;
	play_move(&pos_next, p, computer_play, column);
	int final = 0;
	for (int i = 0; i < COLUMNS; ++i)
	{
		//check for opponent seeds
		if(!computer_play)
			final += pos_next.cells_computer[i];
		else
			final += pos_next.cells_player[i];

		if(final != 0){			
			//atleast 1 seed is there for opponent
			return 1;
		}
	}
	//opponent has no seeds
	return 0;

}

void print_board(const Position *p, int current_depth){

	//cout << endl << "--" << current_depth << "--";
	for (int i = 0; i < COLUMNS; ++i)
	{
		cout << i << "| ";
	}	
	cout << endl << "------Computer------" << endl;
	for (int i = 0; i < COLUMNS; ++i)
	{
		cout << p->cells_computer[i] << ", ";
	}

	cout << endl;

	for (int i = 0; i < COLUMNS; ++i)
	{
		cout << p->cells_player[i] << ", ";
	}
	cout << endl << "------Human------" << endl;
	for (int i = 0; i < COLUMNS; ++i)
	{
		cout << i << "| ";
	}

	cout << endl << endl << "Seeds Computer: " << p->seeds_computer;
	cout << endl << "Seeds Player: " << p->seeds_player;
	cout << endl;
}

void print_board_clockwise(const Position *p, int current_depth){

	//cout << endl << "--" << current_depth << "--";
	for (int i = 0; i < COLUMNS; ++i)
	{
		cout << i << "| ";
	}	
	cout << endl << "------Human------" << endl;
	for (int i = 0; i < COLUMNS; ++i)
	{
		cout << p->cells_player[i] << ", ";
	}

	cout << endl;

	for (int i = 0; i < COLUMNS; ++i)
	{
		cout << p->cells_computer[i] << ", ";
	}
	cout << endl << "------Computer------" << endl;
	for (int i = 0; i < COLUMNS; ++i)
	{
		cout << i << "| ";
	}

	cout << endl << endl << "Seeds Computer: " << p->seeds_computer;
	cout << endl << "Seeds Player: " << p->seeds_player;
	cout << endl;
}