#include "hardware.h"

//ϵͳʱ��41.94M
int speed=0;
char spe[8];//��ָ��Ļ��жϵ�ָ�뱨��
//��ʱ����
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


//��ʼ��GPIO��
void LnitGPIO()
{

	SIM->SCGC5|= SIM_SCGC5_PORTA_MASK|SIM_SCGC5_PORTB_MASK|SIM_SCGC5_PORTC_MASK|SIM_SCGC5_PORTD_MASK|SIM_SCGC5_PORTE_MASK;//(0x1f<<9);
	//��abcde�˿ڵ�ʱ��
	PORTA->PCR[14]|=PORT_PCR_MUX(1);//�˿ڹ���1 ��ͨio��
	PORTA->PCR[12]|=PORT_PCR_MUX(1)|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK;//���������裬����Ϊ��������ģʽ
	PTA->PDDR&=~GPIO_PDDR_PDD(0x01<<12);//12�����룬14�����
	PTA->PDDR|=GPIO_PDDR_PDD(0x01<<14)|GPIO_PDDR_PDD(0x01<<6)|GPIO_PDDR_PDD(0x01<<7);
	PTC->PDDR|=GPIO_PDDR_PDD(0x01<<2)|GPIO_PDDR_PDD(0x01<<3);
	PTA->PDOR |=GPIO_PDOR_PDO(0x01<<14);//14�����1
}


//��ʼ��FTM2������������
void LnitFTM2()
{	
	SIM->SCGC3|=SIM_SCGC3_FTM2_MASK;//��FTM2ʱ��
	PORTA->PCR[10]=PORT_PCR_MUX(6);//   �˿ڹ���6 FTM2 ��������
	PORTA->PCR[11]=PORT_PCR_MUX(6); 
	
	FTM2->MOD=65535;//ȱʡ״̬�������ϼ���ģʽ������CPWMS�Ĵ����ı����ģʽ
	FTM2->CNTIN=0;//��ʼֵ
	FTM2->MODE|=FTM_MODE_WPDIS_MASK;	//�ر�д����
	FTM2->SC |=FTM_SC_CLKS(3);//�ⲿʱ����Դ,����ʱ��Դһ���ȹر�д������
	FTM2->MODE|=FTM_MODE_FTMEN_MASK;//ʹ��ftm2
	FTM2->QDCTRL|= FTM_QDCTRL_QUADEN_MASK;//ʹ�ܱ���������
	FTM2->QDCTRL&=~FTM_QDCTRL_QUADMODE_MASK;//����Ϊ����ı�������ʽ
	
}

//��ʼ��ftm0������ftm0��������ֱ�����
//Ƶ��1khz
//ͨ��3  ����
//ͨ��4 
//ͨ��1  ����
//ͨ��2
void LnitFTM0_PWM( )
{
	SIM->SCGC6|=SIM_SCGC6_FTM0_MASK;//��ftm0ʱ��
	PORTA->PCR[6]=PORT_PCR_MUX(3);//A6�˿ڵ�3���ܣ�FTM0 3ͨ��
	PORTA->PCR[7]=PORT_PCR_MUX(3);//A7�˿ڵ�3���ܣFFTM0 4ͨ��
	
	PORTC->PCR[2]=PORT_PCR_MUX(4);//C2�˿ڵ�4���ܣ�FTM0 1ͨ��
	PORTC->PCR[3]=PORT_PCR_MUX(4);//C3�˿ڵ�4���ܣFFTM0 2ͨ��
	
	FTM0->MODE|=FTM_MODE_WPDIS_MASK;//�ر�д����
	FTM0->COMBINE=0;               

	FTM0->CONTROLS[3].CnSC=FTM_CnSC_ELSB_MASK;//����pwmΪ���ض��룬�ߵ�ƽ��Ч��ռ�ձ��ɸߵ�ƽ����
	(*FTM0).CONTROLS[3].CnSC&=~FTM_CnSC_ELSA_MASK;
	(*FTM0).CONTROLS[3].CnSC|=FTM_CnSC_MSB_MASK;
	
	(*FTM0).CONTROLS[4].CnSC|=FTM_CnSC_ELSB_MASK;//����pwmΪ���ض��룬�ߵ�ƽ��Ч��ռ�ձ��ɸߵ�ƽ����
	(*FTM0).CONTROLS[4].CnSC&=~FTM_CnSC_ELSA_MASK;
	(*FTM0).CONTROLS[4].CnSC|=FTM_CnSC_MSB_MASK;

  FTM0->CONTROLS[1].CnSC=FTM_CnSC_ELSB_MASK;//����pwmΪ���ض��룬�ߵ�ƽ��Ч��ռ�ձ��ɸߵ�ƽ����
	(*FTM0).CONTROLS[1].CnSC&=~FTM_CnSC_ELSA_MASK;
	(*FTM0).CONTROLS[1].CnSC|=FTM_CnSC_MSB_MASK;
 
   FTM0->CONTROLS[2] .CnSC=FTM_CnSC_ELSB_MASK;//����pwmΪ���ض��룬�ߵ�ƽ��Ч��ռ�ձ��ɸߵ�ƽ����
	(*FTM0).CONTROLS[2].CnSC&=~FTM_CnSC_ELSA_MASK;
	(*FTM0).CONTROLS[2].CnSC|=FTM_CnSC_MSB_MASK;
	
	FTM0->SC|=FTM_SC_CLKS(1)|FTM_SC_PS(1);//ϵͳʱ�ӷ�Ƶ,PWMƵ��=ϵͳƵ��/2/ps/(mod-cntin+1)
  FTM0->CNTIN=0;//
	FTM0->MOD=24999;
	FTM0->MODE&=~1;//��ftm �ر�dma
	FTM0->OUTMASK=0xe1;
	FTM0->PWMLOAD=0XFF;
	FTM0->QDCTRL&=~FTM_QDCTRL_QUADEN_MASK;//�رձ���
//	(*FTM0).CONTROLS[3].CnV=5000;//����pwmռ�ձȣc�� ��ֵ ���� mod-cntin ��ֵ����ռ�ձȬ
//  (*FTM0).CONTROLS[4].CnV=10000;
   FTM0->CNT=0;
	 

}
//mode:0 ����  1 ����
// speed ��ռ�ձ� 0~25000
//dir��0 ���� 1 ǰ��
void FTM0_Setspeed(char mode,int speed,char dir)
{
	 FTM0->CNT=0;//��0
    if(mode)
		{
			 if(dir)
			 { 
				(*FTM0).CONTROLS[4].CnV=speed;//4ͨ�����pwm 3ͨ������� ����ת
        (*FTM0).CONTROLS[3].CnV=0;
			 }
		    else
				{
				(*FTM0).CONTROLS[3].CnV=speed;//3ͨ�����pwm  4ͨ������� �Ƿ�ת
        (*FTM0).CONTROLS[4].CnV=0;
				}
		}
    else
		{
		
			    if(dir)
			  { 
				  (*FTM0).CONTROLS[2].CnV=speed;//2ͨ�����pwm 1ͨ������� ����ת
          (*FTM0).CONTROLS[1].CnV=0;
			  }
		    else
				 {
				  (*FTM0).CONTROLS[1].CnV=speed;//1ͨ�����pwm  2ͨ������� �Ƿ�ת
          (*FTM0).CONTROLS[2].CnV=0;
				 }
		   	
		}
}

void FTM0_Shache()
{

(*FTM0).CONTROLS[3].CnV=24999;//
(*FTM0).CONTROLS[4].CnV=24999;
(*FTM0).CONTROLS[1].CnV=24999;//3ͨ�����pwm  4ͨ������� �Ƿ�ת
(*FTM0).CONTROLS[2].CnV=24999;

}
void FTM0_Kongzhuan()
{
(*FTM0).CONTROLS[3].CnV=0;//
(*FTM0).CONTROLS[4].CnV=0;
(*FTM0).CONTROLS[1].CnV=0;//3ͨ�����pwm  4ͨ������� �Ƿ�ת
(*FTM0).CONTROLS[2].CnV=0;


}


//��ʼ����ʱ��ģ��
// ����n ��ʱ�ж�ʱ�� ��λ����
void LnitPIT(int n)
{
	SIM->SCGC6|=SIM_SCGC6_PIT_MASK;//(0x01<<23)
	//b23  PIT Clock Gate Control	 ��ʱ��
	PIT->MCR = 0;
	//b1   b0 ��ģ��ʱ�ӣ�����ʱ������
	//MDIS FRZ
	(*PIT).CHANNEL[0].LDVAL=PIT_LDVAL_TSV(40*1000*n);//��ʱ���ж�����,ʱ��Ƶ����40m
	(*PIT).CHANNEL[0].TFLG |=PIT_TFLG_TIF_MASK;//ʱ�ӱ��  д1�����ʱ�ӱ�ǣ�����1˵���Ƕ�ʱ���жϷ���
	(*PIT).CHANNEL[0].TCTRL|=PIT_TCTRL_TEN_MASK|PIT_TCTRL_TIE_MASK;//0x03
	//TIE : Timer Interrupt Enable
	//TEN : Timer Enable       ����pit�������address�
	NVIC_EnableIRQ(PIT0_IRQn);
	//vector number 84
	//IRQ number 68
}





//baud=clock/(16)*
void UART4_Lnit(int baud)
{
	
  PORTC->PCR[15]=PORT_PCR_MUX(3);//C15�˿ڵ�3���ܣUUART4_TX
	PORTC->PCR[14]=PORT_PCR_MUX(3);//A7�˿ڵ�3����   UART4_RX
	SIM->SCGC1|=SIM_SCGC1_UART4_MASK;//��ʱ��
	UART4->C2&=~((1<<2)|(1<<3));//��ֹ��д
	

	UART4->BDH=((41940*1000)/(baud*16))/256;//ȡ��λ
	UART4->BDL=((41940*1000)/(baud*16))%256;//ȡ�Ͱ�λ
	
	UART4->C4&=~(0x1f);//BRFA����
	UART4->C4=UART_C4_BRFA(17);//����У׼λ
	
	UART4->C2|=((1<<2)|(1<<3));

}

//����һ���ֽ�
void UART4_Sendbyte(char Onebyte)
{
      while((UART4->S1& UART_S1_TC_MASK)==0);//�ȴ��������,
      UART4->D=Onebyte;
	
}
//�����ַ���
void UART4_Sendstring(char *str)
{
  while(*str!='\0')//�Ƿ��ͽ���
        {  
            UART4_Sendbyte(*str);  
            str++;  
        }  
}

//��ֵת���ַ�����10����
//����num ��ת������ֵ
//*str ���ַ��� 
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

//SPI0��ʼ��
//3��spi
//mode=1 ��ģʽ
//mode=0 ��ģʽ
void SPI0_lnit( char mode )
{

SIM->SCGC6|=SIM_SCGC6_SPI0_MASK;
PORTC->PCR[4]=PORT_PCR_MUX(0X2)|PORT_PCR_DSE_MASK;//���� ��ǿ  ���豸��������豸����
PORTC->PCR[5]=PORT_PCR_MUX(0X2)|PORT_PCR_DSE_MASK;//���� ��ǿ  ʱ���ź�
PORTC->PCR[6]=PORT_PCR_MUX(0X2);//sin   ���豸���룬���豸���
PORTC->PCR[7]=PORT_PCR_MUX(0X2)|PORT_PCR_DSE_MASK;//���� ��ǿ ���豸ʹ��

SPI0->MCR|=SPI_MCR_CLR_TXF_MASK               //���txfifo��txfifo��rx�������� 
	        |SPI_MCR_CLR_RXF_MASK
	        |SPI_MCR_PCSIS_MASK              //��ʹ�ܴӻ�
	        |SPI_MCR_HALT_MASK;             //�رմ���
SPI0->MCR&=~SPI_MCR_MSTR_MASK;//���mdis��ʹ��dspiʱ�ӡ�
	if(mode==1)//��ģʽ
	{
	 SPI0->MCR|=SPI_MCR_MSTR_MASK;//����ģʽѡ��
	 SPI0->CTAR[0]|=SPI_CTAR_BR(0X6)
		         |SPI_CTAR_PBR(0X2)  //baud=(f/pbr)*[(1+dbr)/br]=156kbps
	           |SPI_CTAR_FMSZ(0x07)//����λ8λ
	           |SPI_CTAR_PCSSCK(2) //ʹ�� 0.2ms ��ʱ����Ч
	           |SPI_CTAR_PDT(2)//��ʱ��ͼ ��Ϊ0.2ms
		         |SPI_CTAR_PASC(2)
		         |SPI_CTAR_CPOL_MASK
		         |SPI_CTAR_CPHA_MASK;
	SPI0->CTAR[0]&=~SPI_CTAR_LSBFE_MASK;//��λ��ǰ
	
	
	
	}
 	else
  {
		SPI0->CTAR_SLAVE[0]|=SPI_CTAR_SLAVE_FMSZ(0X07) //8λ����λ
		                   |SPI_CTAR_SLAVE_CPOL_MASK //sck ����״̬Ϊ�ߵ�ƽ
		                   |SPI_CTAR_SLAVE_CPHA_MASK;//ǰ���ط��� ����ؽ���
	

	}
                SPI0->SR|=SPI_SR_EOQF_MASK//��������
		                   |SPI_SR_TFUF_MASK
		                   |SPI_SR_TFFF_MASK
		                   |SPI_SR_RFDF_MASK
		                   |SPI_SR_RFOF_MASK;

}


void SPI0_Sendbyte(char onebyte)
{

SPI0->SR|=SPI_SR_TCF_MASK;//����������
SPI0->PUSHR|=SPI_PUSHR_CTAS(0)//ѡ��ctar0
	         |SPI_PUSHR_EOQ_MASK//���һ֡
            |SPI_PUSHR_PCS(1)//pcs0 ��Ч
            |SPI_PUSHR_TXDATA(onebyte);//д������
	
SPI0->MCR&=~SPI_MCR_HALT_MASK;//����SPI0
	while((SPI0->SR&SPI_SR_TCF_MASK)==0);
	SPI0->SR|=SPI_SR_TCF_MASK;//����������
//	SPI0->POPR �յ�����

}
char SPI0_Receivebyte()
{

while((SPI0->SR&SPI_SR_RFDF_MASK)==0);//û���յ��͵ȴ�

 return SPI0->POPR;



}












//��ʱ���ж�

void PIT0_IRQHandler()
{ 
	
	   
		(*PIT).CHANNEL[0].TFLG |=PIT_TFLG_TIF_MASK;//������λ
		speed=(FTM2->CNT);	//500�����ٶ�
	  itoa(speed,spe);
	UART4_Sendstring("�ٶ�");
	UART4_Sendstring(spe);
	UART4_Sendstring("\r\n");
		FTM2->CNT=0;
		PTA->PTOR|=(0x01<<14);
	
}
