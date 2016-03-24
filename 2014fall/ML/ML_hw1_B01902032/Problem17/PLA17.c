#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int n;
double data[500][6];
double w[5];
int order[500];
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

int shuff()
{
	int i;
	int ran,temp;
	for(i=0; i<n; i++)
		order[i]=i;
	for(i=n-1;i>=0;i--)
	{
		ran=rand()%(i+1);
		temp=order[i];
		order[i]=order[ran];
		order[ran]=temp;
	}
	return 0;
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
	int m,sum=0;
	int count1, i,j, index;
	srand(time(NULL));
	for(m=0;m<2000;m++)
	{
		for(j=0;j<5;j++)
			w[j]=0;
		count1=0;
		index=-1;
		shuff();
		i=0;
		while(1)
		{
			i=i%n;
			if(i==index)break;
			
				if(sign(dot(w,data[order[i]]),data[order[i]][5])<0)
				{
					count1++;
					index=i;	
					//printf("find error!!, w= %lf %lf %lf %lf %lf count1 = %d\n",w[0],w[1],w[2],w[3],w[4],count1);
					for(j=0;j<5;j++)
						w[j]+=0.5*data[order[i]][5]*data[order[i]][j];
				}
				i++;
		}
		sum+=count1;
		printf("error number is %d\n",count1);
	}
	printf("average error number is %d\n",sum/2000);
}