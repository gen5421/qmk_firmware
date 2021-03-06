#include QMK_KEYBOARD_H
#include "hk_util.h"
#include "custom_keycodes.h"

/*
  Implementation memo for hachimitsu-koume layout.
  1. Time-base and pressed only detection
  2. Time-base and pressed and released detection
  3. No time-base and pressed and released detection
  If choose 1., this method has one advantage that is more simple 2. . But this method will cause more longer time to show character.
  If choose 2., this method has detail control between other methods. But this method will be more complex of implementation.
  If choose 3., this method will need large configuration for layout, But this method implementation will be more simply than other methods. 
 */
enum der_shift_for_key {
  HK_NOSHIFT = 0x00,
  HK_A,
  HK_B,
  HK_C,
  HK_D,
  HK_E,
  HK_F,
  HK_G,
  HK_H,
  HK_I,
  HK_J,
  HK_K,
  HK_L,
  HK_M,               /* 0x10 */
  HK_N,
  HK_O,
  HK_P,
  HK_Q,
  HK_R,
  HK_S,
  HK_T,
  HK_U,
  HK_V,
  HK_W,
  HK_X,
  HK_Y,
  HK_Z,
  HK_RSFT,
  HK_LSFT,
  HK_AT,
  HK_SCOLON,          /* ; (and +) */
  HK_COLON,          /* ; (and *) */
  HK_COMMA,           /* , and < */
  HK_DOT,             /* . and > */
  HK_SLASH           /* / and ? */
};


/* variables to implement same-key shift */
static uint16_t hk_current_shift = HK_NOSHIFT;
static bool hk_pressed[2] = {false};
static uint8_t hk_current_index = 0;

/* store to check some modifier key pressed */
static uint8_t der_v_hk_mod_pressed = 0;

/* key sequence mapped keycode what is shifted  */
typedef struct {
  uint16_t shift_id;
  uint16_t keycode;
  uint16_t key_sequence[6];
} der_hk_key_shifter_t;

/* Define keycode and sequence when some key shifted */
const der_hk_key_shifter_t PROGMEM der_hk_key_shifters[] = {

/* qwertyuiop@ */  
  /* HK_Q */
  {.shift_id = HK_Q, .keycode = KC_HK_LSFT, .key_sequence = {KC_X, KC_A}},
  {.shift_id = HK_Q, .keycode = KC_NO, .key_sequence = {KC_DOT}},

  /* HK_W */
  {.shift_id = HK_W, .keycode = KC_HK_LSFT, .key_sequence = {KC_E}},
  {.shift_id = HK_W, .keycode = KC_HK_RSFT, .key_sequence = {KC_G, KC_G}},
  {.shift_id = HK_W, .keycode = KC_NO, .key_sequence = {KC_K, KC_A}},

  /* HK_E */
  {.shift_id = HK_E, .keycode = KC_HK_LSFT, .key_sequence = {KC_R, KC_I}},
  {.shift_id = HK_E, .keycode = KC_HK_RSFT, .key_sequence = {KC_D, KC_A}},
  {.shift_id = HK_E, .keycode = KC_NO, .key_sequence = {KC_T, KC_A}},

  /* HK_R */
  {.shift_id = HK_R, .keycode = KC_HK_LSFT, .key_sequence = {KC_X, KC_Y, KC_A}},
  {.shift_id = HK_R, .keycode = KC_HK_RSFT, .key_sequence = {KC_G, KC_O}},
  {.shift_id = HK_R, .keycode = KC_NO, .key_sequence = {KC_K, KC_O}},

  /* HK_T */
  {.shift_id = HK_T, .keycode = KC_HK_LSFT, .key_sequence = {KC_R, KC_E}},
  {.shift_id = HK_T, .keycode = KC_HK_RSFT, .key_sequence = {KC_Z, KC_A}},
  {.shift_id = HK_T, .keycode = KC_NO, .key_sequence = {KC_S, KC_A}},

  /* HK_Y */
  {.shift_id = HK_Y, .keycode = KC_HK_LSFT, .key_sequence = {KC_P, KC_A}},
  {.shift_id = HK_Y, .keycode = KC_HK_RSFT, .key_sequence = {KC_Y, KC_O}},
  {.shift_id = HK_Y, .keycode = KC_NO, .key_sequence = {KC_R, KC_A}},

/* HK_U */
  {.shift_id = HK_U, .keycode = KC_HK_LSFT, .key_sequence = {KC_D, KC_I}},
  {.shift_id = HK_U, .keycode = KC_HK_RSFT, .key_sequence = {KC_N, KC_I}},
  {.shift_id = HK_U, .keycode = KC_NO, .key_sequence = {KC_C, KC_H, KC_I}},

/* HK_I */
  {.shift_id = HK_I, .keycode = KC_HK_LSFT, .key_sequence = {KC_D, KC_I}},
  {.shift_id = HK_I, .keycode = KC_HK_RSFT, .key_sequence = {KC_N, KC_I}},
  {.shift_id = HK_I, .keycode = KC_NO, .key_sequence = {KC_K, KC_U}},

  
/* HK_O */
  {.shift_id = HK_O, .keycode = KC_HK_LSFT, .key_sequence = {KC_G, KC_U}},
  {.shift_id = HK_O, .keycode = KC_HK_RSFT, .key_sequence = {KC_R, KC_U}},
  {.shift_id = HK_O, .keycode = KC_NO, .key_sequence = {KC_K, KC_U}},

/* HK_P */
  {.shift_id = HK_P, .keycode = KC_HK_LSFT, .key_sequence = {KC_D, KC_U}},
  {.shift_id = HK_P, .keycode = KC_HK_RSFT, .key_sequence = {KC_M, KC_A}},
  {.shift_id = HK_P, .keycode = KC_NO, .key_sequence = {KC_T, KC_U}},

/* HK_AT */
  {.shift_id = HK_AT, .keycode = KC_HK_LSFT, .key_sequence = {JA_RBRC}},
  {.shift_id = HK_AT, .keycode = KC_HK_RSFT, .key_sequence = {KC_LSFT, KC_8, KC_LSFT, KC_9}},
  {.shift_id = HK_AT, .keycode = KC_NO, .key_sequence = {KC_COMM}},

/* asdfghjkl;: */

/* HK_A */
  {.shift_id = HK_A, .keycode = KC_HK_LSFT, .key_sequence = {KC_W, KC_O}},
  {.shift_id = HK_A, .keycode = KC_HK_RSFT, .key_sequence = {KC_V, KC_U}},
  {.shift_id = HK_A, .keycode = KC_NO, .key_sequence = {KC_U}},

/* HK_S */
  {.shift_id = HK_S, .keycode = KC_HK_LSFT, .key_sequence = {KC_A}},
  {.shift_id = HK_S, .keycode = KC_HK_RSFT, .key_sequence = {KC_J, KC_I}},
  {.shift_id = HK_S, .keycode = KC_NO, .key_sequence = {KC_S, KC_I}},

/* HK_D */
  {.shift_id = HK_D, .keycode = KC_HK_LSFT, .key_sequence = {KC_N, KC_A}},
  {.shift_id = HK_D, .keycode = KC_HK_RSFT, .key_sequence = {KC_D, KC_E}},
  {.shift_id = HK_D, .keycode = KC_NO, .key_sequence = {KC_T, KC_E}},

/* HK_F */
  {.shift_id = HK_F, .keycode = KC_HK_LSFT, .key_sequence = {KC_X, KC_Y, KC_U}},
  {.shift_id = HK_F, .keycode = KC_HK_RSFT, .key_sequence = {KC_G, KC_E}},
  {.shift_id = HK_F, .keycode = KC_NO, .key_sequence = {KC_K, KC_E}},

/* HK_G */
  {.shift_id = HK_G, .keycode = KC_HK_LSFT, .key_sequence = {KC_M, KC_O}},
  {.shift_id = HK_G, .keycode = KC_HK_RSFT, .key_sequence = {KC_Z, KC_E}},
  {.shift_id = HK_G, .keycode = KC_NO, .key_sequence = {KC_S, KC_E}},

/* HK_H */
  {.shift_id = HK_H, .keycode = KC_HK_LSFT, .key_sequence = {KC_M, KC_I}},
  {.shift_id = HK_H, .keycode = KC_HK_RSFT, .key_sequence = {KC_B, KC_A}},
  {.shift_id = HK_H, .keycode = KC_NO, .key_sequence = {KC_H, KC_A}},

/* HK_J */
  {.shift_id = HK_J, .keycode = KC_HK_LSFT, .key_sequence = {KC_O}},
  {.shift_id = HK_J, .keycode = KC_HK_RSFT, .key_sequence = {KC_D, KC_O}},
  {.shift_id = HK_J, .keycode = KC_NO, .key_sequence = {KC_T, KC_O}},

/* HK_K */
  {.shift_id = HK_K, .keycode = KC_HK_LSFT, .key_sequence = {KC_N, KC_O}},
  {.shift_id = HK_K, .keycode = KC_HK_RSFT, .key_sequence = {KC_G, KC_I}},
  {.shift_id = HK_K, .keycode = KC_NO, .key_sequence = {KC_K, KC_I}},

/* HK_L */
  {.shift_id = HK_L, .keycode = KC_HK_LSFT, .key_sequence = {KC_X, KC_Y, KC_O}},
  {.shift_id = HK_L, .keycode = KC_HK_RSFT, .key_sequence = {KC_P, KC_O}},
  {.shift_id = HK_L, .keycode = KC_NO, .key_sequence = {KC_I}},

/* HK_SCOLON */
  {.shift_id = HK_SCOLON, .keycode = KC_HK_LSFT, .key_sequence = {KC_X, KC_T, KC_U}},
  {.shift_id = HK_SCOLON, .keycode = KC_NO, .key_sequence = {KC_N, KC_N}},


/* zxcvbnm,./ */

/* HK_Z */
  {.shift_id = HK_Z, .keycode = KC_HK_LSFT, .key_sequence = {KC_X, KC_U}},
  {.shift_id = HK_Z, .keycode = KC_NO, .key_sequence = {HK_SLASH}},

/* HK_X */
  {.shift_id = HK_X, .keycode = KC_HK_LSFT, .key_sequence = {KC_EQL}},
  {.shift_id = HK_X, .keycode = KC_HK_RSFT, .key_sequence = {KC_B, KC_I}},
  {.shift_id = HK_X, .keycode = KC_NO, .key_sequence = {KC_H, KC_I}},

/* HK_C */
  {.shift_id = HK_C, .keycode = KC_HK_LSFT, .key_sequence = {KC_R, KC_O}},
  {.shift_id = HK_C, .keycode = KC_HK_RSFT, .key_sequence = {KC_Z, KC_U}},
  {.shift_id = HK_C, .keycode = KC_NO, .key_sequence = {KC_S, KC_U}},

/* HK_V */
  {.shift_id = HK_V, .keycode = KC_HK_LSFT, .key_sequence = {KC_Y, KC_A}},
  {.shift_id = HK_V, .keycode = KC_HK_RSFT, .key_sequence = {KC_B, KC_U}},
  {.shift_id = HK_V, .keycode = KC_NO, .key_sequence = {KC_H, KC_U}},

/* HK_B */
  {.shift_id = HK_B, .keycode = KC_HK_LSFT, .key_sequence = {KC_X, KC_I}},
  {.shift_id = HK_B, .keycode = KC_HK_RSFT, .key_sequence = {KC_B, KC_E}},
  {.shift_id = HK_B, .keycode = KC_NO, .key_sequence = {KC_E, KC_H}},

/* HK_N */
  {.shift_id = HK_N, .keycode = KC_HK_LSFT, .key_sequence = {KC_N, KC_U}},
  {.shift_id = HK_N, .keycode = KC_HK_RSFT, .key_sequence = {KC_P, KC_U}},
  {.shift_id = HK_N, .keycode = KC_NO, .key_sequence = {KC_M, KC_E}},

/* HK_M */
  {.shift_id = HK_M, .keycode = KC_HK_LSFT, .key_sequence = {KC_Y, KC_U}},
  {.shift_id = HK_M, .keycode = KC_HK_RSFT, .key_sequence = {KC_Z, KC_O}},
  {.shift_id = HK_M, .keycode = KC_NO, .key_sequence = {KC_S, KC_O}},

/* HK_COMMA */
  {.shift_id = HK_COMMA, .keycode = KC_HK_LSFT, .key_sequence = {KC_M, KC_U}},
  {.shift_id = HK_COMMA, .keycode = KC_HK_RSFT, .key_sequence = {KC_P, KC_E}},
  {.shift_id = HK_COMMA, .keycode = KC_NO, .key_sequence = {KC_N, KC_E}},

/* HK_DOT */
  {.shift_id = HK_DOT, .keycode = KC_HK_LSFT, .key_sequence = {KC_W, KC_A}},
  {.shift_id = HK_DOT, .keycode = KC_HK_RSFT, .key_sequence = {KC_B, KC_O}},
  {.shift_id = HK_DOT, .keycode = KC_NO, .key_sequence = {KC_H, KC_O}},

/* HK_SLASH */
  {.shift_id = HK_SLASH, .keycode = KC_HK_LSFT, .key_sequence = {KC_X, KC_O}},
  {.shift_id = HK_SLASH, .keycode = KC_NO, .key_sequence = {HK_SLASH}},
};

/* Initialize variables for japanese implement */
void der_init_hk_variables(void) {
  hk_current_shift = HK_NOSHIFT;
  hk_pressed[0] = false;
  hk_pressed[1] = false;
  hk_current_index = 0;
}

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
  der_init_hk_variables();
}

/* convert keycode to shift id */
uint16_t der_conv_kc_to_shifter(uint16_t keycode) {
  switch (keycode) {
    case KC_Q: return HK_Q; break;
    case KC_W: return HK_W; break;
    case KC_E: return HK_E; break;
    case KC_R: return HK_R; break;
    case KC_T: return HK_T; break;
    case KC_Y: return HK_Y; break;
    case KC_U: return HK_U; break;
    case KC_I: return HK_I; break;
    case KC_O: return HK_O; break;
    case JA_AT: return HK_AT; break;
    case KC_A: return HK_A; break;
    case KC_S: return HK_S; break;
    case KC_D: return HK_D; break;
    case KC_F: return HK_F; break;
    case KC_G: return HK_G; break;
    case KC_H: return HK_H; break;
    case KC_J: return HK_J; break;
    case KC_K: return HK_K; break;
    case KC_L: return HK_L; break;
    case KC_SCLN: return HK_SCOLON; break;
    case JA_CLN: return HK_COLON; break;
    case KC_Z: return HK_Z; break;
    case KC_X: return HK_X; break;
    case KC_C: return HK_C; break;
    case KC_V: return HK_V; break;
    case KC_B: return HK_B; break;
    case KC_N: return HK_N; break;
    case KC_M: return HK_M; break;
    case KC_COMM: return HK_COMMA; break;
    case KC_DOT: return HK_DOT; break;
    case KC_SLSH: return HK_SLASH; break;
    case KC_HK_LSFT: return HK_LSFT; break;
    case KC_HK_RSFT: return HK_RSFT; break;
  }

  return HK_NOSHIFT;
}

der_hk_key_shifter_t der_get_mapping(int index) {
  der_hk_key_shifter_t data;
  memcpy_P(&data, &der_hk_key_shifters[index], sizeof(der_hk_key_shifter_t));
  return data;
}

/* return index of key_shifters if found shifter */
int der_find_mapping_on_shifter(uint16_t shifter, uint16_t keycode) {
  int length = sizeof(der_hk_key_shifters) / sizeof(der_hk_key_shifter_t);
  for (int i = 0; i < length; i++) {
    der_hk_key_shifter_t data = der_get_mapping(i);
    if (data.shift_id == shifter && data.keycode == keycode) {
      return i;
    }
  }

  return HK_SHIFT_NOT_FOUND;
}

/* send keycodes in specified key mapping */
void der_send_key_sequence(uint16_t shifter, uint16_t keycode) {

  int index = der_find_mapping_on_shifter(shifter, keycode);
  if (HK_SHIFT_NOT_FOUND == index) {
    return;
  }

  der_hk_key_shifter_t mapping = der_get_mapping(index);

  int length = sizeof(mapping.key_sequence) / sizeof(uint16_t);
  for (int i = 0; i < length; i++) {
    if (mapping.key_sequence[i] == KC_NO) {
      break;
    }
    register_code(mapping.key_sequence[i]);
    unregister_code(mapping.key_sequence[i]);
  }
}

bool der_is_custom_keycode(uint16_t keycode) {
  switch (keycode) {
    case KC_HK_RSFT:
    case KC_HK_LSFT:
      return false;
    default:
      break;
  }

  return true;
}

/* Process to resolve key shifting when user pressed key */
bool der_process_hk_shift_pressed(uint16_t keycode, uint16_t key_shifter, uint8_t current,
                                  uint8_t previous) {

  /* Update current shift if NOSHIFT */
  if (HK_NOSHIFT == hk_current_shift && HK_NOSHIFT != key_shifter) {
    hk_current_shift = key_shifter;
  }

  /* pass through this keycode to common process */
  if (HK_NOSHIFT == hk_current_shift || HK_ALL_RELEASED()) {
    return der_is_custom_keycode(keycode);
  }

  /* wait next keycode when all key are not pressed and not released */
  if (!HK_ALL_PRESSED()) {
    return false;
  }

  int shifter_index = der_find_mapping_on_shifter(hk_current_shift, keycode);

  /* should re-shift and send key sequence if not found mapping on shifter with keycode */
  if (HK_SHIFT_NOT_FOUND == shifter_index) {
    der_send_key_sequence(hk_current_shift, KC_NO);

    hk_current_shift = key_shifter;
    hk_pressed[previous] = false;
  } else {
    /* send key sequence and reset current state */
    der_send_key_sequence(hk_current_shift, keycode);

    hk_current_shift = HK_NOSHIFT;
    hk_pressed[current] = false;
    hk_pressed[previous] = false;
  }

  return false;
}

/* Process to resolve key sequence when user released key */
bool der_process_hk_shift_released(uint16_t keycode, uint16_t key_shifter, uint8_t current,
                                  uint8_t previous) {

  /* should pass thourgh released key to common process. */
  if (HK_NOSHIFT == hk_current_shift || HK_ALL_RELEASED()) {
    return der_is_custom_keycode(keycode);
  }

  /* send no-shift key sequence when released key are pressed already */
  if (!HK_ALL_PRESSED() && hk_current_shift == key_shifter) {
    der_send_key_sequence(hk_current_shift, KC_NO);
    der_init_hk_variables();
    return false;
  }

  return der_is_custom_keycode(keycode);
}

/* Convert keycode to mod bit */
uint8_t der_keycode_to_mod(uint16_t keycode) {
  switch (keycode) {
    case KC_LCTRL:
    case KC_RCTRL:
      return MOD_LCTL;
    case KC_LSHIFT:
    case KC_RSHIFT:
      return MOD_LSFT;
    case KC_LALT:
    case KC_RALT:
      return MOD_LALT;
    case KC_LGUI:
    case KC_RGUI:
      return MOD_LGUI;
    default:
      break;
  }

  return 0;
}

bool der_mod_pressed() {
  return der_v_hk_mod_pressed != 0;
}

/* Process keycodes as hachimitsu-koume keycodes */
bool der_process_hk(uint16_t keycode, keyrecord_t *record) {
  /* do not process in this function if some modifier keys pressed. */
  if (record->event.pressed) {
    der_v_hk_mod_pressed |= der_keycode_to_mod(keycode);
  } else {
    der_v_hk_mod_pressed ^= der_keycode_to_mod(keycode);
  }

  if (der_mod_pressed()) {
    return true;
  }

  if (record->event.pressed) {
    uint8_t current = hk_current_index ^ 1;
    hk_current_index = current;
    hk_pressed[current] = true;
    uint16_t key_shifter = der_conv_kc_to_shifter(keycode);

    switch (key_shifter) {
      case HK_NOSHIFT:
        der_init_hk_variables();
        break;
      default:
        return der_process_hk_shift_pressed(keycode, key_shifter, current, current ^ 1);
    }
  } else {
    /* If key released when current shifter is NOSHIFT, pass through keycode to common process.  */
    uint16_t key_shifter = der_conv_kc_to_shifter(keycode);
    switch (hk_current_shift) {
      case HK_NOSHIFT:
        /* initialize all variables */
        der_init_hk_variables();
        break;
      default:
        return der_process_hk_shift_released(keycode, key_shifter, hk_current_index, hk_current_index ^ 1);
    }
  }

  return true;
}
