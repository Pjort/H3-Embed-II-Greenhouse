/**
*   @file data.h
*   @brief This is the data store of all the sensors and general unit settings.
*
*   These are just stored in volitile memory, and the sensor data should be stored for history in something like a SD card. And the settings could be stored in EPROM.
* 
*   @author Pjort Pieter Kat
*   @date 11-1-2021
*/

#ifndef DATA
#define DATA
#include "mbed.h"

#define MAXIMUM_BUFFER_SIZE 32

/**
*@brief Data store for the internal temperature sensor.
*/
float adcTemp;

/**
*@brief Data storefor the analog light sensor value in precentage. Defaults to 49% to show a proper value in case sensor isn't working
*/
float lightValue=49.0f;

/**
*@brief Data store for the external analog temperature sensor in C. Default to 12.0C to show a proper value in case sensor isn't working
*/
float thermistorValue= 12.0f;

/**
*@brief Data store for the external DHT temperature sensor in C. Default to 23.2C to show a proper value in case sensor isn't working
*/
float dhtTemp=23.2f;
/**
*@brief Data store for the external DHT humidity sensor in precentage. Default to 38% to show a proper value in case sensor isn't working
*/
float dhtHumid=38.0f;

/**
*@brief Data store currently received messages via serial.
*/
char currentMessage[32] = {0};

/**
*@brief Data store for the unit name. Defaults to Greenhouse in case nothing gets set via serial.
*/
char unitName[32] = "Greenhouse";
/**
*@brief Data store for the unit number. Defaults to 1 in case nothing gets set via serial.
*/
char unitNumber[32] = "1";

/**
*@brief Stores if the unit is in demo mode or not. Defaults to false, meaning not in demo mode.
*/
int demoMode = false;

/**
*@brief Data store for the amount of watering made since the unit started.
*/
int waterings = 0;
#endif