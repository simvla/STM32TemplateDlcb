/*
  ******************************************************************************
  * Stm32f103xx project template
  * by: simvla
  ******************************************************************************
 */

/* USER includes --------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "stm32f103xx_ProjSetup.h"
#include "stm32f103xx_Delay.h"
#include "Drivers/LCD/stm32_adafruit_lcd.h"
/* USER definitions -----------------------------------------------*/
/* USER variables -------------------------------------------------*/
/* USER functions -------------------------------------------------*/

/* USER Program control flow---------------------------------------*/
int main(void)
{
  /* StartupSetup-------------------------------------------------*/
  HAL_Init(); //hardwareAbstractionLayer lib
  SystemClock_Config(); //sysClock(72MHz)
  OnBoardLED_Init();//PC13
  TFT_LCD_Init();//tftLcd set

  //print message
  TFT_LCD_DisplayStringAtLine(1,(uint8_t*)"WELCOME!");

  /* Main Loop CyclicExecution-------------------------------------*/
  while(1){
    HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
    Delay_ms(500);
  }
}





/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

