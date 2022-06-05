/* Copyright 2020 Milestogo */
// copyright milestogo 2022

#include "ergo_reform.h"

#define OLED_MODE_TIMER 1000
#define OLED_BATT_TIMER 2000
static uint16_t oled_status_timer; // how long to keep non battery stuff on oled
static uint16_t oled_batt_timer;   /// track how often to update power data

char vbuf[7]; // used bo oled_fload function. 



__attribute__((weak)) void matrix_init_kb(void) {
// Turn status LED on, with the exception of THK
#if defined(__AVR_ATmega32U4__)
    setPinOutput(E6);
    writePinHigh(E6);
#endif

    // init HW serial to LPC.
    uart_init(57600);

    oled_status_timer = timer_read();

    matrix_init_user();
}

#ifdef OLED_ENABLE
// rotation 180 on reform with 4 pin through hole
oled_rotation_t oled_init_kb(oled_rotation_t rotation) {
    return OLED_ROTATION_180;
}

uint8_t oled_state     = 0; // State sets which laptop display to do.
uint8_t oled_laststate = 0;

static void render_qmk_logo(void) {
    static const char PROGMEM qmk_logo[] = {0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0x00};

    oled_write_P(qmk_logo, false);
}

void insert_bat_icon(char* str, int x, float v) {
    char icon = 0;
    if (v >= 3.3 * 1000 ) {
        icon = 0;
    } else if (v >= 3.1 * 1000 ) {
        icon = 2;
    } else if (v >= 3.0 * 1000 ) {
        icon = 4;
    } else if (v >= 2.9 * 1000 ) {
        icon = 6;
    } else {
        icon = 8;
    }
    str[x]     = 214 + icon  ;
    str[x + 1] = 214 + icon +1 ;
}


void mnt_bat_gauge(void) {
        char str[32];

         oled_clear();
            oled_set_cursor(0, 0);  
            sprintf(str, "[] %1d.%1d  [] %1d.%1d %%%2s",  voltages[0]/1000 , (voltages[0]%1000)/100, \
                                                    voltages[4]/1000 , (voltages[4]%1000)/100 ,bat_gauge );
            insert_bat_icon(str, 0, voltages[0]);
            insert_bat_icon(str, 8, voltages[4]);
            oled_write_ln(str,false);

            oled_set_cursor(0,1);
            sprintf(str, "[] %1d.%1d  [] %1d.%1d", voltages[1]/1000 , (voltages[1]%1000)/100, \
                    voltages[5]/1000 , (voltages[5]%1000)/100   );
            insert_bat_icon(str, 0, voltages[1]);
            insert_bat_icon(str, 8, voltages[5]);
            oled_write_ln(str,false);


           oled_set_cursor(0,2);
           sprintf(str, "[] %1d.%1d  [] %1d.%1d A-%1d.%2d", voltages[2]/1000 , voltages[2]%1000/100, \
                    voltages[6]/1000 , (voltages[6]%1000)/100, bat_amps/100, (bat_amps%100)/10 );
            insert_bat_icon(str, 0, voltages[2]);
            insert_bat_icon(str, 8, voltages[6]);
            oled_write_ln(str,false);


           oled_set_cursor(0,3);
             sprintf(str, "[] %1d.%1d  [] %1d.%1d V%2d.%1d", voltages[3]/1000 , (voltages[3]%1000)/100, \
                    voltages[7]/1000 , (voltages[7]%1000)/100, bat_volts/100, (bat_volts%100)/10 );
            insert_bat_icon(str, 0, voltages[3]);
            insert_bat_icon(str, 8, voltages[7]);
            oled_write_ln(str,false);
}

bool oled_task_kb(void) {

    if (!oled_task_user()) {
        return false;
    }

 
    switch (oled_state) {
        case R_COLD:
            oled_clear();
            render_qmk_logo();
            break;

        case R_BOOT:
            oled_clear();
            oled_write_ln_P(PSTR("Powerup..."), false);
            break;

        case R_STOP:
            oled_clear();
            oled_write_ln_P(PSTR("Shutdown..."), false);
            break;

        case R_RUN: // cruising update
            mnt_bat_gauge();
            break;

        case R_LPC:
            mnt_get_lpc_status();
            break;

        case R_SERR:
            oled_write_ln_P(PSTR("Serial Error "), false);
            break;

        }


   if (oled_state != oled_laststate) {
        oled_status_timer = timer_read();
        oled_laststate    = oled_state;
    } 

    if (timer_elapsed(oled_status_timer) >= OLED_MODE_TIMER) {
        if (oled_state != R_RUN) {
                oled_state = R_RUN;
        }

        if (timer_elapsed(oled_batt_timer) > OLED_BATT_TIMER) {
                // we've been in run for more than 2 seconds, time to do the serial thing again.
                //update_batt_data();
        }

        oled_status_timer = timer_read();

    }


    return false;
}

#endif

bool process_record_kb(uint16_t keycode, keyrecord_t* record) {
    switch (keycode) {
        case M_ON:
            if (record->event.pressed) {
                oled_state = R_BOOT;
                mnt_on();
               // SEND_STRING("power");
            }
            return false;

        case M_OFF:
            if (record->event.pressed) {
                oled_state = R_STOP;
                mnt_off();
            }
            return false;

        case M_LTC:
            if (record->event.pressed) {
                oled_state = R_LPC;
                mnt_get_lpc_status();
            }
            return false;

            /* No longer think this is the way
              case M_AMP:
                    if (record->event.pressed) {
                        mnt_get_amps();
                    }
                    return false;

               case M_CELV:
                    if (record->event.pressed) {
                        mnt_on();
                    }
                    return false;


              case M_VOLTS:
                    if (record->event.pressed) {
                        mnt_get_sys_voltage();
                    }
                    return false;

              case M_FUEL:
                    if (record->event.pressed) {
            #ifdef OLED_ENABLE
                        oled_clear();
            #endif
                       mnt_get_sys_voltage();
                    }
                    return false;



              case M_CELLS:
                    if (record->event.pressed) {
            #ifdef OLED_ENABLE
                        oled_clear();
            #endif
                        mnt_get_cell_amps();
                    }
                    return false;


            */

#ifdef OLED_ENABLE
        case M_OLEDU:
            if (record->event.pressed) {
                mnt_oled_brighter();
            }
            return false;

        case M_OLEDD:
            if (record->event.pressed) {
                mnt_oled_dim();
            }
            return false;

        case M_OLEDY:
            if (record->event.pressed) {
                oled_on();
            }
            return false;

        case M_OLEDN:
            if (record->event.pressed) {
                oled_off();
            }
            return false;

#endif

#ifdef BACKLIGHT_ENABLE
        case M_KBDU:
            if (record->event.pressed) {
                backlight_increase();
            }
            return false;

        case M_KBDD:
            if (record->event.pressed) {
                backlight_decrease();
            }
            return false;
#endif
      }
    
    return process_record_user(keycode,record);
}

// TODO make suspend more ambitious.
// see https://github.com/qmk/qmk_firmware/blob/master/docs/power.txt


/// Stock QMK suspend won't work, since it's based on USB. 
void suspend_power_down_kb(void) {
    // code will run multiple times while keyboard is suspended
#ifdef OLED_ENABLE
    oled_off();
#endif
    suspend_power_down_user();
}

void suspend_wakeup_init_kb(void) {
    // code will run on keyboard wakeup
#ifdef OLED_ENABLE
    oled_on();
#endif
#ifdef RGBLIGHT

#endif
    suspend_wakeup_init_user();
}

//good stuff for power saving in keyboards/bioi/main.c 

