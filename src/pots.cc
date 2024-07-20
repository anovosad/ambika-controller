#include "pots.h"
#include "pins.h"

Pot::Pot(ParameterID paramId, uint8_t dmuxAddr, uint8_t pin)
  : paramId(paramId), dmuxAddr(dmuxAddr), pin(pin) {}

ParameterID Pot::getParamID() const {
  return paramId;
}

uint8_t Pot::getDmuxAddr() const {
  return dmuxAddr;
}

uint8_t Pot::getPin() const {
  return pin;
}

const std::vector<Pot> pots = {
  Pot(PARAM_OSC2_RANGE, 0, PIN_VAL2),
  Pot(PARAM_OSC1_DETUNE, 1, PIN_VAL2),
  Pot(PARAM_OSC1_PARAM, 2, PIN_VAL2),
  Pot(PARAM_OSC2_SHAPE, 3, PIN_VAL2),
  Pot(PARAM_OSC1_SHAPE, 4, PIN_VAL2),
  Pot(PARAM_OSC2_PARAM, 5, PIN_VAL2),
  Pot(PARAM_OSC1_RANGE, 6, PIN_VAL2),
  Pot(PARAM_OSC2_DETUNE, 7, PIN_VAL2),

  Pot(PARAM_MIX_FUZZ, 0, PIN_VAL3),
  Pot(PARAM_MIX_SUB_SHAPE, 1, PIN_VAL3),
  Pot(PARAM_MIX_NOISE, 2, PIN_VAL3),
  Pot(PARAM_MIX_CROSS_OP, 3, PIN_VAL3),
  Pot(PARAM_MIX_OSC1_OSC2, 4, PIN_VAL3),
  Pot(PARAM_MIX_OP_PARAM, 5, PIN_VAL3),
  Pot(PARAM_MIX_SUB_VOLUME, 6, PIN_VAL3),
  Pot(PARAM_MIX_CRUSH, 7, PIN_VAL3),

  Pot(PARAM_ENV1_SUSTAIN, 0, PIN_VAL1),
  Pot(PARAM_FILTER_LFO2, 1, PIN_VAL1),
  Pot(PARAM_FILTER_RESO, 2, PIN_VAL1),
  Pot(PARAM_ENV1_ATTACK, 3, PIN_VAL1),
  Pot(PARAM_FILTER_FREQ, 4, PIN_VAL1),
  Pot(PARAM_ENV1_DECAY, 5, PIN_VAL1),
  Pot(PARAM_FILTER_ENV2, 6, PIN_VAL1),
  Pot(PARAM_ENV1_RELEASE, 7, PIN_VAL1),

  Pot(PARAM_ENV3_SUSTAIN, 0, PIN_VAL4),
  Pot(PARAM_ENV2_RELEASE, 1, PIN_VAL4),
  Pot(PARAM_ENV2_DECAY, 2, PIN_VAL4),
  Pot(PARAM_ENV3_ATTACK, 3, PIN_VAL4),
  Pot(PARAM_ENV2_ATTACK, 4, PIN_VAL4),
  Pot(PARAM_ENV3_DECAY, 5, PIN_VAL4),
  Pot(PARAM_ENV2_SUSTAIN, 6, PIN_VAL4),
  Pot(PARAM_ENV3_RELEASE, 7, PIN_VAL4),

  Pot(PARAM_LFO4_SHAPE, 0, PIN_VAL5),
  Pot(PARAM_LFO3_RATE, 1, PIN_VAL5),
  Pot(PARAM_LFO1_RATE, 2, PIN_VAL5),
  Pot(PARAM_LFO2_SHAPE, 3, PIN_VAL5),
  Pot(PARAM_LFO1_SHAPE, 4, PIN_VAL5),
  Pot(PARAM_LFO2_RATE, 5, PIN_VAL5),
  Pot(PARAM_LFO3_SHAPE, 6, PIN_VAL5),
  Pot(PARAM_LFO4_RATE, 7, PIN_VAL5),

  Pot(PARAM_TUNING_VOLUME, 2, PIN_VAL6),
  Pot(PARAM_TUNING_RAGA, 4, PIN_VAL6),
  Pot(PARAM_TUNING_SPREAD, 5, PIN_VAL6),
  Pot(PARAM_TUNING_PORTAMENTO, 6, PIN_VAL6),
  Pot(PARAM_TUNING_TUNING, 7, PIN_VAL6),
};