#ifndef DATA
#define DATA

#include "mbed.h"

#define MAXIMUM_BUFFER_SIZE 32

float adcTemp;

float lightValue=60.0f;

float thermistorValue= 12.0f;

float dhtTemp=18.2f;
float dhtHumid=90.0f;

char currentMessage[32] = {0};

char unitName[32] = "Greenhouse";
char unitNumber[32] = "1";


#endif