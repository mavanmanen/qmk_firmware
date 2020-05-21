#include "hid-midi.h"

uint8_t button_states[MIDI_BUTTON_COUNT];

uint8_t button_setup[] = {
  [BUTTON_1  - SAFE_RANGE] = TOGGLE,
  [BUTTON_2  - SAFE_RANGE] = TOGGLE,
  [BUTTON_3  - SAFE_RANGE] = TOGGLE,
  [BUTTON_4  - SAFE_RANGE] = TOGGLE,
  [BUTTON_5  - SAFE_RANGE] = TOGGLE,
  [BUTTON_6  - SAFE_RANGE] = TOGGLE,
  [BUTTON_7  - SAFE_RANGE] = TOGGLE,
  [BUTTON_8  - SAFE_RANGE] = TOGGLE,
  [BUTTON_9  - SAFE_RANGE] = MOMENTARY,
  [BUTTON_10 - SAFE_RANGE] = MOMENTARY,
  [BUTTON_11 - SAFE_RANGE] = MOMENTARY,
  [BUTTON_12 - SAFE_RANGE] = MOMENTARY,
  [BUTTON_13 - SAFE_RANGE] = MOMENTARY,
  [BUTTON_14 - SAFE_RANGE] = MOMENTARY,
  [BUTTON_15 - SAFE_RANGE] = MOMENTARY,
  [BUTTON_16 - SAFE_RANGE] = MOMENTARY
};

bool process_midi_button(uint16_t keycode, keyrecord_t *record) {
  if(biton32(layer_state) == _MIDI) {
    uint16_t index = keycode - SAFE_RANGE;
    uint8_t currentState = button_states[index];

    uint8_t data[32];
    data[0] = BUTTON;
    data[1] = index;

    if(record->event.pressed) {
      if(button_setup[index] == MOMENTARY) {
        button_states[index] = MIDI_MAX;
      } else {
        button_states[index] =
          currentState == MIDI_MIN
            ? MIDI_MAX
            : MIDI_MIN;
      }
    } else {
      if(button_setup[index] == MOMENTARY) {
        button_states[index] = MIDI_MIN;
      }
    }


    if(button_setup[index] == TOGGLE && record->event.pressed == false) {
      return true;
    }

    data[2] = button_states[index];
    raw_hid_send(data, sizeof(data));
    return true;
  }

  return false;
}

uint8_t encoder_left_value = 0;
uint8_t encoder_right_value = 0;

void update_encoder(uint8_t *value, bool ccw) {
  uint8_t val = *value;

  val = ccw
    ? (val > MIDI_MIN ? val-1 : MIDI_MAX)
    : (val < MIDI_MAX ? val+1 : MIDI_MIN);

  (*value) = val;
}

void process_midi_encoder(uint8_t index, bool ccw) {
  if(biton32(layer_state) == _MIDI) {
    uint8_t data[32];
    data[0] = ENCODER;
    data[1] = index;

    if(index == 0) {
      update_encoder(&encoder_left_value, ccw);
      data[2] = encoder_left_value;
    } else {
      update_encoder(&encoder_right_value, ccw);
      data[2] = encoder_right_value;
    }

    raw_hid_send(data, sizeof(data));
  }
}

void send_ack(void) {
  uint8_t data[32];
  data[0] = ACK;

  raw_hid_send(data, sizeof(data));
}

void send_state(void) {
  uint8_t data[32] = {0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0};
  data[0] = STATE;
  data[1] = encoder_left_value;
  data[2] = encoder_right_value;

  for (size_t i = 0; i < MIDI_BUTTON_COUNT; i++) {
    data[i+3] = button_states[i];
  }

  raw_hid_send(data, sizeof(data));
}

void save_state(uint8_t *data) {
  encoder_left_value = data[1];
  encoder_right_value = data[2];

  for (size_t i = 3; i < MIDI_BUTTON_COUNT; i++) {
    button_states[i-3] = data[i];
  }

  send_ack();
}

void raw_hid_receive(uint8_t *data, uint8_t length) {
  raw_hid_send(data, sizeof(data));

  switch(data[0]) {
    case CONNECTED:
      layer_move(_MIDI);
      writePinHigh(LED_GREEN);
      send_ack();
      break;

    case DISCONNECTED:
      layer_clear();
      writePinLow(LED_GREEN);
      send_ack();
      break;

    case GET_STATE:
      send_state();
      break;

    case SAVE_STATE:
      save_state(data);
      break;
  }
}