#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int n;
int map[100][100];
int brick[10000];
int p_now;
int pq_now;
int direc_stack[5];
int direc_num;
int shuff()
{
	int i;
	int ran,temp;
	for(i=0; i<n*n; i++)
		brick[i]=i;
	for(i=n*n-1;i>=0;i--)
	{
		ran=rand()%(i+1);
		temp=brick[i];
		brick[i]=brick[ran];
		brick[ran]=temp;
	}
	return 0;
}
int count(int direct)
{
	int i;
	
	i=0;
	switch(direct)
	{
		case 0:
			while(pq_now%n-i>0 && map[pq_now/n][pq_now%n-i] ==0)
				i++;
			break;
		case 1:
			while(pq_now/n-i>0 && map[pq_now/n-i][pq_now%n] ==0)
				i++;
			break;
		case 2:
			while(i+pq_now%n <n && map[pq_now/n][pq_now%n+i] ==0)
				i++;
			break;
		case 3:
			while(i+pq_now/n <n && map[pq_now/n+i][pq_now%n] ==0)
				i++;
			break;

	}
	//printf("longest step = %d\n",i);
	return i-1;
}
int qtest()
{
	int sum = 15;
	
	if(pq_now%n == 0 || map[pq_now/n][pq_now%n-1] !=0)
		sum -= 1;
	if(pq_now/n == 0 || map[pq_now/n-1][pq_now%n] !=0)
		sum -= 2;
	if(pq_now%n == n-1 || map[pq_now/n][pq_now%n+1] !=0)
		sum -= 4;
	if(pq_now/n == n-1 || map[pq_now/n+1][pq_now%n] !=0)
		sum -= 8;

	return sum;

}
void quest()
{
	
	int i,j;
	int pq_test;
	int flag;
	int pq_start;

	int direct, step,bricks;
	while(1)
	{
		direc_num=0;
		for(i=0;i<n;i++)
				for(j=0;j<n;j++)
					map[i][j]=0;
		pq_start = rand()%(n*n);
		pq_now = pq_start;
		pq_test=qtest();
		flag=1;
		//printf("in while\n");
		while(pq_test!=0)
		{
			//printf("in while while pq_test = %d\n",pq_test);
			if(pq_test%2 == 1)
			{
				direc_stack[direc_num]=0;
				direc_num++;
			}
			pq_test/=2;
			if(pq_test%2 == 1)
			{
				direc_stack[direc_num]=1;
				direc_num++;
			}
			pq_test/=2;
			if(pq_test%2 == 1)
			{
				direc_stack[direc_num]=2;
				direc_num++;
			}
			pq_test/=2;
			if(pq_test%2 == 1)
			{
				direc_stack[direc_num]=3;
				direc_num++;
			}
			//for(i=0;i<direc_num;i++)
			//	printf("%d ",direc_stack[i]);
		//	printf("\n");
			direct = direc_stack[rand()%direc_num];
			direc_num = 0;
			
			int temp = count(direct);
			//printf("temp = %d\n",temp);
			if(temp !=0)
				step=rand()%(temp)+1;
			else break;
			//printf("direct = %d, temp = %d, step = %d, pq_now = %d\n",direct,temp,step,pq_now);
			switch(direct)
			{
				case 0:
					for(i=0;i<step;i++)
						map[pq_now/n][pq_now%n-i]=1;
						if(pq_now%n-step-1>=0 && map[pq_now/n][pq_now%n-step-1]==0)
							map[pq_now/n][pq_now%n-step-1]=-1;
						pq_now=pq_now-step;

					break;
				case 1:
					for(i=0;i<step;i++)
						map[pq_now/n-i][pq_now%n]=1;
						if(pq_now/n-step-1>=0 && map[pq_now/n-step-1][pq_now%n]==0)
							map[pq_now/n-step-1][pq_now%n]=-1;
						pq_now=(pq_now/n-step)*n+pq_now%n;
					break;
				case 2:
					for(i=0;i<step;i++)
						map[pq_now/n][pq_now%n+i]=1;
						if(pq_now%n+step+1<=n-1 && map[pq_now/n][pq_now%n+step+1]==0)
							map[pq_now/n][pq_now%n+step+1]=-1;
						pq_now+=step;
					break;
				case 3:
					for(i=0;i<step;i++)
						map[pq_now/n+i][pq_now%n]=1;
					if(pq_now%n+step+1<=n-1 && map[pq_now/n+step+1][pq_now%n]==0)
							map[pq_now/n+step+1][pq_now%n]=-1;
					pq_now=(pq_now/n+step)*n+pq_now%n;
					break;
			}
		/*	for(i=0;i<n;i++)
		{
			for(j=0;j<n;j++)
				printf("%3d ",map[i][j]);
			printf("\n");
		}
		printf("\n");*/
		//printf("pq_now= %d\n",pq_now);
			pq_test=qtest();
			/*if(pq_test==0&&flag==1)
			{
				pq_now=pq_start;
				flag=0;
				pq_test=qtest();

			}*/
		}
		map[pq_now/n][pq_now%n] = 1;
		//printf("in while brick\n");
		bricks=0;
		for(i=0;i<n;i++)
				{
					for(j=0;j<n;j++)
					{
						if(map[i][j]==1)
							map[i][j] = 0;
						else
						{
							map[i][j] = 1;
							bricks++;
						}
							
					}
					
				}
			//printf("bricks = %d\n", bricks);
		int tmp,fflag=0;
		for(i=0;i<n;i++)
			{
				tmp =0;
				for(j=0;j<n;j++)
					if(map[i][j]==1)tmp++;
				if(tmp>n-1)fflag=1;

			}
			for(j=0;j<n;j++)
			{
				tmp =0;
				for(i=0;i<n;i++)
					if(map[i][j]==1)tmp++;
				if(tmp>n-1)fflag=1;
			}


		if(bricks>0 && bricks <0.3*n*n && fflag == 0)break;
		/*for(i=0;i<n;i++)
		{
			for(j=0;j<n;j++)
				printf("%d ",map[i][j]);
			printf("\n");
		}
		printf("\n");*/
	}



}

int test()
{
	int sum = 15;
	if(map[p_now/n][p_now%n] ==1)
	{
		printf("bad choice!!\n");
		return 0;
	}
	if(p_now%n == 0 || map[p_now/n][p_now%n-1] ==1)
		sum -= 1;
	if(p_now/n == 0 || map[p_now/n-1][p_now%n] ==1)
		sum -= 2;
	if(p_now%n == n-1 || map[p_now/n][p_now%n+1] ==1)
		sum -= 4;
	if(p_now/n == n-1 || map[p_now/n+1][p_now%n] ==1)
		sum -= 8;

	return sum;

}
int run(int direc)
{
	printf("in run\n");
	printf("in run: p_now = %d\n",p_now);
	switch(direc)
	{
		case 0:
			while(p_now%n !=0 && map[p_now/n][p_now%n-1] !=1)
			{
				map[p_now/n][p_now%n] = 1;
				p_now--;
			}
			break;
		case 1:
			while(p_now/n !=0 && map[p_now/n-1][p_now%n] !=1)
			{
				map[p_now/n][p_now%n] = 1;
				p_now=((p_now/n)-1)*n+p_now%n;
			}
			break;
		case 2:
			while(p_now%n != n-1 && map[p_now/n][p_now%n+1] !=1)
			{
				map[p_now/n][p_now%n] = 1;
				p_now++;
			}
			break;
		case 3:
			while(p_now/n != n-1 && map[p_now/n+1][p_now%n] !=1)
			{
				map[p_now/n][p_now%n] = 1;
				p_now=((p_now/n)+1)*n+p_now%n;
			}
			break;
	}
	printf("in run: p_now = %d\n",p_now);
	return 0;
}
int main()
{
	
	int i,j;

	int brick_num;
	srand(time(NULL));
	int p_test,direc;
	while(1)
	{
		printf("map size = ? * ?");
		scanf("%d",&n);
		
		/*printf("brick number = ?");
		scanf("%d",&brick_num);
		shuff();
		for(i=0;i<brick_num;i++)
			map[brick[i]/n][brick[i]%n] = 1;
		*/
		quest();
		for(i=0;i<n;i++)
		{
			for(j=0;j<n;j++)
				printf("%d ",map[i][j]);
			printf("\n");
		}

		printf("choose a start point(0~%d):  ",n*n-1);
		scanf("%d",&p_now);
		p_test = test();
		while(p_test != 0)
		{

			printf("choose a direction (0=left, 1=up, 2=right, 3=down) :?\n");
			printf("you only have:\n");
			if(p_test%2 == 1)
				printf("left\n");
			p_test/=2;
			if(p_test%2 == 1)
				printf("up\n");
			p_test/=2;
			if(p_test%2 == 1)
				printf("right\n");
			p_test/=2;
			if(p_test%2 == 1)
				printf("down\n");
			scanf("%d",&direc);
			run(direc);
			printf("p_now = %d\n",p_now);
			for(i=0;i<n;i++)
			{
				for(j=0;j<n;j++)
				{
					if(i == p_now/n && j == p_now%n)
						printf("%3c ",'p');
					else
						printf("%3d ",map[i][j]);
				}
				printf("\n");
			}
			p_test=test();
			printf("p_test = %d\n",p_test);
		}
		map[p_now/n][p_now%n]=1;
		int die_flag =0;
		for(i=0;i<n;i++)
			for(j=0;j<n;j++)
				if(map[i][j]==0)
					die_flag=1;
		if(die_flag==0)
			printf("you win!!\n");
		else printf("you lose!!\n");
		printf("final map:\n");
		for(i=0;i<n;i++)
			{
				for(j=0;j<n;j++)
				{
					if(i == p_now/n && j == p_now%n)
						printf("%3c ",'p');
					else
						printf("%3d ",map[i][j]);
				}
				printf("\n");
			}


	}
}