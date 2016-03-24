#include <stdio.h>
#include <stdlib.h>
#include "hmm.h"
#include <math.h>


HMM hmm;
double Alpha[MAX_SEQ][MAX_STATE];
double Beta[MAX_SEQ][MAX_STATE];
double Gamma[MAX_SEQ][MAX_STATE];
double Epsilon[MAX_SEQ][MAX_STATE][MAX_STATE];

int observ[MAX_SEQ];
int observ_num = 50;
FILE* fp;

void get_model_observ()
{
	int i, ret;
	char tmpobserv[MAX_SEQ];
	fscanf(fp, "%s",tmpobserv);
	observ_num = strlen(tmpobserv);
	for(i=0; i< observ_num; i++)
		observ[i] = tmpobserv[i]-'A';
	
}

void count_Alpha()
{
	int n, t, i, j;
	int N = hmm.state_num;
	int T = observ_num;
	for(i=0; i<N; i++)
		Alpha[0][i] = hmm.initial[i] * hmm.observation[observ[0]][i];
	for(t=1; t<T; t++)
	{
		for(j=0; j<N; j++)
		{
			double tmpsum = 0;
			for(i=0; i<N; i++)
				tmpsum += Alpha[t-1][i] *hmm.transition[i][j];
			Alpha[t][j] = tmpsum * hmm.observation[observ[t]][j];
		}
	}
	/*printf("Alpha: \n");
	 for( i = 0 ; i < T; i++ ){
      for( j = 0 ; j < N ; j++ )
         printf("%.5lf ", Alpha[i][j] );
      printf("\n");
   }*/


}
void count_Beta()
{
	int n, t, i, j;
	int N = hmm.state_num;
	int T = observ_num;
	for(i=0; i<N; i++)
		Beta[T-1][i] = 1;
	for(t=T-2; t>=0; t--)
	{
		for(i=0; i<N; i++)
		{
			double tmpsum = 0;
			for(j=0; j<N; j++)
			{
				tmpsum += hmm.transition[i][j] * hmm.observation[observ[t+1]][j] * Beta[t+1][j];
			}
			Beta[t][i] = tmpsum;
		}
	}
}
void count_Gamma()
{
	int n, t, i, j;
	int N = hmm.state_num;
	int T = observ_num;
	for(t=0; t<T; t++)
	{
		double tmpsum = 0;
		for(i=0; i<N; i++)
		{
			Gamma[t][i] = Alpha[t][i] * Beta[t][i];
			tmpsum += Gamma[t][i];
		}
		for(i=0; i<N ;i++)
			Gamma[t][i] /= tmpsum;
	}
	}
void count_Epsilon()
{
	int n, t, i, j;
	int N = hmm.state_num;
	int T = observ_num;
	for(t=0; t<T-1; t++)
	{
		double tmpsum = 0;
		for(i=0; i<N; i++)
		{
			for(j=0; j<N; j++)
			{
				Epsilon[t][i][j] = Alpha[t][i] * hmm.transition[i][j] * hmm.observation[observ[t+1]][j] * Beta[t+1][j];
				tmpsum += Epsilon[t][i][j];
			}
		}
		for(i=0; i<N; i++)
			for(j=0; j<N; j++)
				Epsilon[t][i][j] /= tmpsum;
	}


}


int main(int argc, char* argv[])
{



/* -----  train  ----- */
	
	//load_models( "modellist.txt", hmms, 5);
	//dump_models( hmms, 5);

	
	loadHMM( &hmm, argv[2] );
	dumpHMM( stderr, &hmm );
	

	int i, j, k, t, m, n, o;
	int N, T;
	int ITER = atoi(argv[1]);
	fprintf(stderr, "ITER = %d\n",ITER);
	//for(i=0; i<5; i++)
	//	loadHMM( &hmms[i], "model_init.txt");
	//dump_models( hmms, 5);


	
	//fp[1] = open_or_die("seq_model_02.txt","r");
	//fp[2] = open_or_die("seq_model_03.txt","r");
	//fp[3] = open_or_die("seq_model_04.txt","r");
	//fp[4] = open_or_die("seq_model_05.txt","r");



	//in loop

	N = hmm.state_num;
	T = observ_num;
	double new_initial[MAX_STATE];
	double new_transition[MAX_STATE][MAX_STATE];
	double new_observation[MAX_OBSERV][MAX_STATE];

	double acc_gamma[T][N];
	double acc_epsilon[T][N][N];
	fp = open_or_die(argv[3],"r");
	for(m=0; m<ITER; m++) //learn for ITER times
	{
		printf("********** ITERATION %d **************\n",m);
		
		
		rewind(fp);
		for(n=0;n<MAX_STATE;n++) // clear new matrix
		{
			new_initial[n] = 0;
			for(i=0; i<MAX_STATE; i++)
				new_transition[n][i] = 0;
			for(t=0; t<MAX_OBSERV; t++)
			{
				new_observation[t][n] = 0;
			}
		}
		for(n=0; n<N; n++)
		{
			for(t=0; t<T; t++)
			{
				acc_gamma[t][n] = 0;
				for(j=0; j<N; j++)
					acc_epsilon[t][n][j] = 0;
			}
		}
		//printf("initialized\n");
		
		for(o=0; o<10000; o++) //n lines
		{
			get_model_observ();
			count_Alpha();
			count_Beta();
			count_Gamma();
			count_Epsilon();
			//printf("Alpha = %lf, Beta = %e, gamma = %lf, epsilon = %lf\n",Alpha[0][0], Beta[5][0], Gamma[0][0], Epsilon[0][0][0]);
		

			//printf("counted: o = %d, m = %d\n",o,m);

			//accumulate Gamma and Epsilon
			for(i=0; i<N; i++)
				new_initial[i] += Gamma[0][i];

			for(t=0; t<T-1; t++)
				for(i=0; i<N; i++)
					for(j=0; j<N; j++)
						acc_epsilon[t][i][j] += Epsilon[t][i][j];
			for(t=0; t<T; t++)
				for(i=0; i<N; i++)
					acc_gamma[t][i] += Gamma[t][i];


			for(j=0; j<N; j++)
			{
				for(t=0; t<T; t++)
				{
					new_observation[observ[t]][j] += Gamma[t][j];
				}
			}

		}
		//printf("initial = %lf, acc_gamma = %e, acc_epsilon = %e\n",new_initial[0], acc_gamma[0][0], acc_epsilon[0][0][0]);
		for(i=0; i<N; i++)        //initial
			new_initial[i] /= 10000;
		//printf("T = %d, N = %d\n",T,N);
		for(i=0; i<N; i++)			//transition
		{
			double tmpsumg = 0;
			for(t=0; t<T-1; t++)
			{

				tmpsumg += acc_gamma[t][i];
				//printf("accg = %e, tmpsumg = %e\n",acc_gamma[t][i], tmpsumg);
			}
			
			for(j=0; j<N; j++)
			{
				double tmpsume = 0;
				for(t=0; t<T-1; t++)
					tmpsume += acc_epsilon[t][i][j];

				new_transition[i][j] = tmpsume / tmpsumg;
				//printf("%e,%e,%e\n",tmpsume, tmpsumg, new_transition[i][j]);
			}
		}

		for(j=0; j<N; j++)
		{
			double tmpsumg = 0;
			for(t=0; t<T; t++)
				tmpsumg += acc_gamma[t][j];

			for(k=0; k<hmm.observ_num; k++)
			{
				new_observation[k][j] /= tmpsumg;
			}
		}

		//replace original model
		for(n=0;n<MAX_STATE;n++) 
		{
			hmm.initial[n] = new_initial[n];
			for(i=0; i<MAX_STATE; i++)
				hmm.transition[n][i] = new_transition[n][i];
			for(t=0; t<MAX_OBSERV; t++)
			{
				hmm.observation[t][n] = new_observation[t][n];
			}
		}
		dumpHMM( stderr, &hmm );
		fprintf(stderr,"========= ========= =========\n");

		//break;
	}
	fprintf(stderr,"ITERATION END\n");
	dumpHMM( stderr, &hmm );
	FILE * fp_model = open_or_die(argv[4],"w");
	dumpHMM( fp_model, &hmm);

	
	fprintf(stderr,"training completed\n");
	

	printf("make sure the math library is included: %f\n", log(1.5) ); // make sure the math library is included
	return 0;
}
