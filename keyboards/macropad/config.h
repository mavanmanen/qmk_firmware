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

#ifndef CONFIG_H
#define CONFIG_H

#include "config_common.h"

#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x1337
#define DEVICE_VER      0x0001
#define MANUFACTURER    Mavanmanen
#define PRODUCT         Macropad
#define DESCRIPTION     Macropad

#define MATRIX_ROWS 1
#define MATRIX_COLS 5

#define MATRIX_ROW_PINS { F7 }
#define MATRIX_COL_PINS { B5, B4, B1, B2, B3 }

#define ENCODERS_PAD_A { E6 }
#define ENCODERS_PAD_B { D7 }

#define UNUSED_PINS
#define DIODE_DIRECTION COL2ROW

#define DEBOUNCING_DELAY 4
#define TAPPING_TOGGLE 2
#define TAPPING_TERM 200
#define MIDI_ADVANCED

#endif