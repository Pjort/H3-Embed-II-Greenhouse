/**
*   @file state.h
*   @brief This holds all the code concerning the state machine. See runStates for picture.
* 
*   @see runStates
*   @see State
*   @author Pjort Pieter Kat
*   @date 11-1-2021
*/
#ifndef STATE
#define STATE
#include "timers.h"
#include "data.h"
#include "lcd.h"
#include "sensors.h"
#include "touch.h"
#include "interupts.h"

/**
*@brief Enums for the different states for the state machine. See runStates for picture.
* 
*@see runStates
*/
enum State {STARTUP, IDLE, SAMPLE_SENSOR, READ_SENSOR, UPDATE_SCREEN, INTERUPT};

enum StartupInput {NOTHING,NAME,NUMBER};

/**
*@brief This holds the current state of the state machine. It defaults to start in STARTUP but can be changed the IDLE to skip the welcome screen.
* 
*@see State
*/
int currentState = STARTUP;

/**
*@brief Controls what type of information is asked for in the serial connection at STARTUP state
*/
int currentStartupInput = NOTHING;

/**
*@brief This makes sure that the main screen only gets drawn once when entering the IDLE, preventing flickering of the screen.
*/
int mainScreenDrawn = 0;

/**
*@brief This get run repeatedly many times a second and is the state machine.
* 
*@image html stateMachine.png
*/
void runStates(){
    
    switch (currentState) {
        /**
        *STARTUP state just shows the welcome screen and askes for unit name and number via serial connection.
        * 
        */
        case STARTUP:
            serialTimer();
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
        /**
        *IDLE is where the bulk of the time of the loop with reside. From here it will check if it is time to give the state away to sensors or screen updates etc.
        */
        case IDLE:
            serialTimer();
            if(mainScreenDrawn == 0){       //Check that first time IDLE mainscreen is drawn
                drawMainScreen();
                printf("\nWrite: 1=temperature,2=humidity, 3=light");
                mainScreenDrawn = 1;
            }
            if(newTouch){                   //Checks for a touch on the display is detected. If true send to INTERUPT state
                currentState = INTERUPT;
            }
            if(currentMessage[0]!=0){       //Checks if there has been input something through the serial. If true send to INTERUPT state
                currentState = INTERUPT;
            }

            if (sensorReadTimer()){         //Checks if it is time to read sensors. If true send to READ_SENSOR state
                currentState = READ_SENSOR;
            }

            if (sensorSampleTimer()) {      //Checks if it is time to sample sensors. If true send to SAMPLE_SENSOR state
                currentState = SAMPLE_SENSOR;
            }

            if (secondTimer()){             //Checks if a second has passed to update the clock on the display. If true send to UPDATE_SCREEN state
                currentState = UPDATE_SCREEN;
            }
            
            break;

        /**
        *@brief SAMPLE_SENSOR just runs the sensorSampling and returns the state to IDLE.
        */
        case SAMPLE_SENSOR:
            sensorSampling();
            currentState = IDLE;
            break;

        /**
        *@brief READ_SENSOR reads the sensors and check if watering is needed and forwards the state to UPDATE_SCREEN to draw all the value changes.
        */
        case READ_SENSOR:
            readSensors();
            checkWatering();
            currentState = UPDATE_SCREEN;
            break;
        /**
        *@brief UPDATE_SCREEN just runs screenRefresh and returns the state to IDLE.
        */
        case UPDATE_SCREEN:
            screenRefresh();
            currentState = IDLE;
            break;
        /**
        *@brief INTERUPT just checks and runs the touch or serial comand that initiated the interupt and then returns state to IDLE 
        */
        case INTERUPT:
            if(newTouch){
                int buttonReturn = checkTouchButton();
                if(buttonReturn==2){
                    toggleDemoMode();
                }
                else if(buttonReturn!=0){
                    if(demoMode){                       //If in demo mode repurpose the left and right button for +-1 hour to the RTC time
                        addHoursToRTC(buttonReturn);
                        screenRefresh();
                    }else if (buttonReturn==-1) {
                        //Left button code
                    }else if (buttonReturn==1) {
                        //Right button code
                    }

                }
                
                newTouch = false;
            }

            if (currentMessage[0]!=0) {
                newSerialComand();
            }

            currentState = IDLE;
            break;
    }

    
}



#endif