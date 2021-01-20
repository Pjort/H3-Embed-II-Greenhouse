#ifndef STATE
#define STATE
#include "timers.h"
#include "data.h"
#include "lcd.h"
#include "sensors.h"
#include "touch.h"


enum State {STARTUP, IDLE, SAMPLE_SENSOR, READ_SENSOR, UPDATE_SCREEN, INTERUPT};

enum StartupInput {NOTHING,NAME,NUMBER};

int currentState = STARTUP;
//int currentState = IDLE;
int currentStartupInput = NOTHING;

int mainScreenDrawn = 0;

void runStates(){
    serialTimer();
    switch (currentState) {
        
        case STARTUP:
            if (currentStartupInput == NOTHING) {
                printf("Write unit name: (Press enter twice)");
                currentStartupInput = NAME;
            }

            if (currentMessage[0]!=0) {
                for (int i = 0; i<MAXIMUM_BUFFER_SIZE; i++) {
                    if (currentStartupInput == NAME) {
                        unitName[i]=currentMessage[i];
                    }else if (currentStartupInput == NUMBER) {
                        unitNumber[i]=currentMessage[i];
                    }
                    currentMessage[i]=0;
                }

                if (currentStartupInput == NAME) {
                    printf("Write unit number: (Press enter twice)");
                    currentStartupInput = NUMBER;
                }else if (currentStartupInput == NUMBER) {
                    currentState = IDLE;
                }
                
            }

            // Check for touch to skip to IDLE state
            if(newTouch){
                currentState=IDLE;
                newTouch = false;
            }

            // Sample some data while in startup to make the display show proper values from the get go.
            if (sensorSampleTimer()) {
                sensorSampling();
            }
            // Read sensors for some data while in startup to make the display show proper values from the get go.
            if (sensorReadTimer()){
                readSensors();
            }
            
            break;

        case IDLE:
            if(mainScreenDrawn == 0){       //Check that first time IDLE mainscreen is drawn
                drawMainScreen();
                mainScreenDrawn = 1;
            }
            if(newTouch){
                currentState = INTERUPT;
            }

            if (sensorReadTimer()){
                currentState = READ_SENSOR;
            }

            if (sensorSampleTimer()) {
                currentState = SAMPLE_SENSOR;
            }

            if (secondTimer()){
                currentState = UPDATE_SCREEN;
            }
            
            break;
        case SAMPLE_SENSOR:
            sensorSampling();
            currentState = IDLE;
            break;
        case READ_SENSOR:
            readSensors();
            checkWatering();
            currentState = UPDATE_SCREEN;
            break;
        case UPDATE_SCREEN:
            screenRefresh();
            currentState = IDLE;
            break;
        case INTERUPT:
            if(newTouch){
                newTouch = false;
            }
            currentState = IDLE;
            break;
    }

    
}



#endif