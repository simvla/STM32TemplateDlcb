/*stm32 DS18b20 lib by simvla*/

#ifndef _DS18B20_H
#define _DS18B20_H

#include "stm32f1xx_hal.h"

// Public definitions:

#define DS_PIN GPIO_PIN_12
#define DS_PORT GPIOB

// Public func:
void ds18b20_init(void);
void ds18b20_readAddr(uint8_t addr[]);
uint8_t ds18b20_read(char* tstr,uint8_t id[]);





#endif // _DS18B20_H

//this software using STM32 HAL libraries by STMicroelectronics :
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
