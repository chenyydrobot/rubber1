/**
  ******************************************************************************
  * File Name          : loop_main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */



#include "loop_main.h"

#define  SERVO_STEP  40
#define  SERVO_ZERO_POINT 1500

typedef unsigned short u16;

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;


extern void  WTC7514_init_param( void );
extern void main11(void);

extern void set_motor_init_param(void);
void motor_Start_working(uint8_t dir);
extern void set_motor_walking(int8_t dir);

extern void WTC7514_init_gpio(void);
extern void WTC7514_init_param(void);
extern void WTC7514_read_param(void);

void servo_turn_left(void);
void servo_turn_right(void);

uint32_t  adcValue[100] = {0};
    float  valueUmber;
int8_t direct = 1;

void loop_app(void)
{
	  
		HAL_TIM_Base_Start_IT(&htim3);
		HAL_TIM_Base_Start_IT(&htim5);//定时时间不能一样
	   
	  set_motor_init_param();
	  HAL_TIM_Base_Start_IT(&htim2);
		 
	  HAL_TIM_PWM_Start_IT(&htim5,TIM_CHANNEL_1); //舵机控制通道	
	
	  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);//触摸传感器使能
	
	
    /* 这个测试专用 */ 	
	  //GPIO_InitTypeDef GPIO_InitStructure; 
	  //GPIO_InitStructure.Pin   = GPIO_PIN_13;//A7   
	  //GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	  //GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	  //GPIO_InitStructure.Pull = GPIO_NOPULL;//GPIO_NOPULL;
	  //HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	  

	  //脉宽停止函数
	  //HAL_TIM_PWM_Stop_IT;
		//HAL_TIM_PWM_Stop_IT;
		
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&adcValue, 2);
				
		//GPIO_InitTypeDef GPIO_InitStruct;
	  //GPIO_InitStruct.Pin = GPIO_PIN_13;
    //GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    //GPIO_InitStruct.Pull = GPIO_NOPULL;
	  //GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    //HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    //HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);

	  /* 对iic 进行测试 */
	  AW9106_gpio_left_Init();
	  AW9106_left_init();
	
	  AW9106_gpio_right_Init();
	  AW9106_right_init();
		
		AW9106_gpio_front_Init();
	  AW9106_front_init();
		
		WTC7514_init_gpio();
		WTC7514_init_param();
	 
		//MotorData.status = 1;
    while(1)
    {
		
       if(msDelay > 100)
			 {  	 
				 servo_turn_left();
			   servo_turn_right(); 	 
				 motor_Stop_working(0);

				 AW9523_breath_front_loop();
				 AW9523_breath_right_loop();
				 AW9523_breath_left_loop();
				 msDelay = 0; 
       } 				 
			 motor_Start_working(0);
			 valueUmber = (( adcValue[0]&0x00000fff )*3.3 / 4096.0)*180/3.14; 
			 //valueUmber -= 90;
			 

			//读触摸信号
			if(reTouch == true )
			{
			    //读触摸iic,并用串口发送
				  WTC7514_read_param();
				  reTouch = false;
				  ;
			}	
		}
}	

void servo_turn_left(void)
{
    if(direct == 1)
		{	 
        //左右方向控制	
        if(pwmWidth < 2500)
				{ 	 
           pwmWidth += SERVO_STEP;	          
				}
				else
					direct = -1;
		}
}	

void servo_turn_right(void)
{
  	if(direct == -1)		
    {					 
       if(pwmWidth > 500)
			  { 	 
           pwmWidth -= SERVO_STEP;		//左右方向控制//pwmWidth = 500;							
				}
			  else
				  direct = 1;
    }
}

void turn_zero_point(void)
{
    if(direct == 0)		
    {	
			  if(pwmWidth > SERVO_ZERO_POINT)
			  { 		
					pwmWidth -= SERVO_STEP;
        }

        if(pwmWidth < SERVO_ZERO_POINT)
			  { 		
					pwmWidth += SERVO_STEP;
        }				
		}
}	