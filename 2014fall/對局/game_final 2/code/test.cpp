
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
static const SCORE INF=1000001;
static const SCORE WIN=1000000;
SCORE SearchMax(const BOARD&,int,int,SCORE ,SCORE );
SCORE SearchMin(const BOARD&,int,int,SCORE ,SCORE );

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
		if(count==1)
			danger = ChkEats(B.fin[mkpos(x+i,y)],fp);
		if(count==2&&(B.fin[mkpos(x+i,y)]==FIN_C||B.fin[mkpos(x+i,y)]==FIN_c))
			danger = ChkEats(B.fin[mkpos(x+i,y)],fp);

	}
	count = 0;
	for(i=1;x-i>=0;i++)
	{
		if(B.fin[mkpos(x-i,y)]!=FIN_E)
			count++;
		if(count==1)
			danger = ChkEats(B.fin[mkpos(x-i,y)],fp);
		if(count==2&&(B.fin[mkpos(x-i,y)]==FIN_C||B.fin[mkpos(x-i,y)]==FIN_c))
			danger = ChkEats(B.fin[mkpos(x-i,y)],fp);

	}
	count = 0;
	for(i=1;y-i>=0;i++)
	{
		if(B.fin[mkpos(x,y-i)]!=FIN_E)
			count++;
		if(count==1)
			danger = ChkEats(B.fin[mkpos(x,y-i)],fp);
		if(count==2&&(B.fin[mkpos(x,y-i)]==FIN_C||B.fin[mkpos(x,y-i)]==FIN_c))
			danger = ChkEats(B.fin[mkpos(x,y-i)],fp);
	}
	count = 0;
	for(i=1;y+i<4;i++)
	{
		if(B.fin[mkpos(x,y+i)]!=FIN_E)
			count++;
		if(count==1)
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
void flood(const BOARD &B, int ** group, POS p, POS q, int ** block, int depth)
{
	if( p<0||p>31||q<0||q>31||group[p][q] !=INF ||block[p][q]==1)return;
	group[p][q] = depth;
	flood(B,group,p,q+1,block,depth+1);
	flood(B,group,p,q-1,block,depth+1);
	flood(B,group,p,q-4,block,depth+1);
	flood(B,group,p,q+4,block,depth+1);
}
void BuildGraph(const BOARD &B, int ** group, const int ** block)
{
	for(POS p=0;p<32;p++)
	{
		int depth = 0;
		flood(B,group,p,p ,block,depth);
	}
}




int main()
{
	
}

