#ifndef INTERUPTS
#define INTERUPTS

#include "mbed.h"
#include "data.h"
#include "timers.h"
#include "lcd.h"


void toggleDemoMode(){
    demoMode=!demoMode;
    if (demoMode) {
    wateringDelay = 5000;  //5 seconds
    }else {
    wateringDelay = 600000;  //10min
    }

    screenRefresh();
}

void newSerialComand(){
    
}

#endif