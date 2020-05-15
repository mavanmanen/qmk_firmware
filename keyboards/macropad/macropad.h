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

#ifndef MACROPAD_H
#define MACROPAD_H

#include "quantum.h"

#define KEYMAP(k00, k01, k02, k03, k04) {{k00, k01, k02, k03, k04}}

void matrix_init_user(void);
void matrix_scan_user(void);
void encoder_update_user(uint8_t index, bool clockwise);

#endif