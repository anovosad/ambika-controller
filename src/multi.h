#pragma once

#define NUMBER_OF_VOICES 6
#define NUMBER_OF_KNOBS 8

#pragma pack(push, 1)

struct KnobAssignment {
    uint8_t part;
    uint8_t parameter;
    uint8_t instance;
};

struct PartMapping {
    uint8_t midi_channel;
    uint8_t keyrange_low;
    uint8_t keyrange_high;
    uint8_t voice_allocation;
};

struct MultiData {
    // Offset: 0-24
    PartMapping part_mapping_[NUMBER_OF_VOICES];

    // Offset: 24-28
    uint8_t clock_bpm;
    uint8_t clock_groove_template;
    uint8_t clock_groove_amount;
    uint8_t clock_release;

    // Offset: 28-52
    KnobAssignment knob_assignment[NUMBER_OF_KNOBS];

    // Offset: 52-56
    uint8_t padding2[4];
};

#pragma pack(pop)
