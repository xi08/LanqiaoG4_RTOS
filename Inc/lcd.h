#ifndef __LCD_H
#define __LCD_H

#include "main.h"

typedef enum
{
    lcdDispFR_30 = 0x0000,
    lcdDispFR_50 = 0x0008,
    lcdDispFR_60 = 0x000a,
    lcdDispFR_90 = 0x000d,
} lcdDispFR_t;

#define Line0 0
#define Line1 24
#define Line2 48
#define Line3 72
#define Line4 96
#define Line5 120
#define Line6 144
#define Line7 168
#define Line8 192
#define Line9 216

void LCD_Init(lcdDispFR_t lfr);

void LCD_SetFrontColor(uint16_t lc);
void LCD_SetBackColor(uint16_t lc);

void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
void LCD_SetUpdateWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Height, uint16_t Width);
void LCD_ResetUpdateWindow(void);

void LCD_ClearScreen(void);
void LCD_ClearLine(uint16_t Line);

void LCD_DisplayString(uint16_t Line, char *str);
void LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, char ch);

void LCD_DisplayBMP(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, const uint16_t *BitMap);

#endif // !__LCD_H
