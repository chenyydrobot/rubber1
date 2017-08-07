





/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LOOP_MAIN_H
#define __LOOP_MAIN_H
  /* Includes ------------------------------------------------------------------*/
#include "public.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/* Private variables ---------------------------------------------------------*/
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;

extern void AW9106_gpio_left_Init(void);
extern void AW9106_left_init(void);
extern void AW9523_breath_left_loop(void);

extern void AW9106_gpio_right_Init(void);
extern void AW9106_right_init(void);
extern void AW9523_breath_right_loop(void);

extern void AW9106_gpio_front_Init(void);
extern void AW9106_front_init(void);
extern void AW9523_breath_front_loop(void);

extern bool reTouch;
extern int8_t msDelay;
extern int pwmWidth ;
/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */