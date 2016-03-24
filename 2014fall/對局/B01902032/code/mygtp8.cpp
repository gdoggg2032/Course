/*
 * This code is provied as a sample code of Hw 2 of "Theory of Computer Game".
 * The "genmove" function will randomly output one of the legal move.
 * This code can only be used within the class.
 *
 * 2014 Dec. Hung-Jui Chang
 * */
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <math.h>
 #include <algorithm>


#define BOARDSIZE        9
#define BOUNDARYSIZE    11
#define COMMANDLENGTH 1000
#define DEFAULTTIME     10
#define DEFAULTKOMI      7

#define MAXGAMELENGTH 1000
#define MAXSTRING       50
#define MAXDIRECTION     4
#define MAXDIRECTION8    8

#define NUMINTERSECTION 81
#define HISTORYLENGTH   200

#define EMPTY            0
#define BLACK            1
#define WHITE            2
#define BOUNDARY         3

#define SELF             1
#define OPPONENT         2

#define NUMGTPCOMMANDS      15

#define LOCALVERSION      1
#define GTPVERSION        2

#define Exploration       0.1126
#define PRUNING_RATIO      1.5
#define VAR_EQUAL			0.5
#define BOUN_PAIN 			0.9
 
using namespace std;
int _board_size = BOARDSIZE;
int _board_boundary = BOUNDARYSIZE;
double _komi =  DEFAULTKOMI;
const int DirectionX[MAXDIRECTION] = {-1, 0, 1, 0};
const int DirectionY[MAXDIRECTION] = { 0, 1, 0,-1};
const int DirectionX8[MAXDIRECTION8] = {-1, 0, 1, 0, -1, 1, 1, -1};
const int DirectionY8[MAXDIRECTION8] = { 0, 1, 0,-1, -1, -1, 1, 1};

const char LabelX[]="0ABCDEFGHJ";
const int REPEAT[9] = {30, 30, 30, 30, 30, 30, 30, 30, 30};// Normal, eat, conn, attack, bound, eat && conn, conn && attack, attack && bound, one liberty
const double PAIN[9] = {1, 1, 1, 1, 0.7, 1, 1, 0.9, 0.6};

/*
 * This function display the current game board, empty intersection is 0,
 * black intersection is 1, white intersection is 2 and boundary is 3.
 * */


struct MCTNode
{
	int move;
	double N;
	double W;
	int Child_num;
	int turn;
	int expanded;
	struct MCTNode *Child[81];
	struct MCTNode *parent;
	int Board[BOUNDARYSIZE][BOUNDARYSIZE];
	int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE];
	int game_length;
	int end;
	int eat;
	int conn;
	int attack;   
	int boun;
	int lib;
	int undone;
	double UCB;
	int type;
};
struct AMAF
{
	int length;
	int MoveList[MAXGAMELENGTH];
};

void display(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int time_limit) {
    cout << "time limit=" << time_limit << endl;
    for (int i = 1; i <=BOARDSIZE; ++i) {
	cout <<10-i;
	for (int j = 1; j <=BOARDSIZE; ++j) {
	    switch(Board[i][j]) {
		case EMPTY: cout << " .";break;
		case BLACK: cout << " X";break;
		case WHITE: cout << " O";break;
	    }
	}
	cout << endl;
    }
    cout << "  ";
    for (int i = 1; i <=BOARDSIZE; ++i) 
	cout << i <<" ";
    cout << endl;
    cout << endl;
}

/*
 * This function reset the board, the board intersections are labeled with 0,
 * the boundary intersections are labeled with 3.
 * */
void reset(int Board[BOUNDARYSIZE][BOUNDARYSIZE]) {
    for (int i = 1 ; i <= BOARDSIZE; ++i) {
	for (int j = 1 ; j <= BOARDSIZE; ++j) {
	    Board[i][j] = EMPTY;
	}
    }
    for (int i = 0 ; i < BOUNDARYSIZE; ++i) {
	Board[0][i] = Board[BOUNDARYSIZE-1][i] = Board[i][0] = Board[i][BOUNDARYSIZE-1] = BOUNDARY;
    }
}

/*
 * This function return the total number of liberity of the string of (X, Y) and
 * the string will be label with 'label'.
 * */
int find_liberty(int X, int Y, int label, int Board[BOUNDARYSIZE][BOUNDARYSIZE], int ConnectBoard[BOUNDARYSIZE][BOUNDARYSIZE]) {
    // Label the current intersection
    if ( X < 1 || X > BOARDSIZE || Y < 1 || Y > BOARDSIZE)return 0;
    ConnectBoard[X][Y] |= label;
    int total_liberty = 0;
    for (int d = 0 ; d < MAXDIRECTION; ++d) {
	// Check this intersection has been visited or not
	if ((ConnectBoard[X+DirectionX[d]][Y+DirectionY[d]] & (1<<label) )!= 0) continue;

	// Check this intersection is not visited yet
	ConnectBoard[X+DirectionX[d]][Y+DirectionY[d]] |=(1<<label);
	if(X+DirectionX[d] < 1 || X+DirectionX[d] > BOARDSIZE || Y+DirectionY[d] < 1 || Y+DirectionY[d] > BOARDSIZE )continue;
	// This neighboorhood is empty
	if (Board[X+DirectionX[d]][Y+DirectionY[d]] == EMPTY){
	    total_liberty++;
	}
	// This neighboorhood is self stone
	else if (Board[X+DirectionX[d]][Y+DirectionY[d]] == Board[X][Y] && (Board[X][Y] == WHITE || Board[X][Y] == BLACK)) {
	    total_liberty += find_liberty(X+DirectionX[d], Y+DirectionY[d], label, Board, ConnectBoard);
	}
    }
    return total_liberty;
}

/*
 * This function count the liberties of the given intersection's neighboorhod
 * */
void count_liberty(int X, int Y, int Board[BOUNDARYSIZE][BOUNDARYSIZE], int Liberties[MAXDIRECTION]) {
    int ConnectBoard[BOUNDARYSIZE][BOUNDARYSIZE]={0};
    // Initial the ConnectBoard
   /* for (int i = 0 ; i < BOUNDARYSIZE; ++i) {
	for (int j = 0 ; j < BOUNDARYSIZE; ++j) {
	    ConnectBoard[i][j] = 0;
	}
    }*/
    // Find the same connect component and its liberity
    for (int d = 0 ; d < MAXDIRECTION; ++d) {
	Liberties[d] = 0;
	if (Board[X+DirectionX[d]][Y+DirectionY[d]] == BLACK ||  
	    Board[X+DirectionX[d]][Y+DirectionY[d]] == WHITE    ) {
	    Liberties[d] = find_liberty(X+DirectionX[d], Y+DirectionY[d], d, Board, ConnectBoard);
	}
    }
}

/*
 * This function count the number of empty, self, opponent, and boundary intersections of the neighboorhod
 * and saves the type in NeighboorhoodState.
 * */
void count_neighboorhood_state(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int X, int Y, int turn, int* empt, int* self, int* oppo ,int* boun, int NeighboorhoodState[MAXDIRECTION]) {
    for (int d = 0 ; d < MAXDIRECTION; ++d) {
	// check the number of nonempty neighbor
	switch(Board[X+DirectionX[d]][Y+DirectionY[d]]) {
	    case EMPTY:    (*empt)++; 
			   NeighboorhoodState[d] = EMPTY;
			   break;
	    case BLACK:    if (turn == BLACK) {
			       (*self)++;
			       NeighboorhoodState[d] = SELF;
			   }
			   else {
			       (*oppo)++;
			       NeighboorhoodState[d] = OPPONENT;
			   }
			   break;
	    case WHITE:    if (turn == WHITE) {
			       (*self)++;
			       NeighboorhoodState[d] = SELF;
			   }
			   else {
			       (*oppo)++;
			       NeighboorhoodState[d] = OPPONENT;
			   }
			   break;
	    case BOUNDARY: (*boun)++;
			   NeighboorhoodState[d] = BOUNDARY;
			   break;
	}
    }
}
void count_neighboorhood_state8(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int X, int Y, int turn, int* empt, int* self, int* oppo ,int* boun, int NeighboorhoodState[MAXDIRECTION8]) {
    for (int d = 0 ; d < MAXDIRECTION8; ++d) {
	// check the number of nonempty neighbor
	switch(Board[X+DirectionX8[d]][Y+DirectionY8[d]]) {
	    case EMPTY:    (*empt)++; 
			   NeighboorhoodState[d] = EMPTY;
			   break;
	    case BLACK:    if (turn == BLACK) {
			       (*self)++;
			       NeighboorhoodState[d] = SELF;
			   }
			   else {
			       (*oppo)++;
			       NeighboorhoodState[d] = OPPONENT;
			   }
			   break;
	    case WHITE:    if (turn == WHITE) {
			       (*self)++;
			       NeighboorhoodState[d] = SELF;
			   }
			   else {
			       (*oppo)++;
			       NeighboorhoodState[d] = OPPONENT;
			   }
			   break;
	    case BOUNDARY: (*boun)++;
			   NeighboorhoodState[d] = BOUNDARY;
			   break;
	}
    }
}
/*
 * This function remove the connect component contains (X, Y) with color "turn" 
 * And return the number of remove stones.
 * */
int remove_piece(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int X, int Y, int turn) {
    int remove_stones = (Board[X][Y]==EMPTY)?0:1;
    Board[X][Y] = EMPTY;
    for (int d = 0; d < MAXDIRECTION; ++d) {
	if (Board[X+DirectionX[d]][Y+DirectionY[d]] == turn) {
	    remove_stones += remove_piece(Board, X+DirectionX[d], Y+DirectionY[d], turn);
	}
    }
    return remove_stones;
}
/*
 * This function update Board with place turn's piece at (X,Y).
 * Note that this function will not check if (X, Y) is a legal move or not.
 * */
void update_board(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int X, int Y, int turn) {
    int num_neighborhood_self = 0;
    int num_neighborhood_oppo = 0;
    int num_neighborhood_empt = 0;
    int num_neighborhood_boun = 0;
    int Liberties[4];
    int NeighboorhoodState[4];
    count_neighboorhood_state(Board, X, Y, turn,
	    &num_neighborhood_empt,
	    &num_neighborhood_self,
	    &num_neighborhood_oppo,
	    &num_neighborhood_boun, NeighboorhoodState);
    // check if there is opponent piece in the neighboorhood
    if (num_neighborhood_oppo != 0) {
	count_liberty(X, Y, Board, Liberties);
	for (int d = 0 ; d < MAXDIRECTION; ++d) {
	    // check if there is opponent component only one liberty
	    if (NeighboorhoodState[d] == OPPONENT && Liberties[d] == 1 && Board[X+DirectionX[d]][Y+DirectionY[d]]!=EMPTY) {
		remove_piece(Board, X+DirectionX[d], Y+DirectionY[d], Board[X+DirectionX[d]][Y+DirectionY[d]]);
	    }
	}
    }
    Board[X][Y] = turn;
}
/*
 * This function update Board with place turn's piece at (X,Y).
 * Note that this function will check if (X, Y) is a legal move or not.
 * */
int update_board_check(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int X, int Y, int turn) {
    // Check the given coordination is legal or not
    if ( X < 1 || X > BOARDSIZE || Y < 1 || Y > BOARDSIZE || Board[X][Y]!=EMPTY)
	return 0;
    int num_neighborhood_self = 0;
    int num_neighborhood_oppo = 0;
    int num_neighborhood_empt = 0;
    int num_neighborhood_boun = 0;
    int Liberties[4];
    int NeighboorhoodState[4];
    count_neighboorhood_state(Board, X, Y, turn,
	    &num_neighborhood_empt,
	    &num_neighborhood_self,
	    &num_neighborhood_oppo,
	    &num_neighborhood_boun, NeighboorhoodState);
    // Check if the move is a legal move
    // Condition 1: there is a empty intersection in the neighboorhood
    int legal_flag = 0;
    count_liberty(X, Y, Board, Liberties);
    if (num_neighborhood_empt != 0) {
	legal_flag = 1;
    }
    else {
	// Condition 2: there is a self string has more than one liberty
	for (int d = 0; d < MAXDIRECTION; ++d) {
	    if (NeighboorhoodState[d] == SELF && Liberties[d] > 1) {
		legal_flag = 1;
	    }
	}
	if (legal_flag == 0) {
	// Condition 3: there is a opponent string has exactly one liberty
	    for (int d = 0; d < MAXDIRECTION; ++d) {
		if (NeighboorhoodState[d] == OPPONENT && Liberties[d] == 1) {
		    legal_flag = 1;
		}
	    }
	}
    }

    if (legal_flag == 1) {
    // check if there is opponent piece in the neighboorhood
	if (num_neighborhood_oppo != 0) {
	    for (int d = 0 ; d < MAXDIRECTION; ++d) {
		// check if there is opponent component only one liberty
		if (NeighboorhoodState[d] == OPPONENT && Liberties[d] == 1 && Board[X+DirectionX[d]][Y+DirectionY[d]]!=EMPTY) {
		    remove_piece(Board, X+DirectionX[d], Y+DirectionY[d], Board[X+DirectionX[d]][Y+DirectionY[d]]);
		}
	    }
	}
	Board[X][Y] = turn;
    }

    return (legal_flag==1)?1:0;
}

/*
 * This function return the number of legal moves with clor "turn" and
 * saves all legal moves in MoveList
 * */
int gen_legal_move(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int turn, int game_length, int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE], int MoveList[HISTORYLENGTH]) {
    int NextBoard[BOUNDARYSIZE][BOUNDARYSIZE];
    int num_neighborhood_self = 0;
    int num_neighborhood_oppo = 0;
    int num_neighborhood_empt = 0;
    int num_neighborhood_boun = 0;
    int num_neighborhood_self8 = 0;
    int num_neighborhood_oppo8 = 0;
    int num_neighborhood_empt8 = 0;
    int num_neighborhood_boun8 = 0;
    int legal_moves = 0;
    int next_x, next_y;
    int Liberties[4];
    int NeighboorhoodState[4];
    int NeighboorhoodState8[8];
    bool eat_move = 0;
    bool conn_move = 0;
    bool attack_move = 0;
    bool boun_move = 0;
    bool lib_move = 0;

    for (int x = 1 ; x <= BOARDSIZE; ++x) {
	for (int y = 1 ; y <= BOARDSIZE; ++y) {
	    // check if current 
	    if (Board[x][y] == 0) {
		// check the liberty of the neighborhood intersections
		num_neighborhood_self = 0;
		num_neighborhood_oppo = 0;
		num_neighborhood_empt = 0;
		num_neighborhood_boun = 0;
		num_neighborhood_self8 = 0;
		num_neighborhood_oppo8 = 0;
		num_neighborhood_empt8 = 0;
		num_neighborhood_boun8 = 0;
		// count the number of empy, self, opponent, and boundary neighboorhood
		count_neighboorhood_state(Board, x, y, turn,
			&num_neighborhood_empt,
			&num_neighborhood_self,
			&num_neighborhood_oppo,
			&num_neighborhood_boun, NeighboorhoodState);
		count_neighboorhood_state8(Board, x, y, turn,
			&num_neighborhood_empt8,
			&num_neighborhood_self8,
			&num_neighborhood_oppo8,
			&num_neighborhood_boun8, NeighboorhoodState8);
		// check if the emtpy intersection is a legal move
		next_x = 0;
		next_y = 0;
		eat_move = 0;
		conn_move = 0;
		attack_move = 0;
		boun_move = 0;
		lib_move = 0;

		if(num_neighborhood_oppo > 0)
			attack_move = 1;
		if((num_neighborhood_self>1&&num_neighborhood_oppo>0)||num_neighborhood_oppo == 2)
		{
			conn_move = 1;
		}
		if(num_neighborhood_boun>0)
			boun_move = 1;

		count_liberty(x, y, Board, Liberties);
		
		// Case 1: exist empty intersection in the neighborhood
		if (num_neighborhood_empt > 0) {
		     next_x = x;
		     next_y = y;
		     // check if it is a capture move
		     for (int d = 0 ; d < MAXDIRECTION; ++d) {
				 if (NeighboorhoodState[d] == OPPONENT && Liberties[d] == 1) {
				     eat_move = 1;
				 }
		     }
		     if(num_neighborhood_oppo == 3 && num_neighborhood_self == 0 && eat_move ==0)
		 		lib_move = 1;

		 }
		 

		 // Case 2: no empty intersection in the neighborhood
		 else {
		    // Case 2.1: Surround by the self piece
		    if (num_neighborhood_self + num_neighborhood_boun == MAXDIRECTION) {
			int check_flag = 0, check_eye_flag = num_neighborhood_boun;
			for (int d = 0 ; d < MAXDIRECTION; ++d) {
			    // Avoid fill self eye
			    if (NeighboorhoodState[d]==SELF && Liberties[d] > 1) {
				check_eye_flag++;
			    }
			    // Check if there is one self component which has more than one liberty
			    if (NeighboorhoodState[d]==SELF && Liberties[d] > 1) {
				check_flag = 1;
			    }
			}
			if (check_flag == 1 && check_eye_flag!=4) {
			    next_x = x;
			    next_y = y;
			}
		    }	
		    // Case 2.2: Surround by opponent or both side's pieces.
		    else if (num_neighborhood_oppo > 0) {
			int check_flag = 0;
			int eat_flag = 0;
			
			for (int d = 0 ; d < MAXDIRECTION; ++d) {
			    // Check if there is one self component which has more than one liberty
			    if (NeighboorhoodState[d]==SELF && Liberties[d] > 1) {
				check_flag = 1;
			    }
			    // Check if there is one opponent's component which has exact one liberty
			    if (NeighboorhoodState[d]==OPPONENT && Liberties[d] == 1) {
				eat_flag = 1;
			    }
			}
			if (check_flag == 1) {
			    next_x = x;
			    next_y = y;
			    if (eat_flag == 1) {
				eat_move = 1;
			    }
			}
			else { // check_flag == 0
			    if (eat_flag == 1) {
				next_x = x;
				next_y = y;
				eat_move = 1;
			    }
			}
		    }	
		 }
		 if (next_x !=0 && next_y !=0) {
		 	//fprintf(stderr,"8\n");
		 // copy the current board to next board
		    for (int i = 0 ; i < BOUNDARYSIZE; ++i) {
				for (int j = 0 ; j < BOUNDARYSIZE; ++j) {
				    NextBoard[i][j] = Board[i][j];
				}
		    }
		    // do the move
		    // The move is a capture move and the board needs to be updated.
		    if (eat_move == 1) {
				update_board(NextBoard, next_x, next_y, turn);
		    }
		    else {
				NextBoard[x][y] = turn;
		    }
		    // Check the history to avoid the repeat board
		    bool repeat_move = 0;
		    for (int t = game_length-1 ; t >= 0 && t > game_length-5  ; t--) {
				bool repeat_flag = 1;
				for (int i = 1; i <=BOARDSIZE; ++i) {
			 	   for (int j = 1; j <=BOARDSIZE; ++j) {
						if (NextBoard[i][j] != GameRecord[t][i][j]) {
						    repeat_flag = 0;
						}
			 	    }
				}
				if (repeat_flag == 1) {
				    repeat_move = 1;
				    break;
				}
		    }
		    if (repeat_move == 0) {
			// 3 digit zxy, z means eat or not, and put at (x, y)
			MoveList[legal_moves] = lib_move * 1000000 + boun_move*100000 +  attack_move * 10000 + conn_move * 1000 + eat_move * 100 + next_x * 10 + y ;
			legal_moves++;
		    }
		 }
	    }
	}
    }
    return legal_moves;
}
int check_legal_move(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int turn, int game_length, int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE], int *Move) {
    int NextBoard[BOUNDARYSIZE][BOUNDARYSIZE];
    int num_neighborhood_self = 0;
    int num_neighborhood_oppo = 0;
    int num_neighborhood_empt = 0;
    int num_neighborhood_boun = 0;
    int num_neighborhood_self8 = 0;
    int num_neighborhood_oppo8 = 0;
    int num_neighborhood_empt8 = 0;
    int num_neighborhood_boun8 = 0;
    int legal_moves = 0;
    int next_x, next_y;
    int Liberties[4];
    int NeighboorhoodState[4];
    int NeighboorhoodState8[8];
    bool eat_move = 0;
    bool conn_move = 0;
    bool attack_move = 0;
    bool boun_move = 0;
    bool lib_move = 0;

   	int x = (*Move%100)/10;
   	int y = *Move%10;
   	//fprintf(stderr,"x = %d, y = %d\n",x,y);
	    // check if current 
   	//fprintf(stderr,"0\n");
   	//display(Board,10);
	if (Board[x][y] == 0) {
		//fprintf(stderr,"1\n");
		// check the liberty of the neighborhood intersections
		num_neighborhood_self = 0;
		num_neighborhood_oppo = 0;
		num_neighborhood_empt = 0;
		num_neighborhood_boun = 0;
		num_neighborhood_self8 = 0;
		num_neighborhood_oppo8 = 0;
		num_neighborhood_empt8 = 0;
		num_neighborhood_boun8 = 0;
		// count the number of empy, self, opponent, and boundary neighboorhood
		count_neighboorhood_state(Board, x, y, turn,
			&num_neighborhood_empt,
			&num_neighborhood_self,
			&num_neighborhood_oppo,
			&num_neighborhood_boun, NeighboorhoodState);
		count_neighboorhood_state8(Board, x, y, turn,
			&num_neighborhood_empt8,
			&num_neighborhood_self8,
			&num_neighborhood_oppo8,
			&num_neighborhood_boun8, NeighboorhoodState8);
		// check if the emtpy intersection is a legal move
		next_x = next_y = 0;
		eat_move = 0;
		conn_move = 0;
		attack_move = 0;
		boun_move = 0;
		lib_move = 0;
		count_liberty(x, y, Board, Liberties);
		// Case 1: exist empty intersection in the neighborhood

		if(num_neighborhood_oppo > 0)
			attack_move = 1;
		if((num_neighborhood_self>1&&num_neighborhood_oppo>0)||num_neighborhood_oppo == 2)
		{
			conn_move = 1;
		}
		if(num_neighborhood_boun>0)
			boun_move = 1;
		

		 if (num_neighborhood_empt > 0) {
		 	//fprintf(stderr,"3\n");
		     next_x = x;
		     next_y = y;
		     // check if it is a capture move
		     for (int d = 0 ; d < MAXDIRECTION; ++d) {
				 if (NeighboorhoodState[d] == OPPONENT && Liberties[d] == 1) {
				     eat_move = 1;
				 }
		     }
		     if(num_neighborhood_oppo == 3 && num_neighborhood_self == 0 && eat_move ==0)
		 		lib_move = 1;

		 }
		
		 // Case 2: no empty intersection in the neighborhood
		 else {
		 	//fprintf(stderr,"4\n");
		    // Case 2.1: Surround by the self piece
		    if (num_neighborhood_self + num_neighborhood_boun == MAXDIRECTION) {
		    	//fprintf(stderr,"5\n");
				int check_flag = 0, check_eye_flag = num_neighborhood_boun;
				for (int d = 0 ; d < MAXDIRECTION; ++d) {
				    // Avoid fill self eye
				    if (NeighboorhoodState[d]==SELF && Liberties[d] > 1) {
					check_eye_flag++;
				    }
				    // Check if there is one self component which has more than one liberty
				    if (NeighboorhoodState[d]==SELF && Liberties[d] > 1) {
					check_flag = 1;
				    }
				}
				if (check_flag == 1 && check_eye_flag!=4) {
				    next_x = x;
				    next_y = y;
				}
		    }	
		    // Case 2.2: Surround by opponent or both side's pieces.
		    else if (num_neighborhood_oppo > 0) {
		    	//fprintf(stderr,"6\n");
				int check_flag = 0;
				int eat_flag = 0;

				
				for (int d = 0 ; d < MAXDIRECTION; ++d) {
				    // Check if there is one self component which has more than one liberty
				    if (NeighboorhoodState[d]==SELF && Liberties[d] > 1) {
					check_flag = 1;
				    }
				    // Check if there is one opponent's component which has exact one liberty
				    if (NeighboorhoodState[d]==OPPONENT && Liberties[d] == 1) {
				    	//fprintf(stderr,"OKOKOKOKOKOKOKOKtoeat, (x,y) = (%d,%d)\n",x,y);
				    	//display(Board,0);
					eat_flag = 1;
				    }
				}
				if (check_flag == 1) {
				    next_x = x;
				    next_y = y;
				    if (eat_flag == 1) {
					eat_move = 1;
				    }
				}
				else { // check_flag == 0
					//fprintf(stderr,"7\n");
				    if (eat_flag == 1) {
						next_x = x;
						next_y = y;
						eat_move = 1;
			    	}
				}
		    }	
		 }
		 if (next_x !=0 && next_y !=0) {
		 	//fprintf(stderr,"8\n");
		 // copy the current board to next board
		    /*for (int i = 0 ; i < BOUNDARYSIZE; ++i) {
				for (int j = 0 ; j < BOUNDARYSIZE; ++j) {
				    NextBoard[i][j] = Board[i][j];
				}
		    }*/
		    // do the move
		    // The move is a capture move and the board needs to be updated.
		    if (eat_move == 1) {
				update_board(Board, next_x, next_y, turn);
		    }
		    else {
				Board[x][y] = turn;
		    }
		    // Check the history to avoid the repeat board
		    bool repeat_move = 0;
		    int track;
		    for (int t = game_length-1 ; t >= 0 && t > game_length-5  ; t--) {
				bool repeat_flag = 1;
				for (int i = 1; i <=BOARDSIZE; ++i) {
			 	   for (int j = 1; j <=BOARDSIZE; ++j) {
						if (Board[i][j] != GameRecord[t][i][j]) {
						    repeat_flag = 0;
						}
			 	    }
				}
				if (repeat_flag == 1) {
				    repeat_move = 1;
				    track = t;
				   	break;
				}
		    }
		    if(repeat_move == 1)
		    {
		    	for (int i = 0 ; i < BOUNDARYSIZE; ++i) {
					for (int j = 0 ; j < BOUNDARYSIZE; ++j) {
				    	Board[i][j] = GameRecord[track-1][i][j];
					}
		    	}

		    }
		    else if (repeat_move == 0) {
			// 3 digit zxy, z means eat or not, and put at (x, y)
				*Move = lib_move * 1000000 + boun_move*100000 + attack_move * 10000 + conn_move * 1000 + eat_move * 100 + next_x * 10 + y ;
				return 1;
		    }
		 }
		
	    }
	
    return 0;
}
/*
 * This function randomly selects one move from the MoveList.
 * */
int rand_pick_move(int num_legal_moves, int MoveList[HISTORYLENGTH]) {
    if (num_legal_moves == 0)
	return 0;
    else {
	int move_id = rand()%num_legal_moves;
	return MoveList[move_id];
    }
}
/*
 * This function update the Board with put 'turn' at (x,y)
 * where x = (move % 100) / 10 and y = move % 10.
 * Note this function will not check 'move' is legal or not.
 * */
void do_move(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int turn, int move) {
    int move_x = (move % 100) / 10;
    int move_y = move % 10;
    if (move<100) {
	Board[move_x][move_y] = turn;
    }
    else {
	update_board(Board, move_x, move_y, turn);
    }

}
/* 
 * This function records the current game baord with current
 * game length "game_length"
 * */
void record(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE], int game_length) {
		for (int i = 0 ; i < BOUNDARYSIZE; ++i) {
		    for (int j = 0 ; j < BOUNDARYSIZE; ++j) {
			GameRecord[game_length][i][j] = Board[i][j];
		    }
		}
}



int MCS_pick_move(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int turn, int time_limit, int game_length, int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE],int num_legal_moves, int MoveList[HISTORYLENGTH]);
struct MCTNode * selectTree(struct MCTNode * targetNode,int game_length,int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE]);
struct MCTNode * expandTree(struct MCTNode * targetNode ,int game_length,int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE]);
int simulation(struct MCTNode * targetNode, int game_length,int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE], struct AMAF* AMAF_list);
void Backup( MCTNode* targetNode, int delta );
struct MCTNode * BestChild(struct MCTNode* targetNode);
void Record_copy(int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE],int game_length, int copy_Record[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE]);
/* 
 * This function randomly generate one legal move (x, y) with return value x*10+y,
 * if there is no legal move the function will return 0.
 * */
int genmove(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int turn, int time_limit, int game_length, int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE]) {
    clock_t start_t, end_t, now_t;
    // record start time
    start_t = clock();
    // calculate the time bound
    end_t = start_t + CLOCKS_PER_SEC * time_limit;

    int MoveList[HISTORYLENGTH];
    int num_legal_moves = 0;
    int return_move = 0;

    num_legal_moves = gen_legal_move(Board, turn, game_length, GameRecord, MoveList);

    return_move = MCS_pick_move(Board, turn, time_limit, game_length, GameRecord,num_legal_moves, MoveList);

    do_move(Board, turn, return_move);
    display(Board,0);
    return return_move % 100;
}
int random_genmove(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int turn, int time_limit, int game_length, int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE],int type) {
    clock_t start_t, end_t, now_t;
    // record start time
    start_t = clock();
    // calculate the time bound
    end_t = start_t + CLOCKS_PER_SEC * time_limit;

    int MoveList[HISTORYLENGTH];
    int num_legal_moves = 0;
    int return_move = 0;
    int x,y;
    int MoveCan[HISTORYLENGTH];
    int can_num;
    int i,j;
    int count;
    //fprintf(stderr,"random_genmove: game_length = %d\n",game_length);
    if(type == 0)
    {
    	num_legal_moves = gen_legal_move(Board, turn, game_length, GameRecord, MoveList);

    	return_move = rand_pick_move(num_legal_moves, MoveList);
    }
    else
    {

    	
    	if(game_length>45)
    	{
    		can_num = 0;
    		count = 0;
    		for(i=1;i<=BOARDSIZE;i++)
    		{
    			for(j=1;j<=BOARDSIZE;j++)
    			{
    				if(Board[i][j]==0)
    				{
    					MoveCan[can_num] = 10*i+j;
    					can_num++;
    				}
    					

    			}
    		}
    		if(can_num == 0)
    		{
    			return_move = 0;
    			do_move(Board, turn, return_move);
    			return return_move % 100;
	    			
    		}
    		
    		//return_move = MoveCan[rand()%can_num];
    		
    		random_shuffle(MoveCan, MoveCan+can_num);
    		return_move = MoveCan[count];
    		count++;

    	}
    	else
    	{
    		x=rand()%9+1;
	    	y=rand()%9+1;
	    	return_move = 10*x+y;
    	}
    	//fprintf(stderr,"before check: %d\n",return_move);
    	//display(Board,10);
    	while(check_legal_move(Board, turn, game_length, GameRecord, &return_move)==0)
    	{
    		//if(return_move>100)
    			//fprintf(stderr,">>>>>>>>>>>>>>>>>>>>>>after check: %d\n",return_move);
    		if(game_length>45)
	    	{
	    		if(count==can_num)
	    		{
	    			//fprintf(stderr,"pass\n");
	    			return_move = 0;
	    			break;
	    		}
	    		return_move = MoveCan[count];
	    		count++;
	    		

	    	}
	    	else
	    	{
	    		x=rand()%9+1;
		    	y=rand()%9+1;
		    	return_move = 10*x+y;
	    	}

    		//fprintf(stderr,"before check: %d\n",return_move);
    	}

    	//fprintf(stderr,"after check: %d\n",return_move);
    	x=(return_move%100)/10;
    	y=(return_move%100)%10;

    	
    	//if(Board[x][y]!=0&&return_move!=0)fprintf(stderr,"Board[%d][%d] is not NULL!!!\n\n\n\n",x,y);
    }
   
    //fprintf(stderr,"do_move, turn= %d\n",turn);
    //do_move(Board, turn, return_move);

    return return_move % 100;
}
/*
 * This function counts the number of points remains in the board by Black's view
 * */
double final_score(int Board[BOUNDARYSIZE][BOUNDARYSIZE]) {
    int black, white;
    black = white = 0;
    int is_black, is_white;
    for (int i = 1 ; i <= BOARDSIZE; ++i) {
	for (int j = 1; j <= BOARDSIZE; ++j) {
	    switch(Board[i][j]) {
		case EMPTY:
		    is_black = is_white = 0;
		    for(int d = 0 ; d < MAXDIRECTION; ++d) {
			if (Board[i+DirectionX[d]][j+DirectionY[d]] == BLACK) is_black = 1;
			if (Board[i+DirectionX[d]][j+DirectionY[d]] == WHITE) is_white = 1;
		    }
		    if (is_black + is_white == 1) {
			black += is_black;
			white += is_white;
		    }
		    break;
		case WHITE:
		    white++;
		    break;
		case BLACK:
		    black++;
		    break;
	    }
	}
    }
    return black - white;
}

double MCS_gtp_main(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int turn, int game_length, int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE], int type, struct AMAF* AMAF_list) {
	srand(time(NULL));
    int time_limit = DEFAULTTIME;
    int pass_num=0;
    int move;
    int oppo_turn = 3-turn;
    while (pass_num<2) {
		

	   	//if(game_length>70)type = 0;

	    move = random_genmove(Board, turn, time_limit, game_length, GameRecord,type);
	    //fprintf(stderr,"move = %d, length = %d\n",move,game_length);
	    if(move==0)pass_num++;
	    else pass_num=0;
	    record(Board, GameRecord, game_length+1);
	    game_length++;
	    if(move != 0)
	    {
	    	AMAF_list->MoveList[AMAF_list->length] = move;
	    	AMAF_list->length++;
	    }
	    
	    
	    
	    if(pass_num==2)break;

		move = random_genmove(Board, oppo_turn, time_limit, game_length, GameRecord,type);
		
		if(move==0)pass_num++;
	    else pass_num=0;
	    record(Board, GameRecord, game_length+1);
	    game_length++;
	    
		
	
    }
  	double result;
    result = final_score(Board);
    result -= _komi;
	return result;
	

}
void Board_copy(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int copy_Board[BOUNDARYSIZE][BOUNDARYSIZE])
{
	int i;
	for(i=0;i<BOUNDARYSIZE;i++)
	{
		memcpy(copy_Board[i],Board[i],sizeof(Board[i]));
		//for(j=0;j<BOUNDARYSIZE;j++)
		//	copy_Board[i][j] = Board[i][j];
	}
}
void Record_copy(int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE],int game_length, int copy_Record[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE])
{
	int i,k;
	for(k=0;k<game_length;k++)
	{
		for(i=0;i<BOUNDARYSIZE;i++)
		{
			memcpy(copy_Record[k][i],GameRecord[k][i],sizeof(GameRecord[k][i]));
			//for(j=0;j<BOUNDARYSIZE;j++)
			//	copy_Record[k][i][j] = GameRecord[k][i][j];
		}
	}
	
}
struct MCTNode * selectTree(struct MCTNode * targetNode,int game_length, int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE])
{
	
    //num_legal_moves = gen_legal_move(Board, turn, game_length, GameRecord, MoveList);
    struct MCTNode * oldNode = NULL;
    while(targetNode->end !=1 )
    {
    	//fprintf(stderr,"in select loop target child num = %d\n",targetNode->Child_num);

    	if(targetNode->Child_num ==0)
    	{
    		fprintf(stderr, " before expand : %lf / %lf  = %d ,%d\n", targetNode->W, targetNode->N, targetNode->move,targetNode->turn);
    		return expandTree(targetNode,game_length,GameRecord);
    	}
    		
    	else
    	{
    		oldNode = targetNode;
    		targetNode = BestChild(targetNode);
    	}
    	if(oldNode == targetNode)break;
    		
    	//num_legal_moves = gen_legal_move(Board, turn, game_length, GameRecord, MoveList);
    }
    return targetNode;
}
struct MCTNode * expandTree(struct MCTNode * targetNode ,int game_length,int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE])
{

	int MoveList[HISTORYLENGTH];
    int num_legal_moves = 0;
    int i;

    num_legal_moves = gen_legal_move(targetNode->Board, targetNode->turn, game_length, GameRecord, MoveList);
    for(i=0;i<num_legal_moves;i++)
    {
    	struct MCTNode* returnNode = new struct MCTNode;
		targetNode->Child[i] = returnNode;
		targetNode->Child_num++;
		returnNode->move = MoveList[i];
		returnNode->parent = targetNode;
		returnNode->turn = targetNode->turn;
		returnNode->Child_num = 0;
		returnNode->expanded = false;
		returnNode->W = 0;
		returnNode->N = 0;
		returnNode->game_length = targetNode->game_length;
		returnNode->undone = 1;
		
		if((returnNode->move)%1000 / 100 == 1)
			returnNode->eat = 1;
		else
			returnNode->eat = 0;
		if((returnNode->move)%10000 / 1000 == 1)
			returnNode->conn = 1;
		else
			returnNode->conn = 0;
		if((returnNode->move)%100000 / 10000 == 1)
			returnNode->attack = 1;
		else 
			returnNode->attack = 0;
		if((returnNode->move)%1000000 / 100000 == 1)
			returnNode->boun = 1;
		else 
			returnNode->boun = 0;
		if((returnNode->move)%10000000 / 1000000 == 1)
			returnNode->lib = 1;
		else 
			returnNode->lib = 0;

		Board_copy(targetNode->Board,returnNode->Board);
		Record_copy(targetNode->GameRecord,returnNode->game_length, returnNode->GameRecord);
    	do_move(returnNode->Board,returnNode->turn,returnNode->move);
    	    	
    	record(returnNode->Board, returnNode->GameRecord, returnNode->game_length+1);
	    returnNode->game_length++;
	    //returnNode->turn = 3 - returnNode->turn;

		
		//Board_copy(targetNode->Board,returnNode->Board);
	
    }
  //  fprintf(stderr,"in expanded: %d\n",targetNode->Child_num);
    /*random a returnNode */
    if (num_legal_moves == 0)
    {
    	targetNode->end =1;
    	targetNode->expanded = false;
    	return targetNode;
    }
			

	targetNode->expanded = false;
	return targetNode->Child[0];

}
int tell_move(struct MCTNode * targetNode)
{
	int type;
	if(targetNode->eat == 1)
	{
		if(targetNode->conn  == 1)type = 5;
		else type = 1;
	}
	else if(targetNode->conn  == 1)
	{
		if(targetNode->attack == 1)type = 6;
		else type = 2;
	}
	else if(targetNode->lib == 1)
			type = 8;
	else if(targetNode->attack == 1)
	{
		if(targetNode->boun == 1) type = 7;
		else type = 3;
	}
			
	else if(targetNode->boun == 1)type = 4;
	
	else type = 0;
	targetNode->type = type;
	return type;
}
struct MCTNode * BestChild(struct MCTNode* targetNode)
{
	struct MCTNode* returnNode = NULL;
	//double cp = 1.0 / sqrt(2.0);

    double uct = 0;
    double childValue;
    double childVisit;
    double uctN = 0;
    int type;
	int i;

	for(i=0;i<targetNode->Child_num;i++)
	{
		childValue = targetNode->Child[i]->W;
		childVisit = targetNode->Child[i]->N;
        type = tell_move(targetNode->Child[i]);
        double tmpUct = childValue / childVisit + Exploration*sqrt(log((double)(targetNode->N)) / childVisit);
        tmpUct *= PAIN[type];
        targetNode->Child[i]->UCB = tmpUct;
        if(tmpUct > uct /*&& targetNode->undone == 0*/)
        {
               uct = tmpUct;
               uctN = childVisit;
               returnNode = targetNode->Child[i];
        }
        else if (tmpUct == uct /*&& targetNode->undone == 0*/)
        {
        	if(childVisit>uctN)
        	{
        		uct = tmpUct;
	            uctN = childVisit;
	            returnNode = targetNode->Child[i];

        	}
        	else if (childVisit == uctN)
        	{
        		double x1,y1,x2,y2;
        		x1 = (returnNode->move %100) /10;
        		y1 = returnNode->move %10;
        		x2 = (targetNode->Child[i]->move %100) / 10;
        		y2 = targetNode->Child[i]->move %10;
        		if((x1-5)*(x1-5)+(y1-5)*(y1-5) > (x2-5)*(x2-5)+(y2-5)*(y2-5))
        		{
        			uct = tmpUct;
		            uctN = childVisit;
		            returnNode = targetNode->Child[i];
        		}
        	}
        }
        //if(targetNode->undone == 1)targetNode->undone = 0;
        //fprintf(stderr,"in BestChild loop: %lf,%lf,%lf\n",tmpUct,childValue,childVisit);
	}
	if(returnNode == NULL)
	{
		//fprintf(stderr,"in BestChild: returnNode == NULL\n");

		return targetNode;
	}
        
    else
    	return returnNode;
}

int simulation(struct MCTNode * targetNode, int game_length,int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE], struct AMAF * AMAF_list)//type: 0=all, 1=rand
{
	int ret;
	int type = 1;
	//fprintf(stderr,"------------\n");
	//display(targetNode->Board,0);
	//fprintf(stderr,"------------\n");
	int copy_Board[BOUNDARYSIZE][BOUNDARYSIZE];
	Board_copy(targetNode->Board,copy_Board);
	int copy_game_length = targetNode->game_length;
	//fprintf(stderr,"before MCS_gtp_main\n");
	ret = MCS_gtp_main(copy_Board,targetNode->turn,targetNode->game_length,GameRecord,type, AMAF_list);
	//fprintf(stderr,"in sim: ret : %d\n",ret);
	//Board_copy(copy_Board,targetNode->Board);
	targetNode->game_length = copy_game_length;

	/*if(targetNode->turn ==BLACK)
	{
		//fprintf(stderr,"erwr\n");
		if(ret>0)
		{
			return 1;
			//argetNode->W++;
		}
		

	}
	else if (targetNode->turn ==WHITE)
	{//fprintf(stderr,"erwwwwwwwwr\n");
		if(ret<0)
		{
			return 1;
			//targetNode->W++;
		}
		
	}
	return 0;*/
	if(ret>0)return 1;
	else return 0;
	//fprintf(stderr,"QWEQWE>>>>%lf\n",targetNode->W);
	//return ;
}
void Backup( struct MCTNode* targetNode, int delta )
{
      while (targetNode != NULL)
      {
        targetNode->N += 1;
        if(targetNode->expanded == false)
            targetNode->W += delta;
        else
            targetNode->expanded = false;
        targetNode = targetNode->parent;

      }

}
void deBackup(struct MCTNode* targetNode )
{
	double W,N;
	 while (targetNode != NULL)
      {
        W = targetNode->W;
        N = targetNode->N;
        targetNode = targetNode->parent;
        targetNode->W -= W;
        targetNode->N -= N;

      }
}
void AMAF_Backup( struct MCTNode* childNode, int delta ,struct AMAF* AMAF_list)
{

	struct MCTNode* targetNode = childNode->parent;
	int i,j;
	//fprintf(stderr,"child:%d\n",targetNode->Child_num);
	for(i=0;i<targetNode->Child_num;i++)
	{
		for(j=0;j<AMAF_list->length;j++)
		{
			if(targetNode->Child[i]->move == AMAF_list->MoveList[j])
			{
				Backup(targetNode->Child[i],delta);
			}
		}
	}


}
struct Move
{
	double mean;
	double var;
	double ml;
	double mr;

};
void conutingMove(struct MCTNode* targetNode, struct Move *targetMove)
{
	double mean = 0;
	double tmpquasum = 0;
	double var;
	double ml;
	double mr;
	double max=0;
	double max_child = 0;
	int i;
	for(i=0;i<targetNode->Child_num;i++)
	{
		mean += targetNode->Child[i]->W / targetNode->Child[i]->N ;
		tmpquasum += (targetNode->Child[i]->W / targetNode->Child[i]->N)*(targetNode->Child[i]->W / targetNode->Child[i]->N ) ;
	}
	targetMove->mean = mean / targetNode->Child_num;
	tmpquasum = tmpquasum / targetNode->Child_num;
	targetMove->var = sqrt(tmpquasum - mean*mean);
	targetMove->ml = targetMove->mean - targetMove->var * PRUNING_RATIO;
	targetMove->mr = targetMove->mean + targetMove->var * PRUNING_RATIO;


}
 
void pruningTree(struct MCTNode* targetNode)
{
	if(targetNode->Child_num>1)
	{
		int i;
		
		double max=0;
		int max_child = 0;
		struct Move MoveChild[targetNode->Child_num];
	
		int saveChild[targetNode->Child_num];
		int save_num = targetNode->Child_num;
		for(i=0;i<targetNode->Child_num;i++)
		{
			conutingMove(targetNode->Child[i],&MoveChild[i]);	
			if(MoveChild[i].mean >max)
			{
				max = MoveChild[i].mean;
				max_child = i;
			}
			saveChild[i] = 1;
		}

		for(i=0;i<targetNode->Child_num;i++)
		{
			if(MoveChild[i].mr < MoveChild[max_child].ml )
			{
				saveChild[i] = 0;
				save_num--;
			}

		}
		for(i=0;i<targetNode->Child_num;i++)
		{
			if(save_num == 1)break;
			if(MoveChild[i].var >= VAR_EQUAL && saveChild[i] == 1)
			{
				saveChild[i] = 0;
				save_num--;
			}
		}
		int count = 0;
		for(i=0;i<targetNode->Child_num;i++)
		{
			if(saveChild[i]==1)
			{
				targetNode->Child[count] = targetNode->Child[i];
				count++;
			}
			else
			{
				deBackup(targetNode->Child[i]);
				fprintf(stderr,"prune: %d = %lf / %lf\n",targetNode->Child[i]->move, targetNode->Child[i]->W, targetNode->Child[i]->N);
				delete targetNode->Child[i];

			}
				
		}
		targetNode->Child_num = count;
		
		//pruningTree(targetNode->parent);
	}
	

}

int tell33(struct MCTNode * targetNode)
{	
	int count = 0;
	if(targetNode->Board[3][3]== EMPTY)count++;
	if(targetNode->Board[7][3]== EMPTY)count++;
	if(targetNode->Board[3][7]== EMPTY)count++;
	if(targetNode->Board[7][7]== EMPTY)count++;
	return count;
}
int choose_best33(struct MCTNode * targetNode)
{
	int best[8][8];
	int best_count[8] ={0};
	int num_neighborhood_self = 0;
    int num_neighborhood_oppo = 0;
    int num_neighborhood_empt = 0;
    int num_neighborhood_boun = 0;
    int NeighboorhoodState[8];
    int choice[4] = {33,37,73,77};
    int i,j;
    int x,y;
    int move;
    for(i=0;i<4;i++)
    	for(j=0;j<4;j++)
    		best[i][j] = 0;
    for(i=0;i<4;i++)
    {

    	x = choice[i] / 10;
    	y = choice[i] % 10;
    	if(targetNode->Board[x][y]!=0)continue;
    	num_neighborhood_self = 0;
	    num_neighborhood_oppo = 0;
	    num_neighborhood_empt = 0;
	    num_neighborhood_boun = 0;
    	count_neighboorhood_state8(targetNode->Board, x, y, targetNode->turn,
	    &num_neighborhood_empt,
	    &num_neighborhood_self,
	    &num_neighborhood_oppo,
	    &num_neighborhood_boun, NeighboorhoodState);
	    best[8-num_neighborhood_empt][best_count[8-num_neighborhood_empt]] = choice[i];
	    best_count[8-num_neighborhood_empt]++;
    }
    for(i=0;i<2;i++)
    	for(j=0;j<4;j++)
    		if(best[i][j]!=0)
    			return  best[i][j];
    return -1;
}
int tellside(struct MCTNode * targetNode)
{
	int count = 0;
	if(targetNode->Board[5][3]== EMPTY)count++;
	if(targetNode->Board[7][5]== EMPTY)count++;
	if(targetNode->Board[5][7]== EMPTY)count++;
	if(targetNode->Board[3][5]== EMPTY)count++;
	return count;
}
int choose_bestside(struct MCTNode * targetNode)
{
	int best[8][8];
	int best_count[8] ={0};
	int num_neighborhood_self = 0;
    int num_neighborhood_oppo = 0;
    int num_neighborhood_empt = 0;
    int num_neighborhood_boun = 0;
    int NeighboorhoodState[8];
    int choice[4] = {53,35,75,57};
    int i,j;
    int x,y;
    int move;
    for(i=0;i<4;i++)
    	for(j=0;j<4;j++)
    		best[i][j] = 0;
    for(i=0;i<4;i++)
    {

    	x = choice[i] / 10;
    	y = choice[i] % 10;
    	if(targetNode->Board[x][y]!=0)continue;
    	num_neighborhood_self = 0;
	    num_neighborhood_oppo = 0;
	    num_neighborhood_empt = 0;
	    num_neighborhood_boun = 0;
    	count_neighboorhood_state8(targetNode->Board, x, y, targetNode->turn,
	    &num_neighborhood_empt,
	    &num_neighborhood_self,
	    &num_neighborhood_oppo,
	    &num_neighborhood_boun, NeighboorhoodState);
	    best[8-num_neighborhood_empt][best_count[8-num_neighborhood_empt]] = choice[i];
	    best_count[8-num_neighborhood_empt]++;
    }
    int movex;
    int movey;
    int safe = 0;

    for(i=0;i<2;i++)
    	for(j=0;j<4;j++)
    	{
    		if(best[i][j]!=0)
    		{
    			move = best[i][j];
    			movex = (move%100)/10;
    			movey = (move%100)%10;
    			for (int d = 0 ; d < MAXDIRECTION; ++d) 
    			{
    				if(targetNode->Board[movex+2*DirectionX[d]][movey+2*DirectionY[d]] == targetNode->turn)
    					return  best[i][j];
    			}

    		}
    			
    	}
    		
    return -1;
}
int countside(struct MCTNode * targetNode)
{
	int count = 0;
	if(targetNode->Board[5][3]== targetNode->turn)count++;
	if(targetNode->Board[7][5]== targetNode->turn)count++;
	if(targetNode->Board[5][7]== targetNode->turn)count++;
	if(targetNode->Board[3][5]== targetNode->turn)count++;
	return count;
}
int tellmiddle(struct MCTNode * targetNode)
{
	//int best[8][8];
	int best_count[8] ={0};
	int num_neighborhood_self = 0;
    int num_neighborhood_oppo = 0;
    int num_neighborhood_empt = 0;
    int num_neighborhood_boun = 0;
    int NeighboorhoodState[8];
    int choice = 55;
    int i,j;
    int move;
    if(targetNode->Board[5][5]!=0)return -1;
    
	num_neighborhood_self = 0;
    num_neighborhood_oppo = 0;
    num_neighborhood_empt = 0;
    num_neighborhood_boun = 0;
	count_neighboorhood_state8(targetNode->Board, 5, 5, targetNode->turn,
    &num_neighborhood_empt,
    &num_neighborhood_self,
    &num_neighborhood_oppo,
    &num_neighborhood_boun, NeighboorhoodState);
    
    if(num_neighborhood_oppo<=1 && countside(targetNode)>=2)
    	return 1;
    else
    	return -1;
}

int MCS_pick_move(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int turn, int time_limit, int game_length, int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE],int num_legal_moves, int MoveList[HISTORYLENGTH]) {
    if (num_legal_moves == 0)
		return 0;
	else if(num_legal_moves == 1)
		return MoveList[0];
    else {
    	
    	int copy_Board[BOUNDARYSIZE][BOUNDARYSIZE];
    	int copy_Record[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE];
    	double W[num_legal_moves];
    	double N[num_legal_moves];
    	double UCB[num_legal_moves];
    	double Nsum;
    	double Wsum;
    	int i,j;
    	int ret;
    	j=0;
    	memset(W,0,sizeof(W));
    	memset(N,0,sizeof(N));
    	Nsum = 0;
    	Wsum = 0;
    	clock_t start_time = clock();
    	clock_t now_time = clock();
    	struct MCTNode * root = new struct MCTNode;
    	root->expanded = false;
    	root->turn = turn;
    	root->W = 0;
    	root->N = 0;
    	root->move = 0;
    	root->game_length = game_length;
    	struct MCTNode* selectedNode;
    	//root->Child_num = num_legal_moves;
    	int delta;
    	int count=0;
    	int prun_count=0;
    	int type;
    	//if(num_legal_moves > 30)type = 1;
    	//else
    	//	type = 0;
    	type = 0;
    	int repeat_type;
    	int move_33;
    	int move_side;
    	Board_copy(Board,root->Board);
    	Record_copy(GameRecord, root->game_length, root->GameRecord);
    	if(tell33(root) > 0 && root->game_length <=3)
    	{
    		move_33 = choose_best33(root);

    		if(move_33 > 0)
    		{
    			root->game_length++;
    			return move_33;
    		}
    		
    	}
    	/*if(tellside(root) > 0 && root->game_length <= 7)
    	{
    		move_side = choose_bestside(root);
    		if(move_side > 0)
			{
				root->game_length++;
				return move_side;
			}
    	}*/
    	//if(tellmiddle(root) > 0 && root->game_length <=18)
    	//	return 55;
    	struct AMAF *AMAF_list = new struct AMAF;
    	AMAF_list->length = 0;
    	while(now_time-start_time<time_limit*CLOCKS_PER_SEC*0.997)
    	{
    		//fprintf(stderr, "before select\n" );
    		
    		int copy_turn = turn;
    		int copy_time_limit = time_limit;
	    	int copy_game_length = game_length;
    		//selection
    		
    		selectedNode = selectTree(root,root->game_length,root->GameRecord);
    		//fprintf(stderr, "after select\n" );
    		//simulation
    		now_time = clock();
    		for(i=0;i<selectedNode->parent->Child_num&&now_time-start_time<time_limit*CLOCKS_PER_SEC*0.997;i++)
    		{

    			repeat_type = tell_move(selectedNode);
    			    			//fprintf(stderr,">>type: %d\n",type);
    			

				for(j=0;j<REPEAT[repeat_type]&&now_time-start_time<time_limit*CLOCKS_PER_SEC*0.997;j++)
				{
					//fprintf(stderr,"1\n");
					AMAF_list->length = 0;
					AMAF_list->MoveList[AMAF_list->length] = selectedNode->move;
					AMAF_list->length++;
					//fprintf(stderr,"2\n");
					delta = simulation(selectedNode,selectedNode->game_length,selectedNode->GameRecord,AMAF_list);
					//fprintf(stderr,"before backup: %d\n",delta);

					Backup( selectedNode, delta );
					//AMAF_Backup( selectedNode, delta , AMAF_list);
					//fprintf(stderr,"after backup\n");
					now_time = clock();
					if(now_time-start_time>=time_limit*CLOCKS_PER_SEC*0.997)break;
					count++;
					
					now_time = clock();

				}
				

				if(now_time-start_time>=time_limit*CLOCKS_PER_SEC*0.997)break;
			
    			fprintf(stderr,">>>>>>>>>>>%d,%lu,%d , %d = %d,%d\n",count,(now_time-start_time)/CLOCKS_PER_SEC,num_legal_moves,selectedNode->game_length, selectedNode->move,selectedNode->turn);
				
				selectedNode = selectedNode->parent->Child[(i+1)%selectedNode->parent->Child_num];
				
			}
    		if(now_time-start_time>=time_limit*CLOCKS_PER_SEC*0.997)break;
    		pruningTree(root);
    		//if(count>)
    		//fprintf(stderr, "before pruning\n" );
    		//for(i=0;i<root->Child_num;i++)
    		//fprintf(stderr,"%d, %lf, %lf, %d\n",root->Child[i]->move,root->Child[i]->W,root->Child[i]->N,root->Child[i]->Child_num);
    		
			//fprintf(stderr,"ew>>> %d\n",delta);
			//fprintf(stderr, "after simulate\n" );
    		//back up
    		now_time = clock();
    		//fprintf(stderr,"select: %lf / %lf\n",selectedNode->W,selectedNode->N);
    		//fprintf(stderr, "after backup\n" );
    	}
    	BestChild(root);
    	for(i=0;i<root->Child_num;i++)
    		fprintf(stderr,"%d, %lf, %lf, %d, score = %lf, type = %d, length = %d\n",root->Child[i]->move,root->Child[i]->W,root->Child[i]->N,root->Child[i]->Child_num, root->Child[i]->UCB, root->Child[i]->type, game_length);
    	fprintf(stderr,"><><><>>>>%d, score = %lf\n",BestChild(root)->move, BestChild(root)->UCB);
	return BestChild(root)->move;
    }
}


/* 
 * Following are commands for Go Text Protocol (GTP)
 *
 * */
const char *KnownCommands[]={
    "protocol_version",
    "name",
    "version",
    "known_command",
    "list_commands",
    "quit",
    "boardsize",
    "clear_board",
    "komi",
    "play",
    "genmove",
    "undo",
    "quit",
    "showboard",
    "final_score"
};

void gtp_final_score(int Board[BOUNDARYSIZE][BOUNDARYSIZE]) {
    double result;
    result = final_score(Board);
    result -= _komi;
    cout << "= ";
    if (result > 0.0) { // Black win
	cout << "B+" << result << endl << endl<< endl;;
    }
    if (result < 0.0) { // White win
	cout << "W+" << -result << endl << endl<< endl;;
    }
    else { // draw
	cout << "0" << endl << endl<< endl;;
    }
}
void gtp_undo(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int game_length, int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE]) {
    if (game_length!=0) {
	for (int i = 1; i <= BOARDSIZE; ++i) {
	    for (int j = 1; j <= BOARDSIZE; ++j) {
		Board[i][j] = GameRecord[game_length][i][j];
	    }
	}
    }
    cout << "= " << endl << endl;
}
void gtp_showboard(int Board[BOUNDARYSIZE][BOUNDARYSIZE]) {
    for (int i = 1; i <=BOARDSIZE; ++i) {
	cout << "#";
	cout <<10-i;
	for (int j = 1; j <=BOARDSIZE; ++j) {
	    switch(Board[i][j]) {
		case EMPTY: cout << " .";break;
		case BLACK: cout << " X";break;
		case WHITE: cout << " O";break;
	    }
	}
	cout << endl;
    }
    cout << "#  ";
    for (int i = 1; i <=BOARDSIZE; ++i) 
	cout << LabelX[i] <<" ";
    cout << endl;
    cout << endl;

}
void gtp_protocol_version() {
    cout <<"= 2"<<endl<< endl;
}
void gtp_name() {
    cout <<"= Gdog" << endl<< endl;
}
void gtp_version() {
    cout << "= 1.03" << endl << endl;
}
void gtp_list_commands(){
    cout <<"= ";
    for (int i = 0 ; i < NUMGTPCOMMANDS; ++i) {
	cout <<KnownCommands[i] << endl;
    }
    cout << endl;
}
void gtp_known_command(const char Input[]) {
    for (int i = 0 ; i < NUMGTPCOMMANDS; ++i) {
	if (strcmp(Input, KnownCommands[i])==0) {
	    cout << "= true" << endl<< endl;
	    return;
	}
    }
    cout << "= false" << endl<< endl;
}
void gtp_boardsize(int size) {
    if (size!=9) {
	cout << "? unacceptable size" << endl<< endl;
    }
    else {
	_board_size = size;
	cout << "= "<<endl<<endl;
    }
}
void gtp_clear_board(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int NumCapture[]) {
    reset(Board);
    NumCapture[BLACK] = NumCapture[WHITE] = 0;
    cout << "= "<<endl<<endl;
}
void gtp_komi(double komi) {
    _komi = komi;
    cout << "= "<<endl<<endl;
}
void gtp_play(char Color[], char Move[], int Board[BOUNDARYSIZE][BOUNDARYSIZE], int game_length, int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE]) {
    int turn, move_i, move_j;
    if (Color[0] =='b' || Color[0] == 'B')
	turn = BLACK;
    else
	turn = WHITE;
    if (strcmp(Move, "PASS") == 0 || strcmp(Move, "pass")==0) {
	record(Board, GameRecord, game_length+1);
    }
    else {
	// [ABCDEFGHJ][1-9], there is no I in the index.
	Move[0] = toupper(Move[0]);
	move_j = Move[0]-'A'+1;
	if (move_j == 10) move_j = 9;
	move_i = 10-(Move[1]-'0');
	update_board(Board, move_i, move_j, turn);
	record(Board, GameRecord, game_length+1);
    }
    cout << "= "<<endl<<endl;
}
void gtp_genmove(int Board[BOUNDARYSIZE][BOUNDARYSIZE], char Color[], int time_limit, int game_length, int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE]){
    int turn = (Color[0]=='b'||Color[0]=='B')?BLACK:WHITE;
    int move = genmove(Board, turn, time_limit, game_length, GameRecord);
    int move_i, move_j;
    record(Board, GameRecord, game_length+1);
    if (move==0) {
	cout << "= PASS" << endl<< endl<< endl;
    }
    else {
	move_i = (move%100)/10;
	move_j = (move%10);
//	cerr << "#turn("<<game_length<<"): (move, move_i,move_j)" << turn << ": " << move<< " " << move_i << " " << move_j << endl;
	cout << "= " << LabelX[move_j]<<10-move_i<<endl<< endl;
    }
}
/*
 * This main function is used of the gtp protocol
 * */
void gtp_main() {
	srand(time(NULL));
    char Input[COMMANDLENGTH]="";
    char Command[COMMANDLENGTH]="";
    char Parameter[COMMANDLENGTH]="";
    char Move[4]="";
    char Color[6]="";
    int ivalue;
    double dvalue;
    int Board[BOUNDARYSIZE][BOUNDARYSIZE]={{0}};
    int NumCapture[3]={0};// 1:Black, 2: White
    int time_limit = DEFAULTTIME;
    int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE]={{{0}}};
    int game_length = 0;
    while (gets(Input) != 0) {
	sscanf(Input, "%s", Command);
	if (Command[0]== '#')
	    continue;

	if (strcmp(Command, "protocol_version")==0) {
	    gtp_protocol_version();
	}
	else if (strcmp(Command, "name")==0) {
	    gtp_name();
	}
	else if (strcmp(Command, "version")==0) {
	    gtp_version();
	}
	else if (strcmp(Command, "list_commands")==0) {
	    gtp_list_commands();
	}
	else if (strcmp(Command, "known_command")==0) {
	    sscanf(Input, "known_command %s", Parameter);
	    gtp_known_command(Parameter);
	}
	else if (strcmp(Command, "boardsize")==0) {
	    sscanf(Input, "boardsize %d", &ivalue);
	    gtp_boardsize(ivalue);
	}
	else if (strcmp(Command, "clear_board")==0) {
	    gtp_clear_board(Board, NumCapture);
	    game_length = 0;
	}
	else if (strcmp(Command, "komi")==0) {
	    sscanf(Input, "komi %lf", &dvalue);
	    gtp_komi(dvalue);
	}
	else if (strcmp(Command, "play")==0) {
	    sscanf(Input, "play %s %s", Color, Move);
	    gtp_play(Color, Move, Board, game_length, GameRecord);
	    game_length++;
	}
	else if (strcmp(Command, "genmove")==0) {
	    sscanf(Input, "genmove %s", Color);
	    gtp_genmove(Board, Color, time_limit, game_length, GameRecord);
	    game_length++;
	}
	else if (strcmp(Command, "quit")==0) {
	    break;
	}
	else if (strcmp(Command, "showboard")==0) {
	    gtp_showboard(Board);
	}
	else if (strcmp(Command, "undo")==0) {
	    game_length--;
	    gtp_undo(Board, game_length, GameRecord);
	}
	else if (strcmp(Command, "final_score")==0) {
	    gtp_final_score(Board);
	}
    }
}
/*
 * This main function is used for the local test
 * */
void local_main() {
    int Board[BOUNDARYSIZE][BOUNDARYSIZE];
    int NextBoard[BOUNDARYSIZE][BOUNDARYSIZE];
    int time_limit = DEFAULTTIME;
    char bw[2];
    int x, y;
    char command[COMMANDLENGTH];
    bool GameState = 0; // 0 off, 1 on.
    int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE];
    int game_length = 0;
    reset(Board);
    cout << ">";
    while (cin >> command) {
	//reset
	if (strcmp(command,"reset")==0) {
	    reset(Board);
	    game_length = 0;
	    GameState = 0;
	    cout << ">ok" << endl;
	}
	//time
	else if (strcmp(command,"time")==0) {
	    cin >> time_limit;
	    cout << ">ok" << endl;
	}
	//put
	else if (strcmp(command,"put")==0) {
	    cin >> bw >> x >> y;
	    if ((bw[0]=='w'||bw[0]=='b') && 1 <= x && x <= BOARDSIZE && 1 <= y && y <= BOARDSIZE) {
		if (GameState == 1) {
		    // check for repeat board
		    for (int i = 0 ; i < BOUNDARYSIZE; ++i) {
			for (int j = 0 ; j < BOUNDARYSIZE; ++j) {
			    NextBoard[i][j] = Board[i][j];
			}
		    }
		    update_board_check(NextBoard, BOARDSIZE-y+1, x, bw[0]=='b'?1:2);
		    // check for repat board
		    int repeat_flag;
		    int check_flag = 1;
		    for (int t = 0 ; t < game_length; ++t) {
			repeat_flag = 1;
			for (int i = 0 ; i < BOUNDARYSIZE; ++i) {
			    for (int j = 0 ; j < BOUNDARYSIZE; ++j) {
				if (NextBoard[i][j] != GameRecord[t][i][j]) {
				    repeat_flag = 0;
				}
			    }
			}
			if (repeat_flag == 1) {
			    check_flag = 0;
			    break;
			}
		    }
		    // record new move
		    if (check_flag == 1) {
			for (int i = 0 ; i < BOUNDARYSIZE; ++i) {
			    for (int j = 0 ; j < BOUNDARYSIZE; ++j) {
				GameRecord[game_length][i][j] = NextBoard[i][j];
				Board[i][j] = NextBoard[i][j];
			    }
			}
			game_length++;
		    }
		}
		else {
		    update_board_check(Board, BOARDSIZE-y+1, x, bw[0]=='b'?1:2);
		}
		cout << ">ok" << endl;
	    }
	    else {
		cerr << bw << " " << x << " " << y << endl;
		cout << "put b/w x y:put a black/white stone at (x,y), 1 <= x,y <="<< BOARDSIZE << endl;
	    }
	    display(Board, time_limit);
	}
	//display
	else if (strcmp(command,"display")==0) {
	    display(Board, time_limit);
	}
	//start
	else if (strcmp(command,"start")==0) {
		GameState = 1;
		// initial game record
		game_length = 0;
		for (int i = 0 ; i < BOUNDARYSIZE; ++i) {
		    for (int j = 0 ; j < BOUNDARYSIZE; ++j) {
			GameRecord[game_length][i][j] = Board[i][j];
		    }
		}
		game_length++;
		cout << ">ok" << endl;
	}
	// genmove 
	else if (strcmp(command,"genmove")==0) {
	    cin >> bw;
	    if (GameState == 1) {
		int turn = bw[0]=='b' ? BLACK : WHITE ;
		int move;
		move = genmove(Board, turn, time_limit, game_length, GameRecord);
		if (move == 0)
		    cout << "0 0" << endl;
		else
		    cout << (move%10) << " " << 10 - (move/10) << endl;
		record(Board, GameRecord, game_length);
		game_length++;
	    }
	    else {
		cout << "The game is not start."  << endl;
	    }
	}
	else if (strcmp(command, "undo")==0) {
	    if (game_length!=0) {
		game_length--;
		for (int i = 1; i <= BOARDSIZE; ++i) {
		    for (int j = 1; j <= BOARDSIZE; ++j) {
			Board[i][j] = GameRecord[game_length][i][j];
		    }
		}
	    }
	}
	//quit
	else if (strcmp(command, "quit")==0) {
	    break;
	}
	else {
	    cout << "reset: reset the board" << endl;
	    cout << "time N: set N sec per hand" << endl;
	    cout << "put bw X Y: put [bw] stone on (X, Y)" << endl;
	    cout << "display: show the current board" << endl;
	    cout << "start: start game from current board" << endl;
	    cout << "genmove bw: generate one move of bw" << endl;
	    cout << "undo: undo one move" << endl;
	    cout << "quit: end the program" << endl;
	    cout << "help: show this message" << endl;
	}
	cout << ">";
    }
}
int main(int argc, char* argv[]) {
    int type = GTPVERSION;// 1: local version, 2: gtp version
    if (argc > 1) {
	if (strcmp(argv[1],"-local")==0)
	    type = LOCALVERSION;
	if (strcmp(argv[1],"-gtp")==0)
	    type = GTPVERSION;
    }
    switch (type) {
	case LOCALVERSION:
	    local_main();
	    break;
	case GTPVERSION:
	    gtp_main();
	    break;
    }
    return 0;
}
