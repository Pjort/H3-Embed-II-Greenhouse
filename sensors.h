/**
*   @file touch.h
*   @brief This holds all the code concerning the connected sensors.
* 
*   @author Pjort Pieter Kat
*   @date 11-1-2021
*/
#ifndef SENSORS
#define SENSORS

#include "mbed.h"
#include "data.h"
#include "DHT.h"

/**
*@brief Determins how many times between sensor delays that the sensore should be sampled.
* 
*/
#define MAX_SAMPLES  5

/**
*@brief The STM32F746G-DISCO board has a internal analog temperature sensor that needs to be initilized like this.
* 
*/
AnalogIn adc_temp(ADC_TEMP);
/**
*@brief A0 connects to analog light sensor.
* 
*/
AnalogIn lightSensor(A0); 
/**
*@brief D4 connects to DHT22 temperature and humidity sensor.
* 
*/
DHT dht(D4,DHT22);  
/**
*@brief A1 to analog temperature sensor.
* 
*/
AnalogIn thermistor(A1);

/**
*@brief sampI is used for iterating through the amount of samples in between each sensor read.
* 
*/
int sampI = 0;
/**
*@brief Making array for holding light sensor sample data
* 
*/
float light[MAX_SAMPLES] = {0};
/**
*@brief This controls the threshhold for when the light is considered low.
*
*Defaults to 50. But should probably be messured for different values depending on day/night cycle to make more sense.  
*/
float lowLightThreshold=50.0f;




/**
*@brief Reads the DHT and sotres temp and humid into the DATA.h stores.
* 
*@see dhtTemp
*@see dhtHumid
*/
void readDHT(){
    int err;
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

/**
*@brief Reads the internal temperature stores it into the DATA.h store.
* 
*@see adcTemp
*/
void readAdcTemp(){
    adcTemp = (adc_temp.read()*100);
}

/**
*@brief Return the float value of the analog read of the light sensor
* 
*For easy it is multiplied with 100 to make it a value between from 100 to 0 instead of 1 to 0. The sensor works best at 5v volts, but the other sensors require 3v3, so I have tried to compensate by dividing with a constant of 0.63
*/
float readLightSensor(){
    return(lightSensor.read()/0.63*100); //3v3 mode
    //return(lightSensor.read()*100);  //5v mode
}

/**
*@brief This calculates the average light sensor values from the amount of samples it managed to make since last read. Then stores it to the Data.h store.
* 
*Since the code is running in a non-blocking and sort of async way it isn't always know how many samples has been made, therefore it has to go through the samples array and make sure how many holds data. \n
*An improvement to this could be to take out the highest and lowest value
*@see lightValue
*/
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

/**
*@brief Reads the external analog temperature sensor and stores it in DATA.h
*
*@warning this sensor needs 3v3 to work with the calculations of resistance.  
*@see thermistorValue
*/
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

/**
*@brief This reads all the sensors and resets the sample array counter.
* 
*/
void readSensors(){
    readAdcTemp();
    calcLightSensor();
    readDHT();
    readThermistor();
    sampI = 0;
}

/**
*@brief This samples all the sensors. (Currently only the light sensor)
* 
*/
void sensorSampling(){
    if (sampI<MAX_SAMPLES) {
        light[sampI] = readLightSensor();
        sampI ++;
    }
    
}


#endif