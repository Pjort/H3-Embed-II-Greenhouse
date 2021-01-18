#ifndef DATA
#define DATA

#include "mbed.h"

#define MAXIMUM_BUFFER_SIZE 32

float adcTemp;

float lightValue;

char currentMessage[32] = {0};

char unitName[32] = "Greenhouse";
char unitNumber[32] = "1";


#endif