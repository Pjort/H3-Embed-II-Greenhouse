#include "mbed.h"

// Maximum number of element the application buffer can contain
#define MAXIMUM_BUFFER_SIZE 32

// Create a BufferedSerial object with a default baud rate.
static BufferedSerial serial_port(USBTX, USBRX);

// Application buffer to receive the data
char buf[MAXIMUM_BUFFER_SIZE] = {0};

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
    serial_port.read(buf, sizeof(buf));
    
    if(buf[0]!= 0){
        printf("%s \n",buf);
    }

    for (int i = 0; i<MAXIMUM_BUFFER_SIZE; i++) {
        buf[i]=0;
    }
}