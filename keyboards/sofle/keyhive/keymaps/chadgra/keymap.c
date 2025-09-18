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

// Base layer is the number of layers CYCLE selects from.
#define BASE_LAYERS 2

enum custom_keycodes {
    PLACEHOLDER = SAFE_RANGE,   // can always be here (4 bytes)
    MOUSE_SCREEN_RIGHT,         // Move mouse to the right screen
    MOUSE_SCREEN_LEFT,          // Move mouse to the left screen
    MOUSE_MOVE_UP,
    MOUSE_MOVE_DOWN,
    MOUSE_MOVE_RIGHT,
    MOUSE_MOVE_LEFT,
    MOD_SKIP_WORD,
    MOD_SKIP_PAGE,
    MOD_L_MAC_CMD,
    MOD_L_MAC_CTL,
    MOD_R_MAC_CMD,
    MOD_R_MAC_CTL,
};

enum custom_layers {
    _BASE,
    _MOVE,
    _LOWER,
    _RAISE
};

// Tap Dance keycodes
enum td_keycodes {
    ALCB,       // `LALT` when held, `{` when tapped.
    ARCB,       // `RALT` when held, `}` when tapped.
};

// Define a type containing as many tapdance states as you need
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_SINGLE_TAP
} td_state_t;

// Create a global instance of the tapdance state type
static td_state_t td_state;

// Declare your tapdance functions:

// Function to determine the current tapdance state
td_state_t cur_dance(tap_dance_state_t *state);

// `finished` and `reset` functions for each tapdance keycode
void alt_lcb_finished(tap_dance_state_t *state, void *user_data);
void alt_lcb_reset(tap_dance_state_t *state, void *user_data);
void alt_rcb_finished(tap_dance_state_t *state, void *user_data);
void alt_rcb_reset(tap_dance_state_t *state, void *user_data);


#define MS_SC_R     MOUSE_SCREEN_RIGHT
#define MS_SC_L     MOUSE_SCREEN_LEFT
#define MS_U        MOUSE_MOVE_UP
#define MS_D        MOUSE_MOVE_DOWN
#define MS_R        MOUSE_MOVE_RIGHT
#define MS_L        MOUSE_MOVE_LEFT
#define M_WORD      MOD_SKIP_WORD
#define M_PAGE      MOD_SKIP_PAGE
#define LMG         MOD_L_MAC_CMD
#define LMC         MOD_L_MAC_CTL
#define RMG         MOD_R_MAC_CMD
#define RMC         MOD_R_MAC_CTL

//Default keymap. This can be changed in Via. Use oled.c to change beavior that Via cannot change.
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * BASE
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
   KC_TAB, KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   ,C(KC_RGHT),     MS_SC_L, KC_Y    , KC_U   , KC_I   , KC_O   , KC_P   , KC_BSPC,
TT(_MOVE), KC_A   , KC_S   , KC_D   , KC_F   , KC_G   , XXXXXXX,       KC_BTN1, KC_H    , KC_J   , KC_K   , KC_L   , KC_SCLN, KC_QUOT,
  SC_LSPO, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   ,C(KC_LEFT),     MS_SC_R, KC_N    , KC_M   , KC_COMM, KC_DOT , KC_SLSH, SC_RSPC,
                    LMG, TD(ALCB),     LMC,TT(_LOWER), KC_ENT ,           KC_SPC ,TT(_RAISE),     RMC, TD(ARCB),     RMG
),

/*
 * MOVE
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
TO(_BASE), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                         XXXXXXX, XXXXXXX, KC_PGUP, XXXXXXX, KC_HOME, XXXXXXX,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_PGUP,       KC_VOLD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_BSPC,
  _______, KC_LGUI,  M_PAGE,  M_WORD, KC_CAPS, XXXXXXX,   KC_NO,       KC_MPLY, KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, XXXXXXX, KC_END ,
  _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_PGDN,       KC_VOLU, XXXXXXX, XXXXXXX, KC_PGDN, XXXXXXX, XXXXXXX, _______,
                  _______, _______, _______, XXXXXXX, KC_LSFT,           XXXXXXX,  KC_APP, _______, _______, _______
),

/* LOWER
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
/* RAISE
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
  _______, XXXXXXX, XXXXXXX, KC_MS_U, C(KC_R), XXXXXXX,    MS_R,       MS_D,  XXXXXXX, KC_BTN1, XXXXXXX, KC_BTN2, _______, KC_BSPC,
  _______, XXXXXXX, KC_WH_L, KC_WH_U, KC_WH_D, KC_WH_R, _______,    _______,  KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, KC_DEL , KC_BSPC,
  _______, XXXXXXX, XXXXXXX, KC_MS_D, XXXXXXX, C(KC_B),    MS_L,       MS_U,  XXXXXXX, _______, XXXXXXX, _______, XXXXXXX, _______,
                   _______, _______, _______, _______, KC_BTN2,        KC_BTN1, _______, _______, _______, _______
)
};

static bool is_mouse_screen_right = false;
static bool is_mouse_screen_left = false;

static uint16_t mouse_screen_right_timer = 0;
static uint16_t mouse_screen_left_timer = 0;

static bool is_mouse_move_up = false;
static int mouse_move_up_start_timer = 0;
static int mouse_move_up_repeat_timer = 0;

static bool is_mouse_move_down = false;
static int mouse_move_down_start_timer = 0;
static int mouse_move_down_repeat_timer = 0;

static bool is_mouse_move_right = false;
static int mouse_move_right_start_timer = 0;
static int mouse_move_right_repeat_timer = 0;

static bool is_mouse_move_left = false;
static int mouse_move_left_start_timer = 0;
static int mouse_move_left_repeat_timer = 0;

// The string representing which OS it is detected being connected to
char *os_string = "???";

#define MOUSE_SCREEN_TIME       350
#define MOUSE_MOVE_ACL2_TIME    50
#define MOUSE_MOVE_ACL1_TIME    200
#define MOUSE_MOVE_ACL0_TIME    500

void init_mouse_move(uint16_t keycode, int* start_timer, int *repeat_timer, bool *is_move) {
    if (!*is_move) {
        *is_move = true;
        *start_timer = timer_read();
        // register_code(KC_ACL0);
        register_code(keycode);
    }
    *repeat_timer = timer_read();
}

void handle_mouse_move(uint16_t keycode, int start_timer, int repeat_timer, bool *is_move) {
    if (*is_move) {
        if (timer_elapsed(repeat_timer) > 200) {
            *is_move = false;
            unregister_code(keycode);
        }
    }
}

void os_specific_mod(keyrecord_t *record, uint8_t mac_mod, uint8_t win_mod) {
    uint8_t mod = win_mod;
    os_variant_t detected_os = detected_host_os();

    if ((detected_os == OS_MACOS) || (detected_os == OS_IOS)) {
        mod = mac_mod;
    }

    if (record->event.pressed) {
        register_mods(mod);
    } else {
        unregister_mods(mod);
    }
}

void os_specific_sc(keyrecord_t *record, uint16_t mac_code, uint16_t win_code) {
    uint16_t code = win_code;
    os_variant_t detected_os = detected_host_os();

    if ((detected_os == OS_MACOS) || (detected_os == OS_IOS)) {
        code = mac_code;
    }

    process_space_cadet(code, record);
}

// Custom keycode handling.
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MOUSE_SCREEN_RIGHT:
            is_mouse_screen_right = true;
            mouse_screen_right_timer = timer_read();
            register_code(KC_ACL1);
            register_code(KC_MS_R);
            break;
        case MOUSE_SCREEN_LEFT:
            is_mouse_screen_left = true;
            mouse_screen_left_timer = timer_read();
            register_code(KC_ACL1);
            register_code(KC_MS_L);
            break;
        case MOUSE_MOVE_UP:
            init_mouse_move(KC_MS_U, &mouse_move_up_start_timer, &mouse_move_up_repeat_timer, &is_mouse_move_up);
            break;
        case MOUSE_MOVE_DOWN:
            init_mouse_move(KC_MS_D, &mouse_move_down_start_timer, &mouse_move_down_repeat_timer, &is_mouse_move_down);
            break;
        case MOUSE_MOVE_RIGHT:
            init_mouse_move(KC_MS_R, &mouse_move_right_start_timer, &mouse_move_right_repeat_timer, &is_mouse_move_right);
            break;
        case MOUSE_MOVE_LEFT:
            init_mouse_move(KC_MS_L, &mouse_move_left_start_timer, &mouse_move_left_repeat_timer, &is_mouse_move_left);
            break;
        case MOD_SKIP_WORD:
            os_specific_mod(record, MOD_MASK_ALT, MOD_MASK_CTRL);
            break;
        case MOD_SKIP_PAGE:
            os_specific_mod(record, MOD_MASK_CTRL, MOD_MASK_GUI);
            break;
        case MOD_L_MAC_CMD:
            os_specific_mod(record, MOD_BIT(KC_LGUI), MOD_BIT(KC_LCTL));
            break;
        case MOD_L_MAC_CTL:
            os_specific_sc(record, SC_LCPO, SC_LAPO);
            break;
        case MOD_R_MAC_CMD:
            os_specific_mod(record, MOD_BIT(KC_RGUI), MOD_BIT(KC_RCTL));
            break;
        case MOD_R_MAC_CTL:
            os_specific_sc(record, SC_RCPC, SC_RAPC);
            break;

    }

    // this uses less memory than returning in each case.
    return keycode < SAFE_RANGE;
}

void matrix_scan_user(void) {
    encoder_action_unregister();

    if (is_mouse_screen_right) {
        if (timer_elapsed(mouse_screen_right_timer) > MOUSE_SCREEN_TIME) {
            unregister_code(KC_MS_R);
            unregister_code(KC_ACL1);
            is_mouse_screen_right = false;
        }
    }
    if (is_mouse_screen_left) {
        if (timer_elapsed(mouse_screen_left_timer) > MOUSE_SCREEN_TIME) {
            unregister_code(KC_MS_L);
            unregister_code(KC_ACL1);
            is_mouse_screen_left = false;
        }
    }

    handle_mouse_move(KC_MS_U, mouse_move_up_start_timer, mouse_move_up_repeat_timer, &is_mouse_move_up);
    handle_mouse_move(KC_MS_D, mouse_move_down_start_timer, mouse_move_down_repeat_timer, &is_mouse_move_down);
    handle_mouse_move(KC_MS_R, mouse_move_right_start_timer, mouse_move_right_repeat_timer, &is_mouse_move_right);
    handle_mouse_move(KC_MS_L, mouse_move_left_start_timer, mouse_move_left_repeat_timer, &is_mouse_move_left);
}

layer_state_t layer_state_set_user(layer_state_t state) {
    switch(biton32(state)) {
        case _MOVE:
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
            os_string = "Mac";
            break;
        case OS_WINDOWS:
            os_string = "Win";
            break;
        case OS_LINUX:
            os_string = "Linux";
            break;
        case OS_UNSURE:
            os_string = "???";
            retry_ms = 500;
            break;
    }

    return retry_ms;
}

void keyboard_post_init_user(void) {
    rgblight_disable_noeeprom();
    defer_exec(500, custom_os_settings, NULL);
}

// Determine the tapdance state to return
td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) {
            return TD_SINGLE_TAP;
        } else {
            return TD_SINGLE_HOLD;
        }
    }

    if (state->count == 2) {
        return TD_DOUBLE_SINGLE_TAP;
    } else {
        return TD_UNKNOWN; // Any number higher than the maximum state value you return above
    }
}

// Handle the possible states for each tapdance keycode you define:

void tap_dance_finished(tap_dance_state_t *state, void *user_data, uint16_t tap, uint16_t hold) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
            register_code16(tap);
            break;
        case TD_SINGLE_HOLD:
            register_mods(hold);
            break;
        case TD_DOUBLE_SINGLE_TAP: // Allow nesting of 2 taps
            tap_code16(tap);
            register_code16(tap);
            break;
        default:
            break;
    }
}

void tap_dance_reset(tap_dance_state_t *state, void *user_data, uint16_t tap, uint16_t hold) {
    switch (td_state) {
        case TD_SINGLE_TAP:
            unregister_code16(tap);
            break;
        case TD_SINGLE_HOLD:
            unregister_mods(hold); // For a layer-tap key, use `layer_off(_MY_LAYER)` here
            break;
        case TD_DOUBLE_SINGLE_TAP:
            unregister_code16(tap);
            break;
        default:
            break;
    }
}


void alt_lcb_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_finished(state, user_data, KC_LCBR, MOD_BIT(KC_LALT));
}

void alt_lcb_reset(tap_dance_state_t *state, void *user_data) {
    tap_dance_reset(state, user_data, KC_LCBR, MOD_BIT(KC_LALT));
}

void alt_rcb_finished(tap_dance_state_t *state, void *user_data) {
    tap_dance_finished(state, user_data, KC_RCBR, MOD_BIT(KC_RALT));
}

void alt_rcb_reset(tap_dance_state_t *state, void *user_data) {
    tap_dance_reset(state, user_data, KC_RCBR, MOD_BIT(KC_RALT));
}

// Define `ACTION_TAP_DANCE_FN_ADVANCED()` for each tapdance keycode, passing in `finished` and `reset` functions
tap_dance_action_t tap_dance_actions[] = {
    [ALCB] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, alt_lcb_finished, alt_lcb_reset),
    [ARCB] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, alt_rcb_finished, alt_rcb_reset)
};
