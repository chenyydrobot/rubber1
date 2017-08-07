
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
	 unsigned int timer;//100��ʱ������ж�==100*100΢��=10MS-----------��PWM��TIMER�ж��м�ʱ��Ϊ���������ٶȲ���
	
	 float speedSeting[2];//�趨�ٶ�	
	
   int status;
	 int direction;
	
	 unsigned int controlTime;
	 unsigned int pauseflag;	
	
	 volatile unsigned int 	odometer_M1;//��̼�	
}MotorDataTypeDef;

MotorDataTypeDef MotorData;//ͷ�����

void set_motor_init_param(void)
{
   MotorData.status  = MOTOR_STOP;
	
	 MotorData.direction = 3; //0�����ϻ������£���ʱҪ����һ��
	
}	


void motor_Start_working(uint8_t dir)
{	

	MotorData.direction = dir;
	
	if(MotorData.status == MOTOR_STOP)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_SET);//�������ʹ��
		MotorData.status = MOTOR_START;
				
		HAL_TIM_PWM_Start_IT(&htim3,TIM_CHANNEL_3);
		HAL_TIM_PWM_Start_IT(&htim3,TIM_CHANNEL_4);			
	}	
	
}


void motor_Stop_working(void)
{
	HAL_TIM_PWM_Stop_IT(&htim3,TIM_CHANNEL_3);
	HAL_TIM_PWM_Stop_IT(&htim3,TIM_CHANNEL_4);	
  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_RESET);//�������ʹ��
	MotorData.status = MOTOR_STOP;	//ͬ��������־λҲҪ�ر� 
}





