#ifndef __PID_H
#define __PID_H

typedef struct mtPID
{
	float T;//sampling period
	float ref;//value
	float feedBack;
	float KP;//Proportional Const
	float TI;//Integral Const
	float TD;//Derivative Const
	float EK;//Error
	float EK1;//???Error[-1]
	float EK2;//????Error[-2]
	float sumError;//Sums of Errors
	float deltaUK;//???
	float UK;//???
} PID;

void initPID();
void positonPid(PID * p);
void incremePid(PID *p);





#endif