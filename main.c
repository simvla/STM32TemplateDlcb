/* STM32 base template for Data Log and Control Board */

#include "stm32f1xx_hal.h"
#include "delay_stm32_hal.h"
#include "DS18b20_stm32_hal.h"
#include "st7735_stm32_hal.h"
#include "Util.h"
#include "string.h"

// App Definitions:
#define USE_JTAG_asGPIO

// App Vars:
// DS18b20
char tempStr[10];
uint8_t addr1[8]={0}; //only one sensor
// System
uint32_t dispBlTick = 0; //display backlight off time
// Tft
extern const uint16_t lcd_bck_img[]; //Lcd background image

// Function prototypes:
void SetPinsAsIn(void); // set one or more pins as input
void SetPinsAsOut(void); // set one or more pins as output
void SystemClock_Config(void); // !!!

int main(void)
{
  /* OnStart */
  // <each app must include
  HAL_Init();
  SystemClock_Config();
  // >
  ds18b20_init();
  tftLcd_init();
  tftLcd_setBgColor(0x7db3);

  SetPinsAsOut();
  SetPinsAsIn();

  //tftLcd_clear(); //draw new bckg color
  // draw image - landscape mode
  uint16_t h=128, w=160, row, col, buffidx=0;//temp loop variables
  for (row=0; row<h; row++){
    for (col=0; col<w; col++){
      tftLcd_drawPixel(col, row, lcd_bck_img[buffidx]);
      buffidx++;
    }
  }

  // write static text
  tftLcd_setTextSize(2); //new text size
  tftLcd_setTextColor(_WHITE); //new text color
  tftLcd_setTransparentStaticText(1);
  tftLcd_println("T:",0,0);
  tftLcd_println("C",7,0);
  tftLcd_setTransparentStaticText(0);

  // set current tick value before main loop
  dispBlTick=HAL_GetTick(); //tft backlight start time

  /* Cyclic */
  while(1){
    // Led blink
    HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
    Delay_ms(300);

    // Measure
    ds18b20_read(tempStr,addr1);
    tftLcd_println(tempStr,2,0);

    // Disp BL off after time expired
    if ((HAL_GetTick() - dispBlTick) > 300000) { //5min
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
    }

    // Disp BL On when TS1 pressed
    if(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)) {
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
      Delay_ms(200); //wait for release
      dispBlTick = HAL_GetTick(); //set new time
    }

    // TftLcd test
    //tftLcd_println("Hello!",0,0);

  }

}


void SetPinsAsIn(void){

  GPIO_InitTypeDef ioport;

  __HAL_RCC_GPIOA_CLK_ENABLE();

  ioport.Pin = GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_15; //buttons
  ioport.Mode  = GPIO_MODE_INPUT;
  ioport.Pull  = GPIO_PULLUP;
  ioport.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOA, &ioport);

}

void SetPinsAsOut(void){

  GPIO_InitTypeDef ioport;

  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  ioport.Pin = GPIO_PIN_6; //TftBL
  ioport.Mode  = GPIO_MODE_OUTPUT_PP;
  ioport.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOB, &ioport);

  ioport.Pin=GPIO_PIN_13; //Led
  HAL_GPIO_Init(GPIOC, &ioport);

  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); //led off
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET); //tftLcd backlight on

}

// Each app must implement this function as is
// or create custom clock configuration
void SystemClock_Config(void) {
  
  // Use jtag pins as gpio
  #ifdef USE_JTAG_asGPIO
    __HAL_RCC_AFIO_CLK_ENABLE();
    __HAL_AFIO_REMAP_SWJ_NOJTAG();
  #endif

  RCC_ClkInitTypeDef clkinitstruct = {0};
  RCC_OscInitTypeDef oscinitstruct = {0};

  // Enable HSE Oscillator and activate PLL with HSE as source
  oscinitstruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
  oscinitstruct.HSEState        = RCC_HSE_ON;
  oscinitstruct.HSEPredivValue  = RCC_HSE_PREDIV_DIV2;
  oscinitstruct.PLL.PLLState    = RCC_PLL_ON;
  oscinitstruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
  oscinitstruct.PLL.PLLMUL      = RCC_PLL_MUL9; //ext osc 8MHz * 9 = 72MHz
  HAL_RCC_OscConfig(&oscinitstruct);

  // Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
  clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
  clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_0);

}







//this software using STM32 HAL libraries by STMicroelectronics :
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

