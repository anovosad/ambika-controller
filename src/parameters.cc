#pragma once

#include "parameters.h"
#include <cstring>

const char* oscShapes[] = {
 "None", "Sawtooth", "Square", "Triangle", "Sine", "zSaw", "lp zSaw", "pk zsaw", "bp zSaw",
 "hp zSaw", "lp zPulse", "pk zPulse", "bp zPulse", "hp zPulse", "zTriangle", "Pad", "FM",
 "8 bits", "Pwm", "Noise", "Vowel", "Mmale", "Female", "Choir", "Tampura", "Bowed", "Cello",
 "Vibes", "Slap bass", "E-Piano", "Organ", "Waves", "Digital PPG", "Drone 1", "Drone 2", "Metallic",
 "Bell", "Wavquence", nullptr
};

const char* subOscShapes[] = {
  "Square Sub -1", "Triangle Sub -1", "Pulse Sub -1", "Square Sub -2", "Triangle Sub -2", "Pulse Sub -2", "Click", "Glitch Click", "Blow Click", "Metal Click", "Pop Click", nullptr
};


const char* lfoShapes[] = {
 "Triangle", "Square", "S-H", "Ramp", "Wave1", "Wave2", "Wave3", "Wave4", "Wave5", "Wave6", "Wave7", "Wave8", "Wave9", "Wave10", "Wave11", "Wave12", "Wave13", "Wave14", "Wave15", "Wave16", nullptr
};
 
const char* mixOps[] = {
  "Sum", "Sync", "Ring", "Xor", "Foldback", "Bits Redu", nullptr
};

const char* filterModes[] = {
  "Low Pass", "Band Pass", "High Pass", nullptr
};

const char* matrixSources[] = {
  "Env1", "Env2", "Env3", "Lfo1", "Lfo2", "Lfo3", "Lfo4", "Mod1", "Mod2", "Mod3", "Mod4", "Seq1", "Seq2", "Arp", "Velo", "Aftr", "Bend", "MWhl", "Whl2", "Pdal", "Note", "Gate", "Nois", "Rand", "=256", "=32", "=16", "=8", "=4", "Par1", "Par2", nullptr
};

const char* matrixDestinations[] = {
  "Par1", "Par2", "Osc1", "Osc2", "~1+2", "Vibr", "Mix", "XMod", "Nois", "Sub", "Fuzz", "Crsh", "Freq", "Reso", "Attk", "Deca", "Rele", "Lfo4", "Vca", nullptr
};

const char* operators[] = {
  "None", "Add", "Prod", "Attn", "Max", "Min", "Xor", ">=", "<=", "Qtz", "Lag", nullptr
};

const char* lfoSyncModes[] = {
  "Free", "Slave", "Master", nullptr
};

const char *polyphonyModes[] = {
  "Mono", "Poly", "2x Unison", "Cyclic", "Chain", nullptr
};

const char *noteNames[] = {
  "C-", "C#-", "D-", "D#-", "E-", "F-", "F#-", "G-", "G#", "A-", "A#-", "B-",
  "C0", "C#0", "D0", "D#0", "E0", "F0", "F#0", "G0", "G#", "A0", "A#0", "B0",
  "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#", "A1", "A#1", "B1",
  "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#", "A2", "A#2", "B2",
  "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#", "A3", "A#3", "B3",
  "C4", "C#4", "D4", "D#4", "E4", "F4", "F#4", "G4", "G#", "A4", "A#4", "B4",
  "C5", "C#5", "D5", "D#5", "E5", "F5", "F#5", "G5", "G#", "A5", "A#5", "B5",
  "C6", "C#6", "D6", "D#6", "E6", "F6", "F#6", "G6", "G#", "A6", "A#6", "B6",
  "C7", "C#7", "D7", "D#7", "E7", "F7", "F#7", "G7", "G#", "A7", "A#7", "B7",
  "C8", "C#8", "D8", "D#8", "E8", "F8", "F#8", "G8", "G#", "A8", "A#8", "B8",
  "C9", "C#9", "D9", "D#9", "E9", "F9", "F#9", "G9", nullptr
};

 

Parameter::Parameter(const char *fullName, uint8_t cc, uint8_t nrpn, int8_t min, int8_t max, int8_t defaultValue, const char **valConverter) : 
  fullName(fullName), cc(cc), nrpn(nrpn), min(min), max(max), defaultValue(defaultValue), rawValue(0.0), dirty(false), valConverter(valConverter)
  {
    name = std::strchr(fullName, ' ');
    if (name == nullptr) {
      name = fullName;
    } 

    setNRPNValue(defaultValue);
  }

bool Parameter::isDirty() {
  return dirty;
}

void Parameter::removeDirty() {
  dirty = false;
}

unsigned long Parameter::getLastChanged() {
  return lastChanged;
}

const char * Parameter::getFullName() {
  return fullName;
}

const char * Parameter::getName() {
  return name;
}

String Parameter::getDisplayValue() {
  if (valConverter == nullptr) {
    return String(getNRPNValue());
  } else {
    int16_t index = getNormalizedValue();
    return String(valConverter[index]);
  }
}

void Parameter::setNRPNValue(int16_t value) {
  if (value < min) value = min;
  if (value > max) value = max;

  int16_t ticks = max - min + 1;
  float boundary = 4096.0 / ticks;
  setRawValue((value - min) * boundary + boundary / 2);
}

// NRPN value is in range <min, max>
int16_t Parameter::getNRPNValue() {
  return getNormalizedValue() + min;
}

// normalized value is in range <0, max-min>
int16_t Parameter::getNormalizedValue() {
  int16_t ticks = std::max(std::abs(max - min + 1), 1);
  float boundary = 4096.0 / ticks;
  return floor(rawValue / boundary);
}

// value in range <0, 127>
int8_t Parameter::getCCValue() {
  return floor(rawValue / 32.0);
}

// raw value is in range <0.0, 4095.0>
float Parameter::getRawValue() {
  return rawValue;
} 

void Parameter::setRawValue(float value) {
  if (value < 0.0) value = 0.0;
  if (value > 4095.0) value = 4095.0;
  if (rawValue == value) return;
  
  rawValue = value;
  dirty = true;
  lastChanged = millis();
}

uint8_t Parameter::getCC() {
  return cc;
}

uint8_t Parameter::getNRPN() {
  return nrpn;
}

std::vector<Parameter> params = {
  Parameter{"Osc1 Shape", 16, 0, 0, 37, 1, oscShapes},
  Parameter{"Osc1 Param", 17, 1, 0, 127, 0, nullptr},
  Parameter{"Osc1 Range", 14, 2, -24, 24, 0, nullptr},
  Parameter{"Osc1 Detune", 15, 3, -64, 63, 0, nullptr},

  Parameter{"Osc2 Shape", 18, 4, 0, 37, 2, oscShapes},
  Parameter{"Osc2 Param", 19, 5, 0, 127, 0, nullptr},
  Parameter{"Osc2 Range", 20, 6, -24, 24, 0, nullptr},
  Parameter{"Osc2 Detune", 21, 7, -64, 63, 0, nullptr},

  Parameter{"Mix Osc1/Osc2", 22, 8, 0, 63, 32, nullptr},
  Parameter{"Mix Cross Op", 23, 9, 0, 5, 0, mixOps},
  Parameter{"Mix Op Param", 24, 10, 0, 63, 0, nullptr},
  Parameter{"Mix Sub Shape", 25, 11, 0, 10, 0, subOscShapes},
  Parameter{"Mix Sub Volume", 26, 12, 0, 63, 32, nullptr},
  Parameter{"Mix Noise", 27, 13, 0, 63, 0, nullptr},
  Parameter{"Mix Fuzz", 12, 14, 0, 63, 0, nullptr},
  Parameter{"Mix Crush", 13, 15, 0, 31, 0, nullptr},

  Parameter{"Filter Freq", 74, 16, 0, 127, 63, nullptr},
  Parameter{"Filter Reso", 71, 17, 0, 63, 0, nullptr},
  Parameter{"Filter Mode", 28, 18, 0, 2, 0, filterModes},
  Parameter{"Filter Env2", 3, 22, 0, 63, 0, nullptr},
  Parameter{"Filter LFO2", 9, 23, 0, 63, 0, nullptr},
  Parameter{"Filter Velocity", 0, 104, 0, 63, 0, nullptr},
  Parameter{"Filter Keyboard", 0, 105, -64, 63, 0, nullptr},

  Parameter{"Env1 Attack", 73, 24, 0, 127, 5, nullptr},
  Parameter{"Env1 Decay", 75, 25, 0, 127, 5, nullptr},
  Parameter{"Env1 Sustain", 70, 26, 0, 127, 63, nullptr},
  Parameter{"Env1 Release", 72, 27, 0, 127, 10, nullptr},

  Parameter{"Env2 Attack", 81, 32, 0, 127, 5, nullptr},
  Parameter{"Env2 Decay", 83, 33, 0, 127, 5, nullptr},
  Parameter{"Env2 Sustain", 78, 34, 0, 127, 63, nullptr},
  Parameter{"Env2 Release", 80, 35, 0, 127, 10, nullptr},

  Parameter{"Env3 Attack", 89, 40, 0, 127, 5, nullptr},
  Parameter{"Env3 Decay", 91, 41, 0, 127, 5, nullptr},
  Parameter{"Env3 Sustain", 86, 42, 0, 127, 63, nullptr},
  Parameter{"Env3 Release", 88, 43, 0, 127, 10, nullptr},

  Parameter{"Mtx1 Source", 0, 50, 0, 24, 0, matrixSources},
  Parameter{"Mtx1 Dest", 0, 51, 0, 18, 0, matrixDestinations},
  Parameter{"Mtx1 Amount", 0, 52, -63, 63, 0, nullptr},

  Parameter{"Mtx2 Source", 0, 53, 0, 24, 0, matrixSources},
  Parameter{"Mtx2 Dest", 0, 54, 0, 18, 1, matrixDestinations},
  Parameter{"Mtx2 Amount", 0, 55, -63, 63, 0, nullptr},

  Parameter{"Mtx3 Source", 0, 56, 0, 24, 3, matrixSources},
  Parameter{"Mtx3 Dest", 0, 57, 0, 18, 2, matrixDestinations},
  Parameter{"Mtx3 Amount", 0, 58, -63, 63, 0, nullptr},

  Parameter{"Mtx4 Source", 0, 59, 0, 24, 3, matrixSources},
  Parameter{"Mtx4 Dest", 0, 60, 0, 18, 3, matrixDestinations},
  Parameter{"Mtx4 Amount", 0, 61, -63, 63, 0, nullptr},

  Parameter{"Mtx5 Source", 0, 62, 0, 24, 4, matrixSources},
  Parameter{"Mtx5 Dest", 0, 63, 0, 18, 0, matrixDestinations},
  Parameter{"Mtx5 Amount", 0, 64, -63, 63, 0, nullptr},

  Parameter{"Mtx6 Source", 0, 65, 0, 24, 4, matrixSources},
  Parameter{"Mtx6 Dest", 0, 66, 0, 18, 1, matrixDestinations},
  Parameter{"Mtx6 Amount", 0, 67, -63, 63, 0, nullptr},

  Parameter{"Mtx7 Source", 0, 68, 0, 24, 5, matrixSources},
  Parameter{"Mtx7 Dest", 0, 69, 0, 18, 6, matrixDestinations},
  Parameter{"Mtx7 Amount", 0, 70, -63, 63, 0, nullptr},

  Parameter{"Mtx8 Source", 0, 71, 0, 24, 6, matrixSources},
  Parameter{"Mtx8 Dest", 0, 72, 0, 18, 12, matrixDestinations},
  Parameter{"Mtx8 Amount", 0, 73, -63, 63, 0, nullptr},

  Parameter{"Mtx9 Source", 0, 74, 0, 24, 11, matrixSources},
  Parameter{"Mtx9 Dest", 0, 75, 0, 18, 12, matrixDestinations},
  Parameter{"Mtx9 Amount", 0, 76, -63, 63, 0, nullptr},

  Parameter{"Mtx10 Source", 0, 77, 0, 24, 12, matrixSources},
  Parameter{"Mtx10 Dest", 0, 78, 0, 18, 6, matrixDestinations},
  Parameter{"Mtx10 Amount", 0, 79, -63, 63, 0, nullptr},

  Parameter{"Mtx11 Source", 0, 80, 0, 24, 2, matrixSources},
  Parameter{"Mtx11 Dest", 0, 81, 0, 18, 18, matrixDestinations},
  Parameter{"Mtx11 Amount", 0, 82, -63, 63, 63, nullptr},

  Parameter{"Mtx12 Source", 0, 83, 0, 24, 14, matrixSources},
  Parameter{"Mtx12 Dest", 0, 84, 0, 18, 18, matrixDestinations},
  Parameter{"Mtx12 Amount", 0, 85, -63, 63, 16, nullptr},

  Parameter{"Mtx13 Source", 0, 86, 0, 24, 16, matrixSources},
  Parameter{"Mtx13 Dest", 0, 87, 0, 18, 4, matrixDestinations},
  Parameter{"Mtx13 Amount", 0, 88, -63, 63, 32, nullptr},

  Parameter{"Mtx14 Source", 0, 89, 0, 24, 6, matrixSources},
  Parameter{"Mtx14 Dest", 0, 90, 0, 18, 4, matrixDestinations},
  Parameter{"Mtx14 Amount", 0, 91, -63, 63, 16, nullptr},

  Parameter{"Op1 Source1", 0, 92, 0, 30, 3, matrixSources},
  Parameter{"Op1 Source2", 0, 93, 0, 30, 4, matrixSources},
  Parameter{"Op1 Operation", 0, 94, 0, 10, 0, operators},

  Parameter{"Op2 Source1", 0, 95, 0, 30, 4, matrixSources},
  Parameter{"Op2 Source2", 0, 96, 0, 30, 5, matrixSources},
  Parameter{"Op2 Operation", 0, 97, 0, 10, 0, operators},

  Parameter{"Op3 Source1", 0, 98, 0, 30, 5, matrixSources},
  Parameter{"Op3 Source2", 0, 99, 0, 30, 11, matrixSources},
  Parameter{"Op3 Operation", 0, 100, 0, 10, 0, operators},

  Parameter{"Op4 Source1", 0, 101, 0, 30, 11, matrixSources},
  Parameter{"Op4 Source2", 0, 102, 0, 30, 12, matrixSources},
  Parameter{"Op4 Operation", 0, 103, 0, 10, 0, operators},

  Parameter{"Lfo1 Shape", 46, 28, 0, 19, 0, oscShapes},
  Parameter{"Lfo1 Rate", 45, 29, 0, 127, 20, nullptr},
  Parameter{"Lfo1 Trigger", 44, 31, 0, 2, 0, lfoSyncModes},

  Parameter{"Lfo2 Shape", 54, 36, 0, 19, 0, oscShapes},
  Parameter{"Lfo2 Rate", 53, 37, 0, 127, 20, nullptr},
  Parameter{"Lfo2 Trigger", 52, 39, 0, 2, 0, lfoSyncModes},

  Parameter{"Lfo3 Shape", 62, 44, 0, 19, 0, oscShapes},
  Parameter{"Lfo3 Rate", 61, 45, 0, 127, 20, nullptr},
  Parameter{"Lfo3 Trigger", 60, 47, 0, 2, 0, lfoSyncModes},

  Parameter{"Lfo4 Shape", 48, 48, 0, 3, 0, oscShapes},
  Parameter{"Lfo4 Rate", 47, 49, 0, 127, 20, nullptr},

  Parameter{"Volume", 7, 112, 0, 127, 127, nullptr},
  Parameter{"Octave", 0, 113, -2, 2, 0, nullptr},
  Parameter{"Tuning", 94, 114, -127, 126, 0, nullptr},
  Parameter{"Spread", 95, 115, 0, 40, 0, nullptr},
  Parameter{"Raga", 0, 116, 0, 31, 0, nullptr},
  Parameter{"Legato", 68, 117, 0, 1, 0, nullptr},
  Parameter{"Portamento", 0, 118, 0, 63, 0, nullptr},
  Parameter{"Poly Mode", 107, 127, 0, 4, 0, polyphonyModes},

  Parameter{"Part1 Channel", 0, 0, 0, 16, 1, nullptr},
  Parameter{"Part1 Low Key", 0, 0, 0, 127, 0, noteNames},
  Parameter{"Part1 High Key", 0, 0, 0, 127, 127, noteNames},
  Parameter{"Part1 Voices", 0, 0, 0, 63, 0, nullptr},

  Parameter{"Part2 Channel", 0, 0, 0, 16, 1, nullptr},
  Parameter{"Part2 Low Key", 0, 0, 0, 127, 0, noteNames},
  Parameter{"Part2 High Key", 0, 0, 0, 127, 127, noteNames},
  Parameter{"Part2 Voices", 0, 0, 0, 63, 0, nullptr},

  Parameter{"Part3 Channel", 0, 0, 0, 16, 1, nullptr},
  Parameter{"Part3 Low Key", 0, 0, 0, 127, 0, noteNames},
  Parameter{"Part3 High Key", 0, 0, 0, 127, 127, noteNames},
  Parameter{"Part3 Voices", 0, 0, 0, 63, 0, nullptr},

  Parameter{"Part4 Channel", 0, 0, 0, 16, 1, nullptr},
  Parameter{"Part4 Low Key", 0, 0, 0, 127, 0, noteNames},
  Parameter{"Part4 High Key", 0, 0, 0, 127, 127, noteNames},
  Parameter{"Part4 Voices", 0, 0, 0, 63, 0, nullptr},

  Parameter{"Part5 Channel", 0, 0, 0, 16, 1, nullptr},
  Parameter{"Part5 Low Key", 0, 0, 0, 127, 0, noteNames},
  Parameter{"Part5 High Key", 0, 0, 0, 127, 127, noteNames},
  Parameter{"Part5 Voices", 0, 0, 0, 63, 0, nullptr},

  Parameter{"Part6 Channel", 0, 0, 0, 16, 1, nullptr},
  Parameter{"Part6 Low Key", 0, 0, 0, 127, 0, noteNames},
  Parameter{"Part6 High Key", 0, 0, 0, 127, 127, noteNames},
  Parameter{"Part6 Voices", 0, 0, 0, 63, 0, nullptr},
};