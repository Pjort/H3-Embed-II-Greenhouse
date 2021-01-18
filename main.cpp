/**
*   @file main.cpp
*   @brief Test description.
* 
*   Lorem Ipsum er ganske enkelt fyldtekst fra print- og typografiindustrien. Lorem Ipsum har været standard fyldtekst siden 1500-tallet, hvor en ukendt trykker sammensatte en tilfældig spalte for at trykke en bog til sammenligning af forskellige skrifttyper. Lorem Ipsum har ikke alene overlevet fem århundreder, men har også vundet indpas i elektronisk typografi uden væsentlige ændringer. Sætningen blev gjordt kendt i 1960'erne med lanceringen af Letraset-ark, som indeholdt afsnit med Lorem Ipsum, og senere med layoutprogrammer som Aldus PageMaker, som også indeholdt en udgave af Lorem Ipsum.
*   @author Pjort Pieter Kat
    @date 11-1-2021
*/
#include "mbed.h"
#include "lcd.h"
#include "timers.h"
#include "serialCom.h"
#include "sensors.h"
#include "state.h"


/**
*@brief Test description.
* 
*Lorem Ipsum er ganske enkelt fyldtekst fra print- og typografiindustrien. Lorem Ipsum har været standard fyldtekst siden 1500-tallet, hvor en ukendt trykker sammensatte en tilfældig spalte for at trykke en bog til sammenligning af forskellige skrifttyper. Lorem Ipsum har ikke alene overlevet fem århundreder, men har også vundet indpas i elektronisk typografi uden væsentlige ændringer. Sætningen blev gjordt kendt i 1960'erne med lanceringen af Letraset-ark, som indeholdt afsnit med Lorem Ipsum, og senere med layoutprogrammer som Aldus PageMaker, som også indeholdt en udgave af Lorem Ipsum.
*@param a asdasdasd
*/
int main()
{
    printf("STARTING UP! \n");

    lcdInit();

    serialInit();

    timersInit();

    drawStartupScreen();

    printf("All initialized. Press Enter to start. \n");

    while (true) {

        runStates();
        readAdcTemp();
        drawDateTime();

        //printf("DEBUG - Message \n");
    }
}