#include "plaid.h"

enum plaid_layers {
  _QWERTY,
  _LOWER,
  _RAISE,
  _FN
};

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)
#define KC_EURO LCA(KC_5)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT_plaid_grid(
      KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
      KC_ESC,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
      KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT,
      KC_LCTL, KC_RALT, KC_LGUI, KC_LALT, LOWER,   KC_SPC,  KC_SPC,  RAISE,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
  ),

  [_LOWER] = LAYOUT_plaid_grid(
      KC_GRV,  KC_1,    KC_2,    KC_3,    KC_0,    _______, _______, _______, _______, _______, KC_PSCR, KC_DEL,
      _______, KC_4,    KC_5,    KC_6,    KC_MINS, _______, _______, _______, _______, _______, _______, _______,
      KC_CAPS, KC_7,    KC_8,    KC_9,    KC_EQL,  _______, _______, _______, KC_LBRC, KC_RBRC, KC_BSLS, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END
  ),

  [_RAISE] = LAYOUT_plaid_grid(
      KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_RPRN, _______, _______, _______, _______, _______, _______, KC_DEL,
      _______, KC_DLR,  KC_PERC, KC_CIRC, KC_UNDS, _______, _______, _______, _______, _______, _______, _______,
      _______, KC_AMPR, KC_ASTR, KC_LPRN, KC_PLUS, _______, _______, _______, KC_LCBR, KC_RCBR, KC_PIPE, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
  ),

  [_FN] = LAYOUT_plaid_grid(
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
      _______, _______, KC_EURO, _______, _______, _______, _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
  ),

  // [] = LAYOUT_plaid_grid(
  //     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
  //     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
  //     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
  //     _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
  // ),
};

void keyboard_post_init_user(void) {
  writePinHigh(LED_RED);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
      writePinHigh(LED_GREEN);
    } else {
      writePinLow(LED_GREEN);
    }

    return true;
}