/**
*   @file main.cpp
*   @brief This is the main cpp file of project Case 2 - Greenhouse.
* 
*   "Case 2 - Greenhouse" is a school project for embeded controller II. It is a prototype of a unit ment to sit in multiple greenhouses monitoring and controlling lighting and watering.
*   The code is meant to run on a STM32F746G-DISCO with a Grove-starter Kit.
*   @author Pjort Pieter Kat
*   @date 11-1-2021
*/
#include "mbed.h"
#include "lcd.h"
#include "timers.h"
#include "serialCom.h"
#include "sensors.h"
#include "state.h"
#include "touch.h"


/**
*@brief The main mostly just initializes everthing and then hands it off to the state machine.
* 
*/
int main()
{
    printf("STARTING UP! \n");

    lcdInit();

    touchInit();

    serialInit();

    timersInit();

    drawStartupScreen();

    printf("mbed-os-rev: %d.%d.%d\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);

    printf("All initialized. Press Enter to start. \n");

    while (true) {      //Never-endning main loop

        runStates();

        //printf("DEBUG - Message \n");
    }
}