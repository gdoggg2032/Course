#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//compressed Column storage
#define maxlink 67108864 //2^26
#define maxnode 2097152 //2^21

double Data[maxlink];
int Row_ind[maxlink];
int Col_start_ptr[maxnode];
int null_col[maxnode];
int node_num;
int null_col_num;
int N, L;
double DAMPING_FACTOR = 0.85;
double EPSILON = 0.000001;
char OUTPUT[100];
char INPUT[100];

double Page[maxnode];
double nextPage[maxnode];

FILE * out_ptr;


void Read_matrix(char* filename)
{
	int n;
	for(n=0;n<maxnode;n++)
		null_col[n] = 1;
	FILE *fp = fopen(filename,"r");
	char tmp[10];
	
	N = 1;
	L = 1;
	fscanf(fp, "%s %d\n", tmp, &node_num);
	null_col_num = node_num;
	int row, num;
	Col_start_ptr[N] = L;

	while(fscanf(fp, "%d:%d", &row, &num)>0)
	{
		null_col[row] = 0;
		null_col_num -= 1;
		while(N<=row)
		{
			Col_start_ptr[N] = L;
			N++;
		}
		int i, col;
		for(i=0; i<num; i++)
		{
			fscanf(fp, "%d", &col);
			Data[L] = 1.0/num;
			Row_ind[L] = col;
			L++;

		}
		//N++;
		

	}
	while(N<=node_num+1)
	{
		Col_start_ptr[N] = L;
		N++;
	}

	fclose(fp);
}
double Get_matrix(int row, int col)
{
	if (col>=N) return -1;
	int RN = Col_start_ptr[col+1]-Col_start_ptr[col];
	int i;
	int start = Col_start_ptr[col];
	int end = Col_start_ptr[col+1];
	for(i=start;i<end;i++)
	{
		if(Row_ind[i] == row)
			return Data[i];
	}
	return -1;

}
void Dump_matrix()
{

	int i,j;
	for(i=1;i<10;i++)
		printf("%d ",i);
	printf("\n");
	for(i=1;i<10;i++)
		printf("%d ",Col_start_ptr[i]);
	printf("\n");
	for(i=1;i<14;i++)
		printf("%d ",i);
	printf("\n");
	for(i=1;i<14;i++)
		printf("%d ",Row_ind[i]);
	printf("\n");
	for(i=1;i<14;i++)
		printf("%lf ",Data[i]);
	printf("\n");

	for(i=0; i<10;i++)
	{
		for(j=0;j<10;j++)
			printf("(%d,%d) = %lf\n", i, j, Get_matrix(i,j));
	}
}
void Get_PageRank()
{
	//fprintf(stderr,"%d!\n",null_col_num);
	int i, j;
	for(i=1;i<=node_num;i++)
		Page[i]=1;
	while(1)
	{

		
		for(i=1;i<=node_num;i++)
		{
			nextPage[i] = 1 - DAMPING_FACTOR;
		}
			


		for(i=1;i<=node_num;i++)
		{
		
			int col_start = Col_start_ptr[i];
			int col_end = Col_start_ptr[i+1];
			for(j=col_start;j<col_end;j++)
			{
				nextPage[Row_ind[j]] += DAMPING_FACTOR*Data[j]*Page[i];
			}
			
			//nextPage[i] += DAMPING_FACTOR * null_col_num/node_num*nP_sum;
			//nextPage[i] = nextPage[i]*Page[i]
			
		}
		double nP_sum = 0;
		for(i=1;i<=node_num;i++)
			if(null_col[i]==1)
				nP_sum+=Page[i];

		for(j=1;j<=node_num;j++)
		{
			if(null_col[j]==1)
				nextPage[j]+=DAMPING_FACTOR*1/(node_num-1)*(nP_sum-Page[j]);
			else
				nextPage[j]+=DAMPING_FACTOR*1/(node_num-1)*(nP_sum);

		}
			

		


		double dis = 0;
		for(i=1;i<=node_num;i++)
		{
			dis+=(nextPage[i]-Page[i])*(nextPage[i]-Page[i]);
		}
		//fprintf(stderr,"%e : %e\n",dis,EPSILON*EPSILON);
		if(dis<EPSILON*EPSILON)break;
		else
		{
			for(i=1;i<=node_num;i++)
				Page[i] = nextPage[i];
		}
	}
	for(i=1;i<=node_num;i++)
		Page[i] = nextPage[i];

	return;

}
int main(int argc, char* argv[]) //./executePR.sh [-d DAMPING FACTOR] [-e EPSILON] [-o OUTPUT] INPUT
{
	int arg;
	for(arg=1; arg<argc; arg++)
	{
			if(strcmp(argv[arg],"-d") == 0){
				DAMPING_FACTOR = atof(argv[arg+1]);
				fprintf(stderr, "DAMPING_FACTOR: %lf\n", DAMPING_FACTOR);
				arg += 1;
			}
			else if(strcmp(argv[arg],"-e") == 0){
				EPSILON = atof(argv[arg+1]);
				fprintf(stderr, "EPSILON: %lf\n", EPSILON);
				arg += 1;
			}
			else if(strcmp(argv[arg],"-o") == 0){
				strcpy(OUTPUT, argv[arg+1]);
				fprintf(stderr,"OUTPUT: %s\n",OUTPUT);
				out_ptr = fopen(OUTPUT, "w");
				arg += 1;
			}
			else{
				strcpy(INPUT, argv[arg]);
				fprintf(stderr,"INPUT: %s\n",INPUT);

				
			}
		
	}
	if(out_ptr == NULL)
		out_ptr = stdout;
	Read_matrix(INPUT);
	//printf("(N, L) = (%d, %d)\n",N,L);
	//Dump_matrix();
	Get_PageRank();
	int i;
	for(i=1;i<=node_num;i++)
		fprintf(out_ptr, "%d:%lf\n",i,Page[i]);



}


