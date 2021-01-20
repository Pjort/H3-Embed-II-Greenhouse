#ifndef TOUCH
#define TOUCH

#include "mbed.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"

TS_StateTypeDef TS_State;
uint16_t x, y;
uint8_t text[30];
uint8_t status;
uint8_t idx;
int newTouch = false;


void touchInit(){
    status = BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
}

void getTouch(){
    BSP_TS_GetState(&TS_State);
    if (TS_State.touchDetected) {
      for (idx = 0; idx < TS_State.touchDetected; idx++) {
        x = TS_State.touchX[idx];
        y = TS_State.touchY[idx];
        //printf("Touch %d: x=%d y=%d    \n", idx + 1, x, y);
        newTouch = true;
      }
    }
}



#endif