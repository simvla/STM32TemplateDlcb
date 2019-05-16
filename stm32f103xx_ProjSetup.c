/*stm32f103xx Project Setup(implement functions related to specific project)*/

#include "stm32f103xx_ProjSetup.h"

/* Private variables -------------------------------------------------*/
uint32_t SpixTimeout = SPIx_TIMEOUT_MAX;
static SPI_HandleTypeDef h_Spi;

/* Private functions -------------------------------------------------*/
//SPI IO/COM functions
static void SPIx_GpioInit(void)
{
  GPIO_InitTypeDef  gpioinitstruct = {0};

  //Enable GPIO clock
  SPIx_SCK_GPIO_CLK_ENABLE();
  SPIx_MISO_MOSI_GPIO_CLK_ENABLE();
  //SPI SCK GPIO Pin
  gpioinitstruct.Pin = SPIx_SCK_PIN;
  gpioinitstruct.Mode = GPIO_MODE_AF_PP;
  gpioinitstruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SPIx_SCK_GPIO_PORT, &gpioinitstruct);
  //SPI MISO and MOSI GPIO Pin
  gpioinitstruct.Pin = SPIx_MOSI_PIN;
  HAL_GPIO_Init(SPIx_MISO_MOSI_GPIO_PORT, &gpioinitstruct);

  gpioinitstruct.Pin = SPIx_MISO_PIN;
  gpioinitstruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(SPIx_MISO_MOSI_GPIO_PORT, &gpioinitstruct);
  //Enable SPI clock
  SPIx_CLK_ENABLE();
}

static void SPIx_Init(void)
{
  if(HAL_SPI_GetState(&h_Spi) == HAL_SPI_STATE_RESET)
  {
    /* SPI Config for ST7735 LCD SPI interface */
    h_Spi.Instance = SPIx;
    h_Spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    h_Spi.Init.Direction = SPI_DIRECTION_2LINES;
    h_Spi.Init.CLKPhase = SPI_PHASE_1EDGE;
    h_Spi.Init.CLKPolarity = SPI_POLARITY_LOW;
    h_Spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    h_Spi.Init.CRCPolynomial = 7;
    h_Spi.Init.DataSize = SPI_DATASIZE_8BIT;
    h_Spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
    h_Spi.Init.NSS = SPI_NSS_SOFT;
    h_Spi.Init.TIMode = SPI_TIMODE_DISABLE;
    h_Spi.Init.Mode = SPI_MODE_MASTER;

    SPIx_GpioInit();
    HAL_SPI_Init(&h_Spi);
  }
}

static void SPIx_Write(uint8_t Value)
{
  uint8_t data;

  HAL_SPI_TransmitReceive(&h_Spi, (uint8_t*) &Value, &data, 1, SpixTimeout);
}

//LCD IO/COM functions
void LCD_IO_Init(void)
{
  GPIO_InitTypeDef  gpioinitstruct;

  /* LCD_CS_GPIO and LCD_DC_GPIO Periph clock enable */
  LCD_CS_GPIO_CLK_ENABLE();
  LCD_DC_GPIO_CLK_ENABLE();

  /* Configure LCD_CS_PIN pin: LCD Card CS pin */
  gpioinitstruct.Pin = LCD_CS_PIN;
  gpioinitstruct.Mode = GPIO_MODE_OUTPUT_PP;
  gpioinitstruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LCD_CS_GPIO_PORT, &gpioinitstruct);

  /* Configure LCD_DC_PIN pin: LCD Card DC pin */
  gpioinitstruct.Pin = LCD_DC_PIN;
  HAL_GPIO_Init(LCD_DC_GPIO_PORT, &gpioinitstruct);

  /* LCD chip select high */
  LCD_CS_HIGH();

  /* LCD SPI Config */
  SPIx_Init();
}

void LCD_IO_WriteReg(uint8_t LCDReg)
{
  /* Reset LCD control line CS */
  LCD_CS_LOW();

  /* Set LCD data/command line DC to Low */
  LCD_DC_LOW();

  /* Send Command */
  SPIx_Write(LCDReg);

  /* Deselect : Chip Select high */
  LCD_CS_HIGH();
}

void LCD_IO_WriteMultipleData(uint8_t *pData, uint32_t Size)
{
  uint32_t counter = 0;

  LCD_CS_LOW();
  LCD_DC_HIGH();

  if (Size == 1)
  {
    /* Only 1 byte to be sent to LCD - general interface can be used */
    /* Send Data */
    SPIx_Write(*pData);
  }
  else
  {
    /* Several data should be sent in a raw */
    /* Direct SPI accesses for optimization */
    for (counter = Size; counter != 0; counter--)
    {
      while(((h_Spi.Instance->SR) & SPI_FLAG_TXE) != SPI_FLAG_TXE)
      {
      }
      /* Need to invert bytes for LCD*/
      *((__IO uint8_t*)&h_Spi.Instance->DR) = *(pData+1);

      while(((h_Spi.Instance->SR) & SPI_FLAG_TXE) != SPI_FLAG_TXE)
      {
      }
      *((__IO uint8_t*)&h_Spi.Instance->DR) = *pData;
      counter--;
      pData += 2;
    }

    /* Wait until the bus is ready before releasing Chip select */
    while(((h_Spi.Instance->SR) & SPI_FLAG_BSY) != RESET)
    {
    }
  }
  /* Deselect : Chip Select high */
  LCD_CS_HIGH();
}

void LCD_Delay(uint32_t Delay)
{
  HAL_Delay(Delay);
}

/* Public functions---------------------------------------------------*/
//OnBoard LED(PC13) GPIO config
void OnBoardLED_Init() {

  GPIO_InitTypeDef  OnBoard_led; //Led object

  __HAL_RCC_GPIOC_CLK_ENABLE(); //enable led gpio clock

  //set parameters
  OnBoard_led.Mode = GPIO_MODE_OUTPUT_PP;
  OnBoard_led.Pull = GPIO_PULLUP;
  OnBoard_led.Speed = GPIO_SPEED_FREQ_HIGH;
  OnBoard_led.Pin = LED_PIN;

  HAL_GPIO_Init(LED_PORT, &OnBoard_led); //constructor(initialize gpio with OnBoard_led settings)
  //default state
  HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET); //led off
}

//SysClock config(SysClock source=PLL(HSE),SYSCLK(Hz)=72000000,HSE Freq(Hz)=8000000,PLLMUL=9..)
void SystemClock_Config(void)
{
  //check if jtag pins set as gpio
  #ifdef USE_JTAG_asGPIO
    __HAL_RCC_AFIO_CLK_ENABLE();
    __HAL_AFIO_REMAP_SWJ_NOJTAG();
  #endif

  RCC_OscInitTypeDef oscinitstruct = {0};
  RCC_ClkInitTypeDef clkinitstruct = {0};

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  oscinitstruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
  oscinitstruct.HSEState        = RCC_HSE_ON;
  oscinitstruct.HSEPredivValue  = RCC_HSE_PREDIV_DIV2;
  oscinitstruct.PLL.PLLState    = RCC_PLL_ON;
  oscinitstruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
  oscinitstruct.PLL.PLLMUL      = RCC_PLL_MUL9; //ext osc 8MHz * 9 = 72MHz

  HAL_RCC_OscConfig(&oscinitstruct);

  //Select PLL as system clock source and config the HCLK, PCLK1 and PCLK2 clocks dividers */
  clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
  clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV1;

  HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_0);

}






/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
