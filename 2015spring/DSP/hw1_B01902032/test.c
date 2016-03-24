#include <stdio.h>
#include <stdlib.h>
#include "hmm.h"
#include <math.h>


HMM hmms[5];
int observ[MAX_SEQ];
int observ_num = 50;
FILE* fp;
FILE* fp_result;
double Delta[5][MAX_SEQ][MAX_STATE];
int traceback[5][MAX_SEQ];

void get_model_observ()
{
	int i, ret;
	char tmpobserv[MAX_SEQ];
	fscanf(fp, "%s",tmpobserv);
	observ_num = strlen(tmpobserv);
	for(i=0; i< observ_num; i++)
		observ[i] = tmpobserv[i]-'A';
	
}
double count_Delta(int model)
{
	int i, j, n, t;
	int N, T;
	double result_prob;
	int result_state;
	N = hmms[model].state_num;
	T = observ_num;
	for(i=0; i<N; i++)
		Delta[model][0][i] = hmms[model].initial[i] * hmms[model].observation[observ[0]][i];
		
	traceback[model][0] = 0;
	for(t=1; t<T; t++)
	{
		for(j=0; j<N; j++)
		{
			//Delta[model][t][j] = max [i =1 to N] (Delta[model][t-1][i] * aij)     * bj(ot)
			int best_i = 0;
			double best_Delta = -1;
			for(i=0; i<N; i++)
			{
				double tmp = Delta[model][t-1][i] * hmms[model].transition[i][j];
				if( tmp > best_Delta)
				{
					best_Delta = tmp;
					traceback[model][t] = i;
					best_i = i;
				}		
			}
			Delta[model][t][j] = Delta[model][t-1][best_i] * hmms[model].transition[best_i][j] * hmms[model].observation[observ[t]][j];
		}
	}
	result_prob = 0;
	result_state = 0;
	for(i=0; i<N; i++)
	{
		if(Delta[model][T-1][i] > result_prob)
		{
			result_prob = Delta[model][T-1][i];
			result_state = i;
		}
	}
	return result_prob;
}

int main(int argc, char* argv[])
{
/*
	HMM hmms[5];
	load_models( "modellist.txt", hmms, 5);
	dump_models( hmms, 5);
*/
	load_models( argv[1], hmms, 5);
	dump_models( hmms, 5);

	int i, j, k, t, m, n, o;
	
	int best_model;
	double best_prob;

	
	fp = open_or_die(argv[2],"r");
	fp_result = open_or_die(argv[3],"w");

	for(o=0; o<2500; o++)  //2500 samples
	{
		fprintf(stderr,"testing sample %d\n",o);
		get_model_observ();
		best_model = 0;
		best_prob = -1;
		for(m=0; m<5; m++)
		{
			double tmpprob = count_Delta(m);
			if(tmpprob > best_prob)
			{
				best_prob = tmpprob;
				best_model = m;
			}
		}
		fprintf(fp_result, "model_0%d.txt\t%e\n",best_model+1,best_prob);
	}
	fprintf(stderr,"testing completed\n");


	printf("%f\n", log(1.5) ); // make sure the math library is included
	return 0;
}
