/*stm32f103xx Project Setup */

#ifndef _SETUP_H
#define _SETUP_H

#include "stm32f1xx_hal.h"
#include "stm32f103xx_Delay.h"

/* Public definitions ----------------------------------------------------*/
//LED
#define LED_PIN GPIO_PIN_13
#define LED_PORT GPIOC
//SPI
#define SPIx SPI2
#define SPIx_CLK_ENABLE() __HAL_RCC_SPI2_CLK_ENABLE()
#define SPIx_SCK_GPIO_PORT GPIOB
#define SPIx_SCK_PIN GPIO_PIN_13
#define SPIx_SCK_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPIx_SCK_GPIO_CLK_DISABLE() __HAL_RCC_GPIOB_CLK_DISABLE()
#define SPIx_MISO_MOSI_GPIO_PORT GPIOB
#define SPIx_MISO_MOSI_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPIx_MISO_MOSI_GPIO_CLK_DISABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPIx_MISO_PIN GPIO_PIN_14
#define SPIx_MOSI_PIN GPIO_PIN_15
#define SPIx_TIMEOUT_MAX 1000
//TFTLCD
#define LCD_CS_LOW() HAL_GPIO_WritePin(LCD_CS_GPIO_PORT, LCD_CS_PIN, GPIO_PIN_RESET)
#define LCD_CS_HIGH() HAL_GPIO_WritePin(LCD_CS_GPIO_PORT, LCD_CS_PIN, GPIO_PIN_SET)
#define LCD_DC_LOW() HAL_GPIO_WritePin(LCD_DC_GPIO_PORT, LCD_DC_PIN, GPIO_PIN_RESET)
#define LCD_DC_HIGH() HAL_GPIO_WritePin(LCD_DC_GPIO_PORT, LCD_DC_PIN, GPIO_PIN_SET)
#define LCD_CS_PIN GPIO_PIN_9
#define LCD_CS_GPIO_PORT GPIOB
#define LCD_CS_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define LCD_CS_GPIO_CLK_DISABLE() __HAL_RCC_GPIOB_CLK_DISABLE()
#define LCD_DC_PIN GPIO_PIN_8
#define LCD_DC_GPIO_PORT GPIOB
#define LCD_DC_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define LCD_DC_GPIO_CLK_DISABLE() __HAL_RCC_GPIOB_CLK_DISABLE()

//Comment/uncomment this lines if gpio pins not use/use jtag pins
#ifndef USE_JTAG_asGPIO
  #define USE_JTAG_asGPIO
#endif

/* Public functions -------------------------------------------------------*/
void OnBoardLED_Init(void);
void SystemClock_Config(void);




#endif // _SETUP_H

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

