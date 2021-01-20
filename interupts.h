/**
*   @file interupts.h
*   @brief This holds all the code concerning the interupts from either touch, buttons or serial commands.
* 
*   @author Pjort Pieter Kat
*   @date 11-1-2021
*/

#ifndef INTERUPTS
#define INTERUPTS

#include "mbed.h"
#include "data.h"
#include "timers.h"
#include "lcd.h"
#include <cstdio>

/**
*@brief This toggles the demo mode to the oposite of what it was and deping of this it changes the waterDelay timer to either 5 seconds or 10min.
*/
void toggleDemoMode(){
    demoMode=!demoMode;
    if (demoMode) {
    wateringDelay = 5000;  //5 seconds
    }else {
    wateringDelay = 600000;  //10min
    }

    screenRefresh();    //Refreshing the screen, mostly to instantly show the DEMO MODE message appear on display imidiately after toggling.
}

/**
*@brief This checks the input from the serial and if 1,2 or 3 was send it will return corresponding sensor values.
*/
void newSerialComand(){
    printf("%s #%s: ",unitName,unitNumber);
    if (currentMessage[0]==49){                         //49 = 1 in ascii
        printf("Temperature: %2.1fC \n", dhtTemp);
    }
    else if (currentMessage[0]==50) {                   //50 = 2 in ascii
        printf("Humidity: %2.0f%% \n", dhtHumid);
    }
    else if (currentMessage[0]==51) {                   //51 = 3 in ascii
        printf("Light: %2.0f%% \n", lightValue);}
    else{                                               //Any other char will give instructions on what to write.
        printf("1=temperature,2=humidity, 3=light");
    }
    //printf("%c",currentMessage[0]);
    for (int i = 0; i<MAXIMUM_BUFFER_SIZE; i++) {       //Empties out the currentMessage array after everything is done.
        currentMessage[i]=0;
    }
    
}

#endif