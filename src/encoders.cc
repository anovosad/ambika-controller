#include "encoders.h"

Encoder::Encoder(int id, int mcpIntPin, uint8_t pin1, uint8_t pin2)
  : id(id), mcpIntPin(mcpIntPin), pin1(pin1), pin2(pin2), state(INIT) {
    pin1Mask = 1 << pin1;
    pin2Mask = 1 << pin2;
  }

int Encoder::process(uint16_t captured) {
  bool cw = captured & pin1Mask;
  bool ccw = captured & pin2Mask;

  switch (state) {
    case INIT:
      if (cw && ccw) return 0;
      if (cw) state = CW1;
      if (ccw) state = CCW1;
      break;
    case CW1:
      if (cw && ccw) {
        state = CW2;
      }
      break;
    case CW2:
      if (!cw && !ccw) {
        state = INIT;
        return 1;
      }
      break;
    case CCW1:
      if (cw && ccw) {
        state = CCW2;
      }
      break;
    case CCW2:
      if (!cw && !ccw) {
        state = INIT;
        return -1;
      }
      break;
  }

  return 0;
}

int Encoder::getMcpIntPin() const {
  return mcpIntPin;
}

int Encoder::getId() const {
  return id;
}

std::vector<Encoder> encoders = {
  Encoder(ENC1, PIN_MCP_INT3, PIN_ENC1_CW, PIN_ENC1_CCW),
  Encoder(ENC2, PIN_MCP_INT3, PIN_ENC2_CW, PIN_ENC2_CCW),
  Encoder(ENC3, PIN_MCP_INT3, PIN_ENC3_CW, PIN_ENC3_CCW),
  Encoder(ENC4, PIN_MCP_INT3, PIN_ENC4_CW, PIN_ENC4_CCW),
  Encoder(ENC5, PIN_MCP_INT2, PIN_ENC5_CW, PIN_ENC5_CCW),
};
