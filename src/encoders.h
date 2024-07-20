#pragma once

#include "parameters.h"
#include "pins.h"
#include <vector>
#include <Adafruit_MCP23X17.h>

#define ENC1 0
#define ENC2 1
#define ENC3 2
#define ENC4 3
#define ENC5 4

class Encoder {
public:
  enum State {
    INIT = 0,
    CW1,
    CW2,
    CCW1,
    CCW2,
  };

  Encoder(int id, int mcpIntPin, uint8_t pin1, uint8_t pin2);

  int process(uint16_t captured);
  int getMcpIntPin() const;
  int getId() const;

private:
  int id;
  int mcpIntPin;
  uint8_t pin1;
  uint8_t pin2;
  uint16_t pin1Mask;
  uint16_t pin2Mask;
  State state;
};

extern std::vector<Encoder> encoders;
