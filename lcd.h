#ifndef LCD
#define LCD

//Screen size 480x272

#define LCD_COLOR_SKY_DAY    ((uint32_t)0xFFAAEEFF)
#define LCD_COLOR_INDOOR_DAY ((uint32_t)0xFFd5f6ff)

#include "stm32746g_discovery_lcd.h"

void lcdInit(){
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
}

void drawMainScreenTexts(){
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(3, 5, (uint8_t *)"15:33", LEFT_MODE);
    BSP_LCD_SetFont(&Font12);
    BSP_LCD_DisplayStringAt(5, 23, (uint8_t *)"12-01-2021", LEFT_MODE);

    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_SetFont(&Font24);
    BSP_LCD_DisplayStringAt(0, 6, (uint8_t *)"GREENHOUSE #3", CENTER_MODE);

    BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(2, 5, (uint8_t *)"Version", RIGHT_MODE);
    BSP_LCD_SetFont(&Font12);
    BSP_LCD_DisplayStringAt(5, 23, (uint8_t *)"6.01.2", RIGHT_MODE);

}

void drawMainScreen(){
    //Draw background
    BSP_LCD_Clear(LCD_COLOR_SKY_DAY);

    //Draw top bar
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_FillRect (0, 0, BSP_LCD_GetXSize()-1, 38);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DrawRect (0, 0, BSP_LCD_GetXSize()-1, 38);

    //Draw sun
    BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
    BSP_LCD_FillCircle(390+28, 46+28, 28);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DrawCircle(390+28, 46+28, 28);
    //BSP_LCD_SetTextColor(LCD_COLOR_SKY_DAY); //Moon test
    //BSP_LCD_FillCircle(390+28+15, 46+28, 28);

    //Draw grass bottom
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_FillRect (0, BSP_LCD_GetYSize()-15, BSP_LCD_GetXSize(), BSP_LCD_GetYSize());

    //Draw greenhouse from 5 points
    Point housePoints[5];
    housePoints[0].X = BSP_LCD_GetXSize()/2;
    housePoints[0].Y = 75;
    housePoints[1].X = BSP_LCD_GetXSize()/2+118;
    housePoints[1].Y = 150;
    housePoints[2].X = BSP_LCD_GetXSize()/2+118;
    housePoints[2].Y = BSP_LCD_GetYSize()-16;
    housePoints[3].X = BSP_LCD_GetXSize()/2-118;
    housePoints[3].Y = BSP_LCD_GetYSize()-16;
    housePoints[4].X = BSP_LCD_GetXSize()/2-118;
    housePoints[4].Y = 150;
    BSP_LCD_SetTextColor(LCD_COLOR_INDOOR_DAY);
    BSP_LCD_FillPolygon(housePoints, 5);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DrawPolygon(housePoints, 5);

    //Draw left sqaure for outside temp
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_FillRect (5, 100, 110, 66);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DrawRect (5, 100, 110, 66);

    //Draw right sqaure for light
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_FillRect (BSP_LCD_GetXSize()/2+125, 130, 110, 66);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DrawRect (BSP_LCD_GetXSize()/2+125, 130, 110, 66);

    drawMainScreenTexts();

}



#endif