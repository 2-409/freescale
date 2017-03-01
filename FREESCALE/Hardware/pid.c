#include "pid.h"

PID pid1;
void initPID()
{
	pid1.ref=0;
	pid1.KP=0;
	pid1.TI=0;
	pid1.TD=0;
	pid1.EK=0;
	pid1.EK1=0;
	pid1.EK2=0;
	pid1.sumError=0;
	pid1.deltaUK=0;
	pid1.UK=0;
}

void positonPid(PID * p)
{
	p->EK = p->ref - p->feedBack;
	p->sumError+=p->EK;
	p->UK=p->KP * p->EK//??????????
	    + p->KP * p->sumError*p->T/p->TI//????
	    + p->KP * (p->EK - p->EK1) * p->TD / p->T;//????
	p->EK1=p->EK;
}

void incremePid(PID *p)
{
	p->EK = p->ref - p->feedBack;
	p->deltaUK=p->KP*(p->EK-p->EK1)//??????????
	     +p->KP * p->sumError*p->T/p->TI//????
	     +p->KP * (p->EK - 2*p->EK1 + p->EK2) * p->TD / p->T;
	p->EK2=p->EK1;
	p->EK1=p->EK;
	p->UK+=p->deltaUK;//????????????
}


