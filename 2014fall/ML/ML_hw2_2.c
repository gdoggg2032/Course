#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 100
#define T_SIZE 1000
double x[SIZE][10];
double t_x[T_SIZE][10];
int y[SIZE];
int dice;
double Eintmp;
double thetatmp;
double Ein;
double Eout;
double aveEin;
double aveEout;
double theta;
int stmp;
int s;
int dim;

struct rec
{
	double theta;
	int dim;
	int s;

};
struct rec Rec[SIZE*10];
int Rec_num;
int main()
{
	int i,j,k,m;
	srand(time(NULL));
	aveEin=0;
	aveEout=0;
	double tmp;
	Rec_num=0;


	
		Ein=0;
		Eout=0;

		for(i=0;i<SIZE;i++)
		{
			for(j=0;j<10;j++)
				scanf("%lf",&x[i][j]);
		}
		for(i=0;i<T_SIZE;i++)
		{
			for(j=0;j<10;j++)
				scanf("%lf",&t_x[i][j]);
		}
		Ein = 100;
		for(k=0;k<9;k++)
		{


		

			for(i=0;i<SIZE-1;i++)
			{
				for(j=0;j<SIZE-i-1;j++)
				{
					if(x[j][k]>x[j+1][k])
					{
						for(m=0;m<10;m++)
						{
							tmp = x[j][m];
							x[j][m] = x[j+1][m];
							x[j+1][m] = tmp;
						}

						
					
					}
				}
			}
			stmp=1;
			while(1){
			for(i=0;i<SIZE;i++)
			{
				Eintmp = 0;
				if(i==0)
					thetatmp=x[i][k]-0.001;
				else if(i==SIZE-1)
					thetatmp=x[i][k]+0.001;
				else
					thetatmp=0.5*(x[i][k]+x[i+1][k]);
				for(j=0;j<SIZE;j++)
					if(stmp*(x[j][k]-thetatmp)*x[j][9]<0)
					{
						Eintmp++;
					}
				if(Eintmp<Ein)
				{
					Rec_num=0;
					Rec[Rec_num].theta=thetatmp;
					Rec[Rec_num].dim=k;
					Rec[Rec_num].s=stmp;
					Rec_num++;
					Ein=Eintmp;
					
				}	
				else if(Eintmp==Ein)
				{
					Rec[Rec_num].theta=thetatmp;
					Rec[Rec_num].dim=k;
					Rec[Rec_num].s=stmp;
					Rec_num++;

				}


			}
			if(stmp==1)stmp=-1;
			else break;
		}
			printf("Ein = %lf\n",Ein/SIZE);
		}



		printf("final Ein = %lf\n",Ein/SIZE);


		printf("Rec_num = %d\n",Rec_num);
		i = rand()%Rec_num;
		theta = Rec[Rec_num].theta;
		dim = Rec[Rec_num].dim;
		s = Rec[Rec_num].s;
		Eout=0;
		for(i=0;i<T_SIZE;i++)
		{
			if(s*(t_x[i][dim]-theta)*t_x[i][9]<0)
			{
				Eout++;
			}
		}
		printf("final Eout = %lf\n",Eout/T_SIZE);

		/*for(i=0;i<SIZE;i++)
			{
				for(j=0;j<10;j++)
					printf("%lf ",x[i][j]);
				printf("\n");
			}*/
			
		//theta=0.000002*(rand()%1000000)-1;
		return 0;}
		/*
		Ein = 20;
		stmp=1;
		while(1){

			for(i=0;i<SIZE;i++)
			{
				Eintmp = 0;
				if(i==0)
					thetatmp=x[i]-0.001;
				else if(i==SIZE-1)
					thetatmp=x[i]+0.001;
				else
					thetatmp=0.5*(x[i]+x[i+1]);
				for(j=0;j<SIZE;j++)
					if(s*(x[j]-thetatmp)*y[j]<0)
					{
						Eintmp++;
					}
				if(Eintmp<Ein)
				{
					
					Ein=Eintmp;
					theta=thetatmp;
					s=stmp;
				}	
				else if(Eintmp==Ein)
				{
					if(rand()%2 == 1)break;
					Ein=Eintmp;
					theta=thetatmp;
					s=stmp;

				}


			}
			if(stmp==1)stmp=-1;
			else break;
		}
		
		aveEin+=Ein;
		if(theta>0)
			Eout=0.5+0.3*s*(theta-1);
		else
			Eout=0.5+0.3*s*(-theta-1);
		aveEout+=Eout;
		printf("Ein = %lf, Eout = %lf\n",Ein/SIZE,Eout);
	
	printf("aveEin = %lf, aveEout = %lf\n",aveEin/SIZE/5000,aveEout/5000);


}*/