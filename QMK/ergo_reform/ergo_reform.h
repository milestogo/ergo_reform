/* Copyright 2020 milestogo
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "quantum.h"
#include "uart.h"
#include "reform_serial.h"
#include "stdio.h"

// for reasons I cant' say, 
// platforms/avr/drivers/i2c_master.h
// needs #include "stdint.h" hardcoded


enum reform_keycodes {
    M_ON = SAFE_RANGE , // powerup // key1
    M_OFF, // key 0
    M_VOLTS, // cell volts 
    M_FUEL, // system volts // Key Y 
    M_LTC,// LTS state // key S
    M_OLEDU, // brighter OLED // key F4
    M_OLEDD, // dim OLED // key F3
    M_KBDU, // brighter keyboard backlight // key F2
    M_KBDD, // dim keyboard backlight// key F1
    M_OLEDY, // oled on // key 3
    M_OLEDN, // oled off // key 2
    M_AMP, // current amps drawn
    M_CELLS, // cell status in charge
    M_CELMH, // Mah guess
    M_PWR, // pull volts, ah, cell volts. // Key V
    NEW_SAFE_RANGE,
};



enum laptop_states {
    R_COLD =0 , // cold boot, unknown state
    R_BOOT,
    R_LPC,
    R_RUN, // default states
    R_STOP,
    R_SERR, // serial error
    R_HOLD,
};


#define XXX KC_NO

#define LAYOUT_ergo_reform( \
    k50,k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k0a, k0b, k5b,\
        k10, k11, k12, k13, k14, k15, k16, k17, k18, k19, k1a, k1b, \
        k20, k21, k22, k23, k24, k25, k26, k27, k28, k29, k2a, k2b, \
        k30, k31, k32, k33, k34, k35, k36, k37, k38, k39, k3a, k3b, \
        k40, k41, k42, k43, k44, k45, k46, k47, k48, k49, k4a, k4b \
) \
{ \
    { k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k0a, k0b }, \
    { k10, k11, k12, k13, k14, k15, k16, k17, k18, k19, k1a, k1b }, \
    { k20, k21, k22, k23, k24, k25, k26, k27, k28, k29, k2a, k2b }, \
    { k30, k31, k32, k33, k34, k35, k36, k37, k38, k39, k3a, k3b }, \
    { k40, k41, k42, k43, k44, k45, k46, k47, k48, k49, k4a, k4b }, \
    { k50, XXX, XXX, XXX, XXX, XXX, XXX, XXX, XXX, XXX, XXX, k5b }, \
}
