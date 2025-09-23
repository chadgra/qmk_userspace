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

#include QMK_KEYBOARD_H

#include "oled.c"
#include "encoder.c"

#include "sm_td.h"

// Base layer is the number of layers CYCLE selects from.
#define BASE_LAYERS 2

enum custom_keycodes {
    PLACEHOLDER = SAFE_RANGE,   // can always be here (4 bytes)

    // Custom keys used on both Mac and Windows
    CKC_LALT,
    CKC_RALT,

    // Custom keys used on Mac
    CKM_MOVE,
    CKM_LGUI,
    CKM_RGUI,
    CKM_LCTL,
    CKM_RCTL,

    // Custom keys used on Windows
    CKW_MOVE,
    CKW_LCTL,
    CKW_RCTL,
    CKW_LWIN,
    CKW_RWIN,
};

enum custom_layers {
    _BASE,
    _BASE_W,
    _MOVE,
    _MOVE_W,
    _LOWER,
    _RAISE
};

//Default keymap. This can be changed in Via. Use oled.c to change beavior that Via cannot change.
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * _BASE and _BASE_W
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |-------.  E  ,-------|   6  |   7  |   8  |   9  |   0  |  `   |
 * |------+------+------+------+------+------|C+Right|< N >|SCRN_LT|------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |-------.  C  ,-------|   Y  |   U  |   I  |   O  |   P  | Bspc |
 * |------+------+------+------+------+------|       |< O >|MS_BTN1|------+------+------+------+------+------|
 * | MOVE |   A  |   S  |   D  |   F  |   G  |-------.  D  ,-------|   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------|C+Left |< E >|SCRN_RT|------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   V  |   B  |-------|  R  |-------|   N  |   M  |   ,  |   .  |   /  |RShift|
 * `-----------------------------------------/       /      \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |LOWER | /Enter  /        \Space \  |RAISE | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /          \      \ |      |      |      |      |
 *            `-----------------------------------'            '------''---------------------------'
 */
[_BASE] = LAYOUT(
   KC_ESC, KC_1   , KC_2   , KC_3   , KC_4   , KC_5   ,                         KC_6    , KC_7   , KC_8   , KC_9   , KC_0   , KC_GRV ,
   KC_TAB, KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   ,C(KC_RGHT),     KC_PGUP, KC_Y    , KC_U   , KC_I   , KC_O   , KC_P   , KC_BSPC,
 CKM_MOVE, KC_A   , KC_S   , KC_D   , KC_F   , KC_G   ,TG(_BASE_W),    MS_BTN1, KC_H    , KC_J   , KC_K   , KC_L   , KC_SCLN, KC_QUOT,
  SC_LSPO, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   ,C(KC_LEFT),     KC_PGDN, KC_N    , KC_M   , KC_COMM, KC_DOT , KC_SLSH, SC_RSPC,
               CKM_LGUI,CKC_LALT,CKM_LCTL,TT(_LOWER), KC_ENT ,           KC_SPC ,TT(_RAISE),CKM_RCTL,CKC_RALT,CKM_RGUI
),

[_BASE_W] = LAYOUT(
  _______, _______, _______, _______, _______, _______,                         _______, _______, _______, _______, _______, _______,
  _______, _______, _______, _______, _______, _______, A(KC_TAB),     _______, _______, _______, _______, _______, _______, _______,
 CKW_MOVE, _______, _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______, _______, _______,
  _______, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   ,A(S(KC_TAB)),   _______, KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, _______,
                  CKW_LCTL,CKC_LALT,CKW_LWIN, _______, _______,          _______, _______,CKW_RWIN,CKC_RALT,CKW_RCTL
),

/*
 * _MOVE and _MOVE_W
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | BASE |      |      |      |      |      |-------.  E  ,-------|      |      | PgUp |      | Home |      |
 * |------+------+------+------+------+------| Pg Up |< N >| VolDn |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------.  C  ,-------|      |      |      |      |      | Bspc |
 * |------+------+------+------+------+------| Mute  |< O >|       |------+------+------+------+------+------|
 * |      | LGUI |M_PAGE|M_WORD| CAPS |      |-------.  D  ,-------| Left | Down |  Up  | Right|      | End  |
 * |------+------+------+------+------+------| Pg Dn |< E >| VolUp |------+------+------+------+------+------|
 * |LShift|      |      |      |      |      |-------|  R  |-------|      |      |PgDown|      |      |RShift|
 * `-----------------------------------------/       /      \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |      | /LShift /        \      \  | Menu | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /          \      \ |      |      |      |      |
 *            `-----------------------------------'            '------''---------------------------'
 */
[_MOVE] = LAYOUT(
TO(_BASE), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                         XXXXXXX, XXXXXXX, KC_PGUP, XXXXXXX,G(KC_LEFT), XXXXXXX,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_PGUP,       KC_VOLD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_BSPC,
  _______, KC_LGUI, KC_LCTL, KC_LALT, KC_CAPS, XXXXXXX,   KC_NO,       KC_MPLY, KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, XXXXXXX,G(KC_RGHT),
  _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_PGDN,       KC_VOLU, XXXXXXX, XXXXXXX, KC_PGDN, XXXXXXX, XXXXXXX, _______,
                  _______, _______, _______, XXXXXXX, KC_LSFT,           XXXXXXX,  KC_APP, _______, _______, _______
),

[_MOVE_W] = LAYOUT(
  _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, KC_HOME, _______,
  _______, _______, _______, _______, _______, _______, _______,    _______, _______, _______, _______, _______, _______, _______,
  _______, _______, KC_LALT, KC_LCTL, _______, _______, _______,    _______, _______, _______, _______, _______, _______, KC_END ,
  _______, _______, _______, _______, _______, _______, _______,    _______, _______, _______, _______, _______, _______, _______,
                   _______, _______, _______, _______, _______,      _______, _______, _______, _______, _______
),

/*
 * LOWER
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |  F1  |  F2  |  F3  |  F4  |  F5  |-------.  E  ,-------|  F6  |  F7  |  F8  |  F9  | F10  | F11  |
 * |------+------+------+------+------+------|       |< N >|       |------+------+------+------+------+------|
 * |  `   |   1  |   2  |   3  |   4  |   5  |-------.  C  ,-------|   6  |   7  |   8  |   -  |   =  | F12  |
 * |------+------+------+------+------+------|       |< O >|       |------+------+------+------+------+------|
 * | Tab  |   !  |   @  |   #  |   $  |   %  |-------.  D  ,-------|   ^  |   &  |   *  |   [  |   ]  |   \  |
 * |------+------+------+------+------+------|       |< E >|       |------+------+------+------+------+------|
 * | Shift|  =   |  -   |  +   |   {  |   }  |-------|  R  |-------|   [  |   ]  |   ;  |   :  |   \  |      |
 * `-----------------------------------------/       /      \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |LOWER | /Enter  /        \Space \  |RAISE | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /          \      \ |      |      |      |      |
 *            `-----------------------------------'            '------''---------------------------'
 */
[_LOWER] = LAYOUT(
  _______, KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5  ,                         KC_F6  , KC_F7  , KC_F8  , KC_F9  , KC_F10 , KC_F11 ,
  KC_GRV , KC_F11 , KC_F12 , KC_3   , KC_4   , KC_5   , _______,       _______, KC_6   , KC_7   , KC_8   , KC_MINS, KC_EQL , KC_DEL ,
  _______, KC_EXLM, KC_AT  , KC_HASH, KC_DLR , KC_PERC, _______,       _______, KC_CIRC, KC_AMPR, KC_ASTR, KC_LBRC, KC_RBRC, KC_BSLS,
  _______, KC_EQL , KC_MINS, KC_PLUS, KC_LCBR, KC_RCBR, _______,       _______, KC_LBRC, KC_RBRC, KC_SCLN, KC_COLN, KC_BSLS, _______,
                  _______, _______, _______, _______, _______,           _______, _______, _______, _______, _______
),

/*
 * RAISE
 * ,----------------------------------------.                      ,-----------------------------------------.
 * |Cycle |      |      |      |      |      |-------.  E  ,-------|      |      |      |      |      |      |
 * |------+------+------+------+------+------|       |< N >|       |------+------+------+------+------+------|
 * | Esc  | Ins  | Pscr | Menu |  C+R |      |-------.  C  ,-------|      |      |  Up  |      | DLine| Bspc |
 * |------+------+------+------+------+------|       |< O >|       |------+------+------+------+------+------|
 * | Tab  | LAt  | LCtl |LShift|      | Caps |-------.  D  ,-------|      | Left | Down | Rigth|  Del | Bspc |
 * |------+------+------+------+------+------|       |< E >|       |------+------+------+------+------+------|
 * |Shift | Undo |  Cut | Copy | Paste|  C+B |-------|  R  |-------|      | LStr |      | LEnd |      | Shift|
 * `-----------------------------------------/      /       \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |LOWER | /Enter /         \Space \  |RAISE | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/      /           \      \ |      |      |      |      |
 *            `----------------------------------'             '------''---------------------------'
 */
[_RAISE] = LAYOUT(
  _______, _______, _______, _______, _______, _______,                       _______, _______, _______, _______, _______, _______,
  _______, XXXXXXX, XXXXXXX,   MS_UP, C(KC_R), XXXXXXX, XXXXXXX,    XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, KC_BSPC,
  _______, XXXXXXX, MS_WHLL, MS_WHLU, MS_WHLD, MS_WHLR, _______,    _______,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_DEL , KC_BSPC,
  _______, XXXXXXX, XXXXXXX, MS_DOWN, XXXXXXX, C(KC_B), XXXXXXX,    XXXXXXX,  XXXXXXX, _______, XXXXXXX, _______, XXXXXXX, _______,
                   _______, _______, _______, _______, MS_BTN2,        MS_BTN1, _______, _______, _______, _______
)
};

// Custom keycode handling.
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_smtd(keycode, record)) {
        return false;
    }

    // this uses less memory than returning in each case.
    return keycode < SAFE_RANGE;
}

void matrix_scan_user(void) {
    encoder_action_unregister();
}

layer_state_t layer_state_set_user(layer_state_t state) {
    // _MOVE needs to be active whenever _MOVE_W is active
    // since it contains the first pass of keycodes for that
    // layer. _MOVE_W only contains the modified keys.
    if (IS_LAYER_ON_STATE(state, _MOVE_W)) {
        state |= (1 << _MOVE);
    }

    switch(get_highest_layer(state)) {
        case _MOVE:
        case _MOVE_W:
            rgblight_enable_noeeprom();
            rgblight_sethsv_noeeprom(HSV_GOLDENROD);
            break;
        case _LOWER:
            rgblight_enable_noeeprom();
            rgblight_sethsv_noeeprom(HSV_CORAL);
            break;
        case _RAISE:
            rgblight_enable_noeeprom();
            rgblight_sethsv_noeeprom(HSV_TEAL);
            break;
        default:
            // Default color - disabled
            rgblight_disable_noeeprom();
            break;
    }

    return state;
}

uint32_t custom_os_settings(uint32_t trigger_time, void *cb_arg) {
    os_variant_t detected_os = detected_host_os();
    uint16_t retry_ms = 0;

    switch (detected_os) {
        case OS_MACOS:
        case OS_IOS:
            set_os_string("Mac");
            break;
        case OS_WINDOWS:
            set_os_string("Win");
            layer_on(_BASE_W);
            break;
        case OS_LINUX:
            set_os_string("Linux");
            layer_on(_BASE_W);
            break;
        case OS_UNSURE:
            set_os_string("???");
            retry_ms = 500;
            break;
    }

    return retry_ms;
}

void keyboard_post_init_user(void) {
    rgblight_disable_noeeprom();
    defer_exec(500, custom_os_settings, NULL);
}

smtd_resolution on_smtd_action(uint16_t keycode, smtd_action action, uint8_t tap_count) {
    switch (keycode) {
        SMTD_MT(KC_Z, KC_LSFT)
        SMTD_MT(KC_X, KC_LCMD)
        SMTD_MT(KC_C, KC_LALT)
        SMTD_MT(KC_V, KC_LCTL)
        SMTD_MT(KC_B, KC_LCTL)

        SMTD_MT(KC_SLSH, KC_LSFT)
        SMTD_MT(KC_DOT, KC_LCMD)
        SMTD_MT(KC_COMM, KC_LALT)
        SMTD_MT(KC_M, KC_LCTL)
        SMTD_MT(KC_N, KC_LCTL)

        SMTD_MT_ON_MKEY(CKC_LALT, S(KC_LBRC), KC_LALT)
        SMTD_MT_ON_MKEY(CKC_RALT, S(KC_RBRC), KC_RALT)

        SMTD_MT_ON_MKEY(CKM_LGUI, S(KC_COMM), KC_LGUI)
        SMTD_MT_ON_MKEY(CKM_RGUI, S(KC_DOT), KC_RGUI)

        SMTD_MT_ON_MKEY(CKM_LCTL, KC_LBRC, KC_LCTL)
        SMTD_MT_ON_MKEY(CKM_RCTL, KC_RBRC, KC_RCTL)

        SMTD_MT_ON_MKEY(CKW_LCTL, S(KC_COMM), KC_LCTL)
        SMTD_MT_ON_MKEY(CKW_RCTL, S(KC_DOT), KC_RCTL)

        SMTD_MT_ON_MKEY(CKW_LWIN, KC_LBRC, KC_LWIN)
        SMTD_MT_ON_MKEY(CKW_RWIN, KC_RBRC, KC_RWIN)

        SMTD_LT_ON_MKEY(CKM_MOVE, KC_ESC, _MOVE)
        SMTD_LT_ON_MKEY(CKW_MOVE, KC_ESC, _MOVE_W)
    }

    return SMTD_RESOLUTION_UNHANDLED;
}

