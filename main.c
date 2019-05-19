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

/* USER Program ---------------------------------------------------*/
int main(void)
{
  /* StartupSetup ------------------------------------------------------------*/ 
  //Essential(call for each project)
  HAL_Init(); //"hardwareAbstractionLayer" lib initialize
  SystemClock_Config(); //set sysClock(72MHz)
  
  //Project specific
  OnBoardLED_Init(); //set led port(PC13)
  TFT_LCD_Init(); //set tftLcd gpio and init sequence 
  
  //print message
  TFT_LCD_DisplayStringAtLine(1,(uint8_t*)"WELCOME!"); 
  
  /* Main Loop CyclicExecution ------------------------------------------------*/
  while(1){
   //Led blink 
   HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
   Delay_ms(500);
  }
}





/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

