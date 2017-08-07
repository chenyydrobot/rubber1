/*
  项目名称：  软件模拟IIC信号对触摸芯片WTC7514DSI进行操作的范例程序
  使用主MCU： AT89C51
  项目目的：
    1：演示如何通过IIC总线读取触摸芯片WTC7514DSI的按键信息
    2：演示如何通过IIC总线设定触摸芯片WTC7514DSI的按键灵敏度级数
  项目作者：Wincomtech 
  日期和版本号： 2010.5.8 V1.2  
*/
//---------------------------------------------------------------------------     
#include "public.h"
#define		uchar 		unsigned char
#define		uint		unsigned int

#define HIGH 1     
#define LOW  0     
#define FALSE 0     
#define TRUE 1    
#define TOUCH_CHIP_ADDR_WR  0x70  //从机的地址和方向位W     
#define TOUCH_CHIP_ADDR_RE  0x71  //从机地址和方向位R   

typedef bool bit;
//---------------------------------------------------------------------------
//   与触摸芯片的I2C接口
//---------------------------------------------------------------------------
#define GP2Y_GROUP2_PORT    GPIOC
#define GP2Y_GROUP1_SDA     GPIO_PIN_9

#define GP2Y_GROUP1_PORT    GPIOA
#define GP2Y_GROUP1_SCL     GPIO_PIN_8

#define SCL_H (GP2Y_GROUP1_PORT->ODR |= GP2Y_GROUP1_SCL)
#define SCL_L (GP2Y_GROUP1_PORT->ODR &= ~GP2Y_GROUP1_SCL)

#define SDA_H (GP2Y_GROUP2_PORT->ODR |= GP2Y_GROUP1_SDA)
#define SDA_L (GP2Y_GROUP2_PORT->ODR &= ~GP2Y_GROUP1_SDA)

#define SDA_R (GP2Y_GROUP2_PORT->IDR & GP2Y_GROUP1_SDA) 

#define _nop_() delay(0)

//static __INLINE void _nop_() 
//{ 
 //  __ASM volatile ("nop"); 
//} 

//---------------------------------------------------------------------------     
void  delay(int i)
{
	 int ii=200;
   while((ii) > 0)
   {
   	ii--;
   }	
}

//---------------------------------------------------------------------------     
//函数名:    void I2C_start(void)     
//功能:      产生I2C总线的起始位    
//函数入口:  无
//函数出口:  无
//中间变量:  无
//---------------------------------------------------------------------------     
void I2C_start(void)    
{    
    SDA_H;    //确保SDA为高电平  
    SCL_H;    //确保SCL为高电平
    delay(0);    
    SDA_L;     //SCL为高电平时将SDA由高拉低,产生I2C的起始信号
    delay(0);    
	  SCL_L;
}    
     
//---------------------------------------------------------------------------    
//函数名:    void I2C_start(void)     
//功能:      产生I2C总线的结束位    
//函数入口:  无
//函数出口:  无
//中间变量:  无
//---------------------------------------------------------------------------    
void I2C_stop(void)    
{      
    SDA_L;    //将SDA置低
	
    delay(0); 
	
    SCL_H;   //将SCL置高 
	
    delay(0);    

    SDA_H;   //SCL为高电平时将SDA由低置高,产生I2C的结束信号  
    delay(0);    
	

}    
 
//---------------------------------------------------------------------------   
//函数名:    bit I2C_send_1byte(IData)     
//功能:      向I2C发送8bit数据，并请求一个应答信号ACK。如果成功收到ACK返回1，否则返回0    
//函数入口:  （ IData） 需要发送到I2C从机的数据
//函数出口:  收到I2C的ACK信号 返回1（TRUE），否则为0（FALSE)
//中间变量:  
//---------------------------------------------------------------------------       
bit I2C_send_1byte(uchar IData)    
{    
    uchar i;
    bit   AckStatus;    
    for (i=0; i<8; i++)    	//发送8位数据
    {    
    	//SCL_L;		//将SCL置为低电平,传输数据时必须在SCL为低电平时才能改变SDA的状态
    	     	 	
    	if((IData & 0x80) == 0) //根据8位数据设定SDA的电平
    	{
    	   SDA_L;		//数据最高位为0
    	}
    	else			
    	{
    	   SDA_H;		//数据最高位为1
    	}

       IData = IData << 1;     //从bit7~bit0由高位到低位依次传输
			 _nop_();
	     SCL_H;		//将SCL置为高电平
	     _nop_();
			 SCL_L;
       _nop_();
    } 
    
    //SCL_L;	   //SCL置为低开始发送第9个时钟信号，请求从机的回应信号

    _nop_();    
    SDA_H;    //将SDA设置为输入状态,准备读取应答信号               
    SCL_H;	   //SCL置为高
                
    if(SDA_R)
    {			//SDA为高电平，表示没有收到从机的回应信号
    	AckStatus = FALSE;  //返回错误信息
    }
    else
    {			//SDA为低电平，表示成功收到从机的回应信号
    	AckStatus = TRUE;   //返回成功发送信息
    }
		_nop_();
    SCL_L;	    //SCL置为低
    return(AckStatus);
}    

//---------------------------------------------------------------------------   
//函数名:    uchar I2C_receive_1byte(void)   
//功能:      从I2C上读取8位数据，并作为一个字节返回，不回送ACK。
//函数入口:  无
//函数出口:  读取I2C得到的8位数据
//中间变量:  
//---------------------------------------------------------------------------     
uchar I2C_receive_1byte(void)    
{    
    uchar IData = 0;     
    uchar i;    
    for(i=0;i<8;i++)    
    {    
    	IData <<= 1;       //从高bit到低bit依次读入数据
  	          //将SDA设置为输入状态准备读取数据
    	_nop_();
    	SCL_H;	   //SCL置为高
    	_nop_();	   //延时一段时间再读取数据以保证SDA上的数据稳定
    	if(SDA_R)
    	{
    	   IData |= 0x01;  //读到数据1	
    	}
      //else
    	//{
    	   //IData &= 0xFE;  //读到数据0		
    	//}
			_nop_();
      SCL_L;
    }   
		_nop_();
		SCL_H;
		_nop_();
		SCL_L;
    return (IData);	   //返回从总线上读取的8 bit数据     
}    
    
//---------------------------------------------------------------------------     
//函数名:    void I2C_ack(void)     
//功能:      向I2C总线发送一个应答信号ACK，一般用于连续数据读取时，一个字节读完后通知从机继续传送下一个数据   
//函数入口:  无
//函数出口:  无
//中间变量:  
//---------------------------------------------------------------------------    
void I2C_ack()    
{        
    SCL_L;	    //SCL???
     _nop_();      
    SDA_L;     //SDA???
     _nop_();  
    SCL_H;	   //SCL???
     _nop_(); 
    SCL_L;	    //SCL???
}    

//---------------------------------------------------------------------------     
//函数名:    void I2C_NoAck(void)     
//功能:      不向I2C总线发送应答信号ACK，仅产生SCL的时钟信号。一般用于读取完最后一个字节后通知总线上的从机数据读取完毕  
//函数入口:  无
//函数出口:  无
//中间变量:  
//---------------------------------------------------------------------------    
void I2C_NoAck()    
{        

   SCL_L;	   //SCL置为低
  _nop_(); 
    SDA_H;    //SDA置为高
    _nop_();     
    SCL_H;	   //SCL置为高
    _nop_();  
    SCL_L;	    //SCL置为低
	  _nop_();
}    

//---------------------------------------------------------------------------     
//函数名:    bit WriteData(uchar AddW, uchar *pRData, uchar length)    
//功能:      通过I2C总线向IIC器件写入数据。
//函数入口:  AddW：  IIC器件的地址
//	     pRData: 准备要写入IIC器件数据的首地址
//	     length: 准备写入数据的长度（字节数） 
//函数出口:  正确写入数据返回0 写入数据错误返回1 
//中间变量:  
//---------------------------------------------------------------------------        
bit WriteData(uchar AddW, uchar *pRData, uchar length)
{
   bit 	I2CStatus;
   uchar i;
   
   I2C_start();   //主控MCU在I2C上产生一个起始信号
	
   I2CStatus = I2C_send_1byte(AddW); //向I2C总线传送器件地址和写入方向标记位

   if(I2CStatus)  //有正确的回应
   {
     for(i=0; i<=length-1; i++)	
     {
     	delay(20); 		    //延时等待
     	I2CStatus = I2C_send_1byte(pRData[i]); //向芯片发送一字节数据
     	if(I2CStatus == 0)
     	{
           delay(0); 	
           I2C_stop();    ////主控MCU在I2C上产生一个停止信号
           return(1);     	   
     	}
     }
     delay(0); 
     I2C_stop();    ////主控MCU在I2C上产生一个停止信号
     return(0);
   }
   else
   {
     delay(0); 	
     I2C_stop();    ////主控MCU在I2C上产生一个停止信号
     return(1);
   }
} 

//---------------------------------------------------------------------------     
//函数名:    bit ReadData(uchar AddR, uchar *pRData, uchar length) 
//功能:      通过I2C总线读出IIC器件的数据。
//函数入口:  AddR：  IIC器件的地址
//           pRData: 准备储存IIC数据的缓冲区地址
//           length: 准备读出数据的长度（字节数） 
//函数出口:  正确读取数据返回0 读取数据错误返回1 
//中间变量:  
//---------------------------------------------------------------------------     
bit ReadData(uchar AddR, uchar *pRData, uchar length)   
{   
   bit I2CStatus;
   uchar i;
   
   I2C_start();   //主控MCU在I2C上产生一个起始信号
   I2CStatus = I2C_send_1byte(AddR); //向I2C总线传送触摸芯片的器件地址和读出方向标记位
   if(I2CStatus)  //收到触摸芯片的正确回应
   {
     
     for(i=0; i <= length-1; i++)
     {
     	   
     	delay(20); 
     	pRData[i] = I2C_receive_1byte();
     	if(i < length-1)
     	{
     	  ;//I2C_ack();	
     	}
     	else
     	{
     	  ;//I2C_NoAck();     	//读完数据后的最后一个字节后的第9个时钟不产生回应位，以通知总线上的从机数据读取完毕。	
     	}
     }
     //delay(0);
     I2C_stop();        //主控MCU在I2C上产生一个停止信号，结束操作
		  SCL_H;   //将SCL置高 
     return(0);		//
   }
   else
   {
     I2C_stop();    //主控MCU在I2C上产生一个停止信号 
     return(1);
   }
}

void WTC7514_init_param(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;      	
	

/********i2c引脚配置***************/	
	GPIO_InitStructure.Pin   = GPIO_PIN_8;//A7   
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;//GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	GPIO_InitStructure.Pin   = GPIO_PIN_9; //C4  
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;//GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);


	GPIO_InitStructure.Pin  = GPIO_PIN_7;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET);



}	


void main11(void)
{
   uchar i;
   uint	 temp;
   uchar  ReadBuff[2];
   uchar  WriteBuff = 15;
   bit    ErrFlag;
   
   I2C_stop();    //主控MCU初始化后,在I2C上产生一个停止信号，以停止可能由于上电时IO电平变化引起的I2C总线的“忙”状态。	

   ErrFlag = WriteData(TOUCH_CHIP_ADDR_WR, &WriteBuff, 1);   //向WTC7514DSI写入新的灵敏度级数（15级）

   delay(250);	//延时等待触摸芯片重新初始化后
	 delay(250);	//延时等待触摸芯片重新初始化后
	 delay(250);	//延时等待触摸芯片重新初始化后
   i= 0;
   while(1)
   {
       ErrFlag = ReadData(TOUCH_CHIP_ADDR_RE, ReadBuff, 2); //读取WTC7514DSI的按键值
      
		/* 
       if(ReadBuff[0] & 0x20 == 0 ){ ;} //SW13 上手指已经离开
       else                        { ;} //SW13 上有触摸动作
       if(ReadBuff[0] & 0x10 == 0 ){ ;} //SW12 上手指已经离开
       else                        { ;} //SW12 上有触摸动作   
       if(ReadBuff[0] & 0x08 == 0 ){ ;} //SW11 上手指已经离开
       else                        { ;} //SW11 上有触摸动作    
       if(ReadBuff[0] & 0x04 == 0 ){ ;} //SW10 上手指已经离开
       else                        { ;} //SW10 上有触摸动作      
       if(ReadBuff[0] & 0x02 == 0 ){ ;} //SW9 上手指已经离开
       else                        { ;} //SW9 上有触摸动作                        
       if(ReadBuff[0] & 0x01 == 0 ){ ;} //SW8 上手指已经离开
       else                        { ;} //SW8 上有触摸动作     
  
       if(ReadBuff[1] & 0x80 == 0 ){ ;} //SW7 上手指已经离开
       else                        { ;} //SW7 上有触摸动作
       if(ReadBuff[1] & 0x40 == 0 ){ ;} //SW6 上手指已经离开
       else                        { ;} //SW6 上有触摸动作          
       if(ReadBuff[1] & 0x20 == 0 ){ ;} //SW5 上手指已经离开
       else                        { ;} //SW5 上有触摸动作
       if(ReadBuff[1] & 0x10 == 0 ){ ;} //SW4 上手指已经离开
       else                        { ;} //SW4 上有触摸动作   
       if(ReadBuff[1] & 0x08 == 0 ){ ;} //SW3 上手指已经离开
       else                        { ;} //SW3 上有触摸动作    
       if(ReadBuff[1] & 0x04 == 0 ){ ;} //SW2 上手指已经离开
       else                        { ;} //SW2 上有触摸动作      
       if(ReadBuff[1] & 0x02 == 0 ){ ;} //SW1 上手指已经离开
       else                        { ;} //SW1 上有触摸动作                        
       if(ReadBuff[1] & 0x01 == 0 ){ ;} //SW0 上手指已经离开
       else                        { ;} //SW0 上有触摸动作                
       
       delay(250);
			*/ 
			 delay(250);
   }
}
   
    

