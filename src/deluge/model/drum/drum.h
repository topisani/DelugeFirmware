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

#ifndef DRUM_H
#define DRUM_H

#include "definitions.h"
#include "RZA1/system/r_typedefs.h"
#include "io/midi/learned_midi.h"

class Kit;
class ParamManagerForTimeline;
class Song;
class ModControllable;
class Clip;
class ModelStackWithThreeMainThings;
class ModelStackWithSoundFlags;
class ModelStackWithTimelineCounter;
class MIDIDevice;
class ParamManager;

/*
 * Kits are made up of multiple Drums. Even when they are not drum sounds, the class is called Drum, for better or worse.
 * In most instructional material for users, Synthstrom has referred to them often as
 * “items within kits”, or sometimes “rows” or “sounds” where applicable.
 *
 * Types of Drum are MIDIDrum, GateDrum, and SoundDrum (most often a sample).
 */

class Drum {
public:
	Drum(int newType);
	virtual ~Drum() {}

	Kit* kit;

	const uint8_t type;
	bool noteRowAssignedTemp;
	uint8_t earlyNoteVelocity; // If 0, then there's none
	bool earlyNoteStillActive;

	bool auditioned;
	uint8_t lastMIDIChannelAuditioned; // Primarily for MPE purposes

	int8_t lastExpressionInputsReceived[2][NUM_EXPRESSION_DIMENSIONS];

	Drum* next;

	LearnedMIDI midiInput;
	LearnedMIDI muteMIDICommand;

	virtual void noteOn(ModelStackWithThreeMainThings* modelStack, uint8_t velocity, Kit* kit, int16_t const* mpeValues,
	                    int fromMIDIChannel = MIDI_CHANNEL_NONE, uint32_t sampleSyncLength = 0, int32_t ticksLate = 0,
	                    uint32_t samplesLate = 0) = 0;
	virtual void noteOff(ModelStackWithThreeMainThings* modelStack, int velocity = DEFAULT_LIFT_VALUE) = 0;
	virtual bool allowNoteTails(ModelStackWithSoundFlags* modelStack, bool disregardSampleLoop = false) = 0;
	virtual bool anyNoteIsOn() = 0;
	virtual bool hasAnyVoices() = 0;
	virtual void unassignAllVoices() = 0;

	virtual int loadAllSamples(bool mayActuallyReadFiles) { return NO_ERROR; }
	virtual void prepareForHibernation() {}
	virtual void prepareDrumToHaveNoActiveClip() {}

	virtual void writeToFile(bool savingSong, ParamManager* paramManager) = 0;
	virtual int readFromFile(Song* song, Clip* clip, int32_t readAutomationUpToPos) = 0;
	virtual void drumWontBeRenderedForAWhile();

	virtual void getName(char* buffer) = 0; // May return up to 5 actual characters, so supply at least a char[6]
	virtual void choke(ModelStackWithSoundFlags* modelStack) {} // modelStack can be NULL if you really insist
	void writeMIDICommandsToFile();
	bool readDrumTagFromFile(char const* tagName);
	void recordNoteOnEarly(int velocity, bool noteTailsAllowed);
	void expressionEventPossiblyToRecord(ModelStackWithTimelineCounter* modelStack, int16_t newValue,
	                                     int whichExpressionimension, int level);
	virtual void expressionEvent(int newValue, int whichExpressionimension) {}
	void getCombinedExpressionInputs(int16_t* combined);

	virtual ModControllable* toModControllable() { return NULL; }
};

#endif // DRUM_H
