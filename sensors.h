#ifndef SENSORS
#define SENSORS

#include "mbed.h"
#include "data.h"

AnalogIn adc_temp(ADC_TEMP); 


void readAdcTemp(){
    adcTemp = (adc_temp.read()*100);
}


#endif