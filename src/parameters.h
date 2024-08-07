#pragma once

#include <cstring>
#include <vector>
#include <Arduino.h>

class Parameter {
public:
  Parameter(const char *fullName, uint8_t cc, uint8_t nrpn, int8_t min, int8_t max, int8_t defaultValue, const char **valConverter);

  bool isDirty();
  void removeDirty();
  unsigned long getLastChanged();

  const char * getFullName();
  const char * getName();

  String getDisplayValue();

  // NRPN value is in range <min, max>
  int16_t getNRPNValue();
  void setNRPNValue(int16_t value);

  // normalized value is in range <0, max-min>
  int16_t getNormalizedValue();

  // value in range <0, 127>
  int8_t getCCValue();

  // raw value is in range <0.0, 4095.0>
  float getRawValue();
  void setRawValue(float value);

  uint8_t getCC();
  uint8_t getNRPN();

private:
  const char *fullName;
  const char *name;
  uint8_t cc;
  uint8_t nrpn;

  int8_t min;
  int8_t max;
  int8_t defaultValue;
  float rawValue;
  bool dirty;
  unsigned long lastChanged;

  const char **valConverter;
};

enum ParameterID {
  PARAM_OSC1_SHAPE = 0,
  PARAM_OSC1_PARAM,
  PARAM_OSC1_RANGE,
  PARAM_OSC1_DETUNE,

  PARAM_OSC2_SHAPE,
  PARAM_OSC2_PARAM,
  PARAM_OSC2_RANGE,
  PARAM_OSC2_DETUNE,

  PARAM_MIX_OSC1_OSC2,
  PARAM_MIX_CROSS_OP,
  PARAM_MIX_OP_PARAM,
  PARAM_MIX_SUB_SHAPE,
  PARAM_MIX_SUB_VOLUME,
  PARAM_MIX_NOISE,
  PARAM_MIX_FUZZ,
  PARAM_MIX_CRUSH,

  PARAM_FILTER_FREQ,
  PARAM_FILTER_RESO,
  PARAM_FILTER_MODE,
  PARAM_FILTER_ENV2,
  PARAM_FILTER_LFO2,
  PARAM_FILTER_VELOCITY,
  PARAM_FILTER_KEYBOARD,

  PARAM_ENV1_ATTACK,
  PARAM_ENV1_DECAY,
  PARAM_ENV1_SUSTAIN,
  PARAM_ENV1_RELEASE,

  PARAM_ENV2_ATTACK,
  PARAM_ENV2_DECAY,
  PARAM_ENV2_SUSTAIN,
  PARAM_ENV2_RELEASE,

  PARAM_ENV3_ATTACK,
  PARAM_ENV3_DECAY,
  PARAM_ENV3_SUSTAIN,
  PARAM_ENV3_RELEASE,

  PARAM_MTX1_SOURCE,
  PARAM_MTX1_DEST,
  PARAM_MTX1_AMOUNT,

  PARAM_MTX2_SOURCE,
  PARAM_MTX2_DEST,
  PARAM_MTX2_AMOUNT,

  PARAM_MTX3_SOURCE,
  PARAM_MTX3_DEST,
  PARAM_MTX3_AMOUNT,

  PARAM_MTX4_SOURCE,
  PARAM_MTX4_DEST,
  PARAM_MTX4_AMOUNT,

  PARAM_MTX5_SOURCE,
  PARAM_MTX5_DEST,
  PARAM_MTX5_AMOUNT,

  PARAM_MTX6_SOURCE,
  PARAM_MTX6_DEST,
  PARAM_MTX6_AMOUNT,

  PARAM_MTX7_SOURCE,
  PARAM_MTX7_DEST,
  PARAM_MTX7_AMOUNT,

  PARAM_MTX8_SOURCE,
  PARAM_MTX8_DEST,
  PARAM_MTX8_AMOUNT,

  PARAM_MTX9_SOURCE,
  PARAM_MTX9_DEST,
  PARAM_MTX9_AMOUNT,

  PARAM_MTX10_SOURCE,
  PARAM_MTX10_DEST,
  PARAM_MTX10_AMOUNT,

  PARAM_MTX11_SOURCE,
  PARAM_MTX11_DEST,
  PARAM_MTX11_AMOUNT,

  PARAM_MTX12_SOURCE,
  PARAM_MTX12_DEST,
  PARAM_MTX12_AMOUNT,

  PARAM_MTX13_SOURCE,
  PARAM_MTX13_DEST,
  PARAM_MTX13_AMOUNT,

  PARAM_MTX14_SOURCE,
  PARAM_MTX14_DEST,
  PARAM_MTX14_AMOUNT,

  PARAM_OP1_SOURCE1,
  PARAM_OP1_SOURCE2,
  PARAM_OP1_OPERATION,

  PARAM_OP2_SOURCE1,
  PARAM_OP2_SOURCE2,
  PARAM_OP2_OPERATION,

  PARAM_OP3_SOURCE1,
  PARAM_OP3_SOURCE2,
  PARAM_OP3_OPERATION,

  PARAM_OP4_SOURCE1,
  PARAM_OP4_SOURCE2,
  PARAM_OP4_OPERATION,

  PARAM_LFO1_SHAPE,
  PARAM_LFO1_RATE,
  PARAM_LFO1_TRIGGER,

  PARAM_LFO2_SHAPE,
  PARAM_LFO2_RATE,
  PARAM_LFO2_TRIGGER,

  PARAM_LFO3_SHAPE,
  PARAM_LFO3_RATE,
  PARAM_LFO3_TRIGGER,

  PARAM_LFO4_SHAPE,
  PARAM_LFO4_RATE,

  PARAM_TUNING_VOLUME,
  PARAM_TUNING_OCTAVE,
  PARAM_TUNING_TUNING,
  PARAM_TUNING_SPREAD,
  PARAM_TUNING_RAGA,
  PARAM_TUNING_LEGATO,
  PARAM_TUNING_PORTAMENTO,
  PARAM_TUNING_POLY,

  PARAM_PART1_CHANNEL,
  PARAM_PART1_LOW_KEY,
  PARAM_PART1_HIGH_KEY,
  PARAM_PART1_VOICES,

  PARAM_PART2_CHANNEL,
  PARAM_PART2_LOW_KEY,
  PARAM_PART2_HIGH_KEY,
  PARAM_PART2_VOICES,

  PARAM_PART3_CHANNEL,
  PARAM_PART3_LOW_KEY,
  PARAM_PART3_HIGH_KEY,
  PARAM_PART3_VOICES,

  PARAM_PART4_CHANNEL,
  PARAM_PART4_LOW_KEY,
  PARAM_PART4_HIGH_KEY,
  PARAM_PART4_VOICES,

  PARAM_PART5_CHANNEL,
  PARAM_PART5_LOW_KEY,
  PARAM_PART5_HIGH_KEY,
  PARAM_PART5_VOICES,

  PARAM_PART6_CHANNEL,
  PARAM_PART6_LOW_KEY,
  PARAM_PART6_HIGH_KEY,
  PARAM_PART6_VOICES,

  PARAM_LOAD_PATCH_BANK,
  PARAM_LOAD_PATCH_INDEX,
  PARAM_LOAD_PATCH_PART,
  PARAM_LOAD_PATCH_TYPE,

  PARAM_END,
};  

enum PatchType {
  PATCH_TYPE_PROGRAM,
  PATCH_TYPE_MULTI,
  PATCH_TYPE_PATCH,
  PATCH_TYPE_SEQUENCE,
};

extern std::vector<Parameter> params; 
