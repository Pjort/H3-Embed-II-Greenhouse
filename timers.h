#ifndef TIMERS
#define TIMERS

#include "mbed.h"
#include "serialCom.h"
#include "sensors.h"
#include "touch.h"

int secondTimerDelay = 1000; //Delay in MS
unsigned long lastSecond = 0;

int serialReadDelay = 150; //Delay in MS
unsigned long lastSerialRead = 0;

unsigned long lastSensorSample = 0;

int sensorReadDelay = 2000; //Delay in MS
unsigned long lastSensorRead = 0;

int waterings = 0;
int wateringActive = 0;
int wateringDelay = 600000; //600000 ms =  10 mins
unsigned long lastWatering = 0;

time_t timeNow;

enum {NIGHT,DAY};
int currentDayNightCycle = DAY;

unsigned long lastMillis=0;
int resetCount = 0;

unsigned long millis(){
    unsigned long currentMillis = (us_ticker_read() / 1000L);
    if (currentMillis<lastMillis){                  //us_ticker_read is a unsigned long and therefore resets after about 71min since it can only hold 4294967295us
        resetCount++;
    }
    lastMillis=currentMillis;
    return currentMillis+(4294967*resetCount);
}

void checkDayOrNight(){
    if (((timeNow-21600)/43200)%2==0) {
        currentDayNightCycle = DAY;
    }else {
        currentDayNightCycle = NIGHT;
    }
}

void timersInit(){

    set_time(1611079160); // 19/01/2021 @ 17:59:20 (UTC)
    timeNow = time(NULL);
    checkDayOrNight();
    lastMillis = millis();

}


int secondTimer(){
    if (millis() >= lastSecond + secondTimerDelay){
        checkDayOrNight();
        lastSecond = millis();
        return 1;
    }
    return 0;
}

int sensorSampleTimer(){
    if (millis() >= lastSensorSample + (sensorReadDelay/(MAX_SAMPLES+1))){
        lastSensorSample = millis();
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


void checkWatering(){
    if(waterings%2==0 & wateringActive==0 & currentDayNightCycle==DAY){
        wateringActive=1;
        lastWatering = millis();
    }

    if (waterings%2!=0 & wateringActive==0 & currentDayNightCycle==NIGHT) {
        wateringActive=1;
        lastWatering = millis();
    }

    if(wateringActive==1 & millis() >= lastWatering + wateringDelay){
        wateringActive = 0;
        waterings ++;
        //printf("waterings: %i", waterings);
    }
    //printf("lastWatering %lu \n",lastWatering);
    //printf("wateringDelay %i \n",wateringDelay);
}

void serialTimer(){
    if (millis() >= lastSerialRead + serialReadDelay){
        lastSerialRead = millis();
        timeNow = time(NULL);
        serialRead();       //Read serial data
        getTouch();         //Get touchscreen data

    }   
}

void addHoursToRTC(int hours){
    int unixSeconds = time(NULL);
    unixSeconds = unixSeconds + (3600 *hours);
    set_time(unixSeconds);
}

#endif