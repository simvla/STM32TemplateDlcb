/*stm32f103xx (ms , us) delay routines by simvla*/

#include "delay_stm32_hal.h"
#include "stm32f1xx_hal_tim.h"

// Private functions:
void __attribute__ ((constructor)) TIM2_DelayTimer_init(void); //call before main()

// Private vars:
static TIM_HandleTypeDef timer;//timer handle struct,register(TIM_TypeDef Instance),parm(TIM_Base_InitTyprDef Init)...

// max 65535 micros
void Delay_us(uint16_t us)
{
  HAL_TIM_Base_Start(&timer);
  while(1){
    if(__HAL_TIM_GET_COUNTER(&timer)==us){
      HAL_TIM_Base_Stop(&timer);
      TIM2->CNT=0;
      break;
      }
  }
}

void TIM2_DelayTimer_init(){

  __HAL_RCC_TIM2_CLK_ENABLE();

  timer.Instance=TIM2; //generic timer instance is timer2(TIM2)
  /*parm*/
  timer.Init.Prescaler = 36; //APB1CLK=36MHZ
  timer.Init.CounterMode = TIM_COUNTERMODE_UP;
  timer.Init.Period = 65535; //auto reload value
  timer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  timer.Init.RepetitionCounter = 0;

  HAL_TIM_Base_Init(&timer);//timer2 register init
  //HAL_TIM_Base_Start(&timer);

}



//this software using STM32 HAL libraries by STMicroelectronics :
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


