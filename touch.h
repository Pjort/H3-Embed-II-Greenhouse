/**
*   @file touch.h
*   @brief This holds all the code concerning the touch screen.
* 
*   @author Pjort Pieter Kat
*   @date 11-1-2021
*/
#ifndef TOUCH
#define TOUCH

#include "mbed.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"

/**
*@brief Needed for the touchscreen to tell its state, if pressed etc.
* 
*/
TS_StateTypeDef TS_State;
/**
*@brief Holds the x for the touch locations
* 
*/
uint16_t x;
/**
*@brief Holds the y for the touch locations
* 
*/
uint16_t y;
uint8_t status;
/**
*@brief Holds the points is touching, allowing for multitouch functions
* 
*/
uint8_t idx;
/**
*@brief Gets set to true when a touch is detected
* 
*/
int newTouch = false;

/**
*@brief Initializes what is needed for the touch screen.
* 
*/
void touchInit(){
    status = BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
}

/**
*@brief This gets call in the loop continuesly to get weather there is touch detected and its location
* 
*/
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


/**
*@brief This figures out what button has been pressed from x and y coordinates and returns different values depending what has been pressed.
*
* Left button returns -1 \n
* Right button returns 1 \n
* Press on date and time returns 2 \n
* If pressed anywhere else returns 0 \n
*/
int checkTouchButton(){
    //Left button
    if((x >= 5 && x <= 45) & (y >= 210 && y <= 250)) {
        return -1;
    }
    //Right button
    else if((x >= BSP_LCD_GetXSize()-45 && x <= BSP_LCD_GetXSize()-5) & (y >= 210 && y <= 250)) {
        return 1;
    } 
    //Press on date and time
    else if ((x >= 0 && x <= 100) & (y >= 0 && y <= 38)) {
        return 2;
    }

    return 0;
}


#endif