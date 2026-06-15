
#include QMK_KEYBOARD_H
#include "config.h"
#include "keymap_uk.h"

#define HK_SNIP LGUI(LSFT(KC_S)) // Snipping Tool
//type plain text ctrl + shift + v
#define HK_PASTE LCTL(LSFT(KC_V))
// ctrl + shift + home
#define HK_START LCTL(LSFT(KC_HOME))
// ctrl + shift + end
#define HK_END LCTL(LSFT(KC_END))
//  HotStringer
#define HK_STRING LGUI(KC_INS)
// Image paste
#define HK_IMG_PASTE LCTL(LGUI(KC_INS))

const key_override_t delete_key_override = ko_make_basic(MOD_MASK_SHIFT, KC_BSPC, KC_DEL);

// This globally defines all key overrides to be used
const key_override_t *key_overrides[] = {
	&delete_key_override
};

enum custom_keycodes {
    SWAP_SP_ENT = SAFE_RANGE,
    SS_HELLO,
    SS_PAREN
};

static bool sp_ent_swapped = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // Handle the toggle key
        case SWAP_SP_ENT:
            if (record->event.pressed) {
                sp_ent_swapped = !sp_ent_swapped;
            }
            return false; // Skip further processing for this key

        // Intercept Space
        case KC_SPC:
            if (sp_ent_swapped) {
                if (record->event.pressed) {
                    register_code(KC_ENT); // Send Enter press instead [3]
                } else {
                    unregister_code(KC_ENT); // Send Enter release instead [3]
                }
                return false; // Prevent the original Space keycode from being sent [1]
            }
            break;

        // Intercept Enter
        case KC_ENT:
            if (sp_ent_swapped) {
                if (record->event.pressed) {
                    register_code(KC_SPC); // Send Space press instead [3]
                } else {
                    unregister_code(KC_SPC); // Send Space release instead [3]
                }
                return false; // Prevent the original Enter keycode from being sent [1]
            }
            break;
        // define strings

        // Example of sending a string when a custom keycode is pressed
        case SS_HELLO:
            if (record->event.pressed) {
                SEND_STRING("Hello, world!\n");
            }
            return false;

        // parentheses pair
        case SS_PAREN:
            if (record->event.pressed) {
                SEND_STRING("()");
                tap_code(KC_LEFT); // Move cursor between the parentheses
            }
            return false;
    }

    return true; // Process all other keys normally [1]
}



// clang-format off

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                             KC_6,    KC_7,    KC_8,    KC_9,   KC_0,    KC_BSPC,
    KC_ESC,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                             KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,    KC_QUOT,
    KC_TAB,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                             KC_H,    KC_J,    KC_K,    KC_L,   KC_SCLN, KC_NUHS,
    KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_MUTE,        KC_MPLY, KC_N,    KC_M,    KC_COMM, KC_DOT, KC_SLSH, KC_RSFT,
                      KC_LGUI, KC_LALT, KC_LCTL, MO(1),   KC_ENT,         KC_SPC,  MO(2),   KC_RCTL, KC_RALT, KC_RGUI
  ),
  [1] = LAYOUT(
    HK_SNIP, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                            KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_LCBR,    KC_RCBR,                             KC_6,    KC_EQL,    KC_PLUS,    KC_MINUS,    KC_UNDS,    KC_F12,
    _______, UK_TILD, UK_AT,   XXXXXXX, XXXXXXX,  XXXXXXX,                          KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, UK_PIPE,
    _______, KC_EQL,  KC_MINS, KC_PLUS, HK_PASTE, XXXXXXX, _______,        _______, KC_LBRC, KC_RBRC, KC_SCLN, KC_COLN, UK_BSLS, _______,
                      _______, _______, _______, _______, _______,        _______, MO(3),   _______, _______, _______
  ),
  [2] = LAYOUT(
_______, _______, _______, _______, _______, _______,                          HK_START, KC_INS, HK_IMG_PASTE, HK_STRING, _______ ,LSFT(KC_DEL),
    _______, _______,  HK_SNIP, KC_APP,  XXXXXXX, XXXXXXX,                          HK_END, XXXXXXX, KC_UP,   XXXXXXX, XXXXXXX, KC_BSPC,
    _______, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX, KC_CAPS,                          KC_HOME, KC_LEFT, KC_DOWN, KC_RGHT, KC_DEL,  XXXXXXX,
    _______, C(KC_Z), C(KC_X), C(KC_C), C(KC_V), XXXXXXX, _______,        KC_END, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
                      _______, _______, _______, MO(3),   _______,        _______, _______, _______, _______, _______
  ),
    [3] = LAYOUT(
    QK_CLEAR_EEPROM, XXXXXXX, XXXXXXX ,XXXXXXX, XXXXXXX, XXXXXXX,                          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, SWAP_SP_ENT,
    QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    RM_TOGG, RM_HUEU, RM_SATU, RM_VALU, XXXXXXX, XXXXXXX,                          XXXXXXX, KC_VOLD, KC_MUTE, KC_VOLU, XXXXXXX, XXXXXXX,
    RM_NEXT, RM_HUED, RM_SATD, RM_VALD, XXXXXXX, XXXXXXX, XXXXXXX,        XXXXXXX, XXXXXXX, KC_MPRV, KC_MPLY, KC_MNXT, XXXXXXX, XXXXXXX,
                      _______, _______, _______, _______, _______,        _______, _______, _______, _______, _______
  )
};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
[0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT)},
[1] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
[2] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS)},
[3] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS)}
};
#endif
// clang-format on
