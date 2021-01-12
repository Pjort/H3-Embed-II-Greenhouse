#include "stm32746g_discovery_lcd.h"

void lcdInit(){
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);

    BSP_LCD_Clear(LCD_COLOR_RED);
    BSP_LCD_SetFont(&Font12);
    BSP_LCD_SetBackColor(LCD_COLOR_RED);
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
}