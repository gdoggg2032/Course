/*****************************************************************************\
 * Theory of Computer Games: Fall 2012
 * Chinese Dark Chess Search Engine Template by You-cheng Syu
 *
 * This file may not be used out of the class unless asking
 * for permission first.
 *
 * Modify by Hung-Jui Chang, December 2013
\*****************************************************************************/
#include<cstdio>
#include<cstdlib>
#include"anqi.hh"
#include"Protocol.h"
#include"ClientSocket.h"
#include<math.h>

#ifdef _WINDOWS
#include<windows.h>
#else
#include<ctime>
#endif

const int DEFAULTTIME = 15;
typedef  double SCORE;
static const SCORE INF=1000000001;
static const SCORE WIN=1000000000;
SCORE SearchMax(const BOARD&,int,int,SCORE ,SCORE ,int);
SCORE SearchMin(const BOARD&,int,int,SCORE ,SCORE ,int);
SCORE loc_wei[32] = {2,3,3,2,3,4,4,3,3,4,4,3,3,5,5,3,3,5,5,3,3,4,4,3,3,4,4,3,2,3,3,2};

#ifdef _WINDOWS
DWORD Tick;     // ?}?l?ɨ?
int   TimeOut;  // ?ɭ?
#else
clock_t Tick;     // ?}?l?ɨ?
clock_t TimeOut;  // ?ɭ?
#endif
MOV   BestMove; // ?j?X?Ӫ??̨ε۪k

bool TimesUp() {
#ifdef _WINDOWS
	return GetTickCount()-Tick>=TimeOut;
#else
	return clock() - Tick > TimeOut;
#endif
}
static POS mkpos(int x,int y) {
	return x*4+y;
}
// ?@?ӭ??q?????誺?f??????
/*SCORE Eval(const BOARD &B) {
	int cnt[2]={0,0};
	for(POS p=0;p<32;p++)
	{
		const CLR c=GetColor(B.fin[p]);
		if(c!=-1)
			cnt[c]++;
	}
	for(int i=0;i<14;i++)
		cnt[GetColor(FIN(i))]+=B.cnt[i];
	return cnt[B.who]-cnt[B.who^1];
}*/
	//eat
FIN history[800][32];
int game_length = 0;
bool ChkDanger(const BOARD &B, POS p, FIN fp) // if POS p will be eat
{
	int x = p/4;
	int y = p%4;
	int i;
	bool danger = false;
	int count = 0;
	for(i=1;x+i<8;i++)
	{
		if(B.fin[mkpos(x+i,y)]!=FIN_E)
			count++;
		if(count==1&&i==1)
			danger = ChkEats(B.fin[mkpos(x+i,y)],fp);
		if(count==2&&(B.fin[mkpos(x+i,y)]==FIN_C||B.fin[mkpos(x+i,y)]==FIN_c))
			danger = ChkEats(B.fin[mkpos(x+i,y)],fp);

	}
	count = 0;
	for(i=1;x-i>=0;i++)
	{
		if(B.fin[mkpos(x-i,y)]!=FIN_E)
			count++;
		if(count==1&&i==1)
			danger = ChkEats(B.fin[mkpos(x-i,y)],fp);
		if(count==2&&(B.fin[mkpos(x-i,y)]==FIN_C||B.fin[mkpos(x-i,y)]==FIN_c))
			danger = ChkEats(B.fin[mkpos(x-i,y)],fp);

	}
	count = 0;
	for(i=1;y-i>=0;i++)
	{
		if(B.fin[mkpos(x,y-i)]!=FIN_E)
			count++;
		if(count==1&&i==1)
			danger = ChkEats(B.fin[mkpos(x,y-i)],fp);
		if(count==2&&(B.fin[mkpos(x,y-i)]==FIN_C||B.fin[mkpos(x,y-i)]==FIN_c))
			danger = ChkEats(B.fin[mkpos(x,y-i)],fp);
	}
	count = 0;
	for(i=1;y+i<4;i++)
	{
		if(B.fin[mkpos(x,y+i)]!=FIN_E)
			count++;
		if(count==1&&i==1)
			danger = ChkEats(B.fin[mkpos(x,y+i)],fp);
		if(count==2&&(B.fin[mkpos(x,y+i)]==FIN_C||B.fin[mkpos(x,y+i)]==FIN_c))
			danger = ChkEats(B.fin[mkpos(x,y+i)],fp);
	}
	return danger;

}
bool ChkAttack(const BOARD &B, POS p, FIN fp) // if POS p will be eat
{
	int x = p/4;
	int y = p%4;
	int i;
	bool danger = false;
	int count = 0;
	for(i=1;x+i<8;i++)
	{
		if(B.fin[mkpos(x+i,y)]!=FIN_E)
			count++;
		if(count==1)
			danger = ChkEats(fp,B.fin[mkpos(x+i,y)]);
		if(count==2&&(fp == FIN_C||fp==FIN_c))
			danger = ChkEats(fp,B.fin[mkpos(x+i,y)]);

	}
	count = 0;
	for(i=1;x-i>=0;i++)
	{
		if(B.fin[mkpos(x-i,y)]!=FIN_E)
			count++;
		if(count==1)
			danger = ChkEats(fp,B.fin[mkpos(x-i,y)]);
		if(count==2&&(fp == FIN_C||fp==FIN_c))
			danger = ChkEats(fp,B.fin[mkpos(x-i,y)]);

	}
	count = 0;
	for(i=1;y-i>=0;i++)
	{
		if(B.fin[mkpos(x,y-i)]!=FIN_E)
			count++;
		if(count==1)
			danger = ChkEats(fp,B.fin[mkpos(x,y-i)]);
		if(count==2&&(fp == FIN_C||fp==FIN_c))
			danger = ChkEats(fp,B.fin[mkpos(x,y-i)]);
	}
	count = 0;
	for(i=1;y+i<4;i++)
	{
		if(B.fin[mkpos(x,y+i)]!=FIN_E)
			count++;
		if(count==1)
			danger = ChkEats(fp,B.fin[mkpos(x,y+i)]);
		if(count==2&&(fp == FIN_C||fp==FIN_c))
			danger = ChkEats(fp,B.fin[mkpos(x,y+i)]);
	}
	return danger;

}
bool ChkCannon(const BOARD &B, POS p, POS q)
{
	if(p%4!=q%4 && p/4 != q/4)return false;
	if(p%4==q%4)
	{
		int count=0;
		if(q>p)
			for(int i=p;i<=q;i+=4)
			{
				if(B.fin[i]!=FIN_E)
					count++;
			}
		else
			for(int i=p;i<=q;i-=4)
			{
				if(B.fin[i]!=FIN_E)
					count++;
			}
		if(count==3 && B.fin[q]!=FIN_X)return true;
	}
	else if(p/4==q/4)
	{
		int count=0;
		if(q>p)
			for(int i=p;i<=q;i++)
			{
				if(B.fin[i]!=FIN_E)
					count++;
			}
		else
			for(int i=p;i<=q;i--)
			{
				if(B.fin[i]!=FIN_E)
					count++;
			}
		if(count==3 && B.fin[q]!=FIN_X)return true;
	}
	return false;

}
void flood(const BOARD &B, SCORE ** group, POS p, POS q, SCORE ** block, int depth)
{

	int dep = depth;
	if( p<0||p>31||q<0||q>31 ||block[p][q]==1||B.fin[p]==FIN_X||B.fin[p]==FIN_E||depth>31 )return;
	//fprintf(stderr,"before = %lf\n",group[p][q]);
	/*if(B.fin[p] == FIN_C||B.fin[p] == FIN_c)
	{
		if(p==q)group[p][q]=0;
		else
		{
			if(ChkCannon(B,p,q)==true&&ChkEats(B.fin[p], B.fin[q])==true)
				group[p][q]=1;
			else
				group[p][q]=INF;
		}
	}
	*/
	if(dep<group[p][q])
		group[p][q] = dep;
	else
		return;
	if(B.fin[q] == FIN_E||GetColor(B.fin[p])==GetColor(B.fin[q]))group[p][q]=0;
	if( (q+1)%4!=0)
		flood(B,group,p,q+1,block,dep+1);
	if(q%4!=0)
		flood(B,group,p,q-1,block,dep+1);
	flood(B,group,p,q-4,block,dep+1);
	flood(B,group,p,q+4,block,dep+1);
	
	
	//if(dep!=0)fprintf(stderr,"!=0, p q = %d %d, depth = %d , >> %lf\n",p,q,dep,group[p][q]);
	
}
void BuildStep(const BOARD &B, SCORE ** group, SCORE ** block)
{
	for(POS p=0;p<32;p++)
	{
		if(B.fin[p] == FIN_C||B.fin[p] == FIN_c)
		{
			for(POS q=0;q<32;q++)
			{
				if(p==q)group[p][q]=0;
				else
				{
					if(ChkCannon(B,p,q)==true&&ChkEats(B.fin[p], B.fin[q])==true&&B.fin[q]!=FIN_E)
						group[p][q]=1;
					else
						group[p][q]=INF;
				}
			}
		}
		else
			flood(B,group,p,p ,block,0);
		
		
	}
}
bool Chk(const BOARD &B, POS p, FIN fp)
{
	bool danger = false;
	if(p%4!=3 && (B.fin[p+1]!=FIN_c||B.fin[p+1]!=FIN_C))
		danger = danger | ChkEats(B.fin[p+1],fp);
	if(p%4!=0 && (B.fin[p-1]!=FIN_c||B.fin[p-1]!=FIN_C))
		danger = danger | ChkEats(B.fin[p-1],fp);
	if(p/4 != 0&& (B.fin[p-4]!=FIN_c||B.fin[p-4]!=FIN_C))
		danger = danger | ChkEats(B.fin[p-4],fp);
	if(p/4 != 7&& (B.fin[p+4]!=FIN_c||B.fin[p+4]!=FIN_C))
		danger = danger | ChkEats(B.fin[p+4],fp);


	return danger;



}
	//flip
MOV Flipper(const BOARD &B)
{
	POS flip_can[32];
	POS flip_can_avoid_c[32];
	POS flip_can_avoid_g[32];
	int flip_num = 0;
	int flip_num_c = 0;
	int flip_num_g = 0;
	for(POS p=0;p<32;p++)
		if(B.fin[p]==FIN_X)
		{
			flip_can[flip_num] = p;
			flip_num++;
		}
	if(flip_num==0)return BestMove;
	for(int i=0;i<flip_num;i++)
	{
		for(POS p=0;p<32;p++)
		{
			if((B.fin[p]==FIN_c||B.fin[p]==FIN_C)&&GetColor(B.fin[p])==B.who^1)
			{
				if(ChkCannon(B,flip_can[i],p)==false)
				{
					flip_can_avoid_c[flip_num_c] = flip_can[i];
					flip_num_c++;
				}
			}
		}
	}
	fprintf(stderr,"nor = %d, c = %d\n",flip_num,flip_num_c);
	if(flip_num_c==0)
	{
		int c=0;
		POS p;
		for(p=0;p<32;p++)if(B.fin[p]==FIN_X)c++;
		if(c!=0)
		{
			c=rand()%c;
			for(p=0;p<32;p++)if(B.fin[p]==FIN_X&&--c<0)break;
			BestMove=MOV(p,p);
			//X_num--;
			
		}
		return BestMove;
	}
	int c = rand()%flip_num_c;

	BestMove=MOV(flip_can_avoid_c[c],flip_can_avoid_c[c]);
	for(POS p=0;p<32;p++)
	{
		if((B.fin[p]==FIN_G||B.fin[p]==FIN_g)&&GetColor(B.fin[p])==B.who^1)
		{
			for(int i=0;i<flip_num_c;i++)
			{
				if(B.cnt[0+7*B.who]==1 && flip_num_c ==1)continue;
				else 
				{
					if(flip_can_avoid_c[i]==p+1 && p%4!=3)continue;
					if(flip_can_avoid_c[i]==p-1 && p%4!=0)continue;
					if(flip_can_avoid_c[i]==p-4 && p/4!=0)continue;
					if(flip_can_avoid_c[i]==p+4 && p/4!=7)continue;
					flip_can_avoid_g[flip_num_g] = flip_can_avoid_c[i];
					flip_num_g++;
				}
			}
		}
	}
	if(flip_num_g==0)return BestMove;
	c = rand()%flip_num_g;

	BestMove=MOV(flip_can_avoid_g[c],flip_can_avoid_g[c]);
	return BestMove;


}



const SCORE wei[14] = {6380,3180,1580,780,380,430,200,6380,3180,1580,780,380,430,200};
//const SCORE wei[14] = {128,64,32,16,8,4,2,128,64,32,16,8,4,2,};
//const SCORE wei[14] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1};
const SCORE PAWN_WITH_KING = 10;
const SCORE NONFLIP = 15;
const SCORE DIS_WEI = 0.2;

SCORE **group;
SCORE **block;

SCORE Eval(const BOARD &B) {
	SCORE cnt[2]={0,0};
	//fprintf(stderr,"<<0\n");
	int live_cnt[14] = {0};
	for(POS p=0;p<32;p++)
	{
		if(B.fin[p]!=FIN_E&&B.fin[p]!=FIN_X)
			live_cnt[B.fin[p]]++;
	}
		
	

	for(int i=0;i<14;i++)
		live_cnt[i]+=B.cnt[i];
	//fprintf(stderr,"<<1.3\n");
	SCORE value[14];
	value[0] = 1 + 4*(live_cnt[8]+live_cnt[9]+live_cnt[10]+live_cnt[11]+live_cnt[12]) + (live_cnt[7]);
	value[1] = 1 + 4*(live_cnt[9]+live_cnt[10]+live_cnt[11]+live_cnt[12]+live_cnt[13]) +(live_cnt[8]);
	value[2] = 1 + 4*(live_cnt[10]+live_cnt[11]+live_cnt[12]+live_cnt[13]) + (live_cnt[9]);
	value[3] = 1 + 4*(live_cnt[11]+live_cnt[12]+live_cnt[13]) + (live_cnt[10]);
	value[4] = 1 + 4*(live_cnt[12]+live_cnt[13]) + (live_cnt[11]);
	value[5] = 1 + 4*(live_cnt[0]+live_cnt[1]+live_cnt[2]+live_cnt[3]+live_cnt[4]+live_cnt[5]+live_cnt[6]) + (live_cnt[7]+live_cnt[8]+live_cnt[9]+live_cnt[10]+live_cnt[11]+live_cnt[12]+live_cnt[13]);
	value[6] = 1 + 4*(live_cnt[7]) + (live_cnt[13]);

	value[7] = 1 + 4*(live_cnt[1]+live_cnt[2]+live_cnt[3]+live_cnt[4]+live_cnt[5]) + (live_cnt[0]);
	value[8] = 1 + 4*(live_cnt[2]+live_cnt[3]+live_cnt[4]+live_cnt[5]+live_cnt[6]) +(live_cnt[1]);
	value[9] = 1 + 4*(live_cnt[3]+live_cnt[4]+live_cnt[5]+live_cnt[6]) + (live_cnt[2]);
	value[10] = 1 + 4*(live_cnt[4]+live_cnt[5]+live_cnt[6]) + (live_cnt[3]);
	value[11] = 1 + 4*(live_cnt[5]+live_cnt[6]) + (live_cnt[4]);
	value[12] = 1 + 4*(live_cnt[7]+live_cnt[8]+live_cnt[9]+live_cnt[10]+live_cnt[11]+live_cnt[12]+live_cnt[13]) + (live_cnt[0]+live_cnt[1]+live_cnt[2]+live_cnt[3]+live_cnt[4]+live_cnt[5]+live_cnt[6]);
	value[13] = 1 + 4*(live_cnt[0]) + (live_cnt[6]);
	SCORE power[14];
	power[0] = (value[7] + value[8] + value[9] + value[10] + value[11] + value[12]) * wei[0];
	power[1] = (value[8] + value[9] + value[10] + value[11] + value[12] +value[13]) * wei[1];
	power[2] = (value[9] + value[10] + value[11] + value[12] +value[13]) * wei[2];
	power[3] = (value[10] + value[11] + value[12] +value[13]) * wei[3];
	power[4] = (value[11] + value[12] + value[13]) * wei[4];
	power[5] = (value[7]+value[8] + value[9] + value[10] + value[11] + value[12] +value[13])*wei[5] *3;//4/10 * power[1] /wei[1] * wei[5];//(value[7]+value[8] + value[9] + value[10] + value[11] + value[12] +value[13]) + 4/15 *power[1]; //4/10 * power[1] /wei[1] * wei[5];//4/15 * power[1] /wei[1] * wei[5];
	power[6] = (value[7] + value[13])* wei[6];

	power[7] = (value[0] + value[1] + value[2] + value[3] + value[4] + value[5]) * wei[7];
	power[8] = (value[1] + value[2] + value[3] + value[4] + value[5] +value[6]) * wei[8];
	power[9] = (value[2] + value[3] + value[4] + value[5] +value[6]) * wei[9];
	power[10] = (value[3] + value[4] + value[5] +value[6]) * wei[10];
	power[11] =  (value[4] + value[5] +value[6]) * wei[11];	
	power[12] = (value[0] + value[1] + value[2] + value[3] + value[4] + value[5] +value[6])*wei[12] *3;//4/10 * power[1] /wei[1] * wei[5];//(value[0] + value[1] + value[2] + value[3] + value[4] + value[5] +value[6]) + 4/15 * power[8];//4/10 * power[1] /wei[1] * wei[5];//4/15 * power[8] /wei[8] *wei[12];
	power[13] = (value[0] + value[6]) * wei[13];
	for(int i=0;i<14;i++)
		power[i] += 50;
	//fprintf(stderr,"<<1.2\n");
	for(POS p=0;p<32;p++)
	{
		const CLR c = GetColor(B.fin[p]);
		if(c!=-1)
		{
			cnt[c]+=power[B.fin[p]];
			cnt[c]+=loc_wei[p];
		}
	}
	//fprintf(stderr,"<<1.1\n");
	
	for(int i=0;i<32;i++)
		for(int j=0;j<32;j++)
		{
			group[i][j]=INF;
			block[i][j]=1;
		}
			
	for(POS p=0;p<32;p++)
	{
		//fprintf(stderr,"now is %d, ",p);
		if(B.fin[p]==FIN_C||B.fin[p]==FIN_c)
		{
			for(POS m=0;m<32;m++)
				if(B.fin[m]!=FIN_X&&B.fin[m]!=FIN_E)block[p][m]=0;
		}
		else
			for(POS m=0;m<32;m++)
			{	
				//fprintf(stderr,"(p,m) = (%d, %d) , FIN = (%d, %d)\n",p,m,B.fin[p],B.fin[m]);
				//fprintf(stderr,"<<1.0.1\n");
				if(B.fin[p] == FIN_X)continue;
				//fprintf(stderr,"<<1.0.2\n");

				if(B.fin[p] == FIN_E)continue;
				//fprintf(stderr,"<<1.0.3\n");

				//if(B.fin[p]<14 && B.fin[m] <14 && GetLevel(B.fin[p]) == GetLevel(B.fin[m]) && (GetColor(B.fin[p]) != GetColor(B.fin[m])))continue;
				//fprintf(stderr,"<<1.0.4\n");

				if(ChkEats(B.fin[p],B.fin[m])==false && (GetColor(B.fin[p]) != GetColor(B.fin[m])) )continue;
				//fprintf(stderr,"<<1.0.5\n");

					block[p][m] = 0;
				/*if( GetColor(B.fin[p]) == GetColor(B.fin[m])&&B.fin[p]<14 && B.fin[m] <14)block[p][m] = 0;
				else if(B.fin[p] == FIN_E)block[p][m] = 1;
				else
				{
					if(ChkEats(B.fin[p],B.fin[m]) && !(B.fin[p]%7 == B.fin[m]%7&& B.fin[p]<14 && B.fin[m] <14))
							block[p][m]=0;
				}*/
				/*if(B.fin[p]>=14||B.fin[m]==FIN_X)
					block[p][m] = 1;

				else if(((ChkEats(B.fin[p],B.fin[m]) == false ||  (B.fin[p]%7 == B.fin[m]%7&& B.fin[p]<14 && B.fin[m] <14)) &&   GetColor(B.fin[p]) != GetColor(B.fin[m])))
					block[p][m] = 1;
				if(m==p&&B.fin[p]<14 && B.fin[m] <14)block[p][m] = 0;*/
				//if(block[p][m]==0)
				//	fprintf(stderr, "(%d,%lf) ",m,block[p][m] );
					

			}
		//fprintf(stderr, "\n" );
	}

	/*for(POS p=0;p<32;p++)
	{
		//if(B.fin[p] == FIN_C||B.fin[p] == FIN_c)
		{
			fprintf(stderr,"now is %d, ",p);
			for(POS m=0;m<32;m++)
			{	if(block[p][m]!=1 )
					fprintf(stderr, "(%d,%lf) ",m,block[p][m] );
			}

			fprintf(stderr, "\n" );
		}
			
	}

	*/



	//fprintf(stderr,"<<1.0\n");
	BuildStep(B,group,block);
	//fprintf(stderr,"<<1\n");
	/*for(POS p=0;p<32;p++)
	{
		//if(B.fin[p] == FIN_C||B.fin[p] == FIN_c)
		{
			fprintf(stderr,"now is %d, ",p);
			for(POS m=0;m<32;m++)
			{	if(group[p][m]!=INF )
					fprintf(stderr, "(%d,%lf) ",m,group[p][m] );
			}

			fprintf(stderr, "\n" );
		}
			
	}*/
	int X_num = 0;
	for(int i=0;i<14;i++)
		X_num+=B.cnt[i];

		//fprintf(stderr, "before tmp: %lf\n", cnt[B.who]-cnt[B.who^1]);
	for(POS p=0;p<32;p++)
	{
		const CLR c=GetColor(B.fin[p]);
		for(POS m=0;m<32;m++)
		{	
			if(group[p][m]!=INF&&p!=m&&group[p][m]!=0&&B.fin[m]!=FIN_E&&B.fin[m]!=FIN_X) // && (((int)group[p][m])%2==0||B.fin[p] == FIN_C||B.fin[p] == FIN_c))
			{
				/*if(X_num==0)
				{
					SCORE odd=group[p][m];
					if(((int)odd)%2==0)
						cnt[c]+=power[B.fin[m]]/((SCORE)group[p][m]) * DIS_WEI;
				}*/
				//else
					if(B.fin[p]!=FIN_X&&B.fin[p]!=FIN_E &&  GetLevel(B.fin[p])==GetLevel(B.fin[m]))continue;
					cnt[c]+=power[B.fin[m]]/((SCORE)group[p][m]) * DIS_WEI;
				//fprintf(stderr,"cjocjcojoc = %lf\n",power[B.fin[p]]/group[p][m]);
			}

		}
	}
	//fprintf(stderr, "after tmp: %lf\n", cnt[B.who]-cnt[B.who^1]);




	//fprintf(stderr,"<<2\n");
	return cnt[B.who]-cnt[B.who^1];
	/*SCORE p_cnt[2]={0,0};
	SCORE k_cnt[2]={0,0};
	SCORE b_max[2]={7,7};
	for(POS p=0;p<32;p++)
	{
		const CLR c=GetColor(B.fin[p]);

		if(c!=-1)
		{
			cnt[c]+=wei[B.fin[p]]*2;
			if(GetLevel(B.fin[p])<b_max[c])
				b_max[c] = GetLevel(B.fin[p]);
			/*if(B.fin[p]==FIN_P && B.cnt[9]>0) 
				cnt[c]+=PAWN_WITH_KING/B.cnt[7];
			if(B.fin[p]==FIN_p && B.cnt[1]>0)
				cnt[c]+=PAWN_WITH_KING/B.cnt[15];*/
	/*		if(B.fin[p]==FIN_P)
				p_cnt[0]++;
			else if(B.fin[p]==FIN_p)
				p_cnt[1]++;
			if(B.fin[p]==FIN_K)
				k_cnt[0]++;
			else if(B.fin[p]==FIN_k)
				k_cnt[1]++;
			if(ChkDanger(B,p,B.fin[p])==true)
				cnt[c]-=wei[B.fin[p]]/2;
			//if(ChkAttack(B,p,B.fin[p])==true)
			//	cnt[c]+=1;//wei[B.fin[p]]/2;

		}

			//cnt[c]++;
	}
	p_cnt[0]+=B.cnt[7];
	p_cnt[1]+=B.cnt[15];
	k_cnt[0]+=B.cnt[1];
	k_cnt[1]+=B.cnt[9];
	if(k_cnt[1]>0&&p_cnt[0]>0) 
		cnt[0]+=PAWN_WITH_KING/p_cnt[0];
	if(k_cnt[0]>0&&p_cnt[1]>0)
		cnt[1]+=PAWN_WITH_KING/p_cnt[1];
	SCORE B_max;
	if(b_max[B.who]<b_max[B.who^1])
		B_max = +15;
	else if (b_max[B.who] == b_max[B.who^1])
		B_max = 0;
	else
		B_max = -15;
	 
	for(int i=0;i<14;i++)
		cnt[GetColor(FIN(i))]+=(B.cnt[i])*15;
	*/
	//return cnt[B.who]-cnt[B.who^1] + B_max ;
}

// dep=?{?b?b?ĴX?h
// cut=?٭n?A???X?h
SCORE SearchMax(const BOARD &B,int dep,int cut, SCORE alpha, SCORE beta,int X_numm) {
	//fprintf(stderr,"<<3\n");
	if(B.ChkLose())return -WIN;
	//fprintf(stderr,"dep = %d\n",dep);
	int X_num = X_numm;

	MOVLST lst;
	if(cut==0||TimesUp()||B.MoveGen(lst)==0)return +Eval(B);
	//SCORE ret=-INF;
	//fprintf(stderr,">>>>%d,%d\n",X_num,lst.num);
	if(X_num>0)
	{
		lst.mov[lst.num].st = -1;
		lst.mov[lst.num].ed = -1;
		lst.num++;
	}
//fprintf(stderr,"in max: %d %d %lf %lf %d %d\n",dep,cut,alpha,beta,X_numm,lst.num);



	SCORE *stan = (SCORE*)malloc(lst.num*sizeof(SCORE));
	for(int i=0;i<lst.num;i++)
	{
		BOARD N(B);
		N.Move(lst.mov[i]);
		stan[i] = Eval(N);
	}
	for(int i=0;i<lst.num-1;i++)
	{
		for(int j=0;j<lst.num-1-i;j++)
		{
			MOV tmpmov;
			SCORE tmpstan;
			if(stan[i]<stan[j])
			{
				tmpmov = lst.mov[i];
				lst.mov[i] = lst.mov[j];
				lst.mov[j] = tmpmov;

				tmpstan = stan[i];
				stan[i] = stan[j];
				stan[j] = tmpstan;
			}
		}
	}




	FIN EATEN = FIN_P;
	int eflag = 0;
	SCORE ret=alpha;
	//fprintf(stderr,"in max:lst.num = %d\n",lst.num);
	int flag=0;
	for(int i=0;i<lst.num;i++) {
		//fprintf(stderr,"in max: i = %d\n",i);
		//fprintf(stderr,"<<5\n");
		BOARD N(B);
		int eat_flag=0;
		if(lst.mov[i].ed!=-1 && B.fin[lst.mov[i].ed]!=FIN_E&&lst.mov[i].st!=lst.mov[i].ed)eat_flag=1;
		N.Move(lst.mov[i]);

		if(dep==0)
		{
			bool repeat_move = 0;
		    for (int t = game_length-1 ; t >= 0 && t > game_length-8  ; t--) {
				bool repeat_flag = 1;

				for(int m=0;m<32;m++)
					if(N.fin[m] != history[t][m])
						repeat_flag = 0;
				if (repeat_flag == 1) {
				    repeat_move = 1;
				    break;
				}
		    }
		    if(repeat_move==1)continue;	
		}
				






		if(lst.mov[i].ed==-1) X_num--;
		if(dep == 0)
		{
			if(lst.mov[i].ed==-1)
				fprintf(stderr, "NULL move, (%d / %d) \n",i,lst.num);
			else
				fprintf(stderr, "move = (%d, %d)    (%d / %d)\n", lst.mov[i].st,lst.mov[i].ed,i,lst.num);
		}
		const SCORE tmp=SearchMin(N,dep+1,cut-1,ret,beta,X_num);
		if(lst.mov[i].ed==-1) X_num++;
		if(dep == 0)fprintf(stderr,"tmp = %lf\n",tmp);
		//fprintf(stderr,"tmp = %lf, BestMove = (%d, %d)\n",tmp,BestMove.st,BestMove.ed);
		if(tmp>ret)
		{
			

		    ret=tmp;
			if(dep==0)
			{
				if(lst.mov[i].ed==-1)//flip
				{
					//if(X_num==0)fprintf(stderr,"qweqwe\n");
					/*int c=0;
					POS p;
					for(p=0;p<32;p++)if(B.fin[p]==FIN_X)c++;
					if(c!=0)
					{
						c=rand()%c;
						for(p=0;p<32;p++)if(B.fin[p]==FIN_X&&--c<0)break;
						BestMove=MOV(p,p);
						//X_num--;
						
					}*/
					

					BestMove = Flipper(B);
					
				}
				else
				{
					
					BestMove=lst.mov[i];
					flag=1;
				}
					


			}
				
		}
		//fprintf(stderr,">>>>>>>>PP>>>%d %d\n",BestMove.st,BestMove.ed);
		//fprintf(stderr,">>>>>>>>XX>>>(%d %d)\n",lst.mov[i].st,lst.mov[i].ed);

		else if(tmp==ret)
		{

			if(lst.mov[i].ed==-1&&flag==0)//flip
			{
				if(dep==0)
				{
					/*int c=0;
					POS p;
					for(p=0;p<32;p++)if(B.fin[p]==FIN_X)c++;
					if(c!=0)
					{
						c=rand()%c;
						for(p=0;p<32;p++)if(B.fin[p]==FIN_X&&--c<0)break;
						BestMove=MOV(p,p);
						//X_num--;
						
					}*/
					BestMove = Flipper(B);
				}
				
			}
			else if(eat_flag==1)
			{
				/*if(eflag==0)
				{
					if(dep==0)BestMove=lst.mov[i];
					EATEN = B.fin[lst.mov[i].ed];
					eflag=1;

				}*/
				//else
				{
					if(GetLevel(B.fin[lst.mov[i].ed])<=GetLevel(EATEN))
					{
						if(dep==0)
						{
								BestMove=lst.mov[i];
								EATEN = B.fin[lst.mov[i].ed];
						}
					}
				}
				
			}
			/*
			else if(eat_flag==0 && lst.mov[i].ed!=-1&&BestMove.st==lst.mov[i].st)
			{
				if(dep==0)
				{
					int dice=rand()%2;
					if(dice==1)BestMove=lst.mov[i];
				}
				
			}*/
			
			
		}
		//fprintf(stderr,"<<6\n");

		if(ret>=beta){if(dep==0)fprintf(stderr,">>>>>>>>PP>>>(%d %d)\n",BestMove.st,BestMove.ed);return ret-1;}
	}
	if(dep==0)fprintf(stderr,">>>>>>>>PP>>>(%d %d)\n",BestMove.st,BestMove.ed);
	return ret-1;
}

SCORE SearchMin(const BOARD &B,int dep,int cut, SCORE alpha, SCORE beta, int X_numm) {
	//fprintf(stderr,"<<4\n");
	if(B.ChkLose())return +WIN;
	int X_num = X_numm;
	MOVLST lst;
	if(cut==0||TimesUp()||B.MoveGen(lst)==0)return -Eval(B);
	/*int f=0;
	for(int i=0;i<14;i++)
		if(B.cnt[i]>0)f=1;
	if(f==1)
	{
		lst.mov[lst.num].st = -1;
		lst.mov[lst.num].ed = -1;
		lst.num++;
	}*/

	if(X_num>0)
	{
		lst.mov[lst.num].st = -1;
		lst.mov[lst.num].ed = -1;
		lst.num++;
	}
	//fprintf(stderr,"in min: %d %d %lf %lf %d %d\n",dep,cut,alpha,beta,X_numm,lst.num);


	SCORE *stan = (SCORE*)malloc(lst.num*sizeof(SCORE));
	for(int i=0;i<lst.num;i++)
	{
		BOARD N(B);
		N.Move(lst.mov[i]);
		stan[i] = Eval(N);
	}
	for(int i=0;i<lst.num-1;i++)
	{
		for(int j=0;j<lst.num-1-i;j++)
		{
			MOV tmpmov;
			SCORE tmpstan;
			if(stan[i]>stan[j])
			{
				tmpmov = lst.mov[i];
				lst.mov[i] = lst.mov[j];
				lst.mov[j] = tmpmov;

				tmpstan = stan[i];
				stan[i] = stan[j];
				stan[j] = tmpstan;
			}
		}
	}






	//SCORE ret=+INF;
	SCORE ret=beta;
	int flag=0;
	//fprintf(stderr,"in min:lst.num = %d\n",lst.num);
	for(int i=0;i<lst.num;i++) {
		//fprintf(stderr,"in min: i = %d\n",i);
		//fprintf(stderr,"<<7\n");
		BOARD N(B);
		int eat_flag=0;
		if(lst.mov[i].ed != -1&&B.fin[lst.mov[i].ed]!=FIN_E&&lst.mov[i].st!=lst.mov[i].ed)eat_flag=1;
		if(lst.mov[i].ed==-1) X_num--;
		N.Move(lst.mov[i]);
		const SCORE tmp=SearchMax(N,dep+1,cut-1,alpha,ret,X_num);
		if(lst.mov[i].ed==-1) X_num++;

		if(tmp<ret)
		{
			ret=tmp;
			if(lst.mov[i].ed != -1)
			{
				
				flag=1;
				
			}
			//else
				//X_num--;
				
		}
		else if(tmp==ret)
		{
			if(lst.mov[i].ed==-1&&flag==0)//flip
			{
						//X_num--;		
			}
			if(eat_flag==1)
			{
				ret=tmp;
			}
			/*else
			{
				int dice = rand()%2;
				if(dice ==1)
				{
					ret=tmp;
					if(dep==0)
						BestMove=lst.mov[i];
				}
			}*/
		}
		//fprintf(stderr,"<<8\n");
		if(ret<=alpha)return ret-1;
	}
	return ret-1;
}

MOV Play(const BOARD &B) {
#ifdef _WINDOWS
	Tick=GetTickCount();            
	TimeOut = (DEFAULTTIME-3)*1000; 
#else
	Tick=clock();            
	TimeOut = (DEFAULTTIME-3)*CLOCKS_PER_SEC; 
#endif
	POS p; int c=0;
	int X_num=0;
	for(int m=0;m<32;m++)
		history[game_length][m] = B.fin[m];
	game_length++;
	// ?s?C???H?H??½?l
	if(B.who==-1){p=rand()%32;printf("%d\n",p);return MOV(p,p);}
	for(p=0;p<32;p++)
		if(B.fin[p]==FIN_X)X_num++;
	// ?Y?j?X?Ӫ????G?|???{?b?n?N?ηj?X?Ӫ????k
	fprintf(stderr,"now value = %lf\n",Eval(B));
	if(SearchMax(B,0,7,-INF,INF,X_num)>Eval(B)+0.001)
	{
		fprintf(stderr,">>>>>>>>>>>%d %d\n",BestMove.st,BestMove.ed);

		return BestMove;
	}
	//fprintf(stderr,"<<9\n");
	// ?_?h?H?K½?@?Ӧa?? ???p?ߥi???w?g?S?a??½?F
	/*int flip_can[32];
	for(p=0;p<32;p++)if(B.fin[p]==FIN_X)c++;
	if(c==0)return BestMove;
	c=rand()%c;
	// flip!!
	for(p=0;p<32;p++)if(B.fin[p]==FIN_X&&--c<0)break;
	fprintf(stderr,">>>%d %d %d\n",p,MOV(p,p).st,MOV(p,p).ed);
	return MOV(p,p);*/
	BestMove = Flipper(B);
	return BestMove;
}

FIN type2fin(int type) {
    switch(type) {
	case  1: return FIN_K;
	case  2: return FIN_G;
	case  3: return FIN_M;
	case  4: return FIN_R;
	case  5: return FIN_N;
	case  6: return FIN_C;
	case  7: return FIN_P;
	case  9: return FIN_k;
	case 10: return FIN_g;
	case 11: return FIN_m;
	case 12: return FIN_r;
	case 13: return FIN_n;
	case 14: return FIN_c;
	case 15: return FIN_p;
	default: return FIN_E;
    }
}
FIN chess2fin(char chess) {
    switch (chess) {
	case 'K': return FIN_K;
	case 'G': return FIN_G;
	case 'M': return FIN_M;
	case 'R': return FIN_R;
	case 'N': return FIN_N;
	case 'C': return FIN_C;
	case 'P': return FIN_P;
	case 'k': return FIN_k;
	case 'g': return FIN_g;
	case 'm': return FIN_m;
	case 'r': return FIN_r;
	case 'n': return FIN_n;
	case 'c': return FIN_c;
	case 'p': return FIN_p;
	default: return FIN_E;
    }
}

int main(int argc, char* argv[]) {

#ifdef _WINDOWS
	srand(Tick=GetTickCount());
#else
	srand(Tick=time(NULL));
#endif

	BOARD B;
	if (argc!=3) {
	    TimeOut=(B.LoadGame("board.txt")-3)*1000;
	    if(!B.ChkLose())Output(Play(B));
	    return 0;
	}
	Protocol *protocol; 
	protocol = new Protocol();   
	protocol->init_protocol(argv[1],atoi(argv[2]));   
	int iPieceCount[14];
	char iCurrentPosition[32];
	int type, remain_time;
	bool turn;
	PROTO_CLR color;	



	group=(SCORE**)malloc(32*sizeof(SCORE*));
	for(int i=0;i<32;i++)
		group[i] = (SCORE*)malloc(32*sizeof(SCORE)); 

	block =(SCORE**)malloc(32*sizeof(SCORE*));
	for(int i=0;i<32;i++)
		block[i] = (SCORE*)malloc(32*sizeof(SCORE)); 



	char src[3], dst[3], mov[6];
	History moveRecord;
	protocol->init_board(iPieceCount, iCurrentPosition, moveRecord, remain_time);     
	protocol->get_turn(turn,color);           
	
	TimeOut = (DEFAULTTIME-3)*1000;

	B.Init(iCurrentPosition, iPieceCount, (color==2)?(-1):(int)color);
	
	MOV m;
	if(turn) // ?ڥ?
	{
	    m = Play(B);
	    sprintf(src, "%c%c",(m.st%4)+'a', m.st/4+'1');
	    sprintf(dst, "%c%c",(m.ed%4)+'a', m.ed/4+'1');
	    protocol->send(src, dst);         
	    protocol->recv(mov, remain_time);
	    if( color == 2)
		color = protocol->get_color(mov);
	    B.who = color;
	    B.DoMove(m, chess2fin(mov[3]));
	    protocol->recv(mov, remain_time);
	    m.st = mov[0] - 'a' + (mov[1] - '1')*4;
	    m.ed = (mov[2]=='(')?m.st:(mov[3] - 'a' + (mov[4] - '1')*4);
	    B.DoMove(m, chess2fin(mov[3]));
	}
	else // ??????
	{ 
	    protocol->recv(mov, remain_time); 
	    if( color == 2) 
	    {
		color = protocol->get_color(mov);
		B.who = color;
	    }
	    else { 
		B.who = color;
		B.who^=1;
	    }
	    m.st = mov[0] - 'a' + (mov[1] - '1')*4;
	    m.ed = (mov[2]=='(')?m.st:(mov[3] - 'a' + (mov[4] - '1')*4);
	    B.DoMove(m, chess2fin(mov[3]));
	}
	B.Display();
	while(1)
	{  
	    m = Play(B);
		sprintf(src, "%c%c",(m.st%4)+'a', m.st/4+'1');
	    sprintf(dst, "%c%c",(m.ed%4)+'a', m.ed/4+'1');
	    protocol->send(src, dst);    
	    protocol->recv(mov, remain_time);  
	    m.st = mov[0] - 'a' + (mov[1] - '1')*4;
	    m.ed = (mov[2]=='(')?m.st:(mov[3] - 'a' + (mov[4] - '1')*4);
	    B.DoMove(m, chess2fin(mov[3]));
	    B.Display();

	    protocol->recv(mov, remain_time);
	    m.st = mov[0] - 'a' + (mov[1] - '1')*4;
	    m.ed = (mov[2]=='(')?m.st:(mov[3] - 'a' + (mov[4] - '1')*4);
	    B.DoMove(m, chess2fin(mov[3]));
	    B.Display();
	}  

	return 0;
}
