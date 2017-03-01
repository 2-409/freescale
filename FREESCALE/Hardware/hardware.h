#ifndef __HARDWARE_H
#define __HARDWARE_H

#include "MK60D10.h"
#include  "arm_math.h"//dsp



void LnitGPIO();
void LnitFTM2();
void LnitFTM0_PWM( );
void LnitPIT(int n);
//void PIT0_IRQHandler();
void delay(void);
int Iskeydown();
void UART4_Lnit(int baud);
void UART4_Sendbyte(char Onebyte);
void UART4_Sendstring(char *str);
void itoa(int num,char *str);//ÊýÖµ×ª×Ö·û´®
void SPI0_lnit(char mode );
void SPI0_Sendbyte(char onebyte);
char SPI0_Receivebyte();
void FTM0_Setspeed(char mode,int speed,char dir);
void FTM0_Shache();
void FTM0_Kongzhuan();
#endif