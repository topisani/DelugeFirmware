/*
 * Copyright © 2014-2023 Synthstrom Audible Limited
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

#pragma once

#include "definitions_cxx.hpp"
#include "model/global_effectable/global_effectable.h"
#include "modulation/params/param.h"
#include "storage/storage_manager.h"
#include "util/containers.h"
#include <cstdint>

class AudioClip;
class InstrumentClip;
class Clip;
class Kit;
class Drum;
class ModelStack;
class ModelStackWithThreeMainThings;
class ModelStackWithAutoParam;
enum class MIDIMatchType;

namespace params = deluge::modulation::params;

class MidiFollow final {
public:
	MidiFollow();
	void readDefaultsFromFile();

	ModelStackWithAutoParam* getModelStackWithParam(ModelStackWithTimelineCounter* modelStackWithTimelineCounter,
	                                                Clip* clip, int32_t soundParamId, int32_t globalParamId,
	                                                bool displayError = true);
	void noteMessageReceived(MIDICable& cable, bool on, int32_t channel, int32_t note, int32_t velocity,
	                         bool* doingMidiThru, bool shouldRecordNotesNowNow, ModelStack* modelStack);
	void sendNoteToClip(MIDICable& cable, Clip* clip, MIDIMatchType match, bool on, int32_t channel, int32_t note,
	                    int32_t velocity, bool* doingMidiThru, bool shouldRecordNotesNowNow, ModelStack* modelStack);
	void midiCCReceived(MIDICable& cable, uint8_t channel, uint8_t ccNumber, uint8_t ccValue, bool* doingMidiThru,
	                    ModelStack* modelStack);
	void pitchBendReceived(MIDICable& cable, uint8_t channel, uint8_t data1, uint8_t data2, bool* doingMidiThru,
	                       ModelStack* modelStack);
	void aftertouchReceived(MIDICable& cable, int32_t channel, int32_t value, int32_t noteCode, bool* doingMidiThru,
	                        ModelStack* modelStack);

	void clearStoredClips();
	void removeClip(Clip* clip);

	// midi CC mappings
	int32_t getCCFromParam(deluge::modulation::params::Kind paramKind, int32_t paramID);
	bool isGlobalEffectableContext();

	int32_t paramToCC[kDisplayWidth][kDisplayHeight];

	std::array<uint8_t, kMaxMIDIValue + 1> ccToSoundParam;
	std::array<uint8_t, kMaxMIDIValue + 1> ccToGlobalParam;
	std::array<uint8_t, params::UNPATCHED_START + params::UNPATCHED_SOUND_MAX_NUM> soundParamToCC;
	std::array<uint8_t, params::UNPATCHED_GLOBAL_MAX_NUM> globalParamToCC;

	int32_t previousKnobPos[kMaxMIDIValue + 1];
	uint32_t timeLastCCSent[kMaxMIDIValue + 1];
	uint32_t timeAutomationFeedbackLastSent;

	// public so it can be called from View::sendMidiFollowFeedback
	void sendCCWithoutModelStackForMidiFollowFeedback(int32_t channel, bool isAutomation = false);
	void sendCCForMidiFollowFeedback(int32_t channel, int32_t ccNumber, int32_t knobPos);

	void handleReceivedCC(ModelStackWithTimelineCounter& modelStack, Clip* clip, int32_t ccNumber, int32_t ccValue);

private:
	// initialize
	void init();
	void initState();
	void clearMappings();
	void initDefaultMappings();

	Clip* getSelectedOrActiveClip();
	Clip* getSelectedClip();
	Clip* getActiveClip(ModelStack* modelStack);

	// get model stack with auto param for midi follow cc-param control
	ModelStackWithAutoParam* getModelStackWithParamForSong(ModelStackWithThreeMainThings* modelStackWithThreeMainThings,
	                                                       int32_t soundParamId, int32_t globalParamId);
	ModelStackWithAutoParam* getModelStackWithParamForClip(ModelStackWithTimelineCounter* modelStackWithTimelineCounter,
	                                                       Clip* clip, int32_t soundParamId, int32_t globalParamId);
	ModelStackWithAutoParam*
	getModelStackWithParamForSynthClip(ModelStackWithTimelineCounter* modelStackWithTimelineCounter, Clip* clip,
	                                   int32_t soundParamId, int32_t globalParamId);
	ModelStackWithAutoParam*
	getModelStackWithParamForKitClip(ModelStackWithTimelineCounter* modelStackWithTimelineCounter, Clip* clip,
	                                 int32_t soundParamId, int32_t globalParamId);
	ModelStackWithAutoParam*
	getModelStackWithParamForAudioClip(ModelStackWithTimelineCounter* modelStackWithTimelineCounter, Clip* clip,
	                                   int32_t soundParamId, int32_t globalParamId);
	void displayParamControlError(int32_t soundParamId, int32_t globalParamId);

	MIDIMatchType checkMidiFollowMatch(MIDICable& cable, uint8_t channel);
	bool isFeedbackEnabled();

	// saving
	void writeDefaultsToFile();
	void writeDefaultMappingsToFile();

	// loading
	bool successfullyReadDefaultsFromFile;
	void readDefaultMappingsFromFile(Deserializer& reader);
};

extern MidiFollow midiFollow;
