/**
*   @file serialCom.h
*   @brief This holds all the code concerning serial comunication.
* 
*   @author Pjort Pieter Kat
*   @date 11-1-2021
*/

#ifndef serialCom
#define serialCom

#include "mbed.h"
#include "data.h"

/**
*@brief Create a serialBufferedSerial object to the USB port.
*/
static BufferedSerial serial_port(USBTX, USBRX);

/**
*@brief SerialBuffer to receive the data.
*/
char serialBuf[MAXIMUM_BUFFER_SIZE] = {0};
/**
*@brief storedBuf to store received the data.
*/
char storedBuf[MAXIMUM_BUFFER_SIZE] = {0};

/**
*@brief Initialize the serial connection properties, making sure it isn't blocking.
*/
void serialInit(){
    
 // Set desired properties (9600-8-N-1).
    serial_port.set_baud(9600);
    serial_port.set_format(
        /* bits */ 8,
        /* parity */ BufferedSerial::None,
        /* stop bit */ 1
    );

    serial_port.set_blocking(false);    //Set the serial connection not to block.

}

/**
*@brief My attempt to make a serial read that isn't blocking and is getting called many times a second and making sure that it stores chars untill CR is hit.
*/
void serialRead(){
    serial_port.read(serialBuf, sizeof(serialBuf));
    for (int i = 0; i<MAXIMUM_BUFFER_SIZE; i++) {
        if(serialBuf[i]==13){                               // 13=CR (enter)
            for (int i = 0; i<MAXIMUM_BUFFER_SIZE; i++) {
                currentMessage[i]=storedBuf[i];
                storedBuf[i]=0;                     //clear out the storedBuf when it has been shipped to the currentMessage buffer.
            }
            printf("%s\n",currentMessage);
        }
        else if(serialBuf[i]!=0 & storedBuf[i]==0){
            storedBuf[i]=serialBuf[i];
        }
        else if (serialBuf[i]!=0 & storedBuf[i]!=0) {
            int nextEmpty;
            for (int j = MAXIMUM_BUFFER_SIZE-1; j>=i; j--) {    //Find the next empty char in the stored array to store from the serial buffer.
                if(storedBuf[j]==0){
                    nextEmpty=j;
                }
            }
            storedBuf[nextEmpty]=serialBuf[i];
        }

        serialBuf[i]=0;     //clear out the serialBuffer when all done.
    }

}
#endif