#ifndef SENSORS
#define SENSORS

#include "mbed.h"
#include "data.h"
#include "DHT.h"
#include <cstdio>

#define MAX_SAMPLES  5

AnalogIn adc_temp(ADC_TEMP);
AnalogIn lightSensor(A0); 
DigitalInOut Led1(D3);
DHT dht(D4,DHT22);  
AnalogIn thermistor(A1);

int sampI = 0;
float light[MAX_SAMPLES] = {0};

int err;

void readDHT(){
    err = dht.readData();
    if (err == 0) {
        dhtTemp=dht.ReadTemperature(CELCIUS);
        //printf("Temperature is %4.1f C \r\n",dht.ReadTemperature(CELCIUS));
        //printf("Temperature is %4.2f F \r\n",sensor.ReadTemperature(FARENHEIT));
        //printf("Temperature is %4.2f K \r\n",sensor.ReadTemperature(KELVIN));

        dhtHumid=dht.ReadHumidity();
        //printf("Humidity is %4.1f%% \r\n",dht.ReadHumidity());
        //printf("Dew point is %4.2f  \r\n",sensor.CalcdewPoint(sensor.ReadTemperature(CELCIUS), sensor.ReadHumidity()));
        //printf("Dew point (fast) is %4.2f  \r\n",sensor.CalcdewPointFast(sensor.ReadTemperature(CELCIUS), sensor.ReadHumidity()));
    } else{
        //printf("\r\nErr %i \n",err);
    }
    
}

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

void readThermistor(){
    unsigned int a, beta = 3975;
    float temperature, resistance;

    a = thermistor.read_u16(); /* Read analog value */
    
    /* Calculate the resistance of the thermistor from analog votage read. */
    resistance= (float) 10000.0 * ((65536.0 / a) - 1.0);
    
    /* Convert the resistance to temperature using Steinhart's Hart equation */
    temperature=(1/((log(resistance/10000.0)/beta) + (1.0/298.15)))-273.15; 
    thermistorValue = temperature;
    //printf("%2.1f",temperature);
}

void readSensors(){
    readAdcTemp();
    calcLightSensor();
    readDHT();
    readThermistor();
    sampI = 0;
}

void sensorSampling(){
    if (sampI<MAX_SAMPLES) {
        light[sampI] = readLightSensor();
        sampI ++;
    }
    
}



#endif