#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define SIZE 1200

double **x;
double **y;
double **xt;
int dice;
double Ein;
double Eout;
double la;



double **mult(double **array1, double ** array2, int size, int d_size, int t_size)
{
	int i,j,k;
	double ** result = (double**)malloc(size*sizeof(double*));
	for(i=0;i<size;i++)
		result[i] = (double*)malloc(t_size*sizeof(double));
	for(i=0;i<size;i++)
	{
		for(j=0;j<t_size;j++)
		{
			result[i][j]=0;
			for(k=0;k<d_size;k++)
				result[i][j]+=array1[i][k]*array2[k][j];
		}
	}
	return result;

}
double **tran(double ** array, int size1, int size2)
{
	int i,j;
	double ** result = (double**)malloc(size2*sizeof(double*));
	for(i=0;i<size2;i++)
		result[i] = (double*)malloc(size1*sizeof(double));
	for (i = 0; i < size2; i++)
	{
		for(j=0;j<size1;j++)
			result[i][j]=array[j][i];
	}

	return result;
}
double **row_change(double ** array, int size1, int size2, int row1, int row2)
{
	if(row2==row1)return array;
	double tmp;
	int i;
	for(i=0;i<size2;i++)
	{
		tmp = array[row1][i];
		array[row1][i] = array[row2][i];
		array[row2][i] = tmp;
	}
	return array;

}

int notR0(double ** array, int size1,int size2, int column, int edge)
{
	int i;
	for(i=edge;i<size1;i++)
		if(array[i][column] !=0)
			return i;
	return -1;
}

double ** scaleto1(double ** array, int size1, int size2, int row)
{
	int i;
	double scale = 1/array[row][row];
	for(i=0;i<size2;i++)
		array[row][i] *= scale;

	return array;
}
double ** addto(double ** array , int size1 , int size2, int row, int column)
{
	int i,j;
	double scale = 1/array[row][column];
	double tmp1,tmp2;
	double scale1,scale2;
	for(i=0;i<size1;i++)
	{
		if(i!=row)
		{	
			scale1=array[i][column];
			scale2=array[row][column];
			for(j=0;j<size2;j++)
			{
				tmp1= array[row][j] *scale1;
				tmp2 = array[i][j] *scale2;
				array[i][j] =tmp2-tmp1;
			}
		}
	}
	return array;
}
void printR(double **array, int size1, int size2)
{
	int i,j;
	printf("-----------------------\n");
	for(i=0;i<size1;i++)
	{
		for(j=0;j<size2;j++)
			printf("%lf ",array[i][j]);
		printf("\n");
	}
	printf("------------------------\n");
}

double **inverse(double **array, int size1, int size2)
{
	int i,j,change;
	//printR(array,size1,size2);
	double ** tmp = (double**)malloc(size1*sizeof(double*));
	for(i=0;i<size1;i++)
		tmp[i] = (double*)malloc(2*size2*sizeof(double));
	for (i = 0; i < size1; i++)
	{
		for(j=0;j<size2;j++)
			tmp[i][j]=array[j][i];
		for(j=size2;j<2*size2;j++)
			tmp[i][j]=0;
		tmp[i][i+size2]=1;
	}

	for(i=0;i<size2;i++)
	{
		change = notR0(tmp,size1,2*size2, i, i);
		if(change<0){printf("no inverse\n");return NULL;}
		tmp = row_change(tmp,size1,2*size2, i, change);
		//printR(tmp,size1,2*size2);
		tmp = addto(tmp,size1, 2*size2, i , i);
		//printR(tmp,size1,2*size2);
			
	}
	//printR(tmp,size1,2*size2);
	for(i=0;i<size2;i++)
	{
		tmp = scaleto1(tmp,size1, 2*size2, i);
	}
	double ** result = (double**)malloc(size1*sizeof(double*));
	for(i=0;i<size1;i++)
		result[i] = (double*)malloc(size2*sizeof(double));
	for(i=0;i<size1;i++)
	{
		for(j=0;j<size2;j++)
			result[i][j]=tmp[i][j+size2];
	}
	for(i=0;i<size1;i++)
		free(tmp[i]);
	free(tmp);
	//printR(result,size1,size2);
	return result;

}
int main()
{
	int i,j,k,m;
	srand(time(NULL));
	double aveEin;
	double tmp;
	double **w;
	double **tmpw;
	double err;
	double aveEout;
	double **testx;
	double **testy;
	double **testxt;
	int sign;
	double Fla;
	double FEin=200;
	int Fk;
	
	x=(double**)malloc(SIZE*sizeof(double*));
	for(i=0;i<SIZE;i++)
		x[i]=(double*)malloc(3*sizeof(double));
	testx=(double**)malloc(SIZE*sizeof(double*));
	for(i=0;i<SIZE;i++)
		testx[i]=(double*)malloc(3*sizeof(double));
	y=(double**)malloc(SIZE*sizeof(double*));
	for(i=0;i<SIZE;i++)
		y[i]=(double*)malloc(sizeof(double));
	testy=(double**)malloc(SIZE*sizeof(double*));
	for(i=0;i<SIZE;i++)
		testy[i]=(double*)malloc(sizeof(double));
	
	
		aveEin=0;
		aveEout = 0;
	
	
		



	

		for(i=0;i<SIZE;i++)
		{
			if(i<200)
			{
				x[i][0]=1;
				scanf("%lf %lf %lf",&x[i][1],&x[i][2],&y[i][0]);

			}
			else
			{
				testx[i-200][0]=1;
				scanf("%lf %lf %lf",&testx[i-200][1],&testx[i-200][2],&testy[i-200][0]);

			}
		}



		for(k=-10;k<3;k++)
		{
			la=pow(10,k);
			tmpw=mult(tran(x,200,3),x,3,200,3);
			for(i=0;i<3;i++)
				tmpw[i][i]+=la;
			w=mult(inverse(tmpw,3,3), mult(tran(x,200,3),y, 3,200,1), 3, 3, 1);
			for(i=0;i<3;i++)
				printf("%lf ",w[i][0]);
			printf("\n");
		
		
	

			Ein=0;
			for(i=0;i<200;i++)
			{
				err=0;
				for(j=0;j<3;j++)
				{
					err+=x[i][j]*w[j][0];
				}
				if(err>0)sign = 1;
					else sign = -1;
					if(sign!=y[i][0])Ein++;
			}
			if(Ein<=FEin)
			{
				Fla=la;
				FEin=Ein;
				Fk=k;
			}

		}

			tmpw=mult(tran(x,200,3),x,3,200,3);
			for(i=0;i<3;i++)
				tmpw[i][i]+=Fla;
			w=mult(inverse(tmpw,3,3), mult(tran(x,200,3),y, 3,200,1), 3, 3, 1);
			printf("%d ",k);
			for(i=0;i<3;i++)
				printf("%lf ",w[i][0]);
			printf("\n");
		
		//aveEin+=Ein;
		
		//aveEout+=Eout;
		printf("-----------------try-----------\n");
		Eout=0;
		for(i=0;i<1000;i++)
		{
			err=0;
			for(j=0;j<3;j++)
			{
				err+=testx[i][j]*w[j][0];
			}
			if(err>0)sign = 1;
				else sign = -1;
				if(sign!=testy[i][0])Eout++;
		}
		aveEout+=Eout;
		printf("logla = %d, la = %lf, Ein = %lf, Eout = %lf\n", Fk, Fla, FEin/200, Eout/1000);
		

	
	
	

		//printf("aveEin  = %lf, aveEout = %lf\n", aveEin[0]/1000/SIZE, aveEout/1000/SIZE);

		


}