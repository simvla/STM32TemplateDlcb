/**
  ******************************************************************************
  * @file    stm32_adafruit_lcd.h
  * @author  MCD Application Team
  * @version V3.0.0
  * @date    23-December-2016
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32_adafruit_lcd.c driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32_ADAFRUIT_LCD_H
#define __STM32_ADAFRUIT_LCD_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "st7735.h"
#include "Fonts/fonts.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32_ADAFRUIT
  * @{
  */

/** @addtogroup STM32_ADAFRUIT_LCD
  * @{
  */


/** @defgroup STM32_ADAFRUIT_LCD_Exported_Types
  * @{
  */

/**
  * @brief  Draw Properties structures definition
  */
typedef struct
{
  uint32_t TextColor;
  uint32_t BackColor;
  sFONT    *pFont;

}LCD_DrawPropTypeDef;

/**
  * @brief  Point structures definition
  */
typedef struct
{
  int16_t X;
  int16_t Y;

}Point, * pPoint;

/**
  * @}
  */

/** @defgroup STM32_ADAFRUIT_LCD_Exported_Constants
  * @{
  */

#define __IO    volatile

/**
  * @brief  LCD status structure definition
  */
#define LCD_OK         0x00
#define LCD_ERROR      0x01
#define LCD_TIMEOUT    0x02

/**
  * @brief  LCD color
  */
#define LCD_COLOR_BLACK         0x0000
#define LCD_COLOR_GREY          0xF7DE
#define LCD_COLOR_BLUE          0x001F
#define LCD_COLOR_RED           0xF800
#define LCD_COLOR_GREEN         0x07E0
#define LCD_COLOR_CYAN          0x07FF
#define LCD_COLOR_MAGENTA       0xF81F
#define LCD_COLOR_YELLOW        0xFFE0
#define LCD_COLOR_WHITE         0xFFFF

/**
  * @brief LCD default font
  */
#define LCD_DEFAULT_FONT         Font16

/**
  * @}
  */

/** @defgroup STM32_ADAFRUIT_LCD_Exported_Functions
  * @{
  */
uint8_t  TFT_LCD_Init(void);
uint32_t TFT_LCD_GetXSize(void);
uint32_t TFT_LCD_GetYSize(void);

uint16_t TFT_LCD_GetTextColor(void);
uint16_t TFT_LCD_GetBackColor(void);
void     TFT_LCD_SetTextColor(__IO uint16_t Color);
void     TFT_LCD_SetBackColor(__IO uint16_t Color);
void     TFT_LCD_SetFont(sFONT *fonts);
sFONT    *TFT_LCD_GetFont(void);

void     TFT_LCD_Clear(uint16_t Color);
void     TFT_LCD_ClearStringLine(uint16_t Line);
void     TFT_LCD_DisplayStringAtLine(uint16_t Line, uint8_t *ptr);
void     TFT_LCD_DisplayStringAt(uint16_t Xpos, uint8_t Ypos, uint8_t *Text);
void     TFT_LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii);

void     TFT_LCD_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGB_Code);
void     TFT_LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void     TFT_LCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void     TFT_LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void     TFT_LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void     TFT_LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void     TFT_LCD_DrawPolygon(pPoint Points, uint16_t PointCount);
void     TFT_LCD_DrawEllipse(int Xpos, int Ypos, int XRadius, int YRadius);
void     TFT_LCD_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pBmp);
void     TFT_LCD_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void     TFT_LCD_FillCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
void     TFT_LCD_FillPolygon(pPoint Points, uint16_t PointCount);
void     TFT_LCD_FillEllipse(int Xpos, int Ypos, int XRadius, int YRadius);

void     TFT_LCD_DisplayOff(void);
void     TFT_LCD_DisplayOn(void);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32_ADAFRUIT_LCD_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/