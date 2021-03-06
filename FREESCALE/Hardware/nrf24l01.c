#include "nrf24l01.h"


const char TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; 
const char RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; 
const char TX_ADDRESS1[RX_ADR_WIDTH]={0x12,0x13,0x14,0x15,0x16};
const char RX_ADDRESS1[RX_ADR_WIDTH]={0x12,0x13,0x14,0x15,0x16};


//初始化nrf io口相关
//en和irq端口
void NRF24L01_Init()//???
{ 
	
	SPI0_lnit(1);
 PORTC->PCR[8]=PORT_PCR_MUX(0X1);//功能1普通io口
 PORTC->PCR[9]=PORT_PCR_MUX(0X1);//
  PORTC->PCR[8]&=~PORT_PCR_PS_MASK;//打开下拉电阻、
	PORTC->PCR[8]|=PORT_PCR_PE_MASK;
  PTC->PDDR&=~GPIO_PDDR_PDD(0x01<<8);//  IRQ端口是输入，en端口是输出  这里暂时8口下拉输入，9口输出
	PTC->PDDR|=GPIO_PDDR_PDD(0x01<<9);
	
	PTA->PDOR &=~GPIO_PDOR_PDO(0x01<<9);//EN口输出0
	

}


//写寄存器
void  NRF24L01_Write_Reg(char reg,char value)//
{ 
	 
	SPI0->MCR&=~SPI_MCR_PCSIS(0);  //使能pcs0 
	SPI0_Sendbyte(reg);//??????????
	SPI0_Sendbyte(value);;//????
	SPI0->MCR|=SPI_MCR_PCSIS(0); //不使能
	
} 


//读寄存器
char NRF24L01_Read_Reg(char reg)
{
	char reg_val;	    
 	SPI0->MCR&=~SPI_MCR_PCSIS(0);  //ê1?üpcs0         //		
  SPI0_Sendbyte(reg);   //???????+??
  reg_val=SPI0_Receivebyte();//???????
  SPI0->MCR|=SPI_MCR_PCSIS(0);  //ê1?üpcs0         //??SPI??		    
  return(reg_val);           //?????
}	
	

void  NRF24L01_Read_Buf(char reg,char *pBuf,char len)//??????????
{
	char char_ctr;	       
  	SPI0->MCR&=~SPI_MCR_PCSIS(0);  //ê1?üpcs0          //??SPI??
    SPI0_Sendbyte(reg);//??????(??),??????   
 	for(char_ctr=0;char_ctr<len;char_ctr++)pBuf[char_ctr]=SPI0_Receivebyte();//????
  	 SPI0->MCR|=SPI_MCR_PCSIS(0);      //??SPI??
  	       
}

void NRF24L01_Write_Buf(char reg, char *pBuf, char len)
{
	char char_ctr;	    
 	SPI0->MCR&=~SPI_MCR_PCSIS(0);  //ê1?üpcs0         //??SPI??
  	SPI0_Sendbyte(reg);//??????(??),??????
  	for(char_ctr=0; char_ctr<len; char_ctr++)SPI0_Sendbyte(*pBuf++); //????	 
  	SPI0->MCR|=SPI_MCR_PCSIS(0);  //ê1?üpcs0      //??SPI??
          //????????
}				   

char NRF24L01_Check(void)
{
	char buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	char i;
   	 
	NRF24L01_Write_Buf(WRITE_REG_NRF+TX_ADDR,buf,5);//??5??????.zai mingling+ji chun qi dizhi dui xiang yin jcq chaozuo 	
	NRF24L01_Read_Buf(READ_REG_NRF+TX_ADDR,buf,5); //???????  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 1;//??24L01??	
	return 0;		 //???24L01
}	 	 


//这里需要改
//txbuf:????????
//???:??????
char NRF24L01_TxPacket(char *txbuf)
{
	char sta;
 	
//	NRF24L01_CE=0;
  	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//????TX BUF  32???
// 	NRF24L01_CE=1;//????	   
//	while(NRF24L01_IRQ!=0);//??????
	sta=NRF24L01_Read_Reg(_STATUS);  //?????????	   
	NRF24L01_Write_Reg(WRITE_REG_NRF+_STATUS,sta); //??TX_DS?MAX_RT????
	if(sta&MAX_TX)//????????
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);//??TX FIFO??? 
		return MAX_TX; 
	}
	if(sta&TX_OK)//????
	{
		return TX_OK;
	}
	return 0xff;//????????
}
//??NRF24L01??????
//txbuf:????????
//???:0,????;??,????
char NRF24L01_RxPacket(char *rxbuf)
{
	char sta;		    							   

	sta=NRF24L01_Read_Reg(_STATUS);  //?????????    	 
	NRF24L01_Write_Reg(WRITE_REG_NRF+_STATUS,sta); //??TX_DS?MAX_RT????
	if(sta&RX_OK)//?????
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//????
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//??RX FIFO??? 
		return 0; 
	}	   
	return 1;//???????
}					    







void NRF24L01_RX_Mode(void)
{
//	  NRF24L01_CE=0;	  //??????
  	NRF24L01_Write_Buf(WRITE_REG_NRF+RX_ADDR_P0,(char*)RX_ADDRESS,RX_ADR_WIDTH);//?RX????,??????????
	  NRF24L01_Write_Buf(WRITE_REG_NRF+RX_ADDR_P1,(char*)RX_ADDRESS1,RX_ADR_WIDTH);
	 
//	  NRF24L01_Write_Reg(WRITE_REG_NRF+EN_AA,0x01);//????0
  	NRF24L01_Write_Reg(WRITE_REG_NRF+EN_AA,0x03);	//????0???1?????  
  		
	
//  	NRF24L01_Write_Reg(WRITE_REG_NRF+EN_RXADDR,0x01);//????0?????  	 
  	NRF24L01_Write_Reg(WRITE_REG_NRF+EN_RXADDR,0x03);//????0???1?????  	 
	
	NRF24L01_Write_Reg(WRITE_REG_NRF+RX_PW_P0,RX_PLOAD_WIDTH);//????0???????
  NRF24L01_Write_Reg(WRITE_REG_NRF+RX_PW_P1,RX_PLOAD_WIDTH);//????1???????
   
  	NRF24L01_Write_Reg(WRITE_REG_NRF+RF_CH,40);	     //??RF????		  
    NRF24L01_Write_Reg(WRITE_REG_NRF+RF_SETUP,0x0f);//??TX????,0db??,2Mbps,???????   
  	NRF24L01_Write_Reg(WRITE_REG_NRF+CONFIG, 0x0f);//???????????;PWR_UP,EN_CRC,16BIT_CRC,???? 
//  	NRF24L01_CE = 1; //CE??,?????? 
}						 
//??????NRF24L01?TX??
//??TX??,?TX????,??RX???????,??TX????,??RF??,????LNA HCURR
//PWR_UP,CRC??
//?CE???,???RX??,????????		   
//CE????10us,?????.	 
void NRF24L01_TX_Mode(void)
{														 
//	NRF24L01_CE=0;	//??????    
  	NRF24L01_Write_Buf(WRITE_REG_NRF+TX_ADDR,(char*)TX_ADDRESS1,TX_ADR_WIDTH);//?TX???? ?????? ??1 ?????
  	NRF24L01_Write_Buf(WRITE_REG_NRF+RX_ADDR_P0,(char*)RX_ADDRESS1,RX_ADR_WIDTH); //??TX????,??????ACK,??????	  

  	NRF24L01_Write_Reg(WRITE_REG_NRF+EN_AA,0x01);     //????0?????    
  	NRF24L01_Write_Reg(WRITE_REG_NRF+EN_RXADDR,0x01); //????0?????  
  	NRF24L01_Write_Reg(WRITE_REG_NRF+SETUP_RETR,0x1a);//??????????:500us + 86us;????????:10?
  	NRF24L01_Write_Reg(WRITE_REG_NRF+RF_CH,40);       //??RF???40
  	NRF24L01_Write_Reg(WRITE_REG_NRF+RF_SETUP,0x0f);  //??TX????,0db??,2Mbps,???????   
  	NRF24L01_Write_Reg(WRITE_REG_NRF+CONFIG,0x0e);    //???????????;PWR_UP,EN_CRC,16BIT_CRC,????,??????
//	NRF24L01_CE=1;//CE??,??????
}		  

