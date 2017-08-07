/*
  ��Ŀ���ƣ�  ����ģ��IIC�źŶԴ���оƬWTC7514DSI���в����ķ�������
  ʹ����MCU�� AT89C51
  ��ĿĿ�ģ�
    1����ʾ���ͨ��IIC���߶�ȡ����оƬWTC7514DSI�İ�����Ϣ
    2����ʾ���ͨ��IIC�����趨����оƬWTC7514DSI�İ��������ȼ���
  ��Ŀ���ߣ�Wincomtech 
  ���ںͰ汾�ţ� 2010.5.8 V1.2  
*/
//---------------------------------------------------------------------------     
#include "public.h"
#define		uchar 		unsigned char
#define		uint		unsigned int

#define HIGH 1     
#define LOW  0     
#define FALSE 0     
#define TRUE 1    
#define TOUCH_CHIP_ADDR_WR  0x70  //�ӻ��ĵ�ַ�ͷ���λW     
#define TOUCH_CHIP_ADDR_RE  0x71  //�ӻ���ַ�ͷ���λR   

typedef bool bit;
//---------------------------------------------------------------------------
//   �봥��оƬ��I2C�ӿ�
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
//������:    void I2C_start(void)     
//����:      ����I2C���ߵ���ʼλ    
//�������:  ��
//��������:  ��
//�м����:  ��
//---------------------------------------------------------------------------     
void I2C_start(void)    
{    
    SDA_H;    //ȷ��SDAΪ�ߵ�ƽ
    _nop_();
    _nop_();    
    _nop_();
    _nop_();    
    SCL_H;    //ȷ��SCLΪ�ߵ�ƽ
    delay(0);    
    SDA_L;     //SCLΪ�ߵ�ƽʱ��SDA�ɸ�����,����I2C����ʼ�ź�
    delay(0);    
}    
     
//---------------------------------------------------------------------------    
//������:    void I2C_start(void)     
//����:      ����I2C���ߵĽ���λ    
//�������:  ��
//��������:  ��
//�м����:  ��
//---------------------------------------------------------------------------    
void I2C_stop(void)    
{   
    SCL_L;    //��SCL�õ� 	 
    _nop_();
    _nop_();
    _nop_();
    _nop_();    
    SDA_L;    //��SDA�õ�
    delay(0);    
    SCL_H;   //��SCL�ø� 
    delay(0);    
    SDA_H;   //SCLΪ�ߵ�ƽʱ��SDA�ɵ��ø�,����I2C�Ľ����ź�  
    delay(0);    
}    
 
//---------------------------------------------------------------------------   
//������:    bit I2C_send_1byte(IData)     
//����:      ��I2C����8bit���ݣ�������һ��Ӧ���ź�ACK������ɹ��յ�ACK����1�����򷵻�0    
//�������:  �� IData�� ��Ҫ���͵�I2C�ӻ�������
//��������:  �յ�I2C��ACK�ź� ����1��TRUE��������Ϊ0��FALSE)
//�м����:  
//---------------------------------------------------------------------------       
bit I2C_send_1byte(uchar IData)    
{    
    uchar i;
    bit   AckStatus;    
    for (i=0; i<8; i++)    	//����8λ����
    {    
    	SCL_L;		//��SCL��Ϊ�͵�ƽ,��������ʱ������SCLΪ�͵�ƽʱ���ܸı�SDA��״̬
    	_nop_();
    	_nop_();       	 	
    	if((IData & 0x80) == 0) //����8λ�����趨SDA�ĵ�ƽ
    	{
    	   SDA_L;		//�������λΪ0
    	}
    	else			
    	{
    	   SDA_H;		//�������λΪ1
    	}

        IData = IData << 1;     //��bit7~bit0�ɸ�λ����λ���δ���
	    SCL_H;		//��SCL��Ϊ�ߵ�ƽ
	    _nop_();
    	_nop_();
    } 
    
    SCL_L;	   //SCL��Ϊ�Ϳ�ʼ���͵�9��ʱ���źţ�����ӻ��Ļ�Ӧ�ź�
    _nop_();
    _nop_();    
    SDA_H;    //��SDA����Ϊ����״̬,׼����ȡӦ���ź�    
    _nop_();
    _nop_();             
    SCL_H;	   //SCL��Ϊ��
    _nop_();
    _nop_();            
    if(SDA_R )
    {			//SDAΪ�ߵ�ƽ����ʾû���յ��ӻ��Ļ�Ӧ�ź�
    	AckStatus = FALSE;  //���ش�����Ϣ
    }
    else
    {			//SDAΪ�͵�ƽ����ʾ�ɹ��յ��ӻ��Ļ�Ӧ�ź�
    	AckStatus = TRUE;   //���سɹ�������Ϣ
    }
    SCL_L;	    //SCL��Ϊ��
    return(AckStatus);
}    

//---------------------------------------------------------------------------   
//������:    uchar I2C_receive_1byte(void)   
//����:      ��I2C�϶�ȡ8λ���ݣ�����Ϊһ���ֽڷ��أ�������ACK��
//�������:  ��
//��������:  ��ȡI2C�õ���8λ����
//�м����:  
//---------------------------------------------------------------------------     
uchar I2C_receive_1byte(void)    
{    
    uchar IData = 0;     
    uchar i;    
    for(i=0;i<8;i++)    
    {    
    	IData <<= 1;       //�Ӹ�bit����bit���ζ�������
    	SCL_L;	   //SCL��Ϊ��
    	SDA_H;	   //��SDA����Ϊ����״̬׼����ȡ����
    	_nop_();
    	_nop_();
    	SCL_H;	   //SCL��Ϊ��
    	_nop_();
    	_nop_();	   //��ʱһ��ʱ���ٶ�ȡ�����Ա�֤SDA�ϵ������ȶ�
    	if(SDA_R)
    	{
    	   IData |= 0x01;  //��������1	
    	}
    	//else
    	//{
    	//   IData &= 0xFE;  //��������0		
    	//}
        
    }   
    return (IData);	   //���ش������϶�ȡ��8 bit����     
}    
    
//---------------------------------------------------------------------------     
//������:    void I2C_ack(void)     
//����:      ��I2C���߷���һ��Ӧ���ź�ACK��һ�������������ݶ�ȡʱ��һ���ֽڶ����֪ͨ�ӻ�����������һ������   
//�������:  ��
//��������:  ��
//�м����:  
//---------------------------------------------------------------------------    
void I2C_ack()    
{   
    SCL_L;	    //SCL��Ϊ��
     _nop_(); 
     _nop_();     
    SDA_L;     //SDA��Ϊ��
     _nop_(); 
     _nop_(); 
    SCL_H;	   //SCL��Ϊ��
     _nop_(); 
     _nop_();     
     _nop_(); 
     _nop_(); 
    SCL_L;	    //SCL��Ϊ��
}    

//---------------------------------------------------------------------------     
//������:    void I2C_NoAck(void)     
//����:      ����I2C���߷���Ӧ���ź�ACK��������SCL��ʱ���źš�һ�����ڶ�ȡ�����һ���ֽں�֪ͨ�����ϵĴӻ����ݶ�ȡ���  
//�������:  ��
//��������:  ��
//�м����:  
//---------------------------------------------------------------------------    
void I2C_NoAck()    
{   
    SCL_L;	   //SCL��Ϊ��
    _nop_();   
    _nop_();       
    SDA_H;    //SDA��Ϊ��
    _nop_();   
    _nop_();   
    SCL_H;	   //SCL��Ϊ��
     _nop_(); 
     _nop_();     
     _nop_(); 
     _nop_(); 
    SCL_L;	    //SCL��Ϊ��
}    

//---------------------------------------------------------------------------     
//������:    bit WriteData(uchar AddW, uchar *pRData, uchar length)    
//����:      ͨ��I2C������IIC����д�����ݡ�
//�������:  AddW��  IIC�����ĵ�ַ
//	     pRData: ׼��Ҫд��IIC�������ݵ��׵�ַ
//	     length: ׼��д�����ݵĳ��ȣ��ֽ����� 
//��������:  ��ȷд�����ݷ���0 д�����ݴ��󷵻�1 
//�м����:  
//---------------------------------------------------------------------------        
bit WriteData(uchar AddW, uchar *pRData, uchar length)
{
   bit 	I2CStatus;
   uchar i;
   
   I2C_start();   //����MCU��I2C�ϲ���һ����ʼ�ź�
	
   I2CStatus = I2C_send_1byte(AddW); //��I2C���ߴ���������ַ��д�뷽����λ

   if(I2CStatus)  //����ȷ�Ļ�Ӧ
   {
     for(i=0; i<=length-1; i++)	
     {
     	delay(20); 		    //��ʱ�ȴ�
     	I2CStatus = I2C_send_1byte(pRData[i]); //��оƬ����һ�ֽ�����
     	if(I2CStatus == 0)
     	{
           delay(0); 	
           I2C_stop();    ////����MCU��I2C�ϲ���һ��ֹͣ�ź�
           return(1);     	   
     	}
     }
     delay(0); 
     I2C_stop();    ////����MCU��I2C�ϲ���һ��ֹͣ�ź�
     return(0);
   }
   else
   {
     delay(0); 	
     I2C_stop();    ////����MCU��I2C�ϲ���һ��ֹͣ�ź�
     return(1);
   }
} 

//---------------------------------------------------------------------------     
//������:    bit ReadData(uchar AddR, uchar *pRData, uchar length) 
//����:      ͨ��I2C���߶���IIC���������ݡ�
//�������:  AddR��  IIC�����ĵ�ַ
//           pRData: ׼������IIC���ݵĻ�������ַ
//           length: ׼���������ݵĳ��ȣ��ֽ����� 
//��������:  ��ȷ��ȡ���ݷ���0 ��ȡ���ݴ��󷵻�1 
//�м����:  
//---------------------------------------------------------------------------     
bit ReadData(uchar AddR, uchar *pRData, uchar length)   
{   
   bit I2CStatus;
   uchar i;
   
   I2C_start();   //����MCU��I2C�ϲ���һ����ʼ�ź�
   I2CStatus = I2C_send_1byte(AddR); //��I2C���ߴ��ʹ���оƬ��������ַ�Ͷ���������λ
   if(I2CStatus)  //�յ�����оƬ����ȷ��Ӧ
   {
     
     for(i=0; i <= length-1; i++)
     {
     	   
     	delay(20); 
     	pRData[i] = I2C_receive_1byte();
     	if(i < length-1)
     	{
     	   I2C_ack();	
     	}
     	else
     	{
     	   I2C_NoAck();     	//�������ݺ�����һ���ֽں�ĵ�9��ʱ�Ӳ�������Ӧλ����֪ͨ�����ϵĴӻ����ݶ�ȡ��ϡ�	
     	}
     }
     delay(0);
     I2C_stop();        //����MCU��I2C�ϲ���һ��ֹͣ�źţ���������
     return(0);		//
   }
   else
   {
     I2C_stop();    //����MCU��I2C�ϲ���һ��ֹͣ�ź� 
     return(1);
   }
}

void WTC7514_init_gpio(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;      	
	

/********i2c��������***************/	
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


void WTC7514_init_param(void)
{
   uchar i;
   uint	 temp;
   uchar  ReadBuff[2];
   uchar  WriteBuff = 15;
   bit    ErrFlag;
   
   I2C_stop();    //����MCU��ʼ����,��I2C�ϲ���һ��ֹͣ�źţ���ֹͣ���������ϵ�ʱIO��ƽ�仯�����I2C���ߵġ�æ��״̬��	

   ErrFlag = WriteData(TOUCH_CHIP_ADDR_WR, &WriteBuff, 1);   //��WTC7514DSIд���µ������ȼ�����15����

   delay(250);	//��ʱ�ȴ�����оƬ���³�ʼ����
	 delay(250);	//��ʱ�ȴ�����оƬ���³�ʼ����
	 delay(250);	//��ʱ�ȴ�����оƬ���³�ʼ����
   i= 0;
   while(0)
   {
       ErrFlag = ReadData(TOUCH_CHIP_ADDR_RE, ReadBuff, 2); //��ȡWTC7514DSI�İ���ֵ
       
       if(ReadBuff[0] & 0x20 == 0 ){ ;} //SW13 ����ָ�Ѿ��뿪
       else                        { ;} //SW13 ���д�������
       if(ReadBuff[0] & 0x10 == 0 ){ ;} //SW12 ����ָ�Ѿ��뿪
       else                        { ;} //SW12 ���д�������   
       if(ReadBuff[0] & 0x08 == 0 ){ ;} //SW11 ����ָ�Ѿ��뿪
       else                        { ;} //SW11 ���д�������    
       if(ReadBuff[0] & 0x04 == 0 ){ ;} //SW10 ����ָ�Ѿ��뿪
       else                        { ;} //SW10 ���д�������      
       if(ReadBuff[0] & 0x02 == 0 ){ ;} //SW9 ����ָ�Ѿ��뿪
       else                        { ;} //SW9 ���д�������                        
       if(ReadBuff[0] & 0x01 == 0 ){ ;} //SW8 ����ָ�Ѿ��뿪
       else                        { ;} //SW8 ���д�������     
  
       if(ReadBuff[1] & 0x80 == 0 ){ ;} //SW7 ����ָ�Ѿ��뿪
       else                        { ;} //SW7 ���д�������
       if(ReadBuff[1] & 0x40 == 0 ){ ;} //SW6 ����ָ�Ѿ��뿪
       else                        { ;} //SW6 ���д�������          
       if(ReadBuff[1] & 0x20 == 0 ){ ;} //SW5 ����ָ�Ѿ��뿪
       else                        { ;} //SW5 ���д�������
       if(ReadBuff[1] & 0x10 == 0 ){ ;} //SW4 ����ָ�Ѿ��뿪
       else                        { ;} //SW4 ���д�������   
       if(ReadBuff[1] & 0x08 == 0 ){ ;} //SW3 ����ָ�Ѿ��뿪
       else                        { ;} //SW3 ���д�������    
       if(ReadBuff[1] & 0x04 == 0 ){ ;} //SW2 ����ָ�Ѿ��뿪
       else                        { ;} //SW2 ���д�������      
       if(ReadBuff[1] & 0x02 == 0 ){ ;} //SW1 ����ָ�Ѿ��뿪
       else                        { ;} //SW1 ���д�������                        
       if(ReadBuff[1] & 0x01 == 0 ){ ;} //SW0 ����ָ�Ѿ��뿪
       else                        { ;} //SW0 ���д�������                
       
       delay(250);
   }
}
   
uchar  ReadBuff[2];

void WTC7514_read_param(void)
{
	     bit    ErrFlag;
	
       ErrFlag = ReadData(TOUCH_CHIP_ADDR_RE, ReadBuff, 2); //��ȡWTC7514DSI�İ���ֵ
       
       if(ReadBuff[0] & 0x20 == 0 ){ ;} //SW13 ����ָ�Ѿ��뿪
       else                        { ;} //SW13 ���д�������
       if(ReadBuff[0] & 0x10 == 0 ){ ;} //SW12 ����ָ�Ѿ��뿪
       else                        { ;} //SW12 ���д�������   
       if(ReadBuff[0] & 0x08 == 0 ){ ;} //SW11 ����ָ�Ѿ��뿪
       else                        { ;} //SW11 ���д�������    
       if(ReadBuff[0] & 0x04 == 0 ){ ;} //SW10 ����ָ�Ѿ��뿪
       else                        { ;} //SW10 ���д�������      
       if(ReadBuff[0] & 0x02 == 0 ){ ;} //SW9 ����ָ�Ѿ��뿪
       else                        { ;} //SW9 ���д�������                        
       if(ReadBuff[0] & 0x01 == 0 ){ ;} //SW8 ����ָ�Ѿ��뿪
       else                        { ;} //SW8 ���д�������     
  
       if(ReadBuff[1] & 0x80 == 0 ){ ;} //SW7 ����ָ�Ѿ��뿪
       else                        { ;} //SW7 ���д�������
       if(ReadBuff[1] & 0x40 == 0 ){ ;} //SW6 ����ָ�Ѿ��뿪
       else                        { ;} //SW6 ���д�������          
       if(ReadBuff[1] & 0x20 == 0 ){ ;} //SW5 ����ָ�Ѿ��뿪
       else                        { ;} //SW5 ���д�������
       if(ReadBuff[1] & 0x10 == 0 ){ ;} //SW4 ����ָ�Ѿ��뿪
       else                        { ;} //SW4 ���д�������   
       if(ReadBuff[1] & 0x08 == 0 ){ ;} //SW3 ����ָ�Ѿ��뿪
       else                        { ;} //SW3 ���д�������    
       if(ReadBuff[1] & 0x04 == 0 ){ ;} //SW2 ����ָ�Ѿ��뿪
       else                        { ;} //SW2 ���д�������      
       if(ReadBuff[1] & 0x02 == 0 ){ ;} //SW1 ����ָ�Ѿ��뿪
       else                        { ;} //SW1 ���д�������                        
       if(ReadBuff[1] & 0x01 == 0 ){ ;} //SW0 ����ָ�Ѿ��뿪
       else                        { ;} //SW0 ���д�������  

}	