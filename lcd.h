#ifndef LCD
#define LCD

#include "mbed.h"
#include "data.h"
#include "timers.h"

//Screen size 480x272

#define LCD_COLOR_SKY_DAY    ((uint32_t)0xFFAAEEFF)
#define LCD_COLOR_INDOOR_DAY ((uint32_t)0xFFd5f6ff)

#include "stm32746g_discovery_lcd.h"

//declare variables
int currentMainScreenPage = 0;
int currentDrawnDayNight = NIGHT;
int wateringDrawn = 0;

//declare methods
void drawMainScreenTexts();
void updateTexts();
void drawTopMiddleText();
void drawDateTime();
void drawDrop(int xOffset,int yOffset, int scale);
void drawWatering(int xOffset,int yOffset, int scale);
void drawBulb(int xOffset,int yOffset, int scale);
void drawDay();
void drawNight();
void drawWatering();
void drawLightValue();
void drawHouseText();
void screenRefresh();
void drawOutTemp();

void lcdInit(){
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
}


void drawStartupScreen(){
    //Set font background
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);

    //Draw background
    BSP_LCD_Clear(LCD_COLOR_WHITE);

    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_SetFont(&Font16);

    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()/2, (uint8_t *)"Connect via serial and follow instructions", CENTER_MODE);
}

void drawMainScreen(){
    //Set font background
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);


    if(currentDayNightCycle==DAY){
        drawDay();
        currentDrawnDayNight = DAY;

    }else if (currentDayNightCycle==NIGHT) {
        drawNight();
        currentDrawnDayNight= NIGHT;
    }

    //Draw top bar
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_FillRect (0, 0, BSP_LCD_GetXSize()-1, 38);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DrawRect (0, 0, BSP_LCD_GetXSize()-1, 38);


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

    drawBulb(BSP_LCD_GetXSize()/2-15,90,3);

    drawMainScreenTexts();

}

void drawMainScreenTexts(){
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

    //Topbar left text
    drawDateTime();

    //Topbar middle text
    drawTopMiddleText();

    //Topbar right text
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(2, 5, (uint8_t *)"MBED OS", RIGHT_MODE);
    char buffer[32];
    sprintf(buffer, "v%d.%d.%d", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
    BSP_LCD_SetFont(&Font12);
    BSP_LCD_DisplayStringAt(5, 23, (uint8_t *)buffer, RIGHT_MODE);

    //Left box text
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(15, 105, (uint8_t *)"Out Temp", LEFT_MODE);
    drawOutTemp();

    //Right box text
    BSP_LCD_SetFont(&Font20);
    BSP_LCD_DisplayStringAt(20, 135, (uint8_t *)"Light", RIGHT_MODE);
    drawLightValue();

    //House text
    drawHouseText();

}

void screenRefresh(){
    drawDateTime();
    drawLightValue();
    drawHouseText();
    drawOutTemp();
    drawBulb(BSP_LCD_GetXSize()/2-15,90,3);

    if(currentDrawnDayNight == DAY && currentDayNightCycle == NIGHT){
        drawMainScreen();
    }else if (currentDrawnDayNight == NIGHT && currentDayNightCycle == DAY ) {
        drawMainScreen();
    }

    if(wateringActive==1 & wateringDrawn==0){
        drawWatering();
        wateringDrawn = 1;
    }else if(wateringActive==0 & wateringDrawn==1){
        BSP_LCD_SetTextColor(LCD_COLOR_INDOOR_DAY);
        BSP_LCD_FillRect(BSP_LCD_GetXSize()/2+100-30,BSP_LCD_GetYSize()-46, 30, 30);
        BSP_LCD_FillRect(BSP_LCD_GetXSize()/2-100,BSP_LCD_GetYSize()-46, 30, 30);
        wateringDrawn = 0;
    }
    //printf("wateringActive: %i \n",wateringActive);
    //printf("wateringDrawn: %i \n",wateringDrawn);
}


void drawTopMiddleText(){
    //Topbar middle text
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_SetFont(&Font24);
    char buffer[32] = {0};
    sprintf(buffer,"%s #%s", unitName,unitNumber);
    BSP_LCD_DisplayStringAt(0, 6, (uint8_t *)buffer, CENTER_MODE);
}

void drawDateTime(){
    //Topbar left text
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_SetFont(&Font16);

    char buffer[32];
    strftime(buffer, sizeof(buffer), "%H:%M:%S", localtime(&timeNow));
    BSP_LCD_DisplayStringAt(3, 5, (uint8_t *)buffer, LEFT_MODE);
    BSP_LCD_SetFont(&Font12);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", localtime(&timeNow));
    BSP_LCD_DisplayStringAt(5, 23, (uint8_t *)buffer, LEFT_MODE);

}

// Drop pixelart
int pixelArtDrop[10][10] = {    
    {0,0,0,0,1,1,0,0,0,0},
    {0,0,0,1,1,1,1,0,0,0},
    {0,0,1,1,1,1,1,1,0,0},
    {0,1,1,1,1,1,1,1,1,0},
    {1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,0,0,1},
    {1,1,1,1,1,0,0,0,1,1},
    {0,1,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
};

// Watering pixelart
int pixelArtWatering[10][10] = {    
	{0,0,1,0,0,0,0,1,0,0},
    {1,0,0,1,0,0,1,0,0,1},
	{0,1,0,0,0,0,0,0,1,0},
    {0,0,0,1,0,0,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,1,1,0,0,0,0},
    {0,0,0,1,1,1,1,0,0,0},
    {0,0,0,0,1,1,0,0,0,0},
    {0,0,0,0,1,1,0,0,0,0},
    {0,0,0,0,1,1,0,0,0,0},
};

// Bulb watering
int pixelArtBulb[10][10] = {    
	{0,0,0,1,1,1,1,0,0,0},
    {0,0,0,1,1,1,1,0,0,0},
	{0,0,0,1,1,1,1,0,0,0},
    {0,0,1,0,1,1,0,1,0,0},
    {0,1,0,0,1,1,0,0,1,0},
    {1,0,0,1,0,0,1,0,0,1},
    {1,0,1,0,0,0,0,1,0,1},
    {1,0,0,0,1,1,0,0,0,1},
    {0,1,0,0,0,0,0,0,1,0},
    {0,0,1,1,1,1,1,1,0,0},
};

//Scaleable function for drawing a drop
void drawDrop(int xOffset,int yOffset, int scale){
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
                if (pixelArtDrop[i][j] == 1) {
                    BSP_LCD_FillRect(xOffset+(j*scale), yOffset+(i*scale),scale, scale);
                }
            }
            
    }
}

//Scaleable function for drawing a drop
void drawWatering(int xOffset,int yOffset, int scale){
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
                if (pixelArtWatering[i][j] == 1) {
                    BSP_LCD_FillRect(xOffset+(j*scale), yOffset+(i*scale),scale, scale);
                }
            }
            
    }
}

void drawBulb(int xOffset,int yOffset, int scale){
    if (currentDayNightCycle==DAY & lightValue<50.0f) {
        BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
    }else{
        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    }
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
                if (pixelArtBulb[i][j] == 1) {
                    BSP_LCD_FillRect(xOffset+(j*scale), yOffset+(i*scale),scale, scale);
                }
            }
            
    }
}

void drawDay(){
    //Draw day background
    BSP_LCD_Clear(LCD_COLOR_SKY_DAY);

    //Draw sun
    BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
    BSP_LCD_FillCircle(390+28, 46+28, 28);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DrawCircle(390+28, 46+28, 28);

    //Draw day grass bottom
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_FillRect (0, BSP_LCD_GetYSize()-15, BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DrawHLine (0,BSP_LCD_GetYSize()-16,BSP_LCD_GetXSize());
}

void drawNight(){
    //Draw night background
    BSP_LCD_Clear(LCD_COLOR_DARKBLUE);

    //Draw moon
    BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
    BSP_LCD_FillCircle(390+28, 46+28, 28);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DrawCircle(390+28, 46+28, 28);
    BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
    BSP_LCD_FillCircle(390+28+15, 46+28, 28);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DrawCircle(390+28+15, 46+28, 28);
    BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
    BSP_LCD_FillRect (390+28+8, 46, 50, 66);

    //Draw night grass bottom
    BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN);
    BSP_LCD_FillRect (0, BSP_LCD_GetYSize()-15, BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DrawHLine (0,BSP_LCD_GetYSize()-16,BSP_LCD_GetXSize());
}

void drawWatering(){
    //Draw watering
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
    drawWatering(BSP_LCD_GetXSize()/2+100-30,BSP_LCD_GetYSize()-46,3);
    drawWatering(BSP_LCD_GetXSize()/2-100,BSP_LCD_GetYSize()-46,3);
}

void drawLightValue(){
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_SetFont(&Font24);
    char buffer[10] = {0};
    sprintf(buffer," %2.0f%%", lightValue);
    BSP_LCD_DisplayStringAt(25, 160, (uint8_t *)buffer, RIGHT_MODE);
}

void drawHouseText(){
    //House text
    BSP_LCD_SetFont(&Font24);
    BSP_LCD_SetBackColor(LCD_COLOR_INDOOR_DAY);
    char buffer[10] = {0};
    sprintf(buffer,"  %2.1fC  ", dhtTemp);
    BSP_LCD_DisplayStringAt(0, 145, (uint8_t *)buffer, CENTER_MODE);
    sprintf(buffer,"%2.0f%%  ", dhtHumid);
    BSP_LCD_DisplayStringAt(0, 200, (uint8_t *)buffer, CENTER_MODE);
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
    drawDrop(BSP_LCD_GetXSize()/2+15,195,3);    //Draw drop
}

void drawOutTemp(){
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_SetFont(&Font24);
    char buffer[10] = {0};
    sprintf(buffer,"%2.1fC ", thermistorValue);
    BSP_LCD_DisplayStringAt(13, 130, (uint8_t *)buffer, LEFT_MODE);
}



#endif