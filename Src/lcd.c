#include "lcd.h"
#include "font.h"
#include "swDelay.h"
#include <rtthread.h>

uint16_t lcdBackColor = 0x0000, lcdFrontColor = 0xffff;

#define lcdDelay() (__NOP, __NOP, __NOP, __NOP, __NOP, __NOP, __NOP, __NOP, __NOP)
#define lcdDelayL(t) (delay_ms(t));

/**
 * @brief Set bus to input mode
 *
 */
void LCD_BusIn(void)
{
    LL_GPIO_SetPinMode(LC0_GPIO_Port, LC0_Pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(LC1_GPIO_Port, LC1_Pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(LC2_GPIO_Port, LC2_Pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(LC3_GPIO_Port, LC3_Pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(LC4_GPIO_Port, LC4_Pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(LC5_GPIO_Port, LC5_Pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(LC6_GPIO_Port, LC6_Pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(LC7_GPIO_Port, LC7_Pin, LL_GPIO_MODE_INPUT);

    LL_GPIO_SetPinMode(LD0_GPIO_Port, LD0_Pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(LD1_GPIO_Port, LD1_Pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(LD2_GPIO_Port, LD2_Pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(LD3_GPIO_Port, LD3_Pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(LD4_GPIO_Port, LD4_Pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(LD5_GPIO_Port, LD5_Pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(LD6_GPIO_Port, LD6_Pin, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinMode(LD7_GPIO_Port, LD7_Pin, LL_GPIO_MODE_INPUT);
}

/**
 * @brief Set bus to output mode
 *
 */
void LCD_BusOut(void)
{
    LL_GPIO_SetPinMode(LC0_GPIO_Port, LC0_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LC1_GPIO_Port, LC1_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LC2_GPIO_Port, LC2_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LC3_GPIO_Port, LC3_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LC4_GPIO_Port, LC4_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LC5_GPIO_Port, LC5_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LC6_GPIO_Port, LC6_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LC7_GPIO_Port, LC7_Pin, LL_GPIO_MODE_OUTPUT);

    LL_GPIO_SetPinMode(LD0_GPIO_Port, LD0_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LD1_GPIO_Port, LD1_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LD2_GPIO_Port, LD2_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LD3_GPIO_Port, LD3_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LD4_GPIO_Port, LD4_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LD5_GPIO_Port, LD5_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LD6_GPIO_Port, LD6_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LD7_GPIO_Port, LD7_Pin, LL_GPIO_MODE_OUTPUT);
}

/**
 * @brief Write the register
 *
 * @param regAddr Register address
 * @param regVal Register value
 */
void LCD_WriteREG(uint8_t regAddr, uint16_t regVal)
{
    LL_GPIO_ResetOutputPin(nCS_GPIO_Port, nCS_Pin);

    LL_GPIO_ResetOutputPin(RS_GPIO_Port, RS_Pin);
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);
    LL_GPIO_WriteOutputPort(LC0_GPIO_Port, regAddr);
    LL_GPIO_ResetOutputPin(nWR_GPIO_Port, nWR_Pin);
    lcdDelay();
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);

    LL_GPIO_SetOutputPin(RS_GPIO_Port, RS_Pin);
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);
    LL_GPIO_WriteOutputPort(LC0_GPIO_Port, regVal);
    LL_GPIO_ResetOutputPin(nWR_GPIO_Port, nWR_Pin);
    lcdDelay();
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);

    LL_GPIO_SetOutputPin(nCS_GPIO_Port, nCS_Pin);
}

/**
 * @brief Read the register
 *
 * @param regAddr Register address
 * @return uint16_t Register value
 */
uint16_t LCD_ReadREG(uint8_t regAddr)
{
    uint16_t dat = 0;

    LL_GPIO_ResetOutputPin(nCS_GPIO_Port, nCS_Pin);

    LL_GPIO_ResetOutputPin(RS_GPIO_Port, RS_Pin);
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);
    LL_GPIO_WriteOutputPort(LC0_GPIO_Port, regAddr);
    LL_GPIO_ResetOutputPin(nWR_GPIO_Port, nWR_Pin);
    lcdDelay();
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);

    LL_GPIO_ResetOutputPin(RS_GPIO_Port, RS_Pin);
    LCD_BusIn();
    LL_GPIO_ResetOutputPin(nRD_GPIO_Port, nRD_Pin);
    lcdDelay();
    dat = LL_GPIO_ReadInputPort(LC0_GPIO_Port);
    LL_GPIO_SetOutputPin(nRD_GPIO_Port, nRD_Pin);
    LCD_BusOut();

    LL_GPIO_SetOutputPin(nCS_GPIO_Port, nCS_Pin);

    return dat;
}

/**
 * @brief Write to GRAM
 *
 * @param RGBcode RGB565 code array
 * @param dataSize Array sizr
 */
void LCD_WriteGRAM(const uint16_t *RGBcode, uint32_t dataSize)
{
    uint32_t i = 0;

    LL_GPIO_ResetOutputPin(nCS_GPIO_Port, nCS_Pin);

    LL_GPIO_ResetOutputPin(RS_GPIO_Port, RS_Pin);
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);
    LL_GPIO_WriteOutputPort(LC0_GPIO_Port, 0x22);
    LL_GPIO_ResetOutputPin(nWR_GPIO_Port, nWR_Pin);
    lcdDelay();
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);

    for (i = 0; i < dataSize; i++)
    {
        LL_GPIO_SetOutputPin(RS_GPIO_Port, RS_Pin);
        LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);
        LL_GPIO_WriteOutputPort(LC0_GPIO_Port, RGBcode[i]);
        LL_GPIO_ResetOutputPin(nWR_GPIO_Port, nWR_Pin);
        lcdDelay();
        LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);
    }

    LL_GPIO_SetOutputPin(nCS_GPIO_Port, nCS_Pin);
}

/**
 * @brief Prepare for write to GRAM
 *
 */
void LCD_WriteGRAM0(void)
{
    LL_GPIO_ResetOutputPin(nCS_GPIO_Port, nCS_Pin);

    LL_GPIO_ResetOutputPin(RS_GPIO_Port, RS_Pin);
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);
    LL_GPIO_WriteOutputPort(LC0_GPIO_Port, 0x22);
    LL_GPIO_ResetOutputPin(nWR_GPIO_Port, nWR_Pin);
    lcdDelay();
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);
}

/**
 * @brief Write to GRAM
 *
 * @param RGBcode RGB565 code
 */
void LCD_WriteGRAM1(const uint16_t RGBcode)
{
    LL_GPIO_SetOutputPin(RS_GPIO_Port, RS_Pin);
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);
    LL_GPIO_WriteOutputPort(LC0_GPIO_Port, RGBcode);
    LL_GPIO_ResetOutputPin(nWR_GPIO_Port, nWR_Pin);
    lcdDelay();
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);
}

/**
 * @brief End write to GRAM
 *
 */
void LCD_WriteGRAM2(void)
{
    LL_GPIO_SetOutputPin(nCS_GPIO_Port, nCS_Pin);
}

/**
 * @brief Read GRAM
 *
 * @param RGBcode RGB565 code buffer
 * @param dataSize read size
 */
void LCD_ReadGRAM(uint16_t *RGBcode, uint32_t dataSize)
{
    uint32_t i = 0;

    LL_GPIO_ResetOutputPin(nCS_GPIO_Port, nCS_Pin);

    LL_GPIO_ResetOutputPin(RS_GPIO_Port, RS_Pin);
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);
    LL_GPIO_WriteOutputPort(LC0_GPIO_Port, 0x22);
    LL_GPIO_ResetOutputPin(nWR_GPIO_Port, nWR_Pin);
    lcdDelay();
    LL_GPIO_SetOutputPin(nWR_GPIO_Port, nWR_Pin);

    LCD_BusIn();
    LL_GPIO_SetOutputPin(RS_GPIO_Port, RS_Pin);
    LL_GPIO_ResetOutputPin(nRD_GPIO_Port, nRD_Pin);
    lcdDelay();
    LL_GPIO_ReadInputPort(LC0_GPIO_Port);
    LL_GPIO_SetOutputPin(nRD_GPIO_Port, nRD_Pin);

    for (i = 0; i < dataSize; i++)
    {
        LL_GPIO_SetOutputPin(RS_GPIO_Port, RS_Pin);
        LL_GPIO_ResetOutputPin(nRD_GPIO_Port, nRD_Pin);
        lcdDelay();
        RGBcode[i] = LL_GPIO_ReadInputPort(LC0_GPIO_Port);
        LL_GPIO_SetOutputPin(nRD_GPIO_Port, nRD_Pin);
    }
    LCD_BusOut();

    LL_GPIO_SetOutputPin(nCS_GPIO_Port, nCS_Pin);
}

/**
 * @brief LCD init. for ILI9325/ILI9328
 *
 * @param lfr LCD framerate
 */
void LCD_Init932x(lcdDispFR_t lfr)
{
    LCD_WriteREG(0xe3, 0x3008); // Set internal timing
    LCD_WriteREG(0xe7, 0x0012); // Set internal timing
    LCD_WriteREG(0xef, 0x1231); // Set internal timing

    LCD_WriteREG(0x10, 0x0000); // Set power control
    LCD_WriteREG(0x11, 0x0007); // Set power control
    LCD_WriteREG(0x12, 0x0000); // Set power control
    LCD_WriteREG(0x13, 0x0000); // Set power control
    lcdDelayL(200);             // Set power control
    LCD_WriteREG(0x10, 0x1690); // Set power control
    LCD_WriteREG(0x11, 0x0227); // Set power control
    lcdDelayL(50);              // Set power control
    LCD_WriteREG(0x12, 0x001d); // Set power control
    lcdDelayL(50);              // Set power control
    LCD_WriteREG(0x13, 0x0800); // Set power control
    LCD_WriteREG(0x29, 0x0014); // Set power control
    lcdDelayL(50);              // Set power control

    LCD_WriteREG(0x30, 0x0007); // Adjust the gamma curve
    LCD_WriteREG(0x31, 0x0707); // Adjust the gamma curve
    LCD_WriteREG(0x32, 0x0006); // Adjust the gamma curve
    LCD_WriteREG(0x35, 0x0704); // Adjust the gamma curve
    LCD_WriteREG(0x36, 0x1f04); // Adjust the gamma curve
    LCD_WriteREG(0x37, 0x0004); // Adjust the gamma curve
    LCD_WriteREG(0x38, 0x0000); // Adjust the gamma curve
    LCD_WriteREG(0x39, 0x0706); // Adjust the gamma curve
    LCD_WriteREG(0x3c, 0x0701); // Adjust the gamma curve
    LCD_WriteREG(0x3d, 0x000f); // Adjust the gamma curve

    LCD_WriteREG(0x90, 0x0010); // Set panel interface
    LCD_WriteREG(0x92, 0x0000); // Set panel interface
    LCD_WriteREG(0x95, 0x0110); // Set panel interface
    LCD_WriteREG(0x97, 0x0000); // Set panel interface

    LCD_WriteREG(0x61, 0x0001); // NDL, VLE, REV
    LCD_WriteREG(0x6a, 0x0000); // Set scrolling line
    LCD_WriteREG(0x02, 0x0700); // Set line inversion
    LCD_WriteREG(0x04, 0x0000); // Set resize register
    LCD_WriteREG(0x07, 0x0173); // Display Control
    LCD_WriteREG(0x08, 0x0207); // Set back & front porch
    LCD_WriteREG(0x09, 0x0000); // Set non-disp. area mode
    LCD_WriteREG(0x50, 0x0000); // Horizontal GRAM Start Address
    LCD_WriteREG(0x51, 0x00ef); // Horizontal GRAM End Address
    LCD_WriteREG(0x52, 0x0000); // Vertical GRAM Start Address
    LCD_WriteREG(0x53, 0x013f); // Vertical GRAM Start Address
    LCD_WriteREG(0x20, 0x0000); // GRAM horizontal Address
    LCD_WriteREG(0x21, 0x0000); // GRAM Vertical Address

    LCD_WriteREG(0x60, 0x2700); // Gate Scan Line & GS
    LCD_WriteREG(0x01, 0x0000); // SS & SM
    LCD_WriteREG(0x03, 0x1018); // Set entry mode
    LCD_WriteREG(0x2b, lfr);    // Set framerate

    LCD_WriteREG(0x07, 0x0173); // 262K color and display ON
}

/**
 * @brief LCD init. for uc8230
 *
 */
void LCD_Init8230(void)
{
    LCD_WriteREG(0x00, 0x0001); // Start auto power-up
    lcdDelayL(40);

    LCD_WriteREG(0x10, 0x1790); // Set power control
    LCD_WriteREG(0x12, 0x80fe); // Set power control
    LCD_WriteREG(0x13, 0x8010); // Set power control

    LCD_WriteREG(0x30, 0x0303); // Adjust the gamma curve
    LCD_WriteREG(0x31, 0x0303); // Adjust the gamma curve
    LCD_WriteREG(0x32, 0x0303); // Adjust the gamma curve
    LCD_WriteREG(0x33, 0x0300); // Adjust the gamma curve
    LCD_WriteREG(0x34, 0x0003); // Adjust the gamma curve
    LCD_WriteREG(0x35, 0x0303); // Adjust the gamma curve
    LCD_WriteREG(0x36, 0x0014); // Adjust the gamma curve
    LCD_WriteREG(0x37, 0x0303); // Adjust the gamma curve
    LCD_WriteREG(0x38, 0x0303); // Adjust the gamma curve
    LCD_WriteREG(0x39, 0x0303); // Adjust the gamma curve
    LCD_WriteREG(0x3a, 0x0300); // Adjust the gamma curve
    LCD_WriteREG(0x3b, 0x0003); // Adjust the gamma curve
    LCD_WriteREG(0x3c, 0x0303); // Adjust the gamma curve
    LCD_WriteREG(0x3d, 0x1400); // Adjust the gamma curve

    LCD_WriteREG(0x90, 0x080d); // Set panel interface
    LCD_WriteREG(0x92, 0x0200); // Set panel interface
    LCD_WriteREG(0x93, 0x0303); // Set panel interface

    LCD_WriteREG(0x61, 0x0001); // NDL, VLE, REV
    LCD_WriteREG(0x02, 0x0500); // Set line inversion
    LCD_WriteREG(0x04, 0x0000); // Set resize register
    LCD_WriteREG(0x07, 0x0173); // Display Control
    LCD_WriteREG(0x50, 0x0000); // Horizontal GRAM Start Address
    LCD_WriteREG(0x51, 0x00ef); // Horizontal GRAM End Address
    LCD_WriteREG(0x52, 0x0000); // Vertical GRAM Start Address
    LCD_WriteREG(0x53, 0x013f); // Vertical GRAM Start Address
    LCD_WriteREG(0x20, 0x0000); // GRAM horizontal Address
    LCD_WriteREG(0x21, 0x0000); // GRAM Vertical Address

    LCD_WriteREG(0x60, 0x2700); // Gate Scan Line & GS
    LCD_WriteREG(0x01, 0x0000); // SS & SM
    LCD_WriteREG(0x03, 0x1018); // Set entry mode
}

/**
 * @brief LCD display character
 *
 * @param Xst Start position in X
 * @param Yst Start position in Y
 * @param fn Character font
 */
void LCD_DisplayFont(uint16_t Xst, uint16_t Yst, const uint16_t *fn)
{
    uint16_t Xaddr = Xst;
    uint8_t fontX, fontY;

    LCD_SetCursor(Xaddr, Yst);
    for (fontX = 0; fontX < 24; fontX++)
    {
        LCD_WriteGRAM0();
        for (fontY = 0; fontY < 16; fontY++)
        {
            if (fn[fontX] & (1 << fontY))
                LCD_WriteGRAM1(lcdFrontColor);
            else
                LCD_WriteGRAM1(lcdBackColor);
        }
        LCD_WriteGRAM2();
        LCD_SetCursor(++Xaddr, Yst);
    }
}

/**
 * @brief LCD Init.
 *
 * @param lfr LCD framerate
 */
void LCD_Init(lcdDispFR_t lfr)
{
    uint16_t lcdID = LCD_ReadREG(0x00);
    if (lcdID == 0x8230)
        LCD_Init8230();
    else
        LCD_Init932x(lfr);
}

/**
 * @brief Set back color
 *
 * @param lc RGB565 code
 */
void LCD_SetBackColor(uint16_t lc)
{
    lcdBackColor = lc;
}

/**
 * @brief Set front color
 *
 * @param lc RGB565 code
 */
void LCD_SetFrontColor(uint16_t lc)
{
    lcdFrontColor = lc;
}

/**
 * @brief Set the refresh cursor
 *
 * @param Xpos Position in X
 * @param Ypos Position in Y
 */
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
    LCD_WriteREG(0x20, Xpos);
    LCD_WriteREG(0x21, Ypos);
}

/**
 * @brief Clear the screen & fill with back color
 *
 */
void LCD_ClearScreen(void)
{
    uint32_t i = (320 * 240);
    LCD_SetCursor(0, 0);
    LCD_WriteGRAM0();
    while (i--)
        LCD_WriteGRAM1(lcdBackColor);
    LCD_WriteGRAM2();
}

/**
 * @brief Display character
 *
 * @param Xpos Display position in X
 * @param Ypos Display position in Y
 * @param ch Character
 */
void LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, char ch)
{
    LCD_DisplayFont(Xpos, Ypos, ascii_map[ch - 32]);
}

/**
 * @brief Display string
 *
 * @param Line Line number
 * @param str String
 */
void LCD_DisplayString(uint16_t Line, char *str)
{
    uint8_t i = 20;
    uint16_t colPos = 319;

    while (*str && i--)
    {
        LCD_DisplayChar(Line, colPos, *str++);
        colPos -= 16;
    }
}

/**
 * @brief Clear display line
 *
 * @param Line Line number
 */
void LCD_ClearLine(uint16_t Line)
{
    uint8_t i = 20;
    uint16_t colPos = 319;

    while (i--)
    {
        LCD_DisplayChar(Line, colPos, ' ');
        colPos -= 16;
    }
}

void LCD_SetUpdateWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
    if (Xpos >= Width)
        LCD_WriteREG(0x50, Xpos - Width + 1);
    else
        LCD_WriteREG(0x50, 0);
    LCD_WriteREG(0x51, Xpos);
    if (Ypos >= Height)
        LCD_WriteREG(0x52, Ypos - Height + 1);
    else
        LCD_WriteREG(0x52, 0);
    LCD_WriteREG(0x53, Ypos);
    LCD_SetCursor(Xpos, Ypos);
}

void LCD_ResetUpdateWindow(void)
{
    LCD_SetUpdateWindow(239, 319, 240, 320);
    LCD_WriteREG(0x03, 0x1018);
}

void LCD_DisplayBMP(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height, const uint16_t *BitMap)
{
    LCD_SetUpdateWindow(Xpos, Ypos, Width, Height);
    LCD_WriteGRAM(BitMap, Height * Width);
    LCD_ResetUpdateWindow();
}
