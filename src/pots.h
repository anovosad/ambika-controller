#pragma once

#include <vector>
#include "parameters.h"

class Pot {
public:
  Pot(ParameterID paramId, uint8_t dmuxAddr, uint8_t pin);
  ParameterID getParamID() const;
  uint8_t getDmuxAddr() const;
  uint8_t getPin() const;

private:
  ParameterID paramId;
  uint8_t dmuxAddr;
  uint8_t pin;
};

extern const std::vector<Pot> pots;