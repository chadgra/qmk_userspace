 /* Copyright 2020 Josef Adamcik
  * Modification for VIA support and RGB underglow by Jens Bonk-Wiltfang
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

/* The way how "handedness" is decided (which half is which),
see https://docs.qmk.fm/#/feature_split_keyboard?id=setting-handedness
for more options.
*/

// More lighting options in quantum/rgblight/rgblight.h
#ifdef RGBLIGHT_ENABLE
#   define RGBLIGHT_SLEEP
#   define RGBLIGHT_DISABLE_KEYCODES
#    define RGBLIGHT_DEFAULT_MODE RGBLIGHT_MODE_STATIC_LIGHT

// Disabling some of these is a good way to save flash space.
//#    define RGBLIGHT_EFFECT_ALTERNATING     // 108
//#    define RGBLIGHT_EFFECT_RGB_TEST        // 158
//#    define RGBLIGHT_EFFECT_RAINBOW_MOOD    // 160
//#    define RGBLIGHT_EFFECT_STATIC_GRADIENT // 168
//#    define RGBLIGHT_EFFECT_RAINBOW_SWIRL   // 192
//#    define RGBLIGHT_EFFECT_BREATHING       // 348
//#    define RGBLIGHT_EFFECT_KNIGHT          // 336
//#    define RGBLIGHT_EFFECT_SNAKE           // 406
//#    define RGBLIGHT_EFFECT_CHRISTMAS       // 508
/*
#define RGBLIGHT_EFFECT_TWINKLE         // 1156
*/
#endif

// #define MK_3_SPEED
// #define MK_MOMENTARY_ACCEL
#define MK_KINETIC_SPEED
// #define MK_COMBINED
#define MOUSEKEY_DELAY          16
#define MOUSEKEY_INTERVAL       50
#define MOUSEKEY_MOVE_DELTA     8

// Disabled to save space
#define NO_ACTION_ONESHOT  // 332
#define NO_ACTION_MACRO    // 0
#define NO_ACTION_FUNCTION // 0
#define DISABLE_LEADER     // 0

#define TAPPING_TOGGLE     2

// Enable Caps word when both left and right shift keys are pressed
#define BOTH_SHIFTS_TURNS_ON_CAPS_WORD

// OS Detection
#define OS_DETECTION_KEYBOARD_RESET
#define OS_DETECTION_SINGLE_REPORT

// Control / paren setup
#define LCPO_KEYS KC_LEFT_CTRL, KC_TRNS, KC_LBRC
#define RCPC_KEYS KC_RIGHT_CTRL, KC_TRNS, KC_RBRC
#define LAPO_KEYS KC_LEFT_GUI, KC_TRNS, KC_LBRC
#define RAPC_KEYS KC_RIGHT_GUI, KC_TRNS, KC_RBRC
