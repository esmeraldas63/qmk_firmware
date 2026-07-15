/*
Copyright 2023 @ Nuphy <https://nuphy.com/>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include QMK_KEYBOARD_H

#define MAC_BASE     0
#define MAC_FN       1
#define MAC_FN_SHIFT 2
#define WIN_BASE     3
#define WIN_FN       4
#define WIN_FN_SHIFT 5
#define EXTRA_FN     6
#define NAV          7
#define UTILS        8
#define SYM          10
#define NUM          11

// Left-hand home row mods
#define HOME_Z LGUI_T(KC_Z)
#define HOME_A LALT_T(KC_A)
#define HOME_S LT(SYM, KC_S)
#define HOME_D LSFT_T(KC_D)
#define HOME_F LT(NAV, KC_F)
#define HOME_V LCTL_T(KC_V)

// Right-hand home row mods
#define HOME_J LT(NUM, KC_J)
#define HOME_K RSFT_T(KC_K)
#define HOME_L LT(SYM, KC_L)
#define HOME_SCLN LALT_T(KC_SCLN)
#define HOME_SLSH RGUI_T(KC_SLSH)
#define HOME_M RCTL_T(KC_M)

#define CBRD_HS LGUI(LSFT(KC_C))
#define LOCK_PC LGUI(LCTL(KC_Q))
#define MAC_OCR LALT(LGUI(LCTL(KC_O)))

enum combos {
    NAV_COMBO,
    ENTER_COMBO,
    NUM_COMBO,
    CAPS_COMBO,
    ESC_COMBO,
    BACKSPACE_COMBO,
    DEL_COMBO,
    CTL_TAB_COMBO,
    SFT_CTL_TAB_COMBO,
    ALT_TAB_COMBO,
    DELETE_WORD_COMBO,
    HOME_COMBO,
    END_COMBO,
    TAB_COMBO,
    CLIPBOARD_HISTORY
};

const uint16_t PROGMEM df_combo[] = {HOME_D, HOME_F, COMBO_END};
const uint16_t PROGMEM mcomm_combo[] = {HOME_M, KC_COMM, COMBO_END};
const uint16_t PROGMEM nm_combo[] = {KC_N, HOME_M, COMBO_END};
const uint16_t PROGMEM fg_combo[] = {HOME_F, KC_G, COMBO_END};
const uint16_t PROGMEM hj_combo[] = {HOME_J, KC_H, COMBO_END};
const uint16_t PROGMEM sd_combo[] = {HOME_S, HOME_D, COMBO_END};
const uint16_t PROGMEM jk_combo[] = {HOME_J, HOME_K, COMBO_END};
const uint16_t PROGMEM kl_combo[] = {HOME_K, HOME_L, COMBO_END};
const uint16_t PROGMEM xc_combo[] = {KC_X, KC_C, COMBO_END};
const uint16_t PROGMEM wr_combo[] = {KC_W, KC_R, COMBO_END};
const uint16_t PROGMEM er_combo[] = {KC_E, KC_R, COMBO_END};
const uint16_t PROGMEM dotcomm_combo[] = {KC_DOT, KC_COMM, COMBO_END};
const uint16_t PROGMEM sdf_combo[] = {HOME_S, HOME_D, HOME_F, COMBO_END};
const uint16_t PROGMEM sf_combo[] = {HOME_S, HOME_F, COMBO_END};
const uint16_t PROGMEM xv_combo[] = {KC_X, HOME_V, COMBO_END};

combo_t key_combos[] = {
    [NAV_COMBO] = COMBO(fg_combo, TG(NAV)),
    [NUM_COMBO] = COMBO(hj_combo, TG(NUM)),
    [CAPS_COMBO] = COMBO(sdf_combo, CW_TOGG),
    [ENTER_COMBO] = COMBO(jk_combo, KC_ENT),
    [BACKSPACE_COMBO] = COMBO(mcomm_combo, KC_BSPC),
    [DEL_COMBO] = COMBO(nm_combo, KC_DEL),
    [TAB_COMBO] = COMBO(sf_combo, KC_TAB),
    [DELETE_WORD_COMBO] = COMBO_ACTION(kl_combo),
    [CTL_TAB_COMBO] = COMBO(er_combo, LCTL(KC_TAB)),
    [SFT_CTL_TAB_COMBO] = COMBO(wr_combo, LSFT(LCTL(KC_TAB))),
    [ALT_TAB_COMBO] = COMBO(sd_combo, LALT(KC_TAB)),
    [ESC_COMBO] = COMBO(df_combo, KC_ESC),
    [HOME_COMBO] = COMBO(xc_combo, KC_HOME),
    [END_COMBO] = COMBO(dotcomm_combo, KC_END),
    [CLIPBOARD_HISTORY] = COMBO_ACTION(xv_combo),
};

bool get_combo_must_tap(uint16_t combo_index, combo_t *combo) {
    switch (combo_index) {
        case ALT_TAB_COMBO:
        case NAV_COMBO:
        case NUM_COMBO:
        case CAPS_COMBO:
        case ENTER_COMBO:
        case DELETE_WORD_COMBO:
        case TAB_COMBO:
        case ESC_COMBO:
        case CLIPBOARD_HISTORY:
            return true;
    }
    return false;
}

static bool nav_locked = false;
static bool num_locked = false;
void process_combo_event(uint16_t combo_index, bool pressed) {
    switch (combo_index) {
        case DELETE_WORD_COMBO:
            if (pressed) {
                if (get_highest_layer(default_layer_state) == WIN_BASE) {
                    tap_code16(LCTL(KC_BSPC));
                } else {
                    tap_code16(LALT(KC_BSPC));
                }
            }
            break;
        case CLIPBOARD_HISTORY:
            if (pressed) {
                tap_code16(CBRD_HS);
                layer_move(NAV);
                nav_locked = true;
            }
            break;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) return true;

    switch (keycode) {
        case TG(NAV):
            nav_locked = !nav_locked;
            break;
        case TG(NUM):
            num_locked = !num_locked;
            break;
    }

    if (layer_state_is(NAV) && nav_locked) {
        switch (keycode) {
            case KC_ESC:
                layer_off(NAV);
                nav_locked = false;
                return false;
            case KC_ENT:
                layer_off(NAV);
                nav_locked = false;
                tap_code(KC_ENT);
                return false;
        }
    }

    if (layer_state_is(NUM) && num_locked) {
        switch (keycode) {
            case KC_ESC:
                layer_off(NUM);
                num_locked = false;
                return false;
            case HOME_J:
            case HOME_K:
                layer_off(NUM);
                num_locked = false;
                return true;
            case KC_ENT:
                layer_off(NUM);
                num_locked = false;
                tap_code(KC_ENT);
                return false;
        }
    }

    return true;
}

uint16_t get_flow_tap_term(uint16_t keycode, keyrecord_t *record, uint16_t prev_keycode) {
    // Only apply Flow Tap right after typing a letter or space, and not mid-hotkey.
    if ((get_tap_keycode(prev_keycode) <= KC_Z || get_tap_keycode(prev_keycode) == KC_SPC) &&
        (get_mods() & (MOD_MASK_CG | MOD_BIT_LALT)) == 0) {
        switch (keycode) {
            case HOME_A:
            case HOME_Z:
                return FLOW_TAP_TERM;

            case HOME_V:
            case HOME_M:
                return FLOW_TAP_TERM - 50;

            // exceptions for hrms that are often rolled but never combined
            case HOME_S:
                // alt + sym
                if (prev_keycode == HOME_A) {
                    return FLOW_TAP_TERM;
                }
                break;

            case HOME_D:
                // alt + shift
                if (prev_keycode == HOME_A) {
                    return FLOW_TAP_TERM;
                }
                break;

            case HOME_F:
                // alt + nav
                if (prev_keycode == HOME_A) {
                    return FLOW_TAP_TERM;
                }
                break;

            case HOME_L:
                // shift + sym
                if (prev_keycode == HOME_K) {
                    return FLOW_TAP_TERM;
                }
                break;

            case HOME_K:
                // shift + sym
                if (prev_keycode == HOME_L) {
                    return FLOW_TAP_TERM;
                }
                break;
        }
    }

    return 0; // Disable Flow Tap otherwise.
}

char chordal_hold_handedness(keypos_t key) {
    if (key.row == MATRIX_ROWS - 1) {
        return '*';
    }

    if (key.col >= 12) {
        return '*';
    }

    // B :)
    if (key.row == 4 && key.col == 6) {
        return 'L';
    }

    return key.col <= 5 ? 'L' : 'R';
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
// layer 0 Mac
[MAC_BASE] = LAYOUT(
	KC_GRV, 	KC_1,   	KC_2,   	KC_3,  		KC_4,   	KC_5,   	KC_6,   	KC_7,   	KC_8,   	KC_9,  		KC_0,   	KC_MINS,	KC_EQL, 	KC_BSPC,
	KC_TAB, 	KC_Q,   	KC_W,   	KC_E,  		KC_R,   	KC_T,   	KC_Y,   	KC_U,   	KC_I,   	KC_O,  		KC_P,   	KC_LBRC,	KC_RBRC, 	KC_BSLS,
	KC_ESC,  	HOME_A,     HOME_S,     HOME_D,     HOME_F,   	KC_G,   	KC_H,   	HOME_J,     HOME_K,     HOME_L,     HOME_SCLN,	KC_QUOT, 	            KC_ENT,
	KC_LSFT,    HOME_Z,   	KC_X,   	KC_C,  		HOME_V,   	KC_B,   	KC_N,   	HOME_M,   	KC_COMM,	KC_DOT,		HOME_SLSH,	QK_REP,     KC_UP,		KC_DEL,
	MO(MAC_FN), KC_LALT,	KC_LGUI,										KC_SPC, 							OSL(UTILS), KC_RCTL,	KC_LEFT,	KC_DOWN,    KC_RGHT),

// layer 1 Mac fn
[MAC_FN] = LAYOUT(
	KC_GRV, 	KC_BRID,  	KC_BRIU,  	MAC_TASK, 	MAC_SEARCH, MAC_VOICE,  MAC_DND,  	KC_MPRV,  	KC_MPLY,  	KC_MNXT, 	KC_MUTE, 	KC_VOLD, 	KC_VOLU, 	_______,
	_______, 	LNK_BLE1,  	LNK_BLE2,  	LNK_BLE3,  	LNK_RF,   	_______,   	_______,   	_______,   	_______,   	_______,  	_______,   	DEV_RESET,	SLEEP_MODE, BAT_SHOW,
	_______, 	_______,   	_______,   	_______,  	_______,   	_______,   	_______,   	_______,   	_______,   	_______,  	_______,   	_______,	            _______,
	MO(MAC_FN_SHIFT),	    _______,   	_______,   	_______,  	_______,   	_______,   	_______,   	MO(EXTRA_FN),   	RGB_SPD,	RGB_SPI,  	_______,	MO(EXTRA_FN),  	RGB_VAI,    RGB_TOG,
	_______,	_______,	_______,										_______, 							_______,	_______,   	RGB_MOD,	RGB_VAD,    RGB_HUI),

// layer 2 Mac Fn+shift
[MAC_FN_SHIFT] = LAYOUT(
	SHIFT_GRV, 	KC_F1,  	KC_F2,  	KC_F3, 		KC_F4,  	KC_F5,  	KC_F6,  	KC_F7,  	KC_F8,  	KC_F9, 		KC_F10, 	KC_F11, 	KC_F12, 	_______,
	_______, 	_______,  	_______,  	_______,  	_______,   	_______,   	_______,   	_______,   	_______,   	_______,  	_______,   	_______,	_______,    _______,
	_______, 	_______,   	_______,   	_______,  	_______,   	_______,   	_______,   	_______,   	_______,   	_______,  	_______,   	_______,	            _______,
	_______,	_______,   	_______,   	RGB_TEST,  	_______,   	_______,   	_______,   	_______,   	_______,   	_______,  	_______,	_______, 	_______,    _______,
	_______,	_______,	_______,										_______, 							_______,	_______,   	_______,	_______,    _______),

// layer 3 Win
[WIN_BASE] = LAYOUT(
	KC_GRV, 	KC_1,   	KC_2,   	KC_3,  		KC_4,   	KC_5,   	KC_6,   	KC_7,   	KC_8,   	KC_9,  		KC_0,   	KC_MINS,	KC_EQL, 	KC_BSPC,
	KC_TAB, 	KC_Q,   	KC_W,   	KC_E,  		KC_R,   	KC_T,   	KC_Y,   	KC_U,   	KC_I,   	KC_O,  		KC_P,   	KC_LBRC,	KC_RBRC, 	KC_BSLS,
	KC_ESC,  	HOME_A,     HOME_S,     HOME_D,     HOME_F,   	KC_G,   	KC_H,   	HOME_J,     HOME_K,     HOME_L,     HOME_SCLN,	KC_QUOT, 	            KC_ENT,
	KC_LSFT,    HOME_Z,   	KC_X,   	KC_C,  		HOME_V,   	KC_B,   	KC_N,   	HOME_M,   	KC_COMM,	KC_DOT,		HOME_SLSH,	QK_REP,     KC_UP,		KC_DEL,
	MO(WIN_FN),	    KC_LALT,	KC_LGUI,										KC_SPC, 						OSL(UTILS), KC_RCTL,	KC_LEFT,	KC_DOWN,    KC_RGHT),

// layer 4 win fn
[WIN_FN] = LAYOUT(
    SHIFT_GRV, 	KC_BRID,   	KC_BRIU,    _______,  	_______,   	_______,   	_______,   	KC_MPRV,   	KC_MPLY,   	KC_MNXT,  	KC_MUTE, 	KC_VOLD, 	KC_VOLU, 	_______,
	_______, 	LNK_BLE1,  	LNK_BLE2,  	LNK_BLE3,  	LNK_RF,   	_______,   	_______,   	_______,   	_______,   	_______,  	_______,   	DEV_RESET,	SLEEP_MODE, BAT_SHOW,
	_______, 	_______,   	_______,   	_______,  	_______,   	_______,   	_______,   	_______,   	_______,   	_______,  	_______,   	_______,	            _______,
	MO(WIN_FN_SHIFT),	    _______,   	_______,   	_______,  	_______,   	_______,   	_______,   	MO(EXTRA_FN),   	RGB_SPD,	RGB_SPI,  	_______,	MO(WIN_FN_SHIFT),  	RGB_VAI,    RGB_TOG,
	_______,	_______,	_______,										_______, 							_______,	_______,   	RGB_MOD,	RGB_VAD,    RGB_HUI),

// layer 5 win fn+shift
[WIN_FN_SHIFT] = LAYOUT(
    KC_GRV, 	KC_F1,  	KC_F2,  	KC_F3, 		KC_F4,  	KC_F5,  	KC_F6,  	KC_F7,  	KC_F8,  	KC_F9, 		KC_F10, 	KC_F11, 	KC_F12, 	_______,
	_______, 	_______,  	_______,  	_______,  	_______,   	_______,   	_______,   	_______,   	_______,   	_______,  	_______,   	_______,	_______,    _______,
	_______, 	_______,   	_______,   	_______,  	_______,   	_______,   	_______,   	_______,   	_______,   	_______,  	_______,   	_______,	            _______,
	_______,	_______,   	_______,   	RGB_TEST,  	_______,   	_______,   	_______,   	_______,   	_______,   	_______,  	_______,	_______, 	_______,    _______,
	_______,	_______,	_______,										_______, 							_______,	_______,   	_______,	_______,    _______),

// layer 6 function
[EXTRA_FN] = LAYOUT(
	_______, 	_______,  	_______,  	_______, 	_______,  	_______,  	_______,  	_______,  	_______,  	_______, 	_______, 	_______, 	_______, 	_______,
	_______, 	_______,  	_______,  	_______, 	_______,  	_______,  	_______,  	_______,  	_______,  	_______, 	_______, 	_______, 	_______, 	_______,
	_______, 	_______,   	_______,   	_______,  	_______,   	_______,   	_______,   	_______,   	_______,   	_______,  	_______,   	_______,	            _______,
	_______,    _______,   	_______,   	_______,  	_______,   	_______,   	_______,   	_______,   	SIDE_SPD,	SIDE_SPI,  	_______,	_______,  	SIDE_VAI,    _______,
	_______,	_______,	_______,										_______, 							_______,	_______,   	SIDE_MOD,	SIDE_VAD,    SIDE_HUI),

// layer 7 nav
[NAV] = LAYOUT(
	_______, 	_______,  	_______,  	_______, 	_______,  	_______,  	_______,  	_______,  	   _______,  	  _______, 	        _______, 	_______, 	_______, 	_______,
	_______, 	_______,  	_______,  	_______, 	_______,  	_______,  	KC_DEL,  	KC_BSPC,  	   KC_HOME,  	  KC_END, 	        KC_PGUP, 	_______, 	_______, 	_______,
	_______, 	_______,    _______,	_______,    _______,    _______,   	KC_LEFT,   	KC_DOWN,   	   KC_UP,  	      KC_RIGHT,         _______,    _______,	            _______,
	_______,    _______,   	_______,   	QK_LLCK,  	_______,   	_______,   	KC_PGDN,    LALT(KC_BSPC), LALT(KC_LEFT), LALT(KC_RGHT),    _______,    _______,  	_______,    _______,
	_______,	_______,	_______,										KC_ENTER, 							      _______,	        _______,   	_______,	_______,    _______),

// layer 8 utils
[UTILS] = LAYOUT(
    _______, 	KC_F1,  	KC_F2,  	KC_F3, 		KC_F4,  	KC_F5,  	KC_F6,  	KC_F7,  	KC_F8,  	KC_F9, 		KC_F10, 	KC_F11, 	KC_F12, 	_______,
	_______, 	LOCK_PC,  	_______,  	_______, 	_______,  	_______,  	_______,  	_______,  	_______,  	MAC_OCR, 	KC_PSCR, 	_______, 	_______, 	_______,
	_______, 	_______,    _______,	_______,    _______,    _______,   	_______,   	_______,   	_______,  	_______,    _______,    _______,	            _______,
	_______,    _______,   	_______,   	CBRD_HS,  	_______,   	_______,   	_______,    _______,	_______,  	_______,	_______,    _______,  	_______,    _______,
	_______,	_______,	_______,										_______, 							_______,	_______,   	_______,	_______,    _______),

[SYM] = LAYOUT(
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, KC_GRV,  KC_LT,   KC_GT,   KC_MINS, KC_PIPE, KC_CIRC, KC_LCBR, KC_RCBR, KC_DLR,  KC_UNDS, _______, _______, _______,
    _______, KC_EXLM, KC_ASTR, KC_SLSH, KC_EQL,  KC_AMPR, KC_HASH, KC_LPRN, KC_RPRN, KC_SCLN, KC_DQUO, _______,          _______,
    _______, KC_TILD, KC_PLUS, KC_LBRC, KC_RBRC, KC_PERC, KC_AT,   KC_COLN, KC_COMM, KC_DOT,  KC_QUOT, _______, _______, _______,
    _______, _______, _______,                    _______,                    _______, _______, _______, _______, _______),

[NUM] = LAYOUT(
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, KC_PLUS, KC_7,    KC_8,    KC_9,    KC_MINS, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, KC_0,    KC_4,    KC_5,    KC_6,    KC_PERC, KC_EQL,  _______, _______, _______, _______, _______,          _______,
    _______, KC_ASTR, KC_1,    KC_2,    KC_3,    KC_SLSH, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______,                    KC_ENTER,                    _______, _______, _______, _______, _______),
};

const is31_led PROGMEM g_is31_leds[RGB_MATRIX_LED_COUNT] = {
    {0, A_16,   B_16,   C_16},      // "Esc"
    {0, A_2,    B_2,    C_2},       // "!1"
    {0, A_3,    B_3,    C_3},       // "@2"
    {0, A_4,    B_4,    C_4},       // "#3"
    {0, A_5,    B_5,    C_5},       // "$4"
    {0, A_6,    B_6,    C_6},       // "%5"
    {0, A_7,    B_7,    C_7},       // "^6"
    {0, A_8,    B_8,    C_8},       // "&7"
    {0, A_9,    B_9,    C_9},       // "*8"
    {0, A_10,   B_10,   C_10},      // "(9"
    {0, A_11,   B_11,   C_11},      // ")0"
    {1, D_1,    E_1,    F_1},       // "_-"
    {1, D_2,    E_2,    F_2},       // "+="
    {1, D_3,    E_3,    F_3},       // "Backsp"
    {0, D_1,    E_1,    F_1},       // "Tab"
    {0, D_2,    E_2,    F_2},       // "Q"
    {0, D_3,    E_3,    F_3},       // "W"
    {0, D_4,    E_4,    F_4},       // "E"
    {0, D_5,    E_5,    F_5},       // "R"
    {0, D_6,    E_6,    F_6},       // "T"
    {0, D_7,    E_7,    F_7},       // "Y"
    {0, D_8,    E_8,    F_8},       // "U"
    {0, D_9,    E_9,    F_9},       // "I"
    {0, D_10,   E_10,   F_10},      // "O"
    {0, D_11,   E_11,   F_11},      // "P"
    {1, G_1,    H_1,    I_1},       // "{["
    {1, G_2,    H_2,    I_2},       // "}]"
    {1, G_3,    H_3,    I_3},       // "|\\"
    {0, G_1,    H_1,    I_1},       // "Caps"
    {0, G_2,    H_2,    I_2},       // "A"
    {0, G_3,    H_3,    I_3},       // "S"
    {0, G_4,    H_4,    I_4},       // "D"
    {0, G_5,    H_5,    I_5},       // "F"
    {0, G_6,    H_6,    I_6},       // "G"
    {0, G_7,    H_7,    I_7},       // "H"
    {0, G_8,    H_8,    I_8},       // "J"
    {0, G_9,    H_9,    I_9},       // "K"
    {0, G_10,   H_10,   I_10},      // "L"
    {0, G_11,   H_11,   I_11},      // ":"
    {1, G_16,   H_16,   I_16},      // "\""
    {1, G_14,   H_14,   I_14},      // "Enter"
    {0, J_1,    K_1,    L_1},       // "Shift"
    {0, J_3,    K_3,    L_3},       // "Z"
    {0, J_4,    K_4,    L_4},       // "X"
    {0, J_5,    K_5,    L_5},       // "C"
    {0, J_6,    K_6,    L_6},       // "V"
    {0, J_7,    K_7,    L_7},       // "B"
    {0, J_8,    K_8,    L_8},       // "N"
    {0, J_9,    K_9,    L_9},       // "M"
    {0, J_10,   K_10,   L_10},      // "<,"
    {0, J_11,   K_11,   L_11},      // ">."
    {1, J_1,    K_1,    L_1},       // "?/"
    {1, J_3,    K_3,    L_3},       // "Shift"
    {1, J_4,    K_4,    L_4},       // "↑"
    {1, G_4,    H_4,    I_4},       // "Del"
    {0, J_16,   K_16,   L_16},      // "Ctrl"
    {0, J_15,   K_15,   L_15},      // "Opt"
    {0, J_14,   K_14,   L_14},      // "Cmd"
    {0, J_13,   K_13,   L_13},      // "Space"
    {0, J_12,   K_12,   L_12},      // "Cmd"
    {1, J_16,   K_16,   L_16},      // "Fn"
    {1, J_13,   K_13,   L_13},      // "←"
    {1, J_12,   K_12,   L_12},      // "↓"
    {1, J_11,   K_11,   L_11},      // "→"

    {1, A_5,    B_5,    C_5},       // logo left
    {1, A_4,    B_4,    C_4},       //
    {1, A_3,    B_3,    C_3},       //
    {1, A_2,    B_2,    C_2},       //
    {1, A_1,    B_1,    C_1},       //

    {1, A_6,    B_6,    C_6},       // logo right
    {1, A_7,    B_7,    C_7},       //
    {1, A_8,    B_8,    C_8},       //
    {1, A_9,    B_9,    C_9},       //
    {1, A_10,   B_10,   C_10}       //
};

bool rgb_matrix_indicators_user(void) {
    uint8_t r = 0, g = 0, b = 0;

    if (host_keyboard_led_state().caps_lock) {
         r = 0; g = 155; b = 155;
    } else if (is_caps_word_on()) {
         r = 255; g = 0; b = 0;
    } else {
        switch (biton32(layer_state)) {
            case NAV:
                r = 0; g = 255; b = 0;
                break;
            case UTILS:
                r = 255; g = 155; b = 0;
                break;
            case SYM:
                r = 200; g = 0; b = 200;
                break;
            case NUM:
                r = 0; g = 0; b = 255;
                break;
            default:
                return true;
        }
    }
    rgb_matrix_set_color(67, r, g, b);
    rgb_matrix_set_color(70, r, g, b);
    // for (int i = 64; i <= 73; i++) {
    //     rgb_matrix_set_color(i, r, g, b);
    // }

    return true;
}
