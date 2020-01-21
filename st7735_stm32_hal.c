/*1.8 inch color tft lcd c lib by simvla
 *based on adafruit libs
 */

#include "stm32f1xx_hal.h"
#include "delay_stm32_hal.h"

#include "st7735_stm32_hal.h"

// Private definitions:
// Disp size
#define ST7735_WIDTH  128
#define ST7735_HEIGHT 160
// Disp commands
#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09
#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13
#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E
#define ST7735_PTLAR   0x30
#define ST7735_COLMOD  0x3A
#define ST7735_MADCTL  0x36
#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6
#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5
#define ST7735_RDID1   0xDA
#define ST7735_RDID2   0xDB
#define ST7735_RDID3   0xDC
#define ST7735_RDID4   0xDD
#define ST7735_PWCTR6  0xFC
#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1
// Disp io
#define CS_HI HAL_GPIO_WritePin(LCD_CS_GPIO_PORT, LCD_CS_PIN, GPIO_PIN_SET)
#define CS_LO HAL_GPIO_WritePin(LCD_CS_GPIO_PORT, LCD_CS_PIN, GPIO_PIN_RESET)
#define RS_HI HAL_GPIO_WritePin(LCD_DC_GPIO_PORT, LCD_DC_PIN, GPIO_PIN_SET)
#define RS_LO HAL_GPIO_WritePin(LCD_DC_GPIO_PORT, LCD_DC_PIN, GPIO_PIN_RESET)

// Private vars:
extern const unsigned char font[];
uint8_t  textsize, statTransparentText;
uint16_t _width, _height, textcolor, bgcolor;
static SPI_HandleTypeDef h_Spi;

// Private func:
void spi_init(void);
void spi_write(uint8_t byte);

void st7735_initIO(void);
void st7735_writecmd(uint8_t c);
void st7735_writedata(uint8_t d);

void st7735_setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void st7735_writePixel(int16_t x, int16_t y, uint16_t color);
void st7735_drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void st7735_drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void st7735_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void st7735_drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
void st7735_setRotation(uint8_t r);
void st7735_commandList(const uint8_t *addr); //read init commands array

uint16_t st7735_swapcolor(uint16_t x);


void spi_init(void) {

  GPIO_InitTypeDef  spiGpio = {0};

  __HAL_RCC_SPI2_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  spiGpio.Pin        = GPIO_PIN_13|GPIO_PIN_15; // SCK, MOSI
  spiGpio.Mode       = GPIO_MODE_AF_PP;
  spiGpio.Speed      = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &spiGpio);

  spiGpio.Pin  = GPIO_PIN_14; // MISO
  spiGpio.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(GPIOB, &spiGpio);

  // Config SPI
  h_Spi.Instance                = SPI2;
  h_Spi.Init.BaudRatePrescaler  = SPI_BAUDRATEPRESCALER_8;
  h_Spi.Init.Direction          = SPI_DIRECTION_2LINES;
  h_Spi.Init.CLKPhase           = SPI_PHASE_1EDGE;
  h_Spi.Init.CLKPolarity        = SPI_POLARITY_LOW;
  h_Spi.Init.CRCCalculation     = SPI_CRCCALCULATION_DISABLE;
  h_Spi.Init.CRCPolynomial      = 7;
  h_Spi.Init.DataSize           = SPI_DATASIZE_8BIT;
  h_Spi.Init.FirstBit           = SPI_FIRSTBIT_MSB;
  h_Spi.Init.NSS                = SPI_NSS_SOFT;
  h_Spi.Init.TIMode             = SPI_TIMODE_DISABLE;
  h_Spi.Init.Mode               = SPI_MODE_MASTER;

  HAL_SPI_Init(&h_Spi);
}

void spi_write(uint8_t byte) {
  uint8_t data;
  HAL_SPI_TransmitReceive(&h_Spi, (uint8_t*) &byte, &data, 1, 1000);
}

void st7735_initIO(void){

  GPIO_InitTypeDef  gpioinitstruct;

  LCD_CS_GPIO_CLK_ENABLE();
  LCD_DC_GPIO_CLK_ENABLE();

  gpioinitstruct.Pin    = LCD_CS_PIN;
  gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
  gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LCD_CS_GPIO_PORT, &gpioinitstruct);

  gpioinitstruct.Pin    = LCD_DC_PIN;
  HAL_GPIO_Init(LCD_DC_GPIO_PORT, &gpioinitstruct);

  CS_HI;;

}

void st7735_writecmd(uint8_t c){
  RS_LO;
  CS_LO;

  spi_write(c);

  CS_HI;
}

void st7735_writedata(uint8_t d){
  RS_HI;
  CS_LO;

  spi_write(d);

  CS_HI;
}

void st7735_setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){
  st7735_writecmd(ST7735_CASET); // Column addr set
  st7735_writedata(0x00);
  st7735_writedata(x0);     // XSTART
  st7735_writedata(0x00);
  st7735_writedata(x1);     // XEND

  st7735_writecmd(ST7735_RASET); // Row addr set
  st7735_writedata(0x00);
  st7735_writedata(y0);     // YSTART
  st7735_writedata(0x00);
  st7735_writedata(y1);     // YEND

  st7735_writecmd(ST7735_RAMWR); // write to RAM
}

void st7735_writePixel(int16_t x, int16_t y, uint16_t color){
  // limit
  if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;

  st7735_setAddrWindow(x,y,x+1,y+1);
  color = st7735_swapcolor(color); //need to invert bytes

  RS_HI;
  CS_LO;

  spi_write(color >> 8);
  spi_write(color);

  CS_HI;
}

void st7735_drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color){
  // limit
  if((x >= _width) || (y >= _height)) return;
  if((y+h-1) >= _height) h = _height-y;

  st7735_setAddrWindow(x, y, x, y+h-1);
  color = st7735_swapcolor(color);

  uint8_t hi = color >> 8, lo = color;

  RS_HI;
  CS_LO;

  while (h--) {
    spi_write(hi);
    spi_write(lo);
  }

  CS_HI;
}

void st7735_drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color){
  // limit
  if((x >= _width) || (y >= _height)) return;
  if((x+w-1) >= _width)  w = _width-x;

  st7735_setAddrWindow(x, y, x+w-1, y);
  color = st7735_swapcolor(color);

  uint8_t hi = color >> 8, lo = color;

  RS_HI;
  CS_LO;

  while (w--) {
    spi_write(hi);
    spi_write(lo);
  }

  CS_HI;
}

void st7735_fillRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color){
  // limit
  if((x >= _width) || (y >= _height)) return;
  if((x + w - 1) >= _width)  w = _width  - x;
  if((y + h - 1) >= _height) h = _height - y;

  st7735_setAddrWindow(x, y, x+w-1, y+h-1);
  color = st7735_swapcolor(color);

  uint8_t hi = color >> 8, lo = color;

  RS_HI;
  CS_LO;

  for(y=h; y>0; y--) {
    for(x=w; x>0; x--) {
      spi_write(hi);
      spi_write(lo);
    }
  }

  CS_HI;
}

void st7735_drawChar(int16_t x, int16_t y, unsigned char c,uint16_t color, uint16_t bg, uint8_t size){
  uint8_t i,j;
  if((x >= _width)		|| // clip right
     (y >= _height)		|| // clip bottom
     ((x + 6 * size - 1) < 0)	|| // clip left
     ((y + 8 * size - 1) < 0))     // clip top
    return;

  for (i=0; i<6; i++ ) {
    uint8_t line;
    if (i == 5)
      line = 0x0;
    else
      line = *(font+(c*5)+i);
    for (j=0; j<8; j++) {
      if (line & 0x1) {
        if (size == 1) // default size
          st7735_writePixel(x+i, y+j, color);
        else {  // big size
          st7735_fillRect(x+(i*size), y+(j*size), size, size, color);
        }
      } else if (!statTransparentText) { //static transparent text
        if (size == 1) // default size
          st7735_writePixel(x+i, y+j, bg);
        else {  // big size
          st7735_fillRect(x+i*size, y+j*size, size, size, bg);
        }
      }
      line >>= 1;
    }
  }
}

#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x08
#define MADCTL_MH  0x04

void st7735_setRotation(uint8_t r){
  st7735_writecmd(ST7735_MADCTL);

  switch (r) {
   case 0:
    st7735_writedata(MADCTL_MX | MADCTL_MY | MADCTL_RGB);
    _width  = ST7735_WIDTH;
    _height = ST7735_HEIGHT;
    break;
   case 1:
    st7735_writedata(MADCTL_MY | MADCTL_MV | MADCTL_RGB);
    _width  = ST7735_HEIGHT;
    _height = ST7735_WIDTH;
    break;
  }
}

void tftLcd_setTextColor(uint16_t c){
  textcolor=c;
}

void tftLcd_setTextSize(uint8_t s){
  textsize=s;
}

void tftLcd_setBgColor(uint16_t color){
 bgcolor=color;
}

void tftLcd_clear(void){
 st7735_fillRect(0, 0,_width, _height, bgcolor);
}

//suport breakdown text in newline '\n' char
//pixel position calc, based on text size, ex: textsize=2
//base text size is 1 (7*5px),size 2 is (14*10px)+ 2space become (16*12px)+ start from x,y(2)
//start: col(2),row(2) ,next line: 2,14+2+2(18) .. 2,3*16+2(50) .. 14,50 ..
void tftLcd_print(const char * text, int16_t col, int16_t row){

  while(*text){
    if (*text == '\n') {
      row += textsize*8;
      col = 0;
    } else if (*text == '\r') {
      // skip em
    } else {
      st7735_drawChar(col, row, *text, textcolor, bgcolor, textsize);
      col += textsize*6;
    }
    text++;
  }
}

//print text at specified line,line based position,ex: cur(0),line(0) 0,1 1,3 ..
void tftLcd_println(const char * text, uint8_t cpos, uint8_t line){

  uint8_t col,row;
  //start position
  row=(line*(textsize*8))+2;
  col=(cpos*(textsize*6))+2;

  while(*text){
    st7735_drawChar(col, row, *text, textcolor, bgcolor, textsize);
    cpos++;
    col=(cpos*(textsize*6))+2;
    text++;
  }
}

inline void tftLcd_drawPixel(uint16_t col, uint16_t row,uint16_t c){
  st7735_writePixel(col,row,c);
}

inline void tftLcd_setTransparentStaticText(uint8_t con){
  statTransparentText=con;
}

// DISP_INIT_Commands
#define DELAY 0x80
const unsigned char
  Rcmd[] = {                 // Init for 7735R chip
    21,                       // 21 commands in list:
    ST7735_SWRESET,   DELAY,  //  1: Software reset, 0 args, w/delay
      150,                    //     150 ms delay
    ST7735_SLPOUT ,   DELAY,  //  2: Out of sleep mode, 0 args, w/delay
      255,                    //     500 ms delay
    ST7735_FRMCTR1, 3      ,  //  3: Frame rate ctrl - normal mode, 3 args:
      0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR2, 3      ,  //  4: Frame rate control - idle mode, 3 args:
      0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR3, 6      ,  //  5: Frame rate ctrl - partial mode, 6 args:
      0x01, 0x2C, 0x2D,       //     Dot inversion mode
      0x01, 0x2C, 0x2D,       //     Line inversion mode
    ST7735_INVCTR , 1      ,  //  6: Display inversion ctrl, 1 arg, no delay:
      0x07,                   //     No inversion
    ST7735_PWCTR1 , 3      ,  //  7: Power control, 3 args, no delay:
      0xA2,
      0x02,                   //     -4.6V
      0x84,                   //     AUTO mode
    ST7735_PWCTR2 , 1      ,  //  8: Power control, 1 arg, no delay:
      0xC5,                   //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
    ST7735_PWCTR3 , 2      ,  //  9: Power control, 2 args, no delay:
      0x0A,                   //     Opamp current small
      0x00,                   //     Boost frequency
    ST7735_PWCTR4 , 2      ,  // 10: Power control, 2 args, no delay:
      0x8A,                   //     BCLK/2, Opamp current small & Medium low
      0x2A,
    ST7735_PWCTR5 , 2      ,  // 11: Power control, 2 args, no delay:
      0x8A, 0xEE,
    ST7735_VMCTR1 , 1      ,  // 12: Power control, 1 arg, no delay:
      0x0E,
    ST7735_INVOFF , 0      ,  // 13: Don't invert display, no args, no delay
    ST7735_MADCTL , 1      ,  // 14: Memory access control (directions), 1 arg:
      0xC8,                   //     row addr/col addr, bottom to top refresh
    ST7735_COLMOD , 1      ,  // 15: set color mode, 1 arg, no delay:
      0x05  ,                 //     16-bit color
    ST7735_CASET  , 4      ,  // 16: Column addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x7F,             //     XEND = 127
    ST7735_RASET  , 4      ,  // 17: Row addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x9F  ,           //     XEND = 159
    ST7735_GMCTRP1, 16      , // 18: gama+, 16 args, no delay:
      0x02, 0x1c, 0x07, 0x12,
      0x37, 0x32, 0x29, 0x2d,
      0x29, 0x25, 0x2B, 0x39,
      0x00, 0x01, 0x03, 0x10,
    ST7735_GMCTRN1, 16      , // 19: Sparkles and rainbows, 16 args, no delay:
      0x03, 0x1d, 0x07, 0x06,
      0x2E, 0x2C, 0x29, 0x2D,
      0x2E, 0x2E, 0x37, 0x3F,
      0x00, 0x00, 0x02, 0x10,
    ST7735_NORON  ,    DELAY, // 20: Normal display on, no args, w/delay
      10,                     //     10 ms delay
    ST7735_DISPON ,    DELAY, // 21: Main screen turn on, no args w/delay
      100 };                  //     100 ms delay

void tftLcd_init(void){

  st7735_initIO();
  spi_init(); // !!!

  bgcolor=_WHITE;
  textsize=1; textcolor=_BLACK; //default text size,color
  statTransparentText=0;

  CS_LO;
  Delay_ms(1);

  st7735_commandList(Rcmd);

  st7735_setRotation(ROTATION);
  st7735_fillRect(0, 0,_width, _height, bgcolor); //default background color
}

void st7735_commandList(const uint8_t *addr){
  uint8_t  numCommands, numArgs;
  uint16_t ms;

  numCommands = *(addr++);                  // Number of commands to follow
  while(numCommands--) {                 // For each command...
    st7735_writecmd(*(addr++));             // Read, issue command
    numArgs  = *(addr++);                   // Number of args to follow
    ms       = numArgs & DELAY;          // If hibit set, delay follows args
    numArgs &= ~DELAY;                   // Mask out delay bit
    while(numArgs--) {                   // For each argument...
      st7735_writedata(*(addr++));          // Read, issue argument
    }

    if(ms) {
      ms = *(addr++);                // Read post-command delay time (ms)
      if(ms == 255) ms = 500;     // If 255, delay for 500 ms
      Delay_ms(ms);
    }
  }
}

uint16_t st7735_swapcolor(uint16_t x){
  return (x << 11) | (x & 0x07E0) | (x >> 11);
}




//this software using STM32 HAL libraries by STMicroelectronics :
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/









