#include "hardware.h"
	
int main()
{    	
	
	   LnitGPIO();
//	   UART4_Lnit(9600);
   	 LnitFTM0_PWM();
	   FTM0_Setspeed(0,5000,0);
	   while(1)
  {		
	   if(Iskeydown()==1)
	  {
			
		   FTM0_Shache();
//      FTM0_Kongzhuan();
////	   LnitFTM2();
//    
//	   LnitPIT(20);
	  }
  }
}
