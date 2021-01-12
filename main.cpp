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
#include "serialCom.h"

AnalogIn adc_temp(ADC_TEMP);


unsigned long millis(){
    return (us_ticker_read() / 1000L);
}

/**
*@brief Test description.
* 
*Lorem Ipsum er ganske enkelt fyldtekst fra print- og typografiindustrien. Lorem Ipsum har været standard fyldtekst siden 1500-tallet, hvor en ukendt trykker sammensatte en tilfældig spalte for at trykke en bog til sammenligning af forskellige skrifttyper. Lorem Ipsum har ikke alene overlevet fem århundreder, men har også vundet indpas i elektronisk typografi uden væsentlige ændringer. Sætningen blev gjordt kendt i 1960'erne med lanceringen af Letraset-ark, som indeholdt afsnit med Lorem Ipsum, og senere med layoutprogrammer som Aldus PageMaker, som også indeholdt en udgave af Lorem Ipsum.
*@param a asdasdasd
*/
int main()
{

    set_time(1610462467); // 12/01/2021 @ 14:41 (UTC)
    time_t timeNow = time(NULL);

    lcdInit();

    serialInit();

    int periodeMs = 1000;

    unsigned long time_ms = 0; // This can hould about 50 days of milliseconds before repeating


    while (true) {
        
        if (millis() >= time_ms + periodeMs){
            time_ms = millis();
            printf("%lu \n",time_ms);
            
            serialRead();
            
        }      

        //printf("DEBUG - Message \n");
    }
}