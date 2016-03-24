#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 20

double x[SIZE];
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
int main()
{
	int i,j,k;
	srand(time(NULL));
	aveEin=0;
	aveEout=0;
	double tmp;


	for(k=0;k<5000;k++)
	{
		Ein=0;
		Eout=0;

		for(i=0;i<SIZE;i++)
		{
			x[i]=0.000002*(rand()%1000000)-1;
		}
		

		for(i=0;i<SIZE-1;i++)
		{
			for(j=0;j<SIZE-i-1;j++)
			{
				if(x[j]>x[j+1])
				{
					tmp = x[j];
					x[j] = x[j+1];
					x[j+1] = tmp;
				}
			}
		}
		for(i=0;i<SIZE;i++)
		{
			if(x[i]>0)
				y[i]=1;
			else
				y[i]=-1;
			dice = rand()%100;
			if(dice<20)
				y[i]=-y[i];
		}

		for(i=0;i<SIZE;i++)
			printf("%lf, %d\n",x[i],y[i]);
		//theta=0.000002*(rand()%1000000)-1;
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
					if(stmp*(x[j]-thetatmp)*y[j]<0)
					{
						Eintmp++;
					}
				if(Eintmp<Ein)
				{
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
	}
	printf("aveEin = %lf, aveEout = %lf\n",aveEin/SIZE/5000,aveEout/5000);


		


}