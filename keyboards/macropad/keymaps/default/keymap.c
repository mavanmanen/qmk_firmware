/*
Copyright 2020 Mitchell van Manen <ma.van.manen@live.nl>
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "macropad.h"
#include "qmk_midi.h"

enum layers {
    _MEDIA,
    _LAYERS,
    _MIDI,
};

enum keycodes {
    BUTTON_1 = SAFE_RANGE,
    BUTTON_2,
    BUTTON_3,
    BUTTON_4,
};

// MIDI
uint8_t encoder_value = 0;
bool button_1 = false;
bool button_2 = false;
bool button_3 = false;
bool button_4 = false;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_MEDIA]  = KEYMAP( KC_MPLY,    KC_MUTE,     KC_MPRV,   KC_MNXT,  TO(_LAYERS) ),
    [_LAYERS] = KEYMAP( TO(_MEDIA), TO(_MIDI),   KC_NO,     KC_NO,    RESET ),
    [_MIDI]   = KEYMAP( BUTTON_1,   BUTTON_2,    BUTTON_3,  BUTTON_4, TO(_LAYERS) )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch(biton32(layer_state)) {
            case _MIDI: {
                uint8_t cc = 0;
                uint8_t value = 0;

                switch(keycode) {
                    case BUTTON_1:
                        button_1 = !button_1;
                        cc = 1;
                        value = button_1 ? 255 : 0;
                        break;

                    case BUTTON_2:
                        button_2 = !button_2;
                        cc = 2;
                        value = button_2 ? 255 : 0;
                        break;

                    case BUTTON_3:
                        button_3 = !button_3;
                        cc = 3;
                        value = button_3 ? 255 : 0;
                        break;

                    case BUTTON_4:
                        button_4 = !button_4;
                        cc = 4;
                        value = button_4 ? 255 : 0;
                        break;

                    default:
                        return true;
                }

                midi_send_cc(&midi_device, 0, cc, value);
                return false;
            }
        }
    }

    return true;
}

void encoder_update_user(uint8_t index, bool clockwise) {
    switch(biton32(layer_state)) {
        case _MIDI:
            encoder_value = clockwise
                ? ( encoder_value == 255 ? 0   : encoder_value + 1 )
                : ( encoder_value == 0   ? 255 : encoder_value - 1 );

            midi_send_cc(&midi_device, 0, 5, encoder_value);
            break;

        case _MEDIA:
            if(clockwise) {
                SEND_STRING(SS_TAP(X_VOLU));
            } else {
                SEND_STRING(SS_TAP(X_VOLD));
            }

        default:
            break;
    }
}
