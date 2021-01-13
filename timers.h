#ifndef TIMERS
#define TIMERS

#include "mbed.h"
#include "serialCom.h"

unsigned long time_ms;
time_t timeNow;

unsigned long millis(){
    return (us_ticker_read() / 1000L);
}

void timersInit(){

    set_time(1610462467); // 12/01/2021 @ 14:41 (UTC)
    timeNow = time(NULL);

    time_ms = 0; // This can hould about 50 days of milliseconds before repeating

}

void serialTimer(int delay){
    if (millis() >= time_ms + delay){
        time_ms = millis();
        //printf("%lu \n",time_ms);

        timeNow = time(NULL);
        //char buffer[32];
        //strftime(buffer, 32, "%H:%M:%S\n", localtime(&timeNow));
        //printf("%s", buffer);
        
        serialRead();
        
    }   
}

#endif