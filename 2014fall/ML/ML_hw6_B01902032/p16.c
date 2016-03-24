#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define SIZE 100
#define T_SIZE 1000
double **x;
double **t_x;
struct Node
{
	int s;
	double theta;
	int dim;
	struct Node *p;
	struct Node *n;
	struct Node *parent;
	double count;
	int b;
	int label;
};
struct REC
{
	int s;
	double theta;
	int dim;
	double Ein;
	double count;
};
struct REC *decision_stump(double **inst, int size)
{
	int Rec_num;
	struct REC Rec[size*10];
	int i,j,k,m;
	srand(time(NULL));
	Rec_num=0;
	double Ein = size;
	int stmp;
	double Eintmp, thetatmp;
	double count;
	double tmp;
	double ** ins = malloc(size*sizeof(double*));
	for(i=0;i<size;i++)
		ins[i] = malloc(3*sizeof(double));
	for(i=0;i<size;i++)
	{
		for(j=0;j<3;j++)
			ins[i][j] = inst[i][j];
	}
	/*
	for(i=0;i<SIZE;i++)
	{
		for(j=0;j<3;j++)
			printf("%lf ",ins[i][j]);
		printf("\n");
	}*/
	for(k=0;k<2;k++)
	{

		for(i=0;i<size-1;i++)
		{
			for(j=0;j<size-i-1;j++)
			{
				if(ins[j][k]>ins[j+1][k])
				{
					for(m=0;m<3;m++)
					{
						tmp = ins[j][m];
						ins[j][m] = ins[j+1][m];
						ins[j+1][m] = tmp;
					}
				}
			}
		}

		stmp=1;
		double gini00 = 0;
		double gini01 = 0;
		double gini10 = 0;
		double gini11 = 0;
		double count0;
		double count1;
		double tmp1,tmp0;
		while(1){
			for(i=0;i<=size;i++)
			{
				Eintmp = 0;
				if(i==0)
					thetatmp=ins[i][k]-0.001;
				else if(i==size)
					thetatmp=ins[i-1][k]+0.001;
				else
					thetatmp=0.5*(ins[i-1][k]+ins[i][k]);
				gini00 = 0;
				gini01 = 0;
				gini10 = 0;
				gini11 = 0;
				count0 = 0;
				count1 = 0;
				for(j=0;j<size;j++)
					if(stmp*(inst[j][k]-thetatmp)>0)
					{
						if(inst[j][2]>0)
							gini11++;
						else
							gini10++;
						count1++;
						
					}
					else
					{
						if(inst[j][2]>0)
							gini01++;
						else
							gini00++;
						count0++;
					}
					if(count1!=0)
						tmp1 = (1 - 0.5*(gini11/count1)*(gini11/count1)- 0.5*(gini10/count1)*(gini10/count1));
					else 
						tmp1 = 1;
					if(count0!=0)
						tmp0 = (1  - 0.5*(gini01/count0)*(gini01/count0)- 0.5*(gini00/count0)*(gini00/count0));
					else
						tmp0 = 1;
				Eintmp = tmp1*count1+tmp0*count0;
				printf(">>>%d %lf, tmp1 0 = %lf %lf, count 1 0 = %lf %lf\n",size,Eintmp,tmp1,tmp0,count1,count0);
				//printf(">>>>>>>>11 10 1 = %lf %lf %lf\n",gini11,gini10,count1);
				//printf(">>>>>>>>01 00 0 = %lf %lf %lf\n",gini01,gini00,count0);

				if(Eintmp<Ein)
				{
					//printf("Eintmp Ein , s,t,d = %lf %lf %d %lf %d\n",Eintmp,Ein,stmp,thetatmp,k);
					Rec_num=0;
					Rec[Rec_num].theta=thetatmp;
					Rec[Rec_num].dim=k;
					Rec[Rec_num].s=stmp;
					Rec[Rec_num].count = count1;
					Rec_num++;
					Ein=Eintmp;
					
					
				}	
				/*else if(Eintmp==Ein)
				{
					Rec[Rec_num].theta=thetatmp;
					Rec[Rec_num].dim=k;
					Rec[Rec_num].s=stmp;
					Rec[Rec_num].count = count1;
					Rec_num++;


				}*/


			}
			if(stmp==1)stmp=-1;
			else break;
		}
	}
	struct REC * ans = malloc(sizeof(struct REC));
	i = rand()%Rec_num;
	ans->theta = Rec[i].theta;
	ans->dim = Rec[i].dim;
	ans->s = Rec[i].s;
	ans->count = Rec[i].count;
	ans->Ein = Ein;
	printf("ans: s,d,t,c = %d, %d, %lf, %lf\n",ans->s, ans->dim,ans->theta,ans->count);
	return ans;
}
struct Node * dtree(double **ins, int size, struct Node *node)
{
	int i,j;
	if(node == NULL)
	{
		printf(" is NULL\n");
		node = malloc(sizeof(struct Node));
		node->p = NULL;
		node->n = NULL;
		node->b = 0;
		if(size == 0)
		{
			return NULL;
		}
		if(size == 1)
		{
			node->label = ins[0][2];
			return node;
		}
			
		struct REC * result = decision_stump(ins, size);
		printf("r: s,d,t,c = %d, %d, %lf, %lf\n",result->s, result->dim,result->theta,result->count);
		
		node->s = result->s;
		node->dim = result->dim;
		node->theta = result->theta;
		node->count = result->count;
		node->b = 0;
		if(node->count == 0 || node->count == size)
		{
			node->label = ins[0][2];
			for(i=0;i<size;i++)
				printf("%lf ",ins[i][2]);
			printf("\n");
			return node;
		}
			

		double **insp = malloc((result->count)*sizeof(double*));
		for(i=0;i<result->count;i++)
			insp[i] = malloc(3*sizeof(double));
		double **insn = malloc((size - result->count)*sizeof(double*));
		for(i=0;i<size - result->count;i++)
			insn[i] = malloc(3*sizeof(double));
		int inspc = 0, insnc = 0;
		for(i=0;i<size;i++)
		{
			if ((node->s) * (ins[i][node->dim]- node->theta)>0)
			{
				for(j=0;j<3;j++)
					insp[inspc][j] = ins[i][j];
				inspc++;
			}
			else
			{
				for(j=0;j<3;j++)
					insn[insnc][j] = ins[i][j];
				insnc++;
			}
		}
		node->p = dtree(insp, node->count, node->p);
		node->n = dtree(insn, size - node->count, node->n);
		//node->b = node->p->b+node->n->b;
		node->b = 1;
		if(node->p !=NULL)
			node->b +=node->p->b;
		if(node->n != NULL)
			node->b += node->n->b;
		return node;
	}
	else
		fprintf(stderr, "node != NULL\n");
	return NULL;
}
int predict(struct Node*root, double* ins)
{
	struct Node*now = root;
	int predict;
	while(now != NULL)
	{
		if((now->s) * (ins[now->dim]- now->theta)>0)
		{
			if(now->p == NULL)predict = now->label;
			now = now->p;
		}
		else
		{
			if(now->n == NULL)predict = now->label;
			now = now->n;
		}
	}
	return predict;
}

int main()
{
	int i,j;
	x = malloc(SIZE*sizeof(double*));
	for(i=0;i<SIZE;i++)
		x[i] = malloc(3*sizeof(double));
	t_x = malloc(T_SIZE*sizeof(double*));
	for(i=0;i<T_SIZE;i++)
		t_x[i] = malloc(3*sizeof(double));
	for(i=0;i<SIZE;i++)
	{
		for(j=0;j<3;j++)
			scanf("%lf",&x[i][j]);
	}
	for(i=0;i<T_SIZE;i++)
	{
		for(j=0;j<3;j++)
			scanf("%lf",&t_x[i][j]);
	}
	for(i=0;i<SIZE;i++)
	{
		for(j=0;j<3;j++)
			printf("%lf ",x[i][j]);
		printf("\n");
	}

	struct Node * root;
	root = dtree(x,SIZE,root);

	fprintf(stderr,"END, s,t,d,b = %d %lf %d %d\n",root->s,root->theta,root->dim,root->b);
	/*double ds[3];
	ds[0]=0.502864; ds[1]=0.721677; ds[2]=-1.000000;
	
	printf("======%d\n",predict(root,ds));*/
	double Ein = 0;
	for(i=0;i<SIZE;i++)
	{
		if(predict(root,x[i]) != x[i][2])
			Ein++;
	}
	Ein = Ein / SIZE;
	double Eout = 0;
	for(i=0;i<T_SIZE;i++)
	{
		if(predict(root,t_x[i]) != t_x[i][2])
			Eout++;
	}
	Eout = Eout / T_SIZE;
	printf("Ein = %lf, Eout = %lf\n",Ein,Eout);

	return 0;

}



