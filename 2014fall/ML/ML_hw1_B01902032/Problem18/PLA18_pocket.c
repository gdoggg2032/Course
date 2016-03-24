#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int n,k;
double data[500][6];
double test_data[500][6];
double w[5],best_w[5];
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

int main()
{
	n=0;
	k=0;
	while(1)
	{
		
		if(n<500){
			data[n][0] = 1;
			scanf("%lf %lf %lf %lf %lf",&data[n][1],&data[n][2],&data[n][3],&data[n][4],&data[n][5]);
			n++;
		}
		else if(k>=500)break;
		else 
		{
			test_data[k][0]=1;
			scanf("%lf %lf %lf %lf %lf",&test_data[k][1],&test_data[k][2],&test_data[k][3],&test_data[k][4],&test_data[k][5]);
			k++;

		}
	}
	int m,sum=0;
	int count1, i,j, index;
	srand(time(NULL));
	int ran;
	int old_mistake,best_mistake;
	for(m=0;m<2000;m++)
	{
		for(j=0;j<5;j++)
			w[j]=0;
		for(j=0;j<5;j++)
			best_w[j]=w[j];
		best_mistake=0;
		for(j=0;j<n;j++)
			if(sign(dot(best_w,data[j]),data[j][5])<0)
				best_mistake++;
		i=0;
		while(i<50)		
		{
			ran=rand()%n;
			if(sign(dot(w,data[ran]),data[ran][5])<0)
			{
				for(j=0;j<5;j++)
					w[j]=w[j]+data[ran][5]*data[ran][j];
				old_mistake=0;
				for(j=0;j<n;j++)
					if(sign(dot(w,data[j]),data[j][5])<0)
						old_mistake++;
				if(old_mistake<best_mistake)
				{		
					best_mistake=old_mistake;
					for(j=0;j<5;j++)
						best_w[j]=w[j];
				}
				i++;					
				//printf("find error!!, w= %lf %lf %lf %lf %lf count1 = %d\n",w[0],w[1],w[2],w[3],w[4],count1);
			}		
		}
		count1=0;
		for(j=0;j<k;j++)
		{
			if(sign(dot(best_w,test_data[j]),test_data[j][5])<0)
				count1++;
		}
		sum+=count1;
		printf("error number is %d\n",count1);
	}
	double err_rate = (double)sum/(double)(2000*k);
	printf("average error rate is %lf\n",err_rate);
}