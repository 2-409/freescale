#include "hardware.h"

//ÏµÍ³Ê±ÖÓ41.94M
int speed=0;
char spe[8];//ÓÃÖ¸ÕëµÄ»°ÖÐ¶Ïµ÷Ö¸Õë±¨´í
//ÑÓÊ±º¯Êý
void delay(void)
{
	
	int i;
	for(i=0;i<1000;i++)
	{
		;
	}
}

int Iskeydown()
{
   if ((PTA->PDIR&1<<12)==0) 
	 {
		    delay();
		 if ((PTA->PDIR&1<<12)==0) 
		 {
	   return 1;
		 }
	 }
  else
		return 0;

}


//³õÊ¼»¯GPIO¿Ú
void LnitGPIO()
{

	SIM->SCGC5|= SIM_SCGC5_PORTA_MASK|SIM_SCGC5_PORTB_MASK|SIM_SCGC5_PORTC_MASK|SIM_SCGC5_PORTD_MASK|SIM_SCGC5_PORTE_MASK;//(0x1f<<9);
	//´ò¿ªabcde¶Ë¿ÚµÄÊ±ÖÓ
	PORTA->PCR[14]|=PORT_PCR_MUX(1);//¶Ë¿Ú¹¦ÄÜ1 ÆÕÍ¨io¿Ú
	PORTA->PCR[12]|=PORT_PCR_MUX(1)|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK;//´ò¿ªÉÏÀ­µç×è£¬ÉèÖÃÎªÉÏÀ­ÊäÈëÄ£Ê½
	PTA->PDDR&=~GPIO_PDDR_PDD(0x01<<12);//12¿ÚÊäÈë£¬14¿ÚÊä³ö
	PTA->PDDR|=GPIO_PDDR_PDD(0x01<<14)|GPIO_PDDR_PDD(0x01<<6)|GPIO_PDDR_PDD(0x01<<7);
	PTC->PDDR|=GPIO_PDDR_PDD(0x01<<2)|GPIO_PDDR_PDD(0x01<<3);
	PTA->PDOR |=GPIO_PDOR_PDO(0x01<<14);//14¿ÚÊä³ö1
}


//³õÊ¼»¯FTM2£¬±àÂëÆ÷¹¦ÄÜ
void LnitFTM2()
{	
	SIM->SCGC3|=SIM_SCGC3_FTM2_MASK;//´ò¿ªFTM2Ê±ÖÓ
	PORTA->PCR[10]=PORT_PCR_MUX(6);//   ¶Ë¿Ú¹¦ÄÜ6 FTM2 Á½ÏàÊäÈë
	PORTA->PCR[11]=PORT_PCR_MUX(6); 
	
	FTM2->MOD=65535;//È±Ê¡×´Ì¬ÏÂÊÇÏòÉÏ¼ÆÊýÄ£Ê½£¬ÉèÖÃCPWMS¼Ä´æÆ÷¸Ä±ä¼ÆÊýÄ£Ê½
	FTM2->CNTIN=0;//³õÊ¼Öµ
	FTM2->MODE|=FTM_MODE_WPDIS_MASK;	//¹Ø±ÕÐ´±£»¤
	FTM2->SC |=FTM_SC_CLKS(3);//Íâ²¿Ê±ÖÓÀ´Ô´,ÉèÖÃÊ±ÖÓÔ´Ò»¶¨ÏÈ¹Ø±ÕÐ´±£»¤¡£
	FTM2->MODE|=FTM_MODE_FTMEN_MASK;//Ê¹ÄÜftm2
	FTM2->QDCTRL|= FTM_QDCTRL_QUADEN_MASK;//Ê¹ÄÜ±àÂëÆ÷¹¦ÄÜ
	FTM2->QDCTRL&=~FTM_QDCTRL_QUADMODE_MASK;//ÉèÖÃÎªÁ½ÏàµÄ±àÂëÆ÷¸ñÊ½
	
}

//³õÊ¼»¯ftm0£¬ÉèÖÃftm0¿ØÖÆÁ½¸öÖ±Á÷µç»ú
//ÆµÂÊ1khz
//Í¨µÀ3  ÓÒÂÖ
//Í¨µÀ4 
//Í¨µÀ1  ×óÂÖ
//Í¨µÀ2
void LnitFTM0_PWM( )
{
	SIM->SCGC6|=SIM_SCGC6_FTM0_MASK;//¿ªftm0Ê±ÖÓ
	PORTA->PCR[6]=PORT_PCR_MUX(3);//A6¶Ë¿ÚµÚ3¹¦ÄÜ£¬FTM0 3Í¨µÀ
	PORTA->PCR[7]=PORT_PCR_MUX(3);//A7¶Ë¿ÚµÚ3¹¦ÄÜ£FFTM0 4Í¨µÀ
	
	PORTC->PCR[2]=PORT_PCR_MUX(4);//C2¶Ë¿ÚµÚ4¹¦ÄÜ£¬FTM0 1Í¨µÀ
	PORTC->PCR[3]=PORT_PCR_MUX(4);//C3¶Ë¿ÚµÚ4¹¦ÄÜ£FFTM0 2Í¨µÀ
	
	FTM0->MODE|=FTM_MODE_WPDIS_MASK;//¹Ø±ÕÐ´±£»¤
	FTM0->COMBINE=0;               

	FTM0->CONTROLS[3].CnSC=FTM_CnSC_ELSB_MASK;//ÉèÖÃpwmÎª±ßÑØ¶ÔÆë£¬¸ßµçÆ½ÓÐÐ§£¬Õ¼¿Õ±ÈÓÉ¸ßµçÆ½À´Ëã
	(*FTM0).CONTROLS[3].CnSC&=~FTM_CnSC_ELSA_MASK;
	(*FTM0).CONTROLS[3].CnSC|=FTM_CnSC_MSB_MASK;
	
	(*FTM0).CONTROLS[4].CnSC|=FTM_CnSC_ELSB_MASK;//ÉèÖÃpwmÎª±ßÑØ¶ÔÆë£¬¸ßµçÆ½ÓÐÐ§£¬Õ¼¿Õ±ÈÓÉ¸ßµçÆ½À´Ëã
	(*FTM0).CONTROLS[4].CnSC&=~FTM_CnSC_ELSA_MASK;
	(*FTM0).CONTROLS[4].CnSC|=FTM_CnSC_MSB_MASK;

  FTM0->CONTROLS[1].CnSC=FTM_CnSC_ELSB_MASK;//ÉèÖÃpwmÎª±ßÑØ¶ÔÆë£¬¸ßµçÆ½ÓÐÐ§£¬Õ¼¿Õ±ÈÓÉ¸ßµçÆ½À´Ëã
	(*FTM0).CONTROLS[1].CnSC&=~FTM_CnSC_ELSA_MASK;
	(*FTM0).CONTROLS[1].CnSC|=FTM_CnSC_MSB_MASK;
 
   FTM0->CONTROLS[2] .CnSC=FTM_CnSC_ELSB_MASK;//ÉèÖÃpwmÎª±ßÑØ¶ÔÆë£¬¸ßµçÆ½ÓÐÐ§£¬Õ¼¿Õ±ÈÓÉ¸ßµçÆ½À´Ëã
	(*FTM0).CONTROLS[2].CnSC&=~FTM_CnSC_ELSA_MASK;
	(*FTM0).CONTROLS[2].CnSC|=FTM_CnSC_MSB_MASK;
	
	FTM0->SC|=FTM_SC_CLKS(1)|FTM_SC_PS(1);//ÏµÍ³Ê±ÖÓ·ÖÆµ,PWMÆµÂÊ=ÏµÍ³ÆµÂÊ/2/ps/(mod-cntin+1)
  FTM0->CNTIN=0;//
	FTM0->MOD=24999;
	FTM0->MODE&=~1;//´ò¿ªftm ¹Ø±Õdma
	FTM0->OUTMASK=0xe1;
	FTM0->PWMLOAD=0XFF;
	FTM0->QDCTRL&=~FTM_QDCTRL_QUADEN_MASK;//¹Ø±Õ±àÂë
//	(*FTM0).CONTROLS[3].CnV=5000;//ÉèÖÃpwmÕ¼¿Õ±È£cÕâ ¸öÖµ ³ýÒÔ mod-cntin µÄÖµ¾ÍÊÇÕ¼¿Õ±È¬
//  (*FTM0).CONTROLS[4].CnV=10000;
   FTM0->CNT=0;
	 

}
//mode:0 ×óÂÖ  1 ÓÒÂÖ
// speed £ºÕ¼¿Õ±È 0~25000
//dir£º0 µ¹ÍË 1 Ç°½ø
void FTM0_Setspeed(char mode,int speed,char dir)
{
	 FTM0->CNT=0;//Çå0
    if(mode)
		{
			 if(dir)
			 { 
				(*FTM0).CONTROLS[4].CnV=speed;//4Í¨µÀÊä³öpwm 3Í¨µÀÊä³öµÍ ÊÇÕý×ª
        (*FTM0).CONTROLS[3].CnV=0;
			 }
		    else
				{
				(*FTM0).CONTROLS[3].CnV=speed;//3Í¨µÀÊä³öpwm  4Í¨µÀÊä³öµÍ ÊÇ·´×ª
        (*FTM0).CONTROLS[4].CnV=0;
				}
		}
    else
		{
		
			    if(dir)
			  { 
				  (*FTM0).CONTROLS[2].CnV=speed;//2Í¨µÀÊä³öpwm 1Í¨µÀÊä³öµÍ ÊÇÕý×ª
          (*FTM0).CONTROLS[1].CnV=0;
			  }
		    else
				 {
				  (*FTM0).CONTROLS[1].CnV=speed;//1Í¨µÀÊä³öpwm  2Í¨µÀÊä³öµÍ ÊÇ·´×ª
          (*FTM0).CONTROLS[2].CnV=0;
				 }
		   	
		}
}

void FTM0_Shache()
{

(*FTM0).CONTROLS[3].CnV=24999;//
(*FTM0).CONTROLS[4].CnV=24999;
(*FTM0).CONTROLS[1].CnV=24999;//3Í¨µÀÊä³öpwm  4Í¨µÀÊä³öµÍ ÊÇ·´×ª
(*FTM0).CONTROLS[2].CnV=24999;

}
void FTM0_Kongzhuan()
{
(*FTM0).CONTROLS[3].CnV=0;//
(*FTM0).CONTROLS[4].CnV=0;
(*FTM0).CONTROLS[1].CnV=0;//3Í¨µÀÊä³öpwm  4Í¨µÀÊä³öµÍ ÊÇ·´×ª
(*FTM0).CONTROLS[2].CnV=0;


}


//³õÊ¼»¯¶¨Ê±Æ÷Ä£¿é
// ²ÎÊýn ¶¨Ê±ÖÐ¶ÏÊ±¼ä µ¥Î»ºÁÃë
void LnitPIT(int n)
{
	SIM->SCGC6|=SIM_SCGC6_PIT_MASK;//(0x01<<23)
	//b23  PIT Clock Gate Control	 ´ò¿ªÊ±ÖÓ
	PIT->MCR = 0;
	//b1   b0 ´ò¿ªÄ£¿éÊ±ÖÓ£¬µ÷ÊÔÊ±¿ÉÒÔÅÜ
	//MDIS FRZ
	(*PIT).CHANNEL[0].LDVAL=PIT_LDVAL_TSV(40*1000*n);//¶¨Ê±Æ÷ÖÐ¶ÏÖÜÆÚ,Ê±ÖÓÆµÂÊÊÇ40m
	(*PIT).CHANNEL[0].TFLG |=PIT_TFLG_TIF_MASK;//Ê±ÖÓ±ê¼Ç  Ð´1ÊÇÇå³ýÊ±ÖÓ±ê¼Ç£¬¶Áµ½1ËµÃ÷ÊÇ¶¨Ê±Æ÷ÖÐ¶Ï·¢Éú
	(*PIT).CHANNEL[0].TCTRL|=PIT_TCTRL_TEN_MASK|PIT_TCTRL_TIE_MASK;//0x03
	//TIE : Timer Interrupt Enable
	//TEN : Timer Enable       ÕâÀïpit¶¨ÒåµÄÊÇaddressÇ
	NVIC_EnableIRQ(PIT0_IRQn);
	//vector number 84
	//IRQ number 68
}





//baud=clock/(16)*
void UART4_Lnit(int baud)
{
	
  PORTC->PCR[15]=PORT_PCR_MUX(3);//C15¶Ë¿ÚµÚ3¹¦ÄÜ£UUART4_TX
	PORTC->PCR[14]=PORT_PCR_MUX(3);//A7¶Ë¿ÚµÚ3¹¦ÄÜ   UART4_RX
	SIM->SCGC1|=SIM_SCGC1_UART4_MASK;//¿ªÊ±ÖÓ
	UART4->C2&=~((1<<2)|(1<<3));//½ûÖ¹¶ÁÐ´
	

	UART4->BDH=((41940*1000)/(baud*16))/256;//È¡¸ßÎ»
	UART4->BDL=((41940*1000)/(baud*16))%256;//È¡µÍ°ËÎ»
	
	UART4->C4&=~(0x1f);//BRFAÇåÁã
	UART4->C4=UART_C4_BRFA(17);//ÉèÖÃÐ£×¼Î»
	
	UART4->C2|=((1<<2)|(1<<3));

}

//·¢ËÍÒ»¸ö×Ö½Ú
void UART4_Sendbyte(char Onebyte)
{
      while((UART4->S1& UART_S1_TC_MASK)==0);//µÈ´ý·¢ËÍÍê³É,
      UART4->D=Onebyte;
	
}
//·¢ËÍ×Ö·û´®
void UART4_Sendstring(char *str)
{
  while(*str!='\0')//ÊÇ·ñ·¢ËÍ½áÊø
        {  
            UART4_Sendbyte(*str);  
            str++;  
        }  
}

//ÊýÖµ×ª»»×Ö·û´®£¨10½øÖÆ
//²ÎÊýnum ±»×ª»»µÄÊýÖµ
//*str ´æ×Ö·û´® 
void itoa(int num,char *str)
{

char index[]="0123456789";
unsigned unum;
	char temp;
int i=0,j,k;
	if(num<0)
	{
	  unum=-num;
		str[i++]='-';
	
	}
 else unum=(unsigned)num;

 do{
 str[i++]=index[unum%10];
	 unum/=10;
 }while(unum);
 str[i]='\0';
 if(str[0]=='-')k=1;
 else k=0;
 
for(j=k;j<=(i-1)/2;j++)
 {
   temp=str[j];
	 str[j]=str[i-1+k-j];
	 str[i-1+k-j]=temp;
 
 }

}

//SPI0³õÊ¼»¯
//3¸öspi
//mode=1 Ö÷Ä£Ê½
//mode=0 ´ÓÄ£Ê½
void SPI0_lnit( char mode )
{

SIM->SCGC6|=SIM_SCGC6_SPI0_MASK;
PORTC->PCR[4]=PORT_PCR_MUX(0X2)|PORT_PCR_DSE_MASK;//Çý¶¯ ÔöÇ¿  Ö÷Éè±¸Êä³ö£¬´ÓÉè±¸ÊäÈë
PORTC->PCR[5]=PORT_PCR_MUX(0X2)|PORT_PCR_DSE_MASK;//Çý¶¯ ÔöÇ¿  Ê±ÖÓÐÅºÅ
PORTC->PCR[6]=PORT_PCR_MUX(0X2);//sin   Ö÷Éè±¸ÊäÈë£¬´ÓÉè±¸Êä³ö
PORTC->PCR[7]=PORT_PCR_MUX(0X2)|PORT_PCR_DSE_MASK;//Çý¶¯ ÔöÇ¿ ´ÓÉè±¸Ê¹ÄÜ

SPI0->MCR|=SPI_MCR_CLR_TXF_MASK               //Çå¿ÕtxfifoºÍtxfifo£¨rx£©¼ÆÊýÆ÷ 
	        |SPI_MCR_CLR_RXF_MASK
	        |SPI_MCR_PCSIS_MASK              //²»Ê¹ÄÜ´Ó»ú
	        |SPI_MCR_HALT_MASK;             //¹Ø±Õ´«Êä
SPI0->MCR&=~SPI_MCR_MSTR_MASK;//Çå³ýmdis£¬Ê¹ÄÜdspiÊ±ÖÓ¡£
	if(mode==1)//Ö÷Ä£Ê½
	{
	 SPI0->MCR|=SPI_MCR_MSTR_MASK;//Ö÷»úÄ£Ê½Ñ¡Ôñ
	 SPI0->CTAR[0]|=SPI_CTAR_BR(0X6)
		         |SPI_CTAR_PBR(0X2)  //baud=(f/pbr)*[(1+dbr)/br]=156kbps
	           |SPI_CTAR_FMSZ(0x07)//Êý¾ÝÎ»8Î»
	           |SPI_CTAR_PCSSCK(2) //Ê¹ÄÜ 0.2ms ºóÊ±ÖÓÓÐÐ§
	           |SPI_CTAR_PDT(2)//¼ûÊ±ÐòÍ¼ ¶¼Îª0.2ms
		         |SPI_CTAR_PASC(2)
		         |SPI_CTAR_CPOL_MASK
		         |SPI_CTAR_CPHA_MASK;
	SPI0->CTAR[0]&=~SPI_CTAR_LSBFE_MASK;//¸ßÎ»ÔÚÇ°
	
	
	
	}
 	else
  {
		SPI0->CTAR_SLAVE[0]|=SPI_CTAR_SLAVE_FMSZ(0X07) //8Î»Êý¾ÝÎ»
		                   |SPI_CTAR_SLAVE_CPOL_MASK //sck ¿ÕÏÐ×´Ì¬Îª¸ßµçÆ½
		                   |SPI_CTAR_SLAVE_CPHA_MASK;//Ç°±ßÑØ·¢ËÍ ºó±ßÑØ½ÓÊÕ
	

	}
                SPI0->SR|=SPI_SR_EOQF_MASK//¸÷ÖÖÇåÁã
		                   |SPI_SR_TFUF_MASK
		                   |SPI_SR_TFFF_MASK
		                   |SPI_SR_RFDF_MASK
		                   |SPI_SR_RFOF_MASK;

}


void SPI0_Sendbyte(char onebyte)
{

SPI0->SR|=SPI_SR_TCF_MASK;//Çå³ý·¢ËÍÍê³É
SPI0->PUSHR|=SPI_PUSHR_CTAS(0)//Ñ¡Ôñctar0
	         |SPI_PUSHR_EOQ_MASK//×îºóÒ»Ö¡
            |SPI_PUSHR_PCS(1)//pcs0 ÓÐÐ§
            |SPI_PUSHR_TXDATA(onebyte);//Ð´ÈçÊý¾Ý
	
SPI0->MCR&=~SPI_MCR_HALT_MASK;//Æô¶¯SPI0
	while((SPI0->SR&SPI_SR_TCF_MASK)==0);
	SPI0->SR|=SPI_SR_TCF_MASK;//Çå³ý·¢ËÍÍê³É
//	SPI0->POPR ÊÕµ½µÄÊý

}
char SPI0_Receivebyte()
{

while((SPI0->SR&SPI_SR_RFDF_MASK)==0);//Ã»ÓÐÊÕµ½¾ÍµÈ´ý

 return SPI0->POPR;



}












//¶¨Ê±Æ÷ÖÐ¶Ï

void PIT0_IRQHandler()
{ 
	
	   
		(*PIT).CHANNEL[0].TFLG |=PIT_TFLG_TIF_MASK;//Çå³ý±ê¼ÇÎ»
		speed=(FTM2->CNT);	//500ÏßËãËÙ¶È
	  itoa(speed,spe);
	UART4_Sendstring("ËÙ¶È");
	UART4_Sendstring(spe);
	UART4_Sendstring("\r\n");
		FTM2->CNT=0;
		PTA->PTOR|=(0x01<<14);
	
}
