//copyright milestogo 2022

#pragma once

// Todo, finish moving these to functions

static const uint8_t st_batvolts[6]="v\r"; //returns %d\r\n int * 8 cells
static const uint8_t st_gauge[6]="g\r"; // returns %d\%\r\n or "? %\r\n"
static const uint8_t st_Sstate[6]="S\r"; // returns %d\r\n
// cell status
static const uint8_t st_cells[6]="c\r"; // return 7x %c%c%c%c\r\n


// functions to talk to the MNT Reform LPC
void serial_flush(void) ;
uint8_t remote_receive_string(uint8_t print) ;

void mnt_on(void) ;
void mnt_off(void) ;
void mnt_get_sys_voltage(void); 
void mnt_get_cell_voltage(void) ;
void mnt_get_cell_amps(void) ;
void mnt_get_amps(void) ;

void mnt_get_lpc_status(void);   

void mnt_oled_brighter(void) ;
void mnt_oled_dim(void) ;
void mnt_get_power_data(void) ;/// this is the remote_get_sys_voltage() in  original firmware. (with longer delays)

 // fill global variables so they can be read by the oled function. 
void update_batt_data(void) ;


/// globals
char  bat_gauge[5] ;
uint16_t voltages[8];
uint16_t sum_volts ;
uint16_t bat_volts ;
uint16_t bat_amps  ; //current * 1000

