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

// Disabled to save space
#define NO_ACTION_ONESHOT  // 332
#define NO_ACTION_MACRO    // 0
#define NO_ACTION_FUNCTION // 0
#define DISABLE_LEADER     // 0

// OS Detection
#define OS_DETECTION_KEYBOARD_RESET
#define OS_DETECTION_SINGLE_REPORT

// Setting for sm_td
#define MAX_DEFERRED_EXECUTORS 10

// Allow for faster tapping - at the original value of 50ms some keys
// were not getting recognized.
// They way they were missed would typically be when typing "n" or "m"
// followed by a key with the right hand. The "n" or "m" would get
// interpreted as a ctrl hold (because they are set to ctrl with sm_td)
// then the next character would be some system shortcut.
// Dropping this from 50ms (the default) to 5ms seems to avoid this issue.
// The exception to this is the left and right shift keys that have
// the opposite problem (get interpreted as taps and insert "(" or ")" keys).
// Key overrides are put into the code to handle those keys.
#define SMTD_GLOBAL_RELEASE_TERM 5

