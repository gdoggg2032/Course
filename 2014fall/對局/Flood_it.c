#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"

#define ANSI_COLOR_RESET   "\x1b[0m"

int win(int length);
void paint(char move, int length);
void enque(char move, int length);
int deque();


struct 
{
	int x;
	int y;
	char color;
}typedef queue;

char board[15][15];
queue Q[200];
int q_start = 0,q_end = 1;


int main()
{
	int i,j;
	int length;
	int num_move = 0;
	printf("Number of rows and column: ");
	scanf("%d", &length);

	srand(time(NULL));
	/*
	strcpy(board[0], "43234335154254");
	strcpy(board[1], "20421451243434");
	strcpy(board[2], "23334341411342");
	strcpy(board[3], "30221123201014");
	strcpy(board[4], "10215550514302");
	strcpy(board[5], "44542415325532");
	strcpy(board[6], "30404404035513");
	strcpy(board[7], "33543344202132");
	strcpy(board[8], "02053400045540");
	strcpy(board[9], "51104413042402");
	strcpy(board[10], "24050452023253");
	strcpy(board[11], "12542534240451");
	strcpy(board[12], "44020043410532");
	strcpy(board[13], "21130223352532");
	*/

	

	for(i = 0; i < length; i++)
	{
		for(j = 0; j < length; j++)
		{
			board[i][j] = rand() % 6 + 48;
			if(board[i][j] == '0')
				printf(ANSI_COLOR_CYAN "%c "ANSI_COLOR_RESET, board[i][j]);
			if(board[i][j] == '1')
				printf(ANSI_COLOR_RED "%c "ANSI_COLOR_RESET, board[i][j]);
			if(board[i][j] == '2')
				printf(ANSI_COLOR_GREEN "%c "ANSI_COLOR_RESET, board[i][j]);
			if(board[i][j] == '3')
				printf(ANSI_COLOR_YELLOW "%c "ANSI_COLOR_RESET, board[i][j]);
			if(board[i][j] == '4')
				printf(ANSI_COLOR_BLUE "%c "ANSI_COLOR_RESET, board[i][j]);
			if(board[i][j] == '5')
				printf(ANSI_COLOR_MAGENTA "%c "ANSI_COLOR_RESET, board[i][j]);
		}
		board[i][length] = '\0';
		printf("\n");
	}

	char move[2];

	while(!win(length))
	{
		num_move ++;
		q_start = 0;
		q_end = 1;
		scanf("%s", move);
		paint(move[0], length);		
	}
	
	printf("You use %d steps to win!!\n", num_move);

	return 0;
}

int win(int length)
{
	int c = board[0][0];
	int i,j;
	for(i = 0; i < length; i++)
		for(j = 0; j < length; j++)
			if(board[i][j] != c)
				return 0;
	return 1;
}

void paint(char move, int length)
{
	int ret, i, j;
	

	Q[q_start].color = board[0][0];
	//printf("(%d, %d), color: %c", Q[q_start].x, Q[q_start].y, Q[q_start].color);
	board[0][0] = move;
	
	while(1)
	{
		enque(move, length);
		ret = deque();
		if(ret == 1)
			break;
	}

	for(i = 0; i < length; i++)
	{
		for(j = 0; j < length; j++)
		{
			if(board[i][j] == '0')
				printf(ANSI_COLOR_CYAN "%c "ANSI_COLOR_RESET, board[i][j]);
			if(board[i][j] == '1')
				printf(ANSI_COLOR_RED "%c "ANSI_COLOR_RESET, board[i][j]);
			if(board[i][j] == '2')
				printf(ANSI_COLOR_GREEN "%c "ANSI_COLOR_RESET, board[i][j]);
			if(board[i][j] == '3')
				printf(ANSI_COLOR_YELLOW "%c "ANSI_COLOR_RESET, board[i][j]);
			if(board[i][j] == '4')
				printf(ANSI_COLOR_BLUE "%c "ANSI_COLOR_RESET, board[i][j]);
			if(board[i][j] == '5')
				printf(ANSI_COLOR_MAGENTA "%c "ANSI_COLOR_RESET, board[i][j]);
		}
		printf("\n");
	}
}

void enque(char move, int length)
{
	if(Q[q_start].y - 1 >= 0 && board[Q[q_start].x][Q[q_start].y-1] == Q[q_start].color)    //up
	{
		//printf("Coordinate (%d, %d) have been added\n", Q[q_start].x, Q[q_start].y-1);
		board[Q[q_start].x][Q[q_start].y-1] = move;
		Q[q_end].x = Q[q_start].x;
		Q[q_end].y = Q[q_start].y - 1;
		Q[q_end].color = Q[q_start].color;
		q_end ++;
		
	}
	if(Q[q_start].y + 1 < length && board[Q[q_start].x][Q[q_start].y+1] == Q[q_start].color)    //down
	{
		//printf("Coordinate (%d, %d) have been added\n", Q[q_start].x, Q[q_start].y+1);
		board[Q[q_start].x][Q[q_start].y+1] = move;
		Q[q_end].x = Q[q_start].x;
		Q[q_end].y = Q[q_start].y + 1;
		Q[q_end].color = Q[q_start].color;
		q_end ++;
		
	}
	if(Q[q_start].x - 1 >= 0 && board[Q[q_start].x-1][Q[q_start].y] == Q[q_start].color)    //left
	{
		//printf("Coordinate (%d, %d) have been added\n", Q[q_start].x-1, Q[q_start].y);
		board[Q[q_start].x-1][Q[q_start].y] = move;
		Q[q_end].x = Q[q_start].x - 1;
		Q[q_end].y = Q[q_start].y;
		Q[q_end].color = Q[q_start].color;
		q_end ++;
	}
	if(Q[q_start].x + 1 < length && board[Q[q_start].x+1][Q[q_start].y] == Q[q_start].color)    //right
	{
		//printf("Coordinate (%d, %d) have been added\n", Q[q_start].x+1, Q[q_start].y);
		board[Q[q_start].x+1][Q[q_start].y] = move;
		Q[q_end].x = Q[q_start].x + 1;
		Q[q_end].y = Q[q_start].y;
		Q[q_end].color = Q[q_start].color;
		q_end ++;
	}
}

int deque()
{
	q_start ++;
	if(q_start == q_end)
		return 1;
	else 
		return 0;
}






