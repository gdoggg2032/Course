#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define SIZE 1000
#define T_SIZE 3000
#define T 2000
#define Eta 0.001

double **x;
double **testx;

double theta(double A)
{
	return 1/(1+exp(-A));
}

int main()
{
	int i,j,k;
	double w[21];
	double gradient[21];
	double tmp;
	double Eout;
	int sign;
	x=(double**)malloc(SIZE*sizeof(double*));
	for(i=0; i<SIZE; i++)
		x[i]=(double*)malloc(22*sizeof(double));
	testx=(double**)malloc(T_SIZE*sizeof(double*));
	for(i=0;i<T_SIZE;i++)
		testx[i]=(double*)malloc(22*sizeof(double));

	for(i=0;i<SIZE;i++)
	{
		x[i][0]=1;
		for(j=1;j<22;j++)
			scanf("%lf",&x[i][j]);
	}
		
	for(i=0;i<T_SIZE;i++)
	{
		testx[i][0]=1;
		for(j=1;j<22;j++)
			scanf("%lf",&testx[i][j]);
	}
		
	for(i=0;i<21;i++)
	{
		w[i]=0;
		gradient[i]=0;
	}
		

	
	for(k=0;k<T;k++)
	{
		for(i=0;i<SIZE;i++)
		{
			tmp=0;
			for(j=0;j<21;j++)
				tmp+=w[j]*x[i][j];
			tmp=theta(-x[i][21]*tmp);
			for(j=0;j<21;j++)
				gradient[j]+=tmp*(-x[i][21]*x[i][j]);
		}
		for(i=0;i<21;i++)
			gradient[i]=gradient[i]/SIZE;
		for(i=0;i<21;i++)
			w[i]=w[i]-Eta*gradient[i];
		for(i=0;i<21;i++)
			gradient[i]=0;
	}

	printf("w = ");
	for(i=0;i<21;i++)
		printf("%lf ",w[i]);
	printf("\n");

	Eout=0;
	for(i=0;i<T_SIZE;i++)
	{
		tmp=0;
		for(j=0;j<21;j++)
			tmp+=w[j]*testx[i][j];
		
		if(tmp>0)sign=1;
		else sign=-1;
		if(sign!=testx[i][21])Eout++;
	}

	Eout=Eout/T_SIZE;

	printf("Eout = %lf\n",Eout);



	


}