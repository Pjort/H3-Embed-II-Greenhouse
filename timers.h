#ifndef TIMERS
#define TIMERS

#include "mbed.h"
#include "serialCom.h"

int secondTimerDelay = 1000; //Delay in MS
unsigned long lastSecond = 0;

int serialReadDelay = 100; //Delay in MS
unsigned long lastSerialRead = 0;

int sensorReadDelay = 2000; //Delay in MS
unsigned long lastSensorRead = 0;



time_t timeNow;

unsigned long millis(){
    return (us_ticker_read() / 1000L);
}

void timersInit(){

    set_time(1610462467); // 12/01/2021 @ 14:41 (UTC)
    timeNow = time(NULL);

}

int secondTimer(){
    if (millis() >= lastSecond + secondTimerDelay){
        lastSensorRead = millis();
        return 1;
    }   
    return 0;
}

int sensorReadTimer(){
    if (millis() >= lastSensorRead + sensorReadDelay){
        lastSensorRead = millis();
        return 1;
    }   
    return 0;
}

void serialTimer(){
    if (millis() >= lastSerialRead + serialReadDelay){
        lastSerialRead = millis();
        //printf("%lu \n",lastSerialRead);

        timeNow = time(NULL);
        //char buffer[32];
        //strftime(buffer, 32, "%H:%M:%S\n", localtime(&timeNow));
        //printf("%s", buffer);
        
        serialRead();
        
    }   
}

#endif