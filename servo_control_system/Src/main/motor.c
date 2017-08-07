
#include "public.h"


#define MOTOR_STOP 0
#define MOTOR_START 1

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;

void motor_Start_working(uint8_t dir);

typedef struct
{
	 unsigned int timer;//100个时钟溢出中断==100*100微妙=10MS-----------在PWM的TIMER中断中计时作为编码器的速度测量
	
	 float speedSeting[2];//设定速度	
	
   int status;
	 int direction;
	
	 unsigned int controlTime;
	 unsigned int pauseflag;	
	
	 volatile unsigned int 	odometer_M1;//里程计	
}MotorDataTypeDef;

MotorDataTypeDef MotorData;//头部马达

void set_motor_init_param(void)
{
   MotorData.status  = MOTOR_STOP;
	
	 MotorData.direction = 3; //0是向上还是向下，到时要测量一下
	
}	


void motor_Start_working(uint8_t dir)
{	

	MotorData.direction = dir;
	
	if(MotorData.status == MOTOR_STOP)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_SET);//电机供电使能
		MotorData.status = MOTOR_START;
				
		HAL_TIM_PWM_Start_IT(&htim3,TIM_CHANNEL_3);
		HAL_TIM_PWM_Start_IT(&htim3,TIM_CHANNEL_4);			
	}	
	
}


void motor_Stop_working(void)
{
	HAL_TIM_PWM_Stop_IT(&htim3,TIM_CHANNEL_3);
	HAL_TIM_PWM_Stop_IT(&htim3,TIM_CHANNEL_4);	
  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_RESET);//电机供电使能
	MotorData.status = MOTOR_STOP;	//同进启动标志位也要关闭 
}





