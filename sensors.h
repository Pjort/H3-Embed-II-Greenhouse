#ifndef SENSORS
#define SENSORS

#include "mbed.h"
#include "data.h"

#define MAX_SAMPLES  5

AnalogIn adc_temp(ADC_TEMP);
AnalogIn lightSensor(A0); 
DigitalInOut Led1(D3);

int sampI = 0;
float light[MAX_SAMPLES] = {0};




void readAdcTemp(){
    adcTemp = (adc_temp.read()*100);
}

float readLightSensor(){
    //return(lightSensor.read()/0.630*100); //3v3 mode
    return(lightSensor.read()*100);  //5v mode
}

void calcLightSensor(){
    float lightAvg = 0.0f;
    int lightCount = 0;
    for (int i = 0; i<MAX_SAMPLES; i++) {
        if (light[i]!=0.0f) {
            lightAvg = lightAvg + light[i];
            lightCount++;
        }
        
    }

    lightValue=lightAvg/lightCount;

    for (int i = 0; i<MAX_SAMPLES; i++) {
        light[i]=0.0f;
    }
}

void readSensors(){
    readAdcTemp();
    calcLightSensor();
    
    sampI = 0;
}

void sensorSampling(){
    if (sampI<MAX_SAMPLES) {
        light[sampI] = readLightSensor();
        sampI ++;
    }
}


#endif