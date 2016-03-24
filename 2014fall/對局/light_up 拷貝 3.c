#include <stdio.h>
#include <stdlib.h>
#include <time.h>



#define SIZE 25
#define loo  0.3
#define bigQ 50
#define MAX_ITER 1000
#define NUM_ANT 20
typedef struct
{
	int x;
	int y;

}Point;
typedef struct
{
	int x;
	int y;
	double ph;
	double prob;
	int heu;
	int value;
	int light;
	int brick;
	int cover;
	int pre_process;
}Grid;
typedef struct
{

	int num;
	Point points[SIZE*SIZE];

}Answer;
typedef struct
{
	Point start;
	Point end;
	int num;
	int light;
	double prob;


}Row_group;


Row_group Rows[SIZE*SIZE];
int Row_num;
Grid **map;
void print_map(Grid** testmap)
{
	int i,j;
	for(i=1;i<=SIZE;i++)
	{
		for(j=1;j<=SIZE;j++)
		{

			if(testmap[i][j].brick == 0)//只是空格
			{
				if(testmap[i][j].light==1)
					printf("|%d| ",testmap[i][j].value);
				else
					printf(" %d  ",testmap[i][j].value);
			}
			else
			{
				if(testmap[i][j].value == 5)
					printf("(*) ");
				else
					printf("(%d) ",testmap[i][j].value);

			}
		}
		printf("\n");
	}
	printf("-----------------------\n");
}
Grid** map_copy(Grid** A, Grid** B)
{
	int i,j;
	if(A==NULL)
	{
		A=(Grid**)malloc((SIZE+2)*sizeof(Grid*));
		for(i=0;i<SIZE+2;i++)
		{
			A[i]=(Grid*)malloc((SIZE+2)*sizeof(Grid));
		}
	}
		
	
	
	for(i=0;i<SIZE+2;i++)
	{
		for(j=0;j<SIZE+2;j++)
		{
			A[i][j].x=B[i][j].x;
			A[i][j].y=B[i][j].y;
			A[i][j].ph=B[i][j].ph;
			A[i][j].prob=B[i][j].prob;
			A[i][j].heu=B[i][j].heu;
			A[i][j].value=B[i][j].value;
			A[i][j].light=B[i][j].light;
			A[i][j].brick=B[i][j].brick;
			A[i][j].cover=B[i][j].cover;
			A[i][j].pre_process=B[i][j].pre_process;




		}
	}

	return A;
}
Grid** brick_forbidden(Grid** testmap)
{
	int count,i,j;
	for(i=1;i<=SIZE;i++)
		{
			for(j=1;j<=SIZE;j++)
			{
	if(testmap[i][j].brick==1&&testmap[i][j].value!=0&&testmap[i][j].value!=5)
				{
					//printf("in brick_forbidden: (%d,%d)\n",i,j);
					count = 0;

					if(testmap[i+1][j].brick==0&&testmap[i+1][j].light==1)count++;

					if(testmap[i-1][j].brick==0&&testmap[i-1][j].light==1)count++;

					if(testmap[i][j+1].brick==0&&testmap[i][j+1].light==1)count++;

					if(testmap[i][j-1].brick==0&&testmap[i][j-1].light==1)count++;
					if(count >= testmap[i][j].value)
					{
						
						//printf("step 5\n");
						if(testmap[i+1][j].brick==0)testmap[i+1][j].value=0;

						if(testmap[i-1][j].brick==0)testmap[i-1][j].value=0;

						if(testmap[i][j+1].brick==0)testmap[i][j+1].value=0;

						if(testmap[i][j-1].brick==0)testmap[i][j-1].value=0;
					}
				}
			}
		}
				return testmap;
}


Grid** brick_remain(Grid** testmap)
{
	int k,count,x,y,i,j;
	for(i=1;i<=SIZE;i++)
		{
			for(j=1;j<=SIZE;j++)
			{

			if(testmap[i][j].brick==1&&testmap[i][j].value!=0&&testmap[i][j].value!=5)
				{
					

					count = 0;

					if(testmap[i+1][j].brick==0&&testmap[i+1][j].value!=0&&testmap[i+1][j].cover==0)count++;

					if(testmap[i-1][j].brick==0&&testmap[i-1][j].value!=0&&testmap[i-1][j].cover==0)count++;

					if(testmap[i][j+1].brick==0&&testmap[i][j+1].value!=0&&testmap[i][j+1].cover==0)count++;

					if(testmap[i][j-1].brick==0&&testmap[i][j-1].value!=0&&testmap[i][j-1].cover==0)count++;

					if(count==testmap[i][j].value)
					{
						

						if(testmap[i+1][j].brick==0&&testmap[i+1][j].value!=0&&testmap[i+1][j].cover==0)
						{
							x=i+1;
							y=j;
							testmap[x][y].value=0;
							testmap[x][y].light=1;
							testmap[x][y].cover=1;
							k=0;
							while(testmap[x+k][y].brick==0)
							{
								testmap[x+k][y].value=0;
								testmap[x+k][y].cover = 1;
								k++;
							}
							k=0;
							while(testmap[x-k][y].brick==0)
							{
								testmap[x-k][y].value=0;
								testmap[x-k][y].cover = 1;
								k++;
							}
							k=0;
							while(testmap[x][y+k].brick==0)
							{
								testmap[x][y+k].value=0;
								testmap[x][y+k].cover = 1;
								k++;
							}
							k=0;
							while(testmap[x][y-k].brick==0)
							{
								testmap[x][y-k].value=0;
								testmap[x][y-k].cover = 1;
								k++;
							}
						}
						testmap = brick_forbidden(testmap);
						if(testmap[i-1][j].brick==0&&testmap[i-1][j].value!=0&&testmap[i-1][j].cover==0)
						{
							x=i-1;
							y=j;
							testmap[x][y].value=0;
							testmap[x][y].light=1;
							testmap[x][y].cover=1;
							k=0;
							while(testmap[x+k][y].brick==0)
							{
								testmap[x+k][y].value=0;
								testmap[x+k][y].cover = 1;
								k++;
							}
							k=0;
							while(testmap[x-k][y].brick==0)
							{
								testmap[x-k][y].value=0;
								testmap[x-k][y].cover = 1;
								k++;
							}
							k=0;
							while(testmap[x][y+k].brick==0)
							{
								testmap[x][y+k].value=0;
								testmap[x][y+k].cover = 1;
								k++;
							}
							k=0;
							while(testmap[x][y-k].brick==0)
							{
								testmap[x][y-k].value=0;
								testmap[x][y-k].cover = 1;
								k++;
							}
						}
						testmap = brick_forbidden(testmap);
						if(testmap[i][j+1].brick==0&&testmap[i][j+1].value!=0&&testmap[i][j+1].cover==0)
						{
							x=i;
							y=j+1;
							testmap[x][y].value=0;
							testmap[x][y].light=1;
							testmap[x][y].cover=1;
							k=0;
							while(testmap[x+k][y].brick==0)
							{
								testmap[x+k][y].value=0;
								testmap[x+k][y].cover = 1;
								k++;
							}
							k=0;
							while(testmap[x-k][y].brick==0)
							{
								testmap[x-k][y].value=0;
								testmap[x-k][y].cover = 1;
								k++;
							}
							k=0;
							while(testmap[x][y+k].brick==0)
							{
								testmap[x][y+k].value=0;
								testmap[x][y+k].cover = 1;
								k++;
							}
							k=0;
							while(testmap[x][y-k].brick==0)
							{
								testmap[x][y-k].value=0;
								testmap[x][y-k].cover = 1;
								k++;
							}
						}
						testmap = brick_forbidden(testmap);
						if(testmap[i][j-1].brick==0&&testmap[i][j-1].value!=0&&testmap[i][j-1].cover==0)
						{
							x=i;
							y=j-1;
							testmap[x][y].value=0;
							testmap[x][y].light=1;
							testmap[x][y].cover=1;
							k=0;
							while(testmap[x+k][y].brick==0)
							{
								testmap[x+k][y].value=0;
								testmap[x+k][y].cover = 1;
								k++;
							}
							k=0;
							while(testmap[x-k][y].brick==0)
							{
								testmap[x-k][y].value=0;
								testmap[x-k][y].cover = 1;
								k++;
							}
							k=0;
							while(testmap[x][y+k].brick==0)
							{
								testmap[x][y+k].value=0;
								testmap[x][y+k].cover = 1;
								k++;
							}
							k=0;
							while(testmap[x][y-k].brick==0)
							{
								testmap[x][y-k].value=0;
								testmap[x][y-k].cover = 1;
								k++;
							}
						}
						testmap = brick_forbidden(testmap);
					}

					
						

				}
			}
		}
			return testmap;
}

Grid** row_checker(Grid** testmap)
{
	int i,j,k,check;
	for(i=1;i<=SIZE;i++)
		{
			for(j=1;j<=SIZE;j++)
			{
				if(testmap[i][j].brick==0)
				{
					if(testmap[i][j].value==1&&testmap[i][j].cover==0)
					{
						check=0;
						
							k=0;
							while(testmap[i+k][j].brick==0)
							{
								if(testmap[i+k][j].light==1||testmap[i+k][j].value!=0)check=1;
								k++;
							}
							k=0;
							while(testmap[i-k][j].brick==0)
							{
								if(testmap[i-k][j].light==1||testmap[i-k][j].value!=0)check=1;
								k++;
							}
							k=0;
							while(testmap[i][j+k].brick==0)
							{
								if(testmap[i][j+k].light==1||testmap[i][j+k].value!=0)check=1;
								k++;
							}
							k=0;
							while(testmap[i][j-k].brick==0)
							{
								if(testmap[i][j-k].light==1||testmap[i][j-k].value!=0)check=1;
								k++;
							}
							if(check==0)
							{
								
								//printf("step 2\n");
								testmap[i][j].value=0;
								testmap[i][j].light = 1;
								testmap[i][j].cover = 1;
								k=0;
								while(testmap[i+k][j].brick==0)
								{
									testmap[i+k][j].value=0;
									testmap[i+k][j].cover = 1;
									k++;
								}
								k=0;
								while(testmap[i-k][j].brick==0)
								{
									testmap[i-k][j].value=0;
									testmap[i-k][j].cover = 1;
									k++;
								}
								k=0;
								while(testmap[i][j+k].brick==0)
								{
									testmap[i][j+k].value=0;
									testmap[i][j+k].cover = 1;
									k++;
								}
								k=0;
								while(testmap[i][j-k].brick==0)
								{
									testmap[i][j-k].value=0;
									testmap[i][j-k].cover = 1;
									k++;
								}


							}
						


						
					}
				}
			}
		}
		return testmap;

}

Grid** pre_processing(Grid ** testmap)//O(n*SIZE+n*n)
{

	int i,j,k,check,flag,count,count2,x,y;
	Answer A;
	A.num=0;
	count2=Row_num;
	while(count2>0)
	{
		count2--;
		flag=0;
		for(i=1;i<=SIZE;i++)
		{
			for(j=1;j<=SIZE;j++)
			{//printf("dsa, testmap[i][j].brick= %d\n",testmap[i][j].brick);
				if(testmap[i][j].brick==1)
				{//printf("tere\n");
					if(testmap[i][j].value==0)
					{
						
						//printf("step 1\n");
						if(testmap[i+1][j].brick==0)
							testmap[i+1][j].value=0;
						if(testmap[i-1][j].brick==0)
							testmap[i-1][j].value=0;
						if(testmap[i][j+1].brick==0)
							testmap[i][j+1].value=0;
						if(testmap[i][j-1].brick==0)
							testmap[i][j-1].value=0;
					}
				}
			}
		}
	

		for(i=1;i<=SIZE;i++)
		{
			for(j=1;j<=SIZE;j++)
			{
				if(testmap[i][j].brick == 0&&testmap[i][j].value==0&&testmap[i][j].cover==0)
				{


					check=0;
					k=0;
					while(testmap[i+k][j].brick==0)
					{
						if(testmap[i+k][j].value==1&&testmap[i+k][j].brick==0)check=1;
						k++;
					}
					k=0;
					while(testmap[i-k][j].brick==0)
					{
						if(testmap[i-k][j].value==1&&testmap[i-k][j].brick==0)check=1;
						k++;
					}
					if(check==0)
					{
						count=0;
						k=0;
						while(testmap[i][j+k].brick==0)
						{
							if(testmap[i][j+k].value==1&&testmap[i][j+k].brick==0)
							{
								x=i;
								y=j+k;
								count++;
							}
							k++;
						}
						k=0;
						while(testmap[i][j-k].brick==0)
						{
							if(testmap[i][j-k].value==1&&testmap[i][j-k].brick==0)
							{
								x=i;
								y=j-k;
								count++;
							}
							k++;
						}
						if(count==1)
						{
							flag=1;
							//printf("step 3\n");
							testmap[x][y].value=0;
							testmap[x][y].light=1;
							testmap[x][y].cover=1;
							k=0;
							while(testmap[x+k][y].brick==0)
							{
								testmap[x+k][y].value=0;
								testmap[x+k][y].cover = 1;
								k++;
							}
							k=0;
							while(testmap[x-k][y].brick==0)
							{
								testmap[x-k][y].value=0;
								testmap[x-k][y].cover = 1;
								k++;
							}
							k=0;
							while(testmap[x][y+k].brick==0)
							{
								testmap[x][y+k].value=0;
								testmap[x][y+k].cover = 1;
								k++;
							}
							k=0;
							while(testmap[x][y-k].brick==0)
							{
								testmap[x][y-k].value=0;
								testmap[x][y-k].cover = 1;
								k++;
							}
						}

					}




					check=0;
					k=0;
					while(testmap[i][j+k].brick==0)
					{
						if(testmap[i][j+k].value==1&&testmap[i][j+k].brick==0)check=1;
						k++;
					}
					k=0;
					while(testmap[i][j-k].brick==0)
					{
						if(testmap[i][j-k].value==1&&testmap[i][j-k].brick==0)check=1;
						k++;
					}
					if(check==0)
					{
						count=0;
						k=0;
						while(testmap[i+k][j].brick==0)
						{
							if(testmap[i+k][j].value==1&&testmap[i+k][j].brick==0)
							{
								x=i+k;
								y=j;
								count++;
							}
							k++;
						}
						k=0;
						while(testmap[i-k][j].brick==0)
						{
							if(testmap[i-k][j].value==1&&testmap[i-k][j].brick==0)
							{
								x=i-k;
								y=j;
								count++;
							}
							k++;
						}
						if(count==1)
						{
							flag=1;
							//printf("step 3\n");
							testmap[x][y].value=0;
							testmap[x][y].light=1;
							testmap[x][y].cover=1;
							k=0;
							while(testmap[x+k][y].brick==0)
							{
								testmap[x+k][y].value=0;
								testmap[x+k][y].cover = 1;
								k++;
							}
							k=0;
							while(testmap[x-k][y].brick==0)
							{
								testmap[x-k][y].value=0;
								testmap[x-k][y].cover = 1;
								k++;
							}
							k=0;
							while(testmap[x][y+k].brick==0)
							{
								testmap[x][y+k].value=0;
								testmap[x][y+k].cover = 1;
								k++;
							}
							k=0;
							while(testmap[x][y-k].brick==0)
							{
								testmap[x][y-k].value=0;
								testmap[x][y-k].cover = 1;
								k++;
							}
						}

					}


				}
			}
		}
		testmap=brick_forbidden(testmap);//O(n)
		/*for(i=1;i<=SIZE;i++)
		{
			for(j=1;j<=SIZE;j++)
			{
				
				if(testmap[i][j].brick==1&&testmap[i][j].value!=0&&testmap[i][j].value!=5)
				{
					count = 0;

					if(testmap[i+1][j].brick==0&&testmap[i+1][j].light==1)count++;

					if(testmap[i-1][j].brick==0&&testmap[i-1][j].light==1)count++;

					if(testmap[i][j+1].brick==0&&testmap[i][j+1].light==1)count++;

					if(testmap[i][j-1].brick==0&&testmap[i][j-1].light==1)count++;
					if(count >= testmap[i][j].value)
					{
						
						//printf("step 5\n");
						if(testmap[i+1][j].brick==0&&testmap[i+1][j].value!=0&&testmap[i+1][j].cover==0)testmap[i+1][j].value=0;

						if(testmap[i-1][j].brick==0&&testmap[i-1][j].value!=0&&testmap[i-1][j].cover==0)testmap[i-1][j].value=0;

						if(testmap[i][j+1].brick==0&&testmap[i][j+1].value!=0&&testmap[i][j+1].cover==0)testmap[i][j+1].value=0;

						if(testmap[i][j-1].brick==0&&testmap[i][j-1].value!=0&&testmap[i][j-1].cover==0)testmap[i][j-1].value=0;
					}
				}
			}
		}*/
		testmap=brick_remain(testmap);//O(n*SIZE)
	/*
		for(i=1;i<=SIZE;i++)
		{
			for(j=1;j<=SIZE;j++)
			{
				
				
				if(testmap[i][j].brick==1&&testmap[i][j].value!=0&&testmap[i][j].value!=5)
				{
					count = 0;
					





					if(testmap[i+1][j].brick==0&&testmap[i+1][j].value!=0&&testmap[i+1][j].cover==0)count++;

					if(testmap[i-1][j].brick==0&&testmap[i-1][j].value!=0&&testmap[i-1][j].cover==0)count++;

					if(testmap[i][j+1].brick==0&&testmap[i][j+1].value!=0&&testmap[i][j+1].cover==0)count++;

					if(testmap[i][j-1].brick==0&&testmap[i][j-1].value!=0&&testmap[i][j-1].cover==0)count++;

					if(count==testmap[i][j].value)
					{
						flag=1;
						//printf("step 4\n");
						if(testmap[i+1][j].brick==0&&testmap[i+1][j].value!=0&&testmap[i+1][j].cover==0)
						{
							x=i+1;
							y=j;
							testmap[x][y].value=0;
							testmap[x][y].light=1;
							testmap[x][y].cover=1;
							k=0;
							while(testmap[x+k][y].brick==0)
							{
								testmap[x+k][y].value=0;
								testmap[x+k][y].cover = 1;
								k++;
							}
							k=0;
							while(testmap[x-k][y].brick==0)
							{
								testmap[x-k][y].value=0;
								testmap[x-k][y].cover = 1;
								k++;
							}
							k=0;
							while(testmap[x][y+k].brick==0)
							{
								testmap[x][y+k].value=0;
								testmap[x][y+k].cover = 1;
								k++;
							}
							k=0;
							while(testmap[x][y-k].brick==0)
							{
								testmap[x][y-k].value=0;
								testmap[x][y-k].cover = 1;
								k++;
							}
						}
						if(testmap[i-1][j].brick==0&&testmap[i-1][j].value!=0&&testmap[i-1][j].cover==0)
						{
							x=i-1;
							y=j;
							testmap[x][y].value=0;
							testmap[x][y].light=1;
							testmap[x][y].cover=1;
							k=0;
							while(testmap[x+k][y].brick==0)
							{
								testmap[x+k][y].value=0;
								testmap[x+k][y].cover = 1;
								k++;
							}
							k=0;
							while(testmap[x-k][y].brick==0)
							{
								testmap[x-k][y].value=0;
								testmap[x-k][y].cover = 1;
								k++;
							}
							k=0;
							while(testmap[x][y+k].brick==0)
							{
								testmap[x][y+k].value=0;
								testmap[x][y+k].cover = 1;
								k++;
							}
							k=0;
							while(testmap[x][y-k].brick==0)
							{
								testmap[x][y-k].value=0;
								testmap[x][y-k].cover = 1;
								k++;
							}
						}
						if(testmap[i][j+1].brick==0&&testmap[i][j+1].value!=0&&testmap[i][j+1].cover==0)
						{
							x=i;
							y=j+1;
							testmap[x][y].value=0;
							testmap[x][y].light=1;
							testmap[x][y].cover=1;
							k=0;
							while(testmap[x+k][y].brick==0)
							{
								testmap[x+k][y].value=0;
								testmap[x+k][y].cover = 1;
								k++;
							}
							k=0;
							while(testmap[x-k][y].brick==0)
							{
								testmap[x-k][y].value=0;
								testmap[x-k][y].cover = 1;
								k++;
							}
							k=0;
							while(testmap[x][y+k].brick==0)
							{
								testmap[x][y+k].value=0;
								testmap[x][y+k].cover = 1;
								k++;
							}
							k=0;
							while(testmap[x][y-k].brick==0)
							{
								testmap[x][y-k].value=0;
								testmap[x][y-k].cover = 1;
								k++;
							}
						}
						if(testmap[i][j-1].brick==0&&testmap[i][j-1].value!=0&&testmap[i][j-1].cover==0)
						{
							x=i;
							y=j-1;
							testmap[x][y].value=0;
							testmap[x][y].light=1;
							testmap[x][y].cover=1;
							k=0;
							while(testmap[x+k][y].brick==0)
							{
								testmap[x+k][y].value=0;
								testmap[x+k][y].cover = 1;
								k++;
							}
							k=0;
							while(testmap[x-k][y].brick==0)
							{
								testmap[x-k][y].value=0;
								testmap[x-k][y].cover = 1;
								k++;
							}
							k=0;
							while(testmap[x][y+k].brick==0)
							{
								testmap[x][y+k].value=0;
								testmap[x][y+k].cover = 1;
								k++;
							}
							k=0;
							while(testmap[x][y-k].brick==0)
							{
								testmap[x][y-k].value=0;
								testmap[x][y-k].cover = 1;
								k++;
							}
						}

					}
					

					
						

				}
			}
		}*/
	
		
		testmap=row_checker(testmap);//O(n*SIZE)
		
		flag=0;
		for(i=1;i<=SIZE;i++)
		{
			for(j=1;j<=SIZE;j++)
			{
				if(testmap[i][j].brick==0)
					if(testmap[i][j].value!=0)
						flag=1;
			}
		}

		if(flag==0)
		{
			
			break;
		}
		
		







	}
	//printf("pre_processing :end\n");
	return testmap;
}
Point choose_row(Row_group r,Grid** testmap)
{
	Point Ans;
	double prob;
	double tmp=0;
	int i;
	int x=r.start.x;/*
	if(r.num==1)
	{
		Ans.x=x;
		Ans.y=r.start.y;
		return Ans;
	}*/
	for(i=r.start.y;i<=r.end.y;i++)
	{
		if(testmap[x][i].value!=0&&testmap[x][i].brick==0)
		{
			tmp += testmap[x][i].prob;
		}
		
	}
	prob=(double)(rand()%10000)/10000*tmp;

	for(i=r.start.y;i<=r.end.y;i++)
	{
		if(testmap[x][i].value!=0&&testmap[x][i].brick==0)
		{
			if(testmap[x][i].prob<prob)
			prob-=testmap[x][i].prob;
			else
				break;
		}
		
	}
	//printf("prob = %lf/%lf\n",testmap[x][i].prob,prob);
	Ans.x=x;
	Ans.y=i;
	return Ans;
}
Grid** count_heu(Grid** testmap)
{
	int i,j,k,count;
	for(i=1;i<=SIZE;i++)
	{
		for(j=1;j<=SIZE;j++)
		{
			
				testmap[i][j].heu=0;
			
		}
	}
	for(i=1;i<=SIZE;i++)
	{
		for(j=1;j<=SIZE;j++)
		{
			if(testmap[i][j].brick==0)
			{
				testmap[i][j].heu+=1;
			}
				
			else 
			{
				if(testmap[i][j].value!=5)
				{
					testmap[i+1][j].heu+=testmap[i][j].value;
					testmap[i-1][j].heu+=testmap[i][j].value;
					testmap[i][j+1].heu+=testmap[i][j].value;
					testmap[i][j-1].heu+=testmap[i][j].value;
				}


			}
		}
	}
	for(i=1;i<=SIZE;i++)
	{
		for(j=1;j<=SIZE;j++)
		{
			if(testmap[i][j].brick==0)
			{
				if(testmap[i][j].light==1)
				{
					k=0;
					while(testmap[i+k][j].brick==0)
					{
						testmap[i+k][j].heu=0;
						k++;
					}
					k=0;
					while(testmap[i-k][j].brick==0)
					{
						testmap[i-k][j].heu=0;
						k++;
					}
					k=0;
					while(testmap[i][j+k].brick==0)
					{
						testmap[i][j+k].heu=0;
						k++;
					}
					k=0;
					while(testmap[i][j-k].brick==0)
					{
						testmap[i][j-k].heu=0;
						k++;
					}
				}
			}	
			else 
			{
				if(testmap[i][j].value!=5)
				{
					count = 0;
					if(testmap[i+1][j].brick==0&&testmap[i+1][j].light==1)count++;
					if(testmap[i][j+1].brick==0&&testmap[i][j+1].light==1)count++;
					if(testmap[i-1][j].brick==0&&testmap[i-1][j].light==1)count++;
					if(testmap[i][j-1].brick==0&&testmap[i][j-1].light==1)count++;
					if(testmap[i][j].value<=count)
					{
						testmap[i+1][j].heu=0;
						testmap[i][j+1].heu=0;
						testmap[i][j-1].heu=0;
						testmap[i-1][j].heu=0;
					}
				}


			}
		}
	}
	return testmap;
}
int cost (Grid** testmap)
{
	int i,j;
	int sum = 0;
	int count;
	for(i=1;i<=SIZE;i++)
	{
		for(j=1;j<=SIZE;j++)
		{
			if(testmap[i][j].cover==0&&testmap[i][j].brick==0)
			{
				sum++;
			}
			else if(testmap[i][j].brick==1&&testmap[i][j].value!=0&&testmap[i][j].value!=5)
			{
				count = 0;

					if(testmap[i+1][j].brick==0&&testmap[i+1][j].light==1)count++;

					if(testmap[i-1][j].brick==0&&testmap[i-1][j].light==1)count++;

					if(testmap[i][j+1].brick==0&&testmap[i][j+1].light==1)count++;

					if(testmap[i][j-1].brick==0&&testmap[i][j-1].light==1)count++;
				sum+=testmap[i][j].value-count;


			}
		}
	}
	return sum;
}

Grid** buildSol(Grid** testmap)
{
	int i,j,k,count,decide;
	double totalporb=0;
	double prob=0;
	Point c;
	
	//testmap = map_copy(testmap,map);
	//testmap = pre_processing(testmap);
	if(cost(testmap)==0)return testmap;


	int ran,temp;
	int order[Row_num];
	for(i=0;i<Row_num;i++)
		order[i]=i;

	
	for(i=Row_num-1;i>=0;i--)
	{
		ran=rand()%(i+1);
		temp=order[i];
		order[i]=order[ran];
		order[ran]=temp;
	}
	/*for(i=0;i<Row_num;i++)
	{
		count = 0;
		for(j=Rows[i].start.y;j<=Rows[i].end.y;j++)
		{
			if(testmap[i][j].cover==0)
				count++;
		}
		Rows[i].prob=1-1/(count+1);
		totalporb+=Rows[i].prob;
	}
	prob=rand()/RAND_MAX*totalporb;
	for(i=0;i<Row_num;i++)
	{
		if(Rows[i].prob<prob)
			prob-=Rows[i].prob;
		else
			break;
	}
	decide=i;*/
	//print_map(testmap);
	for(i=0;i<Row_num;i++)
		Rows[i].light=0;

	
	
	for(i=0;i<Row_num;i++)
	{

		count = 0;
		for(j=Rows[order[i]].start.y;j<=Rows[order[i]].end.y;j++)
		{	if(testmap[Rows[order[i]].start.x][j].light==1)
				Rows[i].light=1;
			if(testmap[Rows[order[i]].start.x][j].value==0)
				count++;
		}
		if(Rows[order[i]].num==count)
			Rows[order[i]].light=1;
		if(Rows[order[i]].light==0)
		{
			c = choose_row(Rows[order[i]],testmap);
			if(testmap[c.x][c.y].brick==1)
			{	printf("BRICK!!!\n\n\n\\\n\n\n\n\n\n");
				print_map(testmap);
				for(j=Rows[order[i]].start.y;j<=Rows[order[i]].end.y;j++)
				{
					printf("(%d, %d) ",c.x,j);
				}
				printf("\n");

			}

			testmap[c.x][c.y].value=0;
			testmap[c.x][c.y].light = 1;
			testmap[c.x][c.y].cover = 1;
			testmap[c.x][c.y].pre_process = 0;
			Rows[i].light=1;

			testmap = brick_forbidden(testmap);
			
			
			//printf("in : (%d,%d)\n",c.x,c.y);

			k=0;
			while(testmap[c.x+k][c.y].brick==0)
			{
				testmap[c.x+k][c.y].value=0;
				testmap[c.x+k][c.y].cover = 1;
				k++;
			}
			k=0;
			while(testmap[c.x-k][c.y].brick==0)
			{
				testmap[c.x-k][c.y].value=0;
				testmap[c.x-k][c.y].cover = 1;
				k++;
			}
			k=0;
			while(testmap[c.x][c.y+k].brick==0)
			{
				testmap[c.x][c.y+k].value=0;
				testmap[c.x][c.y+k].cover = 1;
				k++;
			}
			k=0;
			while(testmap[c.x][c.y-k].brick==0)
			{
				testmap[c.x][c.y-k].value=0;
				testmap[c.x][c.y-k].cover = 1;
				k++;
			}
			testmap = pre_processing(testmap);
			testmap = count_heu(testmap);

		}
	}
	//printf("buildSol: end\n");
	return testmap;




}

void print_heu(Grid** testmap)
{
	int i,j;
	for(i=1;i<=SIZE;i++)
	{
		for(j=1;j<=SIZE;j++)
		{

			if(testmap[i][j].brick == 0)//只是空格
			{
				if(testmap[i][j].light==1)
					printf("L ");
				else
					printf("%d ",testmap[i][j].heu);
			}
			else
			{
				if(testmap[i][j].value == 5)
					printf("* ");
				else
					printf("%d ",testmap[i][j].value);

			}
		}
		printf("\n");
	}
}
void print_ph(Grid** testmap)
{
	int i,j;
	for(i=1;i<=SIZE;i++)
	{
		for(j=1;j<=SIZE;j++)
		{

			if(testmap[i][j].brick == 0)//只是空格
			{
			
				printf(" %lf  ",testmap[i][j].ph);
			}
			else
			{
				if(testmap[i][j].value == 5)
					printf("(*) ");
				else
					printf("(%d) ",testmap[i][j].value);

			}
		}
		printf("\n");
	}
}
Grid** update_ph(Grid** targetmap, Grid** testmap)
{
	int i,j;
	int count = 0;
	for(i=1;i<=SIZE;i++)
	{
		for(j=1;j<=SIZE;j++)
		{
			if(testmap[i][j].brick==0&&testmap[i][j].cover==0)
				count++;
		}
	}
	for(i=1;i<=SIZE;i++)
	{
		for(j=1;j<=SIZE;j++)
		{
			targetmap[i][j].ph*=(1-loo);
			if(testmap[i][j].pre_process==0&&testmap[i][j].light==1)
				targetmap[i][j].ph+=(double)bigQ/(double)count;
		}
	}
	return targetmap;
}
Grid** count_prob(Grid** testmap)
{
	int i ,j,x;
	double part;

	for(i=0;i<Row_num;i++)
	{
		part=0;
		x=Rows[i].start.x;
		for(j=Rows[i].start.y;j<=Rows[i].end.y;j++)
		{
			part+=testmap[x][j].ph*testmap[x][j].heu;
		}
		for(j=Rows[i].start.y;j<=Rows[i].end.y;j++)
		{
			testmap[x][j].prob=(double)(testmap[x][j].ph*testmap[x][j].heu)/part;
		}

	}
	return testmap;
	
}

int main()
{
	int i,j,k;
	int black=0;
	int tmp;
	srand(time(NULL));
	map=(Grid**)malloc((SIZE+2)*sizeof(Grid*));
	for(i=0;i<SIZE+2;i++)
	{
		map[i]=(Grid*)malloc((SIZE+2)*sizeof(Grid));
	}
	Grid ** testmap;

	for(i=0;i<=SIZE+1;i++)
	{
		for(j=0;j<=SIZE+1;j++)
		{


			if(i==0||j==0||i==SIZE+1||j==SIZE+1)
			{
				map[i][j].brick=1;
				map[i][j].value=5;
			}

			else
			{
				scanf("%d",&tmp);
				map[i][j].pre_process=1;
				map[i][j].x=i;
				map[i][j].y=j;
				map[i][j].heu=0;
				map[i][j].ph=1;
				if(tmp == 6)//只是空格
				{
					map[i][j].brick=0;
					map[i][j].value=1;
					map[i][j].light = 0;
					map[i][j].cover = 0;
				}
				else if(tmp==7)
				{
					map[i][j].brick=0;
					map[i][j].value=0;
					map[i][j].light = 1;
				}
				else
				{
					map[i][j].brick=1;
					if(tmp!=5)
						black+=tmp;
					map[i][j].value=tmp;

				}
			}
			
		}
	}
	int l,r;
	Row_num=0;
	l=0;r=0;



	for(i=1;i<=SIZE;i++)
	{
		for(j=1;j<=SIZE;j++)
		{
			if(map[i][j].brick==0)
			{
				l=j;
				while(map[i][j].brick==0)
					j++;
				r=j-1;
				Rows[Row_num].start.x=i;
				Rows[Row_num].start.y=l;

				Rows[Row_num].end.x=i;
				Rows[Row_num].end.y=r;
				Rows[Row_num].num=r-l+1;
				Row_num++;
			}
		}

	}
	map = pre_processing(map);
	map=count_heu(map);
	


	
	map = count_prob(map);
	
	
	
	//for(i=0;i<Row_num;i++)
	//	printf("start :(%d, %d), end : (%d, %d), num : %d\n",Rows[i].start.x,Rows[i].start.y,Rows[i].end.x,Rows[i].end.y,Rows[i].num);
	//testmap = map_copy(testmap,map);

	//Answer A;
	//A=pre_processing(testmap);
	//print_map(testmap);
	Grid*** antSOL;
	antSOL = (Grid***)malloc(NUM_ANT*sizeof(Grid**));
	for(i=0;i<NUM_ANT;i++)
		antSOL[i]=NULL;
	int best;
	for(i=0;i<MAX_ITER;i++)
	{
		best=SIZE*SIZE+black;
		//print_ph(map);
		for(k=0;k<NUM_ANT;k++)
		{
			antSOL[k] = map_copy(antSOL[k],map);
			antSOL[k] = buildSol(antSOL[k]);
				//print_map(antSOL[k]);
				//getchar();
			if(cost(antSOL[k])==0)
			{
				printf("solved!!  iteration = %d\n",i);
				print_map(antSOL[k]);
				return 0;
			}
			if(cost(antSOL[k])<best)
			{
				//print_map(antSOL[k]);
				best=cost(antSOL[k]);
			}
		}

		for(k=0;k<NUM_ANT;k++)
		{
			if(cost(antSOL[k])==best)
			{
				map = update_ph(map,antSOL[k]);
				map = count_prob(map);
				//print_ph(map);
				//getchar();
			}
		}

	}
	printf("can't solve\n");
		for(k=0;k<NUM_ANT;k++)
		{
			if(cost(antSOL[k])==best)
			{
				print_map(antSOL[k]);
			}
		}








		
}
