#include "hardware.h"

//系统时钟41.94M
int speed=0;
char spe[8];//用指针的话中断调指针报错
//延时函数
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


//初始化GPIO口
void LnitGPIO()
{

	SIM->SCGC5|= SIM_SCGC5_PORTA_MASK|SIM_SCGC5_PORTB_MASK|SIM_SCGC5_PORTC_MASK|SIM_SCGC5_PORTD_MASK|SIM_SCGC5_PORTE_MASK;//(0x1f<<9);
	//打开abcde端口的时钟
	PORTA->PCR[14]|=PORT_PCR_MUX(1);//端口功能1 普通io口
	PORTA->PCR[12]|=PORT_PCR_MUX(1)|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK;//打开上拉电阻，设置为上拉输入模式
	PTA->PDDR&=~GPIO_PDDR_PDD(0x01<<12);//12口输入，14口输出
	PTA->PDDR|=GPIO_PDDR_PDD(0x01<<14)|GPIO_PDDR_PDD(0x01<<6)|GPIO_PDDR_PDD(0x01<<7);
	PTC->PDDR|=GPIO_PDDR_PDD(0x01<<2)|GPIO_PDDR_PDD(0x01<<3);
	PTA->PDOR |=GPIO_PDOR_PDO(0x01<<14);//14口输出1
}


//初始化FTM2，编码器功能
void LnitFTM2()
{	
	SIM->SCGC3|=SIM_SCGC3_FTM2_MASK;//打开FTM2时钟
	PORTA->PCR[10]=PORT_PCR_MUX(6);//   端口功能6 FTM2 两相输入
	PORTA->PCR[11]=PORT_PCR_MUX(6); 
	
	FTM2->MOD=65535;//缺省状态下是向上计数模式，设置CPWMS寄存器改变计数模式
	FTM2->CNTIN=0;//初始值
	FTM2->MODE|=FTM_MODE_WPDIS_MASK;	//关闭写保护
	FTM2->SC |=FTM_SC_CLKS(3);//外部时钟来源,设置时钟源一定先关闭写保护。
	FTM2->MODE|=FTM_MODE_FTMEN_MASK;//使能ftm2
	FTM2->QDCTRL|= FTM_QDCTRL_QUADEN_MASK;//使能编码器功能
	FTM2->QDCTRL&=~FTM_QDCTRL_QUADMODE_MASK;//设置为两相的编码器格式
	
}

//初始化ftm0，设置ftm0控制两个直流电机
//频率1khz
//通道3  右轮
//通道4 
//通道1  左轮
//通道2
void LnitFTM0_PWM( )
{
	SIM->SCGC6|=SIM_SCGC6_FTM0_MASK;//开ftm0时钟
	PORTA->PCR[6]=PORT_PCR_MUX(3);//A6端口第3功能，FTM0 3通道
	PORTA->PCR[7]=PORT_PCR_MUX(3);//A7端口第3功能FTM0 4通道
	
	PORTC->PCR[2]=PORT_PCR_MUX(4);//C2端口第4功能，FTM0 1通道
	PORTC->PCR[3]=PORT_PCR_MUX(4);//C3端口第4功能FTM0 2通道
	
	FTM0->MODE|=FTM_MODE_WPDIS_MASK;//关闭写保护
	FTM0->COMBINE=0;               

	FTM0->CONTROLS[3].CnSC=FTM_CnSC_ELSB_MASK;//设置pwm为边沿对齐，高电平有效，占空比由高电平来算
	(*FTM0).CONTROLS[3].CnSC&=~FTM_CnSC_ELSA_MASK;
	(*FTM0).CONTROLS[3].CnSC|=FTM_CnSC_MSB_MASK;
	
	(*FTM0).CONTROLS[4].CnSC|=FTM_CnSC_ELSB_MASK;//设置pwm为边沿对齐，高电平有效，占空比由高电平来算
	(*FTM0).CONTROLS[4].CnSC&=~FTM_CnSC_ELSA_MASK;
	(*FTM0).CONTROLS[4].CnSC|=FTM_CnSC_MSB_MASK;

  FTM0->CONTROLS[1].CnSC=FTM_CnSC_ELSB_MASK;//设置pwm为边沿对齐，高电平有效，占空比由高电平来算
	(*FTM0).CONTROLS[1].CnSC&=~FTM_CnSC_ELSA_MASK;
	(*FTM0).CONTROLS[1].CnSC|=FTM_CnSC_MSB_MASK;
 
   FTM0->CONTROLS[2] .CnSC=FTM_CnSC_ELSB_MASK;//设置pwm为边沿对齐，高电平有效，占空比由高电平来算
	(*FTM0).CONTROLS[2].CnSC&=~FTM_CnSC_ELSA_MASK;
	(*FTM0).CONTROLS[2].CnSC|=FTM_CnSC_MSB_MASK;
	
	FTM0->SC|=FTM_SC_CLKS(1)|FTM_SC_PS(1);//系统时钟分频,PWM频率=系统频率/2/ps/(mod-cntin+1)
  FTM0->CNTIN=0;//
	FTM0->MOD=24999;
	FTM0->MODE&=~1;//打开ftm 关闭dma
	FTM0->OUTMASK=0xe1;
	FTM0->PWMLOAD=0XFF;
	FTM0->QDCTRL&=~FTM_QDCTRL_QUADEN_MASK;//关闭编码
//	(*FTM0).CONTROLS[3].CnV=5000;//设置pwm占空比这 个值 除以 mod-cntin 的值就是占空比�
//  (*FTM0).CONTROLS[4].CnV=10000;
   FTM0->CNT=0;
	 

}
//mode:0 左轮  1 右轮
// speed ：占空比 0~25000
//dir：0 倒退 1 前进
void FTM0_Setspeed(char mode,int speed,char dir)
{
	 FTM0->CNT=0;//清0
    if(mode)
		{
			 if(dir)
			 { 
				(*FTM0).CONTROLS[4].CnV=speed;//4通道输出pwm 3通道输出低 是正转
        (*FTM0).CONTROLS[3].CnV=0;
			 }
		    else
				{
				(*FTM0).CONTROLS[3].CnV=speed;//3通道输出pwm  4通道输出低 是反转
        (*FTM0).CONTROLS[4].CnV=0;
				}
		}
    else
		{
		
			    if(dir)
			  { 
				  (*FTM0).CONTROLS[2].CnV=speed;//2通道输出pwm 1通道输出低 是正转
          (*FTM0).CONTROLS[1].CnV=0;
			  }
		    else
				 {
				  (*FTM0).CONTROLS[1].CnV=speed;//1通道输出pwm  2通道输出低 是反转
          (*FTM0).CONTROLS[2].CnV=0;
				 }
		   	
		}
}

void FTM0_Shache()
{

(*FTM0).CONTROLS[3].CnV=24999;//
(*FTM0).CONTROLS[4].CnV=24999;
(*FTM0).CONTROLS[1].CnV=24999;//3通道输出pwm  4通道输出低 是反转
(*FTM0).CONTROLS[2].CnV=24999;

}
void FTM0_Kongzhuan()
{
(*FTM0).CONTROLS[3].CnV=0;//
(*FTM0).CONTROLS[4].CnV=0;
(*FTM0).CONTROLS[1].CnV=0;//3通道输出pwm  4通道输出低 是反转
(*FTM0).CONTROLS[2].CnV=0;


}


//初始化定时器模块
// 参数n 定时中断时间 单位毫秒
void LnitPIT(int n)
{
	SIM->SCGC6|=SIM_SCGC6_PIT_MASK;//(0x01<<23)
	//b23  PIT Clock Gate Control	 打开时钟
	PIT->MCR = 0;
	//b1   b0 打开模块时钟，调试时可以跑
	//MDIS FRZ
	(*PIT).CHANNEL[0].LDVAL=PIT_LDVAL_TSV(40*1000*n);//定时器中断周期,时钟频率是40m
	(*PIT).CHANNEL[0].TFLG |=PIT_TFLG_TIF_MASK;//时钟标记  写1是清除时钟标记，读到1说明是定时器中断发生
	(*PIT).CHANNEL[0].TCTRL|=PIT_TCTRL_TEN_MASK|PIT_TCTRL_TIE_MASK;//0x03
	//TIE : Timer Interrupt Enable
	//TEN : Timer Enable       这里pit定义的是address�
	NVIC_EnableIRQ(PIT0_IRQn);
	//vector number 84
	//IRQ number 68
}





//baud=clock/(16)*
void UART4_Lnit(int baud)
{
	
  PORTC->PCR[15]=PORT_PCR_MUX(3);//C15端口第3功能UART4_TX
	PORTC->PCR[14]=PORT_PCR_MUX(3);//A7端口第3功能   UART4_RX
	SIM->SCGC1|=SIM_SCGC1_UART4_MASK;//开时钟
	UART4->C2&=~((1<<2)|(1<<3));//禁止读写
	

	UART4->BDH=((41940*1000)/(baud*16))/256;//取高位
	UART4->BDL=((41940*1000)/(baud*16))%256;//取低八位
	
	UART4->C4&=~(0x1f);//BRFA清零
	UART4->C4=UART_C4_BRFA(17);//设置校准位
	
	UART4->C2|=((1<<2)|(1<<3));

}

//发送一个字节
void UART4_Sendbyte(char Onebyte)
{
      while((UART4->S1& UART_S1_TC_MASK)==0);//等待发送完成,
      UART4->D=Onebyte;
	
}
//发送字符串
void UART4_Sendstring(char *str)
{
  while(*str!='\0')//是否发送结束
        {  
            UART4_Sendbyte(*str);  
            str++;  
        }  
}

//数值转换字符串（10进制
//参数num 被转换的数值
//*str 存字符串 
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

//SPI0初始化
//3个spi
//mode=1 主模式
//mode=0 从模式
void SPI0_lnit( char mode )
{

SIM->SCGC6|=SIM_SCGC6_SPI0_MASK;
PORTC->PCR[4]=PORT_PCR_MUX(0X2)|PORT_PCR_DSE_MASK;//驱动 增强  主设备输出，从设备输入
PORTC->PCR[5]=PORT_PCR_MUX(0X2)|PORT_PCR_DSE_MASK;//驱动 增强  时钟信号
PORTC->PCR[6]=PORT_PCR_MUX(0X2);//sin   主设备输入，从设备输出
PORTC->PCR[7]=PORT_PCR_MUX(0X2)|PORT_PCR_DSE_MASK;//驱动 增强 从设备使能

SPI0->MCR|=SPI_MCR_CLR_TXF_MASK               //清空txfifo和txfifo（rx）计数器 
	        |SPI_MCR_CLR_RXF_MASK
	        |SPI_MCR_PCSIS_MASK              //不使能从机
	        |SPI_MCR_HALT_MASK;             //关闭传输
SPI0->MCR&=~SPI_MCR_MSTR_MASK;//清除mdis，使能dspi时钟。
	if(mode==1)//主模式
	{
	 SPI0->MCR|=SPI_MCR_MSTR_MASK;//主机模式选择
	 SPI0->CTAR[0]|=SPI_CTAR_BR(0X6)
		         |SPI_CTAR_PBR(0X2)  //baud=(f/pbr)*[(1+dbr)/br]=156kbps
	           |SPI_CTAR_FMSZ(0x07)//数据位8位
	           |SPI_CTAR_PCSSCK(2) //使能 0.2ms 后时钟有效
	           |SPI_CTAR_PDT(2)//见时序图 都为0.2ms
		         |SPI_CTAR_PASC(2)
		         |SPI_CTAR_CPOL_MASK
		         |SPI_CTAR_CPHA_MASK;
	SPI0->CTAR[0]&=~SPI_CTAR_LSBFE_MASK;//高位在前
	
	
	
	}
 	else
  {
		SPI0->CTAR_SLAVE[0]|=SPI_CTAR_SLAVE_FMSZ(0X07) //8位数据位
		                   |SPI_CTAR_SLAVE_CPOL_MASK //sck 空闲状态为高电平
		                   |SPI_CTAR_SLAVE_CPHA_MASK;//前边沿发送 后边沿接收
	

	}
                SPI0->SR|=SPI_SR_EOQF_MASK//各种清零
		                   |SPI_SR_TFUF_MASK
		                   |SPI_SR_TFFF_MASK
		                   |SPI_SR_RFDF_MASK
		                   |SPI_SR_RFOF_MASK;

}


void SPI0_Sendbyte(char onebyte)
{

SPI0->SR|=SPI_SR_TCF_MASK;//清除发送完成
SPI0->PUSHR|=SPI_PUSHR_CTAS(0)//选择ctar0
	         |SPI_PUSHR_EOQ_MASK//最后一帧
            |SPI_PUSHR_PCS(1)//pcs0 有效
            |SPI_PUSHR_TXDATA(onebyte);//写如数据
	
SPI0->MCR&=~SPI_MCR_HALT_MASK;//启动SPI0
	while((SPI0->SR&SPI_SR_TCF_MASK)==0);
	SPI0->SR|=SPI_SR_TCF_MASK;//清除发送完成
//	SPI0->POPR 收到的数

}
char SPI0_Receivebyte()
{

while((SPI0->SR&SPI_SR_RFDF_MASK)==0);//没有收到就等待

 return SPI0->POPR;



}












//定时器中断

void PIT0_IRQHandler()
{ 
	
	   
		(*PIT).CHANNEL[0].TFLG |=PIT_TFLG_TIF_MASK;//清除标记位
		speed=(FTM2->CNT);	//500线算速度
	  itoa(speed,spe);
	UART4_Sendstring("速度");
	UART4_Sendstring(spe);
	UART4_Sendstring("\r\n");
		FTM2->CNT=0;
		PTA->PTOR|=(0x01<<14);
	
}
