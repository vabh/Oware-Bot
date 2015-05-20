import java.io.*;
import java.util.HashMap;
import java.util.Map;
import java.util.TreeMap;
public class Player {

	static int ROWS = 2;
	static int COLUMNS = 6;
	static int INIT_SEEDS_IN_PIT = 4;
	static int MAX_DEPTH = 10;
	static Map<Position, Move> map;
	public static void main(String args[]) throws IOException {
		
		Position init = new Position();
		for (int i = 0; i < COLUMNS; ++i)
		{
			init.cells_player[i] = INIT_SEEDS_IN_PIT;
			init.cells_computer[i] = INIT_SEEDS_IN_PIT;
		}
		
		init.seeds_computer = 0;
		init.seeds_player = 0;		
		
		Position current = init;
		Position next = new Position();
		System.out.println(current.hashCode());
		boolean computer_play = true;
		int user_move = 0;	
		int computer_move = 0;
		int current_depth = 0;

		int alpha = -100;
		int beta = 100;
		
		System.out.print("Human plays first? (y/n)");
		char c = (new BufferedReader(new InputStreamReader(System.in))).readLine().charAt(0);
		computer_play = c == 'y' ? false : true;
		init.computer_play = computer_play;
		
		for (;;)
		{		
			if(!computer_play){

				map = new TreeMap<Position, Move>();
				
				MAX_DEPTH = 20 + (int)((current.seeds_computer + current.seeds_player) / 8.);
				double t = System.currentTimeMillis();
				Move play = minimax(current, computer_play, user_move, current_depth, alpha, beta);
				double t1 = System.currentTimeMillis();
				System.out.println("\nTime taken: " + (t1 - t)/1000);
				System.out.println("size:" + map.size());

				//generate moves while waiting
				//check for validity of returned move and winning conditions
				play_move(next, current, computer_play, play.column);
				computer_play = !computer_play;				
				System.out.println("Human play: " + play.column + "\n");
				print_board(next, 1);

				current = next;
			}
			else{
				do{
					System.out.print("Enter move computer: ");
					computer_move = Integer.parseInt((new BufferedReader(new InputStreamReader(System.in))).readLine());
					if (valid_move(current, computer_play, computer_move)){

						play_move(next, current, computer_play, computer_move);		
						current = next;
						computer_play = !computer_play;
						print_board(next, 1);
						break;
					}
					else{
						System.out.println("Invalid move!");
						continue;
					}
				}while(!valid_move(current, computer_play, computer_move));
			}
		}
	}

	public static Move minimax(Position pos_current, boolean computer_play,
			int column, int current_depth, int alpha, int beta) {
//		System.out.println("minimax");
		if(map.get(pos_current) != null){
//			System.out.println(map.size());
//			if n.lowerbound >= beta then return n.lowerbound;
//			if n.upperbound <= alpha then return n.upperbound;
			Move m = map.get(pos_current);
			if(m.depth_seen < MAX_DEPTH - current_depth){
				alpha = Math.max(alpha, m.alpha);
				beta = Math.min(beta, m.beta);
			}
//			return m;
		}
		
		int max = -100;
		int min = 100;
		int colummn_to_play_max, colummn_to_play_min;

		Move move = new Move();
		move.alpha = alpha;
		move.beta = beta;
		move.depth_seen = MAX_DEPTH - current_depth;

		if (final_position(pos_current, computer_play, current_depth)) {
			// returns =48 if the computer wins, -48 if it loses; 0 if draw
			if (computer_play) {

				// player won
				if (pos_current.seeds_player > 24) {
					move.score = 48;
					move.column = column;
				} else {
					move.score = evaluation(pos_current, computer_play,
							current_depth);
					move.column = column;
				}
			} else {
				// computer won
				if (pos_current.seeds_computer > 24) {
					move.score = -48;
					move.column = column;
				} else {
					move.score = evaluation(pos_current, computer_play,
							current_depth);
					move.column = column;
				}
			}
//			System.out.println(map.containsKey(pos_current));
			map.put(pos_current, move);
			return move;
		}

		if (current_depth == Player.MAX_DEPTH) {
			move.score = evaluation(pos_current, computer_play, current_depth);
			move.column = column;
//			System.out.println(map.containsKey(pos_current));
//			System.out.println(map.put(pos_current, move));
			map.put(pos_current, move);
			return move;
		}

		// player turn
		if (!computer_play) {
			// each possible move
			for (int i = COLUMNS - 1; i >= 0; i--) {

				if (valid_move(pos_current, computer_play, i)) {

					Position pos_next = new Position();
					play_move(pos_next, pos_current, computer_play, i);
					// pos_next is the new current position and we change the
					// player
					Move c = minimax(pos_next, !computer_play, i, current_depth + 1, alpha, beta);
					int score = c.score;

					if (score > max) {
						max = score;
						colummn_to_play_max = i;

						move.score = max;
						move.column = colummn_to_play_max;
					}

					if (alpha < max) {
						alpha = max;
						move.alpha = alpha;
					}
					// pruning
					if (beta <= alpha) {
						break;
					}

				}
			}
//			System.out.println(map.containsKey(pos_current));
			map.put(pos_current, move);
			return move;
		}
		// computer turn
		else {
			// each possible move
			for (int i = 0; i < COLUMNS; i++) {

				if (valid_move(pos_current, computer_play, i)) {

					Position pos_next = new Position();
					play_move(pos_next, pos_current, computer_play, i);
					// pos_next is the new current poisition and we change the
					// player
//					System.out.println("sdadas: "+pos_next.equals(pos_current));
					Move c = minimax(pos_next, !computer_play, i,
							current_depth + 1, alpha, beta);
					int score = c.score;
					// cout << "Depth " << current_depth << ":" << tab_values[i]
					// << endl;

					if (score < min) {
						min = score;
						colummn_to_play_min = i;

						move.score = min;
						move.column = colummn_to_play_min;
					}

					if (beta > min) {
						beta = min;
						move.beta = beta;
					}

					// pruning
					if (beta <= alpha) {
						break;
					}
				}
			}
//			System.out.println(map.containsKey(pos_current));
			map.put(pos_current, move);
			return move;
		}
	}

	public static void play_move(Position next, Position current, boolean computer_play,
			int start) {
		// copying current state to new state
		for (int i = 0; i < COLUMNS; ++i) // improve this?
		{
			next.cells_player[i] = current.cells_player[i];
			next.cells_computer[i] = current.cells_computer[i];
		}
		
		// in move is invalid, do not change anything
		if (!valid_move(current, computer_play, start)) {
			// next->computer_play = current->computer_play;
			next.seeds_computer = current.seeds_computer;
			next.seeds_player = current.seeds_player;
			return;
		}

		// change player turn
		// next->computer_play = !computer_play;
		next.seeds_computer = current.seeds_computer;
		next.seeds_player = current.seeds_player;

		int current_cell;
		int seeds_to_place;
		// abstract the positions as a ring with 12 cells
		// translate start position from respective boards to the ring
		// easier to do the anti clockwise movement (I think)
		if (computer_play) {

			current_cell = COLUMNS - 1 - start;
			seeds_to_place = next.cells_computer[start];
			next.cells_computer[start] = 0;
		} else {

			current_cell = start + COLUMNS;
			seeds_to_place = next.cells_player[start];
			next.cells_player[start] = 0;
		}

		int start_cell = current_cell;
		while ((seeds_to_place--) != 0) {

			current_cell = (current_cell + 1) % 12; // 2 * COLUMNS
			// skip the cell from which seeds are initially drawn
			if (current_cell == start_cell) {
				seeds_to_place++; // seed has not been placed
				continue;
			}

			if (current_cell >= 0 && current_cell <= 5) {
				int pos = COLUMNS - 1 - current_cell;
				next.cells_computer[pos] = next.cells_computer[pos] + 1;
			} else {
				int pos = current_cell - COLUMNS;
				next.cells_player[pos] = next.cells_player[pos] + 1;
			}
			
		}

		// seed capture from computer cells [player turn]
		if (!computer_play && (current_cell >= 0 && current_cell <= 5)) {
			
			for (int i = current_cell; i >= 0; --i) {
				int pos = COLUMNS - 1 - i;
				int seeds = next.cells_computer[pos];
				if (seeds == 2 || seeds == 3) {
					
					next.seeds_player += seeds;
					next.cells_computer[pos] = 0;
					continue;
				} else {
					break;
				}
			}
		}
		// seed capture from player cells [computer turn]
		else if (computer_play && (current_cell >= 6 && current_cell <= 11)) {

			for (int i = current_cell; i >= 6; --i) {
				int pos = i - COLUMNS;
				int seeds = next.cells_player[pos];
				if (seeds == 2 || seeds == 3) {
					
					next.seeds_computer += seeds;
					next.cells_player[pos] = 0;
					continue;
				} else {
					break;
				}
			}
		}
	}

	public static boolean final_position(Position current, boolean computer_play,
			int current_depth) {

		int final_seeds = 0;

		// winning condition
		if (computer_play && current.seeds_computer > 24) {
			// "Computer wins";
			return true;
		} else if (!computer_play && current.seeds_player > 24) {
			// "Player wins";
			return true;
		}

		// if no move to play
		for (int i = 0; i < COLUMNS; ++i) {
			if (computer_play)
				final_seeds += current.cells_computer[i];
			else
				final_seeds += current.cells_player[i];

			if (final_seeds != 0) {
				// "there is move to play";
				return false;
			}
		}
		return true;
	}

	public static int evaluation(Position p, boolean computer_play, int current_depth) {

		// AM I SURE ?!?!?!
		int difference = p.seeds_player - p.seeds_computer;
		return difference;
	}

	public static boolean valid_move(Position p, boolean computer_play, int column) {

		if (column < 0 || column >= COLUMNS) { // invalid column number
			// "invalid column number: "
			return false;
		}

		if (computer_play) {
			if (p.cells_computer[column] != 0) // cell has seeds, valid move
				return true;
			else {
				// "invalid move computer "
				return false;
			}
		} else if (!computer_play) {
			if (p.cells_player[column] != 0) { // cell has seeds, valid move
				return true;
			} else {
				// "invalid move player "
				return false;
			}
		}
		// move should leave at least one seed for opponent
		else {
			Position pos_next = new Position();
			play_move(pos_next, p, computer_play, column);

			int final_seeds = 0;
			for (int i = 0; i < COLUMNS; ++i) {
				// check for opponent seeds
				if (!computer_play)
					final_seeds += pos_next.cells_computer[i];
				else
					final_seeds += pos_next.cells_player[i];

				if (final_seeds != 0) {
					// at least 1 seed is there for opponent
					return true;
				}
			}
			// opponent has no seeds
			return false;
		}
	}

	public static void print_board(Position p, int current_depth) {

		// cout << endl << "--" << current_depth << "--";
		for (int i = 0; i < COLUMNS; ++i) {
			System.out.print(i + "| ");
		}		
		System.out.println("\n------Computer------");

		for (int i = 0; i < COLUMNS; ++i) {
			System.out.print(p.cells_computer[i] + ", ");
		}

		System.out.println();

		for (int i = 0; i < COLUMNS; ++i) {
			System.out.print(p.cells_player[i] + ", ");
		}
		System.out.println("\n------Human------");
		for (int i = 0; i < COLUMNS; ++i) {
			System.out.print(i + "| ");
		}

		System.out.println("\n\nSeeds Computer: " + p.seeds_computer);
		System.out.println("Seeds Player: " + p.seeds_player);
		System.out.println();
	}
	
	public static void print_board_clockwise(Position p, int current_depth) {

		System.out.println();
		for (int i = 0; i < COLUMNS; ++i) {
			System.out.print(i+"| ");
		}
		System.out.println("\n------Human------");
		for (int i = 0; i < COLUMNS; ++i) {
			System.out.print(p.cells_player[i] + ", ");
		}

		System.out.println();

		for (int i = 0; i < COLUMNS; ++i) {
			System.out.print(p.cells_computer[i] + ", ");
		}
		System.out.println("\n------Computer------");
		for (int i = 0; i < COLUMNS; ++i) {
			System.out.print(i + "| ");
		}

		System.out.println("\n\nSeeds Computer: " + p.seeds_computer);
		System.out.println("Seeds Player: " + p.seeds_player);
		System.out.println();
	}
}