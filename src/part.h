#pragma once

static const uint8_t kNumSequences = 3;          

#pragma pack(push, 1)

struct PartData {
  // Offset: 0
  uint8_t volume;

  // Offset: 1-5
  int8_t octave;
  int8_t tuning;
  uint8_t spread;
  uint8_t raga;

  // Offset: 5-8
  uint8_t legato;
  uint8_t portamento_time;
  uint8_t arp_sequencer_mode;

  // Offset: 8-12
  uint8_t arp_direction;
  uint8_t arp_octave;
  uint8_t arp_pattern;
  uint8_t arp_divider;

  // Offset: 12..16
  uint8_t sequence_length[kNumSequences];
  uint8_t polyphony_mode;

  // Offset: 16-80
  //  0..15: step sequence 1
  // 16..31: step sequence 2
  // 32..63: (note value | 0x80 if gate), (note velocity | 0x80 if legato)
  uint8_t sequence_data[64];
  
  // Offset: 80-84
  uint8_t padding[4];  
};    

#pragma pack(pop)