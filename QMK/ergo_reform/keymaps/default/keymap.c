#include QMK_KEYBOARD_H
#include "ergo_reform.h"

#define ____ KC_TRNS
#define XXXX KC_NO 

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐───┐
     * │Cir│ - │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │ + │Off│
     * └───├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤───┘
     *     │Tab│ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │ \ │
     *     ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     *     │Esc│ A │ S │ D │ F │ G │ H │ J │ K │ L │ : │ " │
     *     ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     *     │Sft│ Z │ X │ C │ V │ B │ N │ M │ , │ . │ / │Sft│
     *     ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
     *     │Ctl│Gui│Lft│Rgt│Del│Bak│   │Ent│Up │dwn│Alt│Ctl│
     *     └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
     */
    [0] = LAYOUT_ergo_reform(\
  MO(1),KC_MINS,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_EQL, KC_0,
        KC_TAB,     KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,    KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN,    KC_BSLS,
        KC_ESC,     KC_A,    KC_R,    KC_S,    KC_T,    KC_G,    KC_H,    KC_N,    KC_E,    KC_I,    KC_O, KC_QUOT,
        KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,    KC_K,    KC_H,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
        KC_LCTL,    KC_LGUI, KC_LEFT, KC_RIGHT,M(2), KC_BSPC, KC_SPC,  KC_ENT,  KC_UP,   KC_DOWN, KC_RALT, KC_RCTL
    ),

// circle menu, power & LPC functions

    [1] = LAYOUT_ergo_reform(\
KC_TRNS,XXXX,       M_ON,    M_OLEDU,M_OLEDD,  M_AMP,   M_VOLTS, M_FUEL,  M_LTC,   M_CELLS,  XXXX,   M_OFF,  XXXX,  M_OFF,\
        KC_F12,     KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,\
        XXXX,       KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    XXXX,\
        XXXX,       XXXX,    XXXX,    XXXX,    XXXX,    XXXX,    XXXX,    XXXX,    XXXX,    XXXX,    XXXX,    XXXX,\
        KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
     ),


// hyper mneu functions
 [2] = LAYOUT_ergo_reform(\
KC_TRNS,KC_F12,     KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11, KC_TRNS,\
        KC_TAB,     XXXX,    XXXX,    XXXX,    XXXX,    XXXX,    XXXX,    KC_HOME, KC_UP,   KC_END,   XXXX,    KC_BSLS,
        KC_ESC,     XXXX,    XXXX,    XXXX,    XXXX,    XXXX,    XXXX,    KC_LEFT, KC_DOWN, KC_RIGHT, KC_SCLN, KC_GRAVE,
        KC_LSFT,    XXXX,    XXXX,    XXXX,    XXXX,    XXXX,    XXXX,    XXXX,    KC_LBRC, KC_RBRC,  KC_SLSH, KC_RSFT,
        KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    ),

// Qwerty
 [3] = LAYOUT_ergo_reform(\
    MO(1),KC_MINS,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_EQL, KC_1,
        KC_TAB,     KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
        KC_ESC,     KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
        KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
       ),

};
