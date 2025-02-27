/*
 * Copyright © 2019-2023 Synthstrom Audible Limited
 *
 * This file is part of The Synthstrom Audible Deluge Firmware.
 *
 * The Synthstrom Audible Deluge Firmware is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef CONSEQUENCECLIPLENGTH_H_
#define CONSEQUENCECLIPLENGTH_H_

#include "model/consequence/consequence.h"
#include "RZA1/system/r_typedefs.h"

class Clip;

class ConsequenceClipLength final : public Consequence {
public:
	ConsequenceClipLength(Clip* newClip, int32_t oldLength);
	int revert(int time, ModelStack* modelStack);

	Clip* clip;
	int32_t lengthToRevertTo;

	uint64_t* pointerToMarkerValue;
	uint64_t markerValueToRevertTo;
};

#endif /* CONSEQUENCECLIPLENGTH_H_ */
