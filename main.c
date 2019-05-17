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
  /* ProjConfiguration-------------------------------------------------------------*/ 
  //System config(important calls for each project)
  HAL_Init(); //"hardwareAbstractionLayerLib" initialize
  SystemClock_Config(); //set sysClock(72MHz)
  //User config(project specific)
  OnBoardLED_Init();//set Led port(PC13)
  TFT_LCD_Init();//set tftLcd gpio and init sequence 

  //Startup code
  TFT_LCD_DisplayStringAtLine(1,(uint8_t*)"WELCOME!"); //print message on tftLcd

  /* Main Loop CyclicExecution-------------------------------------------------*/
  while(1){
   //Led blink 
   HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
   Delay_ms(500);
  }
}





/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

