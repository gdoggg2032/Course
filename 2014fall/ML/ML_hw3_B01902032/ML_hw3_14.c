#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000

double **x;
double **y;
double **xt;
int dice;
double Ein;
double Eout;

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
	//result = tran(result,size1,size2);
	return result;

}
int main()
{
	int i,j,k,m;
	srand(time(NULL));
	double aveEin[6];
	double tmp;
	double **w;
	double err;
	double testw[5][6];
	int sign;
	
	x=(double**)malloc(SIZE*sizeof(double*));
	for(i=0;i<SIZE;i++)
		x[i]=(double*)malloc(3*sizeof(double));
	xt=(double**)malloc(SIZE*sizeof(double*));
	for(i=0;i<SIZE;i++)
		xt[i]=(double*)malloc(6*sizeof(double));

	y=(double**)malloc(SIZE*sizeof(double*));
	for(i=0;i<SIZE;i++)
		y[i]=(double*)malloc(sizeof(double));
	for(i=0;i<6;i++)
	{
		aveEin[i]=0;
	}
	testw[0][0]=-1;testw[0][1]=-0.05;testw[0][2]=0.08;testw[0][3]=0.13;testw[0][4]=1.5;testw[0][5]=1.5;
	testw[1][0]=-1;testw[1][1]=-0.05;testw[1][2]=0.08;testw[1][3]=0.13;testw[1][4]=1.5;testw[1][5]=15;
	testw[2][0]=-1;testw[2][1]=-0.05;testw[2][2]=0.08;testw[2][3]=0.13;testw[2][4]=15;testw[2][5]=1.5;
	testw[3][0]=-1;testw[3][1]=-1.5;testw[3][2]=0.08;testw[3][3]=0.13;testw[3][4]=0.05;testw[3][5]=0.05;
	testw[4][0]=-1;testw[4][1]=-1.5;testw[4][2]=0.08;testw[4][3]=0.13;testw[4][4]=0.05;testw[4][5]=1.5;
		



	for(k=0;k<1000;k++)
	{
		

		for(i=0;i<SIZE;i++)
		{
			x[i][0]=1;
			x[i][1]=0.000002*(rand()%1000000)-1;
			x[i][2]=0.000002*(rand()%1000000)-1;

			xt[i][0]=1;
			xt[i][1]=x[i][1];
			xt[i][2]=x[i][2];
			xt[i][3]=x[i][1]*x[i][2];
			xt[i][4]=x[i][1]*x[i][1];
			xt[i][5]=x[i][2]*x[i][2];



			if((x[i][1]*x[i][1]+x[i][2]*x[i][2]-0.6)>0)y[i][0]=1;
			else y[i][0]=-1;
			dice=rand()%100;
			if(dice<10)y[i][0]=-y[i][0];
		}
		w=mult(mult(inverse(mult(tran(xt,SIZE,6),xt,6,SIZE,6), 6, 6), tran(xt, SIZE, 6), 6, 6, SIZE), y, 6, SIZE, 1);
		for(i=0;i<6;i++)
			printf("%lf ",w[i][0]);
		printf("\n");
		
		
		printf("-----------------try-----------\n");

		Ein=0;
		for(i=0;i<SIZE;i++)
		{
			err=0;
			for(j=0;j<6;j++)
			{
				err+=xt[i][j]*w[j][0];
			}
			if(err>0)sign = 1;
				else sign = -1;
				if(sign!=y[i][0])Ein++;
		}
		aveEin[0]+=Ein;
		
		//aveEout+=Eout;
		printf("Ein = %lf\n",Ein/SIZE);

		for(m=0;m<5;m++)
		{
			Ein=0;
			for(i=0;i<SIZE;i++)
			{
				err=0;
				for(j=0;j<6;j++)
				{
					err+=xt[i][j]*testw[m][j];
				}
				if(err>0)sign = 1;
				else sign = -1;
				if(sign!=y[i][0])Ein++;
			}
			aveEin[1+m]+=Ein;
		}

	}
	
	

	for(i=0;i<6;i++)
		printf("aveEin %d = %lf\n", i,aveEin[i]/1000/SIZE);

		


}