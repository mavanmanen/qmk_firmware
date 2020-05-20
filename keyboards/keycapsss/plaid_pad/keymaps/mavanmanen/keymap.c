#include "plaid_pad.h"
#include "layers.h"
#include "hid-midi.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_DEFAULT] = LAYOUT_ortho_4x4(
	  KC_MPLY, KC_SLSH, KC_ASTR, TO(_MIDI),
		KC_1,    KC_2,    KC_3,    KC_MINS,
		KC_4,    KC_5,    KC_6,    KC_PLUS,
		KC_7,    KC_8,    KC_9,    KC_EQL
  ),

  [_MIDI] = LAYOUT_ortho_4x4(
    BUTTON_1,  BUTTON_2,  BUTTON_3,  BUTTON_4,
    BUTTON_5,  BUTTON_6,  BUTTON_7,  BUTTON_8,
    BUTTON_9,  BUTTON_10, BUTTON_11, BUTTON_12,
    BUTTON_13, BUTTON_14, BUTTON_15, BUTTON_16
  )
};

void keyboard_post_init_user(void) {
  writePinHigh(LED_RED);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if(biton32(layer_state) == _MIDI) {
      if(record->event.pressed) {
        process_midi_button(keycode);
        return false;
      }
    }

    return true;
}

void encoder_update_user(uint8_t index, bool ccw) {
  if(biton32(layer_state) == _MIDI) {
    process_midi_encoder(index, ccw);
  }
}