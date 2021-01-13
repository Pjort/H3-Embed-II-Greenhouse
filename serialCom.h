#ifndef serialCom
#define serialCom

#include "mbed.h"

// Maximum number of element the application serialBuffer can contain
#define MAXIMUM_BUFFER_SIZE 32

// Create a serialBufferedSerial object with a default baud rate.
static BufferedSerial serial_port(USBTX, USBRX);

// Application serialBuffer to receive the data
char serialBuf[MAXIMUM_BUFFER_SIZE] = {0};
char storedBuf[MAXIMUM_BUFFER_SIZE] = {0};

void serialInit(){
    
 // Set desired properties (9600-8-N-1).
    serial_port.set_baud(9600);
    serial_port.set_format(
        /* bits */ 8,
        /* parity */ BufferedSerial::None,
        /* stop bit */ 1
    );
    serial_port.set_blocking(false);

}

void serialRead(){
    serial_port.read(serialBuf, sizeof(serialBuf));
    for (int i = 0; i<MAXIMUM_BUFFER_SIZE; i++) {
        if(serialBuf[i]==13){
            printf("%s \n",storedBuf);
            for (int i = 0; i<MAXIMUM_BUFFER_SIZE; i++) {
                storedBuf[i]=0;
            }
        }
        else if(serialBuf[i]!=0 & storedBuf[i]==0){
            storedBuf[i]=serialBuf[i];
        }
        else if (serialBuf[i]!=0 & storedBuf[i]!=0) {
            int temp;
            for (int j = MAXIMUM_BUFFER_SIZE-1; j>=i; j--) {
                if(storedBuf[j]==0){
                    temp=j;
                }
            }
            storedBuf[temp]=serialBuf[i];
        }

        serialBuf[i]=0;
        //printf("%s \n",storedBuf);
    }

}
#endif