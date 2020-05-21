#pragma once

#include "plaid_pad.h"
#include "raw_hid.h"

#define MIDI_MAX 127
#define MIDI_MIN 0
#define MIDI_BUTTON_COUNT 16
#define _MIDI 10

enum button_type {
  TOGGLE,
  MOMENTARY
};

enum midi_buttons {
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

#define MIDI_LAYER [_MIDI] = LAYOUT_ortho_4x4( \
    BUTTON_1,  BUTTON_2,  BUTTON_3,  BUTTON_4, \
    BUTTON_5,  BUTTON_6,  BUTTON_7,  BUTTON_8, \
    BUTTON_9,  BUTTON_10, BUTTON_11, BUTTON_12,\
    BUTTON_13, BUTTON_14, BUTTON_15, BUTTON_16 \
  )

enum MIDI_CONTROL_TYPE {
  ENCODER = 1,
  BUTTON = 2
};

enum raw_hid_message {
  ACK = 100,
  CONNECTED,
  DISCONNECTED,
  GET_STATE,
  SAVE_STATE,
  STATE
};

bool process_midi_button(uint16_t keycode, keyrecord_t *record);
void process_midi_encoder(uint8_t index, bool ccw);