#include "plaid_pad.h"
#include "hid-midi.h"

enum layers {
  _DEFAULT
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_DEFAULT] = LAYOUT_ortho_4x4(
	  KC_MPLY, KC_SLSH, KC_ASTR, _______,
		KC_1,    KC_2,    KC_3,    KC_MINS,
		KC_4,    KC_5,    KC_6,    KC_PLUS,
		KC_7,    KC_8,    KC_9,    KC_EQL
  ),
  MIDI_LAYER
};

void keyboard_post_init_user(void) {
  writePinHigh(LED_RED);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if(process_midi_button(keycode, record)) {
      return false;
    }

    return true;
}

void encoder_update_user(uint8_t index, bool ccw) {
  process_midi_encoder(index, ccw);
}