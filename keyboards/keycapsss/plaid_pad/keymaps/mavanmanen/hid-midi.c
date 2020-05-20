#include "hid-midi.h"

uint8_t buttonStates[MIDI_BUTTON_COUNT];

void process_midi_button(uint16_t keycode) {
    uint16_t index = keycode - SAFE_RANGE;
    uint8_t currentState = buttonStates[index];
    buttonStates[index] =
        currentState == MIDI_MIN
        ? MIDI_MAX
        : MIDI_MIN;

    uint8_t data[32];
    data[0] = BUTTON;
    data[1] = index;
    data[2] = buttonStates[index];
    raw_hid_send(data, sizeof(data));
}

uint8_t encoder_left_value = 0;
uint8_t encoder_right_value = 0;

void process_midi_encoder(uint8_t index, bool ccw) {
    uint8_t data[32];
    data[0] = ENCODER;
    data[1] = index;

    if(index == 0) {
    if(ccw) {
        encoder_left_value = encoder_left_value > MIDI_MIN
        ? encoder_left_value - 1
        : MIDI_MAX;
    } else {
        encoder_left_value = encoder_left_value < MIDI_MAX
        ? encoder_left_value + 1
        : MIDI_MIN;
    }

    data[2] = encoder_left_value;
    } else {
    if(ccw) {
        encoder_right_value = encoder_right_value > MIDI_MIN
        ? encoder_right_value - 1
        : MIDI_MAX;
    } else {
        encoder_right_value = encoder_right_value < MIDI_MAX
        ? encoder_right_value + 1
        : MIDI_MIN;
    }

    data[2] = encoder_right_value;
    }

    raw_hid_send(data, sizeof(data));
}

void send_ack(void) {
  uint8_t data[32];
  data[0] = ACK;

  raw_hid_send(data, sizeof(data));
}

void send_state(void) {
  uint8_t data[32];
  data[0] = STATE;
  data[1] = encoder_left_value;
  data[2] = encoder_right_value;

  for (size_t i = 0; i < MIDI_BUTTON_COUNT; i++) {
    data[i+3] = buttonStates[i];
  }

  raw_hid_send(data, sizeof(data));
}

void save_state(uint8_t *data) {
  encoder_left_value = data[1];
  encoder_right_value = data[2];

  for (size_t i = 3; i < MIDI_BUTTON_COUNT; i++) {
    buttonStates[i-3] = data[i];
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
      layer_move(_DEFAULT);
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