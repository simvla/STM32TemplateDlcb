/*1.8 inch color tft lcd c lib by simvla
 *based on adafruit libs
 */

#ifndef __TFTLCD_H
#define __TFTLCD_H


// Public definitions:
// control pins
#define LCD_CS_PIN               GPIO_PIN_9
#define LCD_CS_GPIO_PORT         GPIOB
#define LCD_CS_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()

#define LCD_DC_PIN               GPIO_PIN_8
#define LCD_DC_GPIO_PORT         GPIOB
#define LCD_DC_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()

// set rotation
#define ROTATION 1 //0-PORTRAIT 1-LANDSCAPE

// colors
#define	_BLACK   0x0000
#define	_BLUE    0x001F
#define	_RED     0xF800
#define	_GREEN   0x07E0
#define _CYAN    0x07FF
#define _MAGENTA 0xF81F
#define _YELLOW  0xFFE0
#define _WHITE   0xFFFF


// Public func:
void tftLcd_init(void); // defaults : text size 1, text color black, bg color white, com interface SPI2!!!
void tftLcd_setTextColor(uint16_t c);
void tftLcd_setTextSize(uint8_t s);
void tftLcd_print(const char * text, int16_t col, int16_t row);
void tftLcd_println(const char * text, uint8_t cpos, uint8_t line);
void tftLcd_setBgColor(uint16_t color);
void tftLcd_clear(void); //fill screen with Bg color;
void tftLcd_drawPixel(uint16_t col, uint16_t row,uint16_t c);
void tftLcd_setTransparentStaticText(uint8_t con);





#endif // __TFTLCD_H




//this software using STM32 HAL libraries by STMicroelectronics :
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/




