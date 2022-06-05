// Copyright 2022 milestogo (@milestogo)
// SPDX-License-Identifier: GPL-2.0-or-later


/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT

#undef LOCKING_SUPPORT_ENABLE
#undef LOCKING_RESYNC_ENABLE

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE    5

#define LAYER_STATE_8BIT

//#define RGBLIGHT_ANIMATIONS
//#define RGB_DI_PIN D5
//#define RGBLED_NUM 4

#define OLED_BRIGHTNESS 128
#define OLED_UPDATE_INTERVAL 99
#define OLED_FONT_H "keyboards/ergo_reform/oledfont.c"

// do not put microcontroller into power down. 
#define NO_SUSPEND_POWER_DOWN
