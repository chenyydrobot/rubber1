
#include "public.h"




/* Private variables ---------------------------------------------------------*/
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;

int pwmWidth = 500;

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


extern MotorDataTypeDef MotorData;//ͷ�����

static uint32_t HAL_TIM_WritePWMValue(TIM_HandleTypeDef *htim, uint32_t Channel,uint32_t pwmvalue)
{
  uint32_t tmpreg = 0;

  __HAL_LOCK(htim);

  switch (Channel)
  {
  case TIM_CHANNEL_1:
    {
      /* Check the parameters */
      assert_param(IS_TIM_CC1_INSTANCE(htim->Instance));

      
      htim->Instance->CCR1 = pwmvalue;

      break;
    }
  case TIM_CHANNEL_2:
    {
      /* Check the parameters */
      assert_param(IS_TIM_CC2_INSTANCE(htim->Instance));

      htim->Instance->CCR2 = pwmvalue;

      break;
    }

  case TIM_CHANNEL_3:
    {
      /* Check the parameters */
      assert_param(IS_TIM_CC3_INSTANCE(htim->Instance));

      htim->Instance->CCR3 = pwmvalue;

      break;
    }

  case TIM_CHANNEL_4:
    {
      /* Check the parameters */
      assert_param(IS_TIM_CC4_INSTANCE(htim->Instance));

      htim->Instance->CCR4 = pwmvalue;

      break;
    }

  default:
    break;
  }

  __HAL_UNLOCK(htim);
  return tmpreg;
}

static void control_pwm3_width(int id)
{

		uint32_t channel[4];		
	
		channel[0] = TIM_CHANNEL_1;
		channel[1] = TIM_CHANNEL_2;	
	  channel[2] = TIM_CHANNEL_3;
		channel[3] = TIM_CHANNEL_4;	

		HAL_TIM_WritePWMValue(&htim3,channel[id],pwmWidth);	
				
}

static void control_pwm5_width(int id)
{

		uint32_t channel[4];		
	
		channel[0] = TIM_CHANNEL_1;
		channel[1] = TIM_CHANNEL_2;	
	  channel[2] = TIM_CHANNEL_3;
		channel[3] = TIM_CHANNEL_4;	

		HAL_TIM_WritePWMValue(&htim5,channel[id],pwmWidth);	
				
}


void control_walkingmotor_speed(int id)
{

		uint32_t channel[2];		
	  int speed_PWMOUT;
	
		//channel[0] = TIM_CHANNEL_1;
		//channel[1] = TIM_CHANNEL_2;	
	
	  channel[0] = TIM_CHANNEL_3;
		channel[1] = TIM_CHANNEL_4;

		speed_PWMOUT = 200;//(PIDparam.Kp * speedCurrentErr + PIDparam.Ki * PIDparam.speedErri[id] + PIDparam.Kd * speedCurrentErrd);
		


		HAL_TIM_WritePWMValue(&htim3,channel[id],(int)speed_PWMOUT);	
				
}
//==================�жϴ���Ļص�����====================================
//=============================================================================
//=============================================================================
#define MOTOR_STOP 0
#define MOTOR_START 1

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//�������ŵĻص�����
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the __HAL_TIM_PeriodElapsedCallback could be implemented in the user file
   */
  if(htim == &htim2)//10KHZ=100us----------------------���PWM���Ƶ�TIMER------------------------------------
	{		
      ;
	}
	else if(htim == &htim3)//64M/640=100K/4=25K HZ      == 40us-------------��������źŵļ�����TIMER-----------------
	{ 
     //����������ת������ź�		
		 //1.ͷ�����µ��
		 MotorData.timer++;
	   if(MotorData.timer > 1000)//��ЧΪ100us*100=10ms
		 {
			 MotorData.timer = 0;
					
			 if(MotorData.status == MOTOR_START)
			 {
					control_walkingmotor_speed( MotorData.direction );
					//control_walkingmotor_speed(1);	
			 }				
		 }		
	}	
  else if(htim == &htim5)	
	{ 
		//�����Ƕ�������ź�
    control_pwm5_width(0);	
	}	
}

#define EXTI_GPIO_PIN0    GPIO_PIN_8
bool reTouch=0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	
	  if(GPIO_Pin == EXTI_GPIO_PIN0 )
		{
			 reTouch = true;
		}
}



int8_t msDelay=0;

//ϵͳ1msʱ���жϻص�����
void HAL_SYSTICK_Callback(void)
{
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_SYSTICK_Callback could be implemented in the user file
   */
	  msDelay++;
}




