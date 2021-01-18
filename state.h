#ifndef STATE
#define STATE
#include "timers.h"
#include "data.h"
#include "lcd.h"
#include <cstdio>


enum State {STARTUP, IDLE, READ_SENSOR, UPDATE_SCREEN, INTERUPT};

enum StartupInput {NOTHING,NAME,NUMBER};

//int currentState = STARTUP;
int currentState = IDLE;
int currentStartupInput = NOTHING;

int mainScreenDrawn = 0;

void runStates(){
    serialTimer();
    switch (currentState) {
        case STARTUP:

            //printf("%i",currentStartupInput);

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
            
            break;

        case IDLE:
            if(mainScreenDrawn == 0){       //Check that first time IDLE mainscreen is drawn
                drawMainScreen();
                mainScreenDrawn = 1;
            }

            if (sensorReadTimer()){
                currentState = READ_SENSOR;
            }
            
            break;
        case READ_SENSOR:

            currentState = UPDATE_SCREEN;
            break;
        case UPDATE_SCREEN:
            
            currentState = IDLE;
            break;
        case INTERUPT:

            break;
    }

    
}



#endif