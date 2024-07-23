#pragma once

#pragma pack(push, 1)

struct OscillatorSettings {
  uint8_t shape;
  uint8_t parameter;
  int8_t range;
  int8_t detune;
};

struct FilterSettings {
  uint8_t cutoff;
  uint8_t resonance;
  uint8_t mode;
};

struct EnvelopeLfoSettings {
  uint8_t attack;
  uint8_t decay;
  uint8_t sustain;
  uint8_t release;
  uint8_t shape;
  uint8_t rate;
  uint8_t padding;
  uint8_t retrigger_mode;
};

struct Modulation {
  uint8_t source;
  uint8_t destination;
  int8_t amount;
};

struct Modifier {
  uint8_t operands[2];
  uint8_t op;
};

static const uint8_t kNumOscillators = 2;
static const uint8_t kNumEnvelopes = 3;
static const uint8_t kNumModulations = 14;
static const uint8_t kNumModifiers = 4;

struct Patch {
  // Offset: 0-8
  OscillatorSettings osc[kNumOscillators];
  
  // Offset: 8-16
  uint8_t mix_balance;
  uint8_t mix_op;
  uint8_t mix_parameter;
  uint8_t mix_sub_osc_shape;
  uint8_t mix_sub_osc;
  uint8_t mix_noise;
  uint8_t mix_fuzz;
  uint8_t mix_crush;

  // Offset: 16-24
  FilterSettings filter[2];
  int8_t filter_env;
  int8_t filter_lfo;
  
  // Offset: 24-48
  EnvelopeLfoSettings env_lfo[kNumEnvelopes];

  // Offset: 48-50
  uint8_t voice_lfo_shape;
  uint8_t voice_lfo_rate;

  // Offset: 50-92
  Modulation modulation[kNumModulations];
  
  // Offset: 92-104
  Modifier modifier[kNumModifiers];
  
  // Offset: 104-112
  uint8_t padding[8];
};

#pragma pack(pop)

