#include "lets_split.h"
#include "action_layer.h"
#include "eeconfig.h"
#include QMK_KEYBOARD_H
#include "debug.h"
#include "config.h"
#include "hk_util.h"
#include "custom_keycodes.h"

#define BASE 0 // default layer
#define SYMB 2 // symbols
#define ARRW 3 // arrow keys
#define HACHIKOUME 1 // Hachimitu-Koume layout for japanese

#define RCTL_T(kc) MT(MOD_RCTL, kc)
#define RSFT_T(kc) MT(MOD_RSFT, kc)
#define RALT_T(kc) MT(MOD_RALT, kc)
#define RGUI_T(kc) MT(MOD_RGUI, kc)

enum custom_functions {
  F_SFT = 1,
  F_CTRL,
  F_LALT,
  F_RALT,
  F_LGUI,
  F_RGUI
};

#ifdef KEYLOGGER_ENABLE
bool log_enable = false
#endif

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer (For JIS)
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |  Esc   |   1  |   2  |   3  |   4  |   5  |      |           |      |   6  |   7  |   8  |   9  |   0  |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |  Tab   |   Q  |   W  |   E  |   R  |   T  |  [{  |           |  }]  |   Y  |   U  |   I  |   O  |   P  |  @`    |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * | LCtrl  |   A  |   S  |   D  |F/LGui|   G  |------|           |------|   H  |J/RGui|   K  |   L  |  ;+  |  :*    |
 * |--------+------+------+------+------+------|  Esc |           | Esc  |------+------+------+------+------+--------|
 * |   L1   |   Z  |   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |   ,  |   .  |  /?  |   L1   |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |  \|  |  ^~  | LAlt | Eisu |                                       | Kana | RAlt |  \_  | Arr  |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | Left |Right |       | Left | Right|
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |  Up  |       |  Up  |      |      |
 *                                 | Space| BSpc |------|       |------| Enter| Shift|
 *                                 |/Shift|      | Down |       | Down |      |      |
 *                                 `--------------------'       `--------------------'
 */
// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*
[BASE] = KEYMAP(  // layer 0 : default
        // left hand
        KC_ESC,     KC_1,    KC_2,   KC_3,      KC_4,        KC_5,    KC_NO,
        KC_TAB,     KC_Q,    KC_W,   KC_E,      KC_R,        KC_T,    JA_LBRC,
        KC_LCTL,    KC_A,    KC_S,   KC_D,      GUI_T(KC_F), KC_G,
        OSL(SYMB),  KC_Z,    KC_X,   KC_C,      KC_V,        KC_B,    KC_ESC,
        KC_NO,      KC_JYEN, JA_HAT, F(F_LALT), M_EISU,
                                                             KC_LEFT, KC_RGHT,
                                                                      KC_UP,
                                       SFT_T(KC_SPACE),      KC_BSPC, KC_DOWN,
        // right hand
             KC_NO,      KC_6,      KC_7,        KC_8,      KC_9,    KC_0,        KC_NO,
             JA_RBRC,    KC_Y,      KC_U,        KC_I,      KC_O,    KC_P,        JA_AT,
                         KC_H,      RGUI_T(KC_J),KC_K,      KC_L,    KC_SCLN,     JA_CLN,
             KC_ESC,     KC_N,      KC_M,        KC_COMM,   KC_DOT,  KC_SLSH,     OSL(SYMB),
                                    M_KANA,      F(F_RALT), KC_RO,   OSL(ARRW),   KC_NO,
             KC_LEFT,    KC_RGHT,
             KC_UP,
             KC_DOWN,    KC_ENTER,  F(F_SFT)
    ),
/* Keymap 1: Symbol Layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |   F11  |  F1  |  F2  |  F3  |  F4  |  F5  |      |           |      |  F6  |  F7  |  F8  |  F9  |  F10 |   F12  |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |   !  |   "  |   #  |   $  |   %  |      |           |      |   &  |   '  |   (  |   )  |   ~  |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |   |  |  ~   |   _  |  =   |      |------|           |------|      |  =   |   _  |   ~  |   |  |        |
 * |--------+------+------+------+------+------|      |           | Del  |------+------+------+------+------+--------|
 * |        |   \  |  ^   |   \  |  -   |      |      |           |      |      |  -   |   \  |   ^  |   \  |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       |      |      |      |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |      |      |------|       |------|      |      |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
// SYMBOLS
[SYMB] = KEYMAP(
       // left hand
       KC_F11,  KC_F1,        KC_F2,       KC_F3,         KC_F4,        KC_F5,     KC_TRNS,
       KC_TRNS, LSFT(KC_1),   LSFT(KC_2),  LSFT(KC_3),    LSFT(KC_4),   LSFT(KC_5),KC_TRNS,
       KC_TRNS, LSFT(KC_JYEN),LSFT(JA_HAT),LSFT(KC_RO),   LSFT(KC_MINS),KC_TRNS,
       KC_TRNS, KC_JYEN,      JA_HAT,      KC_RO,         KC_MINS,      KC_TRNS,   KC_TRNS,
       KC_TRNS, KC_TRNS,      KC_TRNS,     KC_TRNS,       KC_TRNS,
                                           KC_TRNS,       KC_TRNS,
                                                          KC_TRNS,
                              KC_TRNS,     KC_TRNS,       KC_TRNS,
       // right hand
       KC_TRNS, KC_F6,      KC_F7,         KC_F8,       KC_F9,         KC_F10,       KC_F12,
       KC_TRNS, LSFT(KC_6), LSFT(KC_7),    LSFT(KC_8),  LSFT(KC_9),    LSFT(KC_0),   KC_TRNS,
                KC_TRNS,    LSFT(KC_MINS), LSFT(KC_RO), LSFT(JA_HAT),  LSFT(KC_JYEN),KC_TRNS,
       KC_DEL,  KC_TRNS,    KC_MINS,       KC_RO,       JA_HAT,        KC_JYEN,      KC_TRNS,
                            KC_TRNS,       KC_TRNS,     KC_TRNS,       KC_TRNS,      KC_TRNS,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS
),
/* Keymap 2: Media and mouse keys
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * | Teensy |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |------|           |------|      |      |  UP  |      |      |  Play  |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+-------+------+--------|
 * |        |      |      |      |      |      |      |           |      |      | LEFT | DOWN |RIGHT |      |        |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |      |      |      |      |                                       |      |      |      |      |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |Brwser|
 *                                 |      |      |------|       |------|      |Back  |
 *                                 |      |      |      |       |      |      |      |
 *                                 `--------------------'       `--------------------'
 */
// MEDIA AND MOUSE
[ARRW] = KEYMAP(
       RESET,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                           KC_TRNS, KC_TRNS,
                                                    KC_TRNS,
                                  KC_TRNS, KC_TRNS, KC_TRNS,
    // right hand
       KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                 KC_TRNS, KC_TRNS, KC_UP,   KC_TRNS, KC_TRNS, KC_MPLY,
       KC_TRNS,  KC_TRNS, KC_LEFT, KC_DOWN, KC_RGHT, KC_TRNS, KC_TRNS,
                          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
       KC_TRNS, KC_TRNS,
       KC_TRNS,
       KC_TRNS, KC_TRNS, KC_WBAK
),

/* Keymap 3: Japanese layout layer (For JIS)
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |  Esc   |   1  |   2  |   3  |   4  |   5  |      |           |      |   6  |   7  |   8  |   9  |   0  |        |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |  Tab   |   Q  |   W  |   E  |   R  |   T  |  [{  |           |  }]  |   Y  |   U  |   I  |   O  |   P  |  @`    |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * | LCtrl  |   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |  ;+  |  :*    |
 * |--------+------+------+------+------+------|  Gui |           | Gui  |------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |   ,  |   .  |  /?  | RShift |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |      |  \|  |  ^~  | LAlt | Eisu |                                       | Kana | RAlt |  \_  | Arr  |      |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | Left |Right |       | Left | Right|
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |  Up  |       |  Up  |      |      |
 *                                 |LShift|Space |------|       |------| Enter|RShift|
 *                                 |      |      | Down |       | Down |      |      |
 *                                 `--------------------'       `--------------------'
 */
[HACHIKOUME] = KEYMAP(  // layer 3 : for hachi-mitsu koume japanese layout
        // left hand
        KC_ESC,     KC_1,    KC_2,   KC_3,      KC_4,        KC_5,    KC_NO,
        KC_TAB,     KC_Q,    KC_W,   KC_E,      KC_R,        KC_T,    JA_LBRC,
        KC_LCTL,    KC_A,    KC_S,   KC_D,      KC_F,        KC_G,
        KC_LSFT,    KC_Z,    KC_X,   KC_C,      KC_V,        KC_B,    F(F_LGUI),
        KC_NO,      KC_JYEN, JA_HAT, F(F_LALT), M_EISU,
                                                             KC_LEFT, KC_RGHT,
                                                                      KC_UP,
                                     KC_HK_LSPACE,KC_SPACE,  KC_DOWN,
        // right hand
             KC_NO,      KC_6,      KC_7,     KC_8,      KC_9,    KC_0,        KC_NO,
             JA_RBRC,    KC_Y,      KC_U,     KC_I,      KC_O,    KC_P,        JA_AT,
                         KC_H,      KC_J,     KC_K,      KC_L,    KC_SCLN,     JA_CLN,
             F(F_RGUI),  KC_N,      KC_M,     KC_COMM,   KC_DOT,  KC_SLSH,     KC_RSFT,
                                    M_KANA,   F(F_RALT), KC_RO,   OSL(ARRW),   KC_NO,
             KC_LEFT,    KC_RGHT,
             KC_UP,
             KC_DOWN,    KC_ENTER,  KC_HK_RSPACE
    )
};

const uint16_t PROGMEM fn_actions[] = {
  [F_SFT] = ACTION_MODS_ONESHOT(MOD_LSFT),
  [F_CTRL] = ACTION_MODS_ONESHOT(MOD_LCTL),
  [F_LALT] = ACTION_MODS_ONESHOT(MOD_LALT),
  [F_RALT] = ACTION_MODS_ONESHOT(MOD_RALT),
  [F_LGUI] = ACTION_MODS_ONESHOT(MOD_LGUI),
  [F_RGUI] = ACTION_MODS_ONESHOT(MOD_RGUI)
};

typedef struct {
  bool layer_toggle;
  bool sticky;
} td_ta_state_t;

static void der_tap_dance_esc_finished (qk_tap_dance_state_t *state, void *user_data) {
  td_ta_state_t *td_ta = (td_ta_state_t *) user_data;

  if (td_ta->sticky) {
    td_ta->sticky = false;
    td_ta->layer_toggle = false;
    layer_off (SYMB);
    return;
  }

  if (state->count == 1 && !state->pressed) {
    register_code (KC_ESC);
    td_ta->sticky = false;
    td_ta->layer_toggle = false;
  } else {
    td_ta->layer_toggle = true;
    layer_on (ARRW);
    td_ta->sticky = (state->count == 2);
  }
}

static void der_tap_dance_esc_reset (qk_tap_dance_state_t *state, void *user_data) {
  td_ta_state_t *td_ta = (td_ta_state_t *) user_data;

  if (!td_ta->layer_toggle) {
    unregister_code (KC_ESC);
  }

  if (!td_ta->sticky) {
    layer_off(SYMB);
  }
}

qk_tap_dance_action_t tap_dance_actions[] = {
  [0] = {
    .fn = {NULL, der_tap_dance_esc_finished, der_tap_dance_esc_reset},
    .user_data = (void*)&((td_ta_state_t) {false, false})
  }
};

bool process_record_user (uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
    switch(keycode) {
      case M_EISU:
        /* KC_MHEN equals KC_INT5 */
        layer_off(HACHIKOUME);
        SEND_STRING(SS_TAP(X_INT5));
        SEND_STRING(SS_TAP(X_LANG2));
        der_init_hk_variables();
        return false; break;
      case M_KANA:
        /* KC_HENK equals KC_INT4 */
        SEND_STRING(SS_TAP(X_INT4));
        SEND_STRING(SS_TAP(X_LANG1));
        layer_on(HACHIKOUME);
        der_init_hk_variables();
        return false; break;
    }
  }

  uint8_t layer = biton32(layer_state);

  switch (layer) {
    case HACHIKOUME:
      return der_process_hk(keycode, record); break;
    default:
      break;
  }

  return true;
}

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {

    uint8_t layer = biton32(layer_state);

    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();
    switch (layer) {
      // TODO: Make this relevant to the ErgoDox EZ.
        case 1:
            ergodox_right_led_1_on();
            break;
        case 2:
            ergodox_right_led_2_on();
            break;
        case 3:
            ergodox_right_led_3_on();
            break;
        default:
            // none
            break;
    }

};
