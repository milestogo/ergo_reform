/* Copyright 2020 Milestogo */


#include "ergo_reform.h"

#define SERWAIT 8000      // rounds of waiting for uart to be available.
#define SERBUF 64         // size of the serial response buffer
char   ser_in[SERBUF]; // making this a global so we can pluck from it directly. 


/// globals
uint16_t sum_volts    = 1250;
uint16_t bat_volts    = 1123;
uint16_t bat_amps     = 140; //current * 1000
char  bat_gauge[5] = "42"; // normally contains a \r at the end. We get it as a %d%%\r, where 0 <=number <=100

uint16_t voltages[8] = {3400, 3200, 3100 ,3000, 900 ,1000,1100,3210}; // we get a series of 

// take string, prepare a Mnt Reform command for uart_transmit 
// by adding \r automatically, sends out via serial. 
void strToMnt ( const char * string ){
    static uint8_t buff[4+1+1+1]; // four digits, 1 character, one return, one null
    uint8_t len = strlen(string);

    if (len>5 ) { len = 5;}

    for (uint8_t x=0; x<len ; x++) {
        buff[x]=(uint8_t) string[x];
    }
    buff[len]= (uint8_t) '\r';
    buff[len+1]=0;
    
    uart_transmit(buff, len+1);
    _delay_ms(1000);
}



void requestBattVolts(uint8_t  battery) {
    char batrequest[3];
    batrequest[0]=48+ battery; // 48 = '0'
    batrequest[1]='g';
    batrequest[2]=0;

    strToMnt ( batrequest );

}
void serial_flush(void) {
    // copied from mnt reform empty_serial function. Don't know why they eat up to 1000 characters
    uint16_t countr = 0;
    while (!uart_available()) { // spin for a while.
        countr++;
        if (countr > SERWAIT) {
            break;
        }
    }
    countr = 0;
    while (uart_available() && countr < 1000) { //
        uart_read();
        countr++;
    }

}

/// Function to listen for a response from the LPC. 
// if Print is defined, display to oled or debug print
// stuff what we find into global ser_in string. 
uint8_t mntToStr(uint8_t print) {
    extern char ser_in[SERBUF]; // global
    uint16_t       countr =0;
    char    chr;

    while (!uart_available() && countr < SERWAIT) {
        countr++;
    }
    if (countr >= SERWAIT) {
         serial_flush();
        return (0); // bug, we arent changing oled_state to show user the error
    }

    countr=0;

    while (uart_available() && countr < SERBUF-1) {
        chr = (char) uart_read(); //  we know MNT only sends printables 
            /// uart_read is blocking , but we just got a uart_available
        if (chr == '\n') {
            chr = ' '; // write a space
        }
        if (countr < SERBUF-1) {
            ser_in[countr++]   = (uint8_t) chr;
        }
   
        if (chr == '\r') {
            ser_in[countr] = 0;
            if (print) {
                dprintf("%s", ser_in);
            }
            return (1);
            // end the command. Don't flush whatever is in uart?
        } 
            
     }

    if ( countr < SERBUF-1 ){
        ser_in[countr]=0;
    } else  {
       // we got to end of buffer without finding \r
       // oled_state=R_SERR;
        ser_in[SERBUF-1]=0; // make safe for outside functions
   }

    if (print) {
        // bug, this ignores print variable for now in favor of debugging. 
         dprintf("%s abend", ser_in);
        
     }
    serial_flush();
    return (0);
}



#ifdef OLED_ENABLE
void mnt_oled_brighter(void) {
    uint8_t bright;

    bright = oled_get_brightness();
    if (bright < 240) {
        oled_set_brightness(bright + 0x10);
    } else {
        oled_set_brightness(255);
    }
}

void mnt_oled_dim(void) {
    uint8_t bright;
    bright = oled_get_brightness();
    if (bright > 0x10) {
        oled_set_brightness(bright - 0x10);
    } else {
        oled_set_brightness(0);
    }
}

#endif




void mnt_on(void) {
    strToMnt ( "1p");
    serial_flush();
}

void mnt_off(void) {
    strToMnt ( "0p");
    serial_flush();
}

void update_batt_data(void) {
    serial_flush();
    strToMnt ( "V");
    mntToStr(0); 
    bat_volts = (atoi(ser_in));

    serial_flush();
    strToMnt ( "a");
    mntToStr(0);
    bat_amps = (atoi(ser_in));


    serial_flush();
    strToMnt ( "g");
    mntToStr(0);
    strncpy(bat_gauge, ser_in, 4);

    serial_flush();
    sum_volts=0;
    for (uint8_t i = 0; i < 8; i++) {
        requestBattVolts(i);
        mntToStr(0);
        voltages[i] = atoi( ser_in);
        if (voltages[i] < 0 || voltages[i] >= 5) {
            voltages[i] = 0;
        }
        sum_volts += voltages[i];
    }

}

/// Below here are debugging versions. 
void mnt_get_lpc_status(void) { // returns text string of 14chars\ \[%d\]\rf
    serial_flush();
    strToMnt ( "s");
    mntToStr(1);
}


void mnt_get_sys_voltage(void) { // returns %d\r\n int volts

    serial_flush();
    strToMnt ( "V");
    mntToStr(1);
}

void mnt_get_amps(void) { // returns %d\r\n int amps
    serial_flush();
    strToMnt ( "a");
    mntToStr(1);
}

void mnt_get_cell_amps(void) { // return %d\/%d\/%d\r\n  -accurate/min/max
    serial_flush();
    strToMnt ( "C");
    mntToStr(1);
}

