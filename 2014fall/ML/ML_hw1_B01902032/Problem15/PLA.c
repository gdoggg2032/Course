#include <stdio.h>
#include <stdlib.h>

int n;
double data[500][6];
double w[5];
double dot(double t1[5], double t2[5])
{
	int i;
	double sum=0;
	for(i=0;i<5;i++)
		sum+=t1[i]*t2[i];	
	return sum;
}
int sign( double test1, int test2)
{
	if(test1*test2 < 0)
		return -1;
	else if (test1 == 0)
		return -test2;
	else return 1;
}
int main()
{
	n=0;
	while(1)
	{
		data[n][0] = 1;
		if(scanf("%lf %lf %lf %lf %lf",&data[n][1],&data[n][2],&data[n][3],&data[n][4],&data[n][5])>0)
			n++;
		else break;
	}
	int j;
	for(j=0;j<5;j++)
		w[j]=0;
	int count1=0, i=0, index=-1;
	while(1)
	{
		i=i%n;
		if(i==index)break;
		
			if(sign(dot(w,data[i]),data[i][5])<0)
			{
				count1++;
				index=i;	
				//printf("find error!!, w= %lf %lf %lf %lf %lf count1 = %d\n",w[0],w[1],w[2],w[3],w[4],count1);
				for(j=0;j<5;j++)
					w[j]+=data[i][5]*data[i][j];
			}
			i++;
	}
	printf("error number is %d\n",count1);
}