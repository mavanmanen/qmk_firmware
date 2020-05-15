#include "plaid_pad.h"
#include "qmk_midi.h"

enum layers {
  _DEFAULT,
  _MIDI
};

enum keycodes {
  BUTTON_1 = SAFE_RANGE,
  BUTTON_2,
  BUTTON_3,
  BUTTON_4,
  BUTTON_5,
  BUTTON_6,
  BUTTON_7,
  BUTTON_8,
  BUTTON_9,
  BUTTON_10,
  BUTTON_11,
  BUTTON_12,
  BUTTON_13,
  BUTTON_14,
  BUTTON_15,
  BUTTON_16
};

uint8_t button_states[16];
uint8_t encoder_values[2];

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_DEFAULT] = LAYOUT_ortho_4x4(
	  _______, _______, _______, _______,
		KC_1,    KC_2,    KC_3,    _______,
		KC_4,    KC_5,    KC_6,    _______,
		KC_7,    KC_8,    KC_9,    _______
  ),

  [_MIDI] = LAYOUT_ortho_4x4(
	  BUTTON_1,  BUTTON_2,  BUTTON_3,  BUTTON_4,
		BUTTON_5,  BUTTON_6,  BUTTON_7,  BUTTON_8,
		BUTTON_9,  BUTTON_10, BUTTON_11, BUTTON_12,
		BUTTON_13, BUTTON_14, BUTTON_15, BUTTON_16
  )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch(biton32(layer_state)) {
            case _MIDI: {
                uint8_t button = keycode - SAFE_RANGE;
                if(button_states[button] == 0) {
                  button_states[button] = 255;
                } else {
                  button_states[button] = 0;
                }

                midi_send_cc(&midi_device, 0, button + 1, button_states[button]);
                return false;
            }
        }
    }

    return true;
}

void keyboard_post_init_user(void) {
    writePinHigh(LED_RED);
}

void encoder_update_user(uint8_t index, bool clockwise) {
  switch(biton32(layer_state)) {
    case _DEFAULT:
      if(index == 0) {
        if(clockwise) {
          SEND_STRING(SS_TAP(X_VOLU));
        } else {
          SEND_STRING(SS_TAP(X_VOLD));
        }
      } else {
        if(clockwise) {

        } else {

        }
      }
      break;

    case _MIDI:
      encoder_values[index] = clockwise
        ? (encoder_values[index] == 255 ? 0   : encoder_values[index] + 1)
        : (encoder_values[index] == 0   ? 255 : encoder_values[index] - 1);

      midi_send_cc(&midi_device, 0, 17, encoder_values[index]);
      break;
  }
}