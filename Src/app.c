#include "app.h"

#include "adc.h"
#include "dma.h"
#include "gpio.h"
#include "main.h"
#include "tim.h"
#include "usart.h"

#include <rtthread.h>

#include "eeprom.h"
#include "i2c_sw.h"
#include "lcd.h"
#include "ledkey.h"
#include "mcp4017.h"
#include "swDelay.h"

struct rt_mutex mtx_I80, mtx_I2C, mtx_OW, mtx_DHT;

typedef enum
{
    sMode_M4,
    sMode_EXA,
} sMode_t;

typedef enum
{
    kMode_MCP,
    kMode_LED,
    kMode_DS,
    kMode_SEG,
} kMode_t;

typedef enum
{
    S0,
    S1,
    S2,
    S4,
} keyState_t;
rt_tick_t keyStateTS[4];
keyState_t keyState[4];
struct rt_mutex mtx_keyS;

sMode_t sMode = sMode_M4;
char sModeText[6] = "M4";
struct rt_mutex mtx_sMode;

kMode_t kMode = kMode_MCP;
char kModeText[11] = "MCP";
struct rt_mutex mtx_kMode;

const uint8_t bootimeCP[] = {
    ((__TIME__[0]) - '0' + 128 + 0),  ((__TIME__[1]) - '0' + 128 + 10), ((__TIME__[3]) - '0' + 128 + 20),
    ((__TIME__[4]) - '0' + 128 + 30), ((__TIME__[7]) - '0' + 128 + 40), ((__TIME__[8]) - '0' + 128 + 50),
};
uint8_t bootime;

char LCD_DisplayBuffer[32];
struct rt_mutex mtx_LCD;

uint8_t LED_DisplayBuffer;
struct rt_mutex mtx_LED;

uint16_t adc1Val[10][2], adc2Val[10][3];

void lcdRefresh(void *param);
void lcdRefreshEntry(void *param);
void ledRefresh(void *param);
void ledRefreshEntry(void *param);
void mcpRefresh(void *param);
void keyScan(void *param);
void keyProg(void *param);

rt_thread_t lcdU, ledU, mcpU, keyS, keyP;

int main()
{
    uint8_t bTmp, i;

    /* CommPort mutex */
    rt_mutex_init(&mtx_I80, "I80 Port", RT_IPC_FLAG_PRIO);
    rt_mutex_init(&mtx_I2C, "I2C Port", RT_IPC_FLAG_PRIO);
    rt_mutex_init(&mtx_OW, "OW Port", RT_IPC_FLAG_PRIO);
    rt_mutex_init(&mtx_DHT, "DHT Port", RT_IPC_FLAG_PRIO);

    /* EEPROM */
    rt_mutex_take(&mtx_I2C, RT_WAITING_FOREVER);
    bootime = EEPROM_Read(bootimeCP[0]);
    for (i = 1; i < 6; i++)
    {
        bTmp = EEPROM_Read(bootimeCP[i]);
        if (bootime != bTmp)
        {
            bootime = 0;
            break;
        }
    }
    bootime++;
    for (i = 0; i < 6; i++)
        EEPROM_Write(bootimeCP[i], bootime);
    rt_mutex_release(&mtx_I2C);

    /* LCD */
    rt_mutex_init(&mtx_LCD, "LCD Lock", RT_IPC_FLAG_PRIO);
    rt_mutex_take(&mtx_I80, RT_WAITING_FOREVER);
    LCD_Init(lcdDispFR_30);
    LCD_ClearScreen();
    rt_sprintf(LCD_DisplayBuffer, "CT117E-M4  Boot=%u", bootime);
    LCD_DisplayString(Line0, LCD_DisplayBuffer);
    LCD_DisplayString(Line1, "Compiled in");
    rt_sprintf(LCD_DisplayBuffer, "%s,%s", __TIME__, __DATE__);
    LCD_DisplayString(Line2, LCD_DisplayBuffer);
    rt_mutex_release(&mtx_I80);
    lcdU = rt_thread_create("lcdUpdate", lcdRefresh, RT_NULL, 384, 4, 3);

    /* LED */
    rt_mutex_init(&mtx_LED, "LED Lock", RT_IPC_FLAG_PRIO);
    rt_mutex_take(&mtx_I80, RT_WAITING_FOREVER);
    ledUpdate(bootime);
    rt_mutex_release(&mtx_I80);
    ledU = rt_thread_create("ledUpdate", ledRefresh, RT_NULL, 96, 15, 1);

    /* ADC */
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc1Val, 20);
    HAL_ADC_Start_DMA(&hadc2, (uint32_t *)adc2Val, 30);

    /* TIM */

    /* MCP4017 */
    // mcpU = rt_thread_create("mcpRefresh", mcpRefresh, RT_NULL, 256, 10, 5);

    /* Key Scan & Prog */
    rt_mutex_init(&mtx_keyS, "keyState Lock", RT_IPC_FLAG_PRIO);
    keyS = rt_thread_create("keyScan", keyScan, RT_NULL, 96, 4, 1);
    keyP = rt_thread_create("keyProgram", keyProg, RT_NULL, 256, 5, 1);

    /* Mode */
    rt_mutex_init(&mtx_sMode, "SysMode Lock", RT_IPC_FLAG_PRIO);
    rt_mutex_init(&mtx_kMode, "KeyMode Lock", RT_IPC_FLAG_PRIO);

    rt_thread_mdelay(1500);

    /* End init */
    rt_mutex_take(&mtx_I80, RT_WAITING_FOREVER);
    LCD_ClearScreen();
    ledUpdate(0x00);
    rt_mutex_release(&mtx_I80);

    rt_thread_startup(ledU);
    rt_thread_startup(lcdU);
    rt_thread_startup(keyS);
    rt_thread_startup(keyP);

    return 0;
}

void printBootime(void)
{
    rt_kprintf("Boot Time=%u\n", bootime);
}
MSH_CMD_EXPORT(printBootime, print boot time);

void lcdRefresh(void *param)
{
    while (1)
    {
        rt_mutex_take(&mtx_I80, RT_WAITING_FOREVER);

        rt_mutex_take(&mtx_sMode, RT_WAITING_FOREVER);
        rt_mutex_take(&mtx_kMode, RT_WAITING_FOREVER);
        rt_sprintf(LCD_DisplayBuffer, "S=%s,K=%s               ", sModeText, kModeText);
        LCD_DisplayString(Line0, LCD_DisplayBuffer);
        rt_mutex_release(&mtx_sMode);
        rt_mutex_release(&mtx_kMode);

        rt_mutex_release(&mtx_I80);
        rt_thread_mdelay(30);
    }
}

void ledRefresh(void *param)
{
    while (1)
    {
        rt_mutex_take(&mtx_I80, RT_WAITING_FOREVER);
        rt_mutex_take(&mtx_LED, RT_WAITING_FOREVER);
        ledUpdate(LED_DisplayBuffer);
        rt_mutex_release(&mtx_LED);
        rt_mutex_release(&mtx_I80);
        rt_thread_mdelay(500);
    }
}

void mcpRefresh(void *param)
{
}

void keyScan(void *param)
{
    uint8_t i, keyInfo;

    while (1)
    {
        keyInfo = 0xff;
        keyInfo ^= LL_GPIO_IsInputPinSet(B1_GPIO_Port, B1_Pin) << 0;
        keyInfo ^= LL_GPIO_IsInputPinSet(B2_GPIO_Port, B2_Pin) << 1;
        keyInfo ^= LL_GPIO_IsInputPinSet(B3_GPIO_Port, B3_Pin) << 2;
        keyInfo ^= LL_GPIO_IsInputPinSet(B4_GPIO_Port, B4_Pin) << 3;

        rt_mutex_take(&mtx_keyS, RT_WAITING_FOREVER);
        for (i = 0; i < 4; i++)
        {
            if (keyInfo & (1 << i))
            {
                switch (keyState[i])
                {
                case S0:
                    keyState[i] = S1;
                    keyStateTS[i] = rt_tick_get();
                    break;
                default:
                    keyState[i] = keyState[i];
                    break;
                }
            }
            else
            {
                switch (keyState[i])
                {
                case S1:
                    if (rt_tick_get() - keyStateTS[i] >= 800)
                        keyState[i] = S4;
                    else if (rt_tick_get() - keyStateTS[i] >= 50)
                        keyState[i] = S2;
                    else
                        keyState[i] = S0;
                    break;
                default:
                    keyState[i] = S0;
                    break;
                }
            }
        }
        rt_mutex_release(&mtx_keyS);
        rt_thread_mdelay(5);
    }
}

void keyProg(void *param)
{
    char modeDbuf[16];

    while (1)
    {
        rt_mutex_take(&mtx_keyS, RT_WAITING_FOREVER);

        if (keyState[0] == S2) // B1 Short
        {
            // reset state
            keyState[0] = S0;

            // act.
            rt_mutex_take(&mtx_kMode, RT_WAITING_FOREVER);
            switch (kMode)
            {
            case kMode_LED:

                kMode = kMode_MCP;
                rt_sprintf(kModeText, "MCP");
                break;
            case kMode_MCP:
                kMode = kMode_DS;
                rt_sprintf(kModeText, "DSR");
                break;
            case kMode_DS:
                kMode = kMode_SEG;
                rt_sprintf(kModeText, "SEG");
                break;
            case kMode_SEG:
                kMode = kMode_LED;
                rt_sprintf(kModeText, "LED");
                break;
            default:
                kMode = kMode;
                break;
            }
            rt_mutex_release(&mtx_kMode);

            // disp.
            rt_mutex_take(&mtx_I80, RT_WAITING_FOREVER);
            rt_mutex_take(&mtx_LCD, RT_WAITING_FOREVER);
            LCD_ClearLine(Line9);
            rt_sprintf(LCD_DisplayBuffer, "K%s,%s               ", "1S", "keyMode Change");
            LCD_DisplayString(Line9, LCD_DisplayBuffer);
            rt_mutex_release(&mtx_LCD);
            rt_mutex_release(&mtx_I80);
        }

        if (keyState[0] == S4) // B1 Long
        {
            // reset state
            keyState[0] = S0;

            // act.
            rt_mutex_take(&mtx_sMode, RT_WAITING_FOREVER);
            switch (sMode)
            {
            case sMode_M4:
                sMode = sMode_EXA;
                rt_sprintf(sModeText, "EXA");
                break;
            case sMode_EXA:
                sMode = sMode_M4;
                rt_sprintf(sModeText, "M4");
                break;
            default:
                sMode = sMode;
                break;
            }
            rt_mutex_release(&mtx_sMode);

            // disp.
            rt_mutex_take(&mtx_I80, RT_WAITING_FOREVER);
            rt_mutex_take(&mtx_LCD, RT_WAITING_FOREVER);
            rt_sprintf(LCD_DisplayBuffer, "K%s,%s               ", "1L", "sysMode Change");
            LCD_DisplayString(Line9, LCD_DisplayBuffer);
            rt_mutex_release(&mtx_LCD);
            rt_mutex_release(&mtx_I80);
        }

        if (keyState[1] == S2) // B2 Short
        {
            // reset state
            keyState[1] = S0;

            // act.

            // disp.
            rt_mutex_take(&mtx_I80, RT_WAITING_FOREVER);
            rt_mutex_take(&mtx_LCD, RT_WAITING_FOREVER);
            rt_sprintf(LCD_DisplayBuffer, "K%s,%s               ", "2S", modeDbuf);
            LCD_DisplayString(Line9, LCD_DisplayBuffer);
            rt_mutex_release(&mtx_LCD);
            rt_mutex_release(&mtx_I80);
        }

        if (keyState[1] == S4) // B2 Long
        {
            // reset state
            keyState[1] = S0;

            // act.

            // disp.
            rt_mutex_take(&mtx_I80, RT_WAITING_FOREVER);
            rt_mutex_take(&mtx_LCD, RT_WAITING_FOREVER);
            rt_sprintf(LCD_DisplayBuffer, "K%s,%s               ", "2L", modeDbuf);
            LCD_DisplayString(Line9, LCD_DisplayBuffer);
            rt_mutex_release(&mtx_LCD);
            rt_mutex_release(&mtx_I80);
        }

        /*
                if (keyState[2] == S2) // B3 Short
                {
                    // reset state
                    keyState[2] = S0;

                    // act.
                    switch (kMode)
                    {
                    case kMode_MCP:
                        mcpWrite(--MCP_CFGVal);
                        rt_sprintf(modeDbuf, "Res -1");
                        break;
                    case kMode_LED:
                        ledBuffer >>= 1;
                        if (!ledBuffer)
                            ledBuffer = 0x80;
                        rt_sprintf(modeDbuf, "LED >>");
                        break;
                    case kMode_DS:
                        DS18B20_Res--;
                        if (DS18B20_Res < 9)
                            DS18B20_Res = 9;
                        rt_sprintf(modeDbuf, "DS_Res -1");
                        break;
                    case kMode_SEG:
                        segPos[segCP]--;
                        if (segPos[segCP] > 0xf)
                            segPos[segCP] = 0xf;
                        rt_sprintf(modeDbuf, "SEG%u -1", segCP + 1);
                        break;

                    default:
                        rt_sprintf(modeDbuf, "");
                        break;
                    }

                    // disp.
                    lcdClearLine(Line9);
                    rt_sprintf(LCD_DisplayBuffer, "K%s,%s", "3S", modeDbuf);
                    LCD_DisplayString(Line9, LCD_DisplayBuffer);

                    // act.
                }

                if (keyState[2] == S4) // B3 Long
                {
                    // reset state
                    keyState[2] = S0;

                    // act.
                    switch (kMode)
                    {
                    case kMode_MCP:
                        MCP_CFGVal = 0;
                        mcpWrite(MCP_CFGVal);
                        rt_sprintf(modeDbuf, "Res Min");
                        break;
                    case kMode_LED:
                        ledBuffer = 0;
                        rt_sprintf(modeDbuf, "LED Blank");
                        break;

                    case kMode_DS:
                        DS18B20_Res = 9;
                        rt_sprintf(modeDbuf, "DS_Res Min");
                        break;

                    case kMode_SEG:
                        segCP++;
                        if (segCP > 2)
                            segCP = 0;
                        rt_sprintf(modeDbuf, "SEG ChangePos");
                        break;

                    default:
                        rt_sprintf(modeDbuf, "");
                        break;
                    }

                    // disp.
                    lcdClearLine(Line9);
                    rt_sprintf(LCD_DisplayBuffer, "K%s,%s", "3L", modeDbuf);
                    LCD_DisplayString(Line9, LCD_DisplayBuffer);
                }

                if (keyState[3] == S2) // B4 Short
                {
                    // reset state
                    keyState[3] = S0;

                    // act.
                    switch (kMode)
                    {
                    case kMode_MCP:
                        mcpWrite(++MCP_CFGVal);
                        rt_sprintf(modeDbuf, "Res +1");
                        break;
                    case kMode_LED:
                        ledBuffer <<= 1;
                        if (!ledBuffer)
                            ledBuffer = 1;
                        rt_sprintf(modeDbuf, "LED <<");
                        break;
                    case kMode_DS:
                        DS18B20_Res++;
                        if (DS18B20_Res > 12)
                            DS18B20_Res = 12;
                        rt_sprintf(modeDbuf, "DS_Res +1");
                        break;
                    case kMode_SEG:
                        segPos[segCP]++;
                        if (segPos[segCP] > 0xf)
                            segPos[segCP] = 0;
                        rt_sprintf(modeDbuf, "SEG%u +1", segCP + 1);
                        break;

                    default:
                        rt_sprintf(modeDbuf, "");
                        break;
                    }

                    // disp.
                    lcdClearLine(Line9);

                    rt_sprintf(LCD_DisplayBuffer, "K%s,%s", "4S", modeDbuf);
                    LCD_DisplayString(Line9, LCD_DisplayBuffer);
                }

                if (keyState[3] == S4) // B4 Long
                {
                    // reset state
                    keyState[3] = S0;

                    // act.
                    switch (kMode)
                    {
                    case kMode_MCP:
                        MCP_CFGVal = 0x7f;
                        mcpWrite(MCP_CFGVal);
                        rt_sprintf(modeDbuf, "Res Max");
                        break;
                    case kMode_LED:
                        ledBuffer = 0xff;
                        rt_sprintf(modeDbuf, "LED White");
                        break;
                    case kMode_DS:
                        DS18B20_Res = 12;
                        rt_sprintf(modeDbuf, "DS_Res Max");
                        break;

                    case kMode_SEG:
                        segDP[segCP] = (segDP[segCP] ? 0 : 0x80);
                        rt_sprintf(modeDbuf, "SEG%u PT Change", segCP + 1);
                        break;
                    default:
                        rt_sprintf(modeDbuf, "");
                        break;
                    }

                    // disp.
                    lcdClearLine(Line9);
                    rt_sprintf(LCD_DisplayBuffer, "K%s,%s", "4L", modeDbuf);
                    LCD_DisplayString(Line9, LCD_DisplayBuffer);
                }
                */

        rt_mutex_release(&mtx_keyS);
        rt_thread_mdelay(5);
    }
}
