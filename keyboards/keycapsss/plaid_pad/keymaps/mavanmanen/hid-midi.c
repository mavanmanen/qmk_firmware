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

#define BUFFER_SIZE 32
uint8_t buffer[BUFFER_SIZE];

void clear_buffer(void) {
  for (size_t i = 0; i < BUFFER_SIZE; i++)
  {
    buffer[i] = 0;
  }
}

void send_data(void) {
  raw_hid_send(buffer, BUFFER_SIZE);
  clear_buffer();
}

bool process_midi_button(uint16_t keycode, keyrecord_t *record) {
  if(biton32(layer_state) == _MIDI) {
    uint16_t index = keycode - SAFE_RANGE;
    uint8_t currentState = button_states[index];

    buffer[0] = BUTTON;
    buffer[1] = index;

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

    buffer[2] = button_states[index];

    send_data();
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
    buffer[0] = ENCODER;
    buffer[1] = index;

    if(index == 0) {
      update_encoder(&encoder_left_value, ccw);
      buffer[2] = encoder_left_value;
    } else {
      update_encoder(&encoder_right_value, ccw);
      buffer[2] = encoder_right_value;
    }

    send_data();
  }
}

void send_ack(void) {
  buffer[0] = ACK;
  send_data();
}

void send_state(void) {
  buffer[0] = STATE;
  buffer[1] = encoder_left_value;
  buffer[2] = encoder_right_value;

  for (size_t i = 0; i < MIDI_BUTTON_COUNT; i++) {
    buffer[i+3] = button_states[i];
  }

  send_data();
}

void save_state(uint8_t *data) {
  encoder_left_value = data[1];
  encoder_right_value = data[2];

  const int offset = 3;
  for (size_t i = offset; i < MIDI_BUTTON_COUNT+offset; i++) {
    button_states[i-offset] = data[i];
  }

  send_ack();
}

void clear_state(void) {
  encoder_left_value = 0;
  encoder_right_value = 0;
  for (size_t i = 0; i < MIDI_BUTTON_COUNT; i++)
  {
    button_states[i] = 0;
  }
}

void raw_hid_receive(uint8_t *data, uint8_t length) {
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

    case CLEAR_STATE:
      clear_state();
      send_ack();
      break;
  }
}