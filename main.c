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
  //System 
  HAL_Init(); //"hardwareAbstractionLayerLib initialize"-call for each new proj
  SystemClock_Config(); //set sysClock(72MHz)-call for each new proj
  //User 
  OnBoardLED_Init();//set Led port(PC13)
  TFT_LCD_Init();//set tftLcd gpio and init sequence 

  //Do on startup
  TFT_LCD_DisplayStringAtLine(1,(uint8_t*)"WELCOME!"); //print message on tftLcd

  /* Main Loop CyclicExecution-------------------------------------*/
  while(1){
   //Led blink 
   HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
   Delay_ms(500);
  }
}





/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

