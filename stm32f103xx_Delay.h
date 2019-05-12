/*stm32f103xx (ms , us) delay routines by simvla*/

#ifndef _DELAY_H
#define _DELAY_H

#include "stm32f1xx_hal.h"

#define Delay_ms(ms) HAL_Delay(ms)

/*lib functions -------------------------------------------------*/
void Delay_us(uint16_t us);



#endif // _DELAY_H




//this software using STM32 HAL libraries by STMicroelectronics :
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/














