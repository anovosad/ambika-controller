#pragma once

#include "parameters.h"
#include "pins.h"
#include "leds.h"
#include <memory>
#include <vector>
#include <Adafruit_MCP23X17.h>

/*****************************************************************************/

class Switch {
public:
  Switch(int mcpId, uint8_t pin);
  virtual void press(uint16_t captured);
  virtual int getMcpId() const;

protected:
  int mcpId;
  uint8_t pin;
  uint16_t pinMask;
  bool isPressed;
};

/*****************************************************************************/

class LEDSwitch : public Switch {
public:
  LEDSwitch(ParameterID paramId, int mcpId, uint8_t pin, const std::vector<LED> &leds);

  void press(uint16_t captured);

protected:
  uint8_t getValueFromParam();
  ParameterID paramId;
  const std::vector<LED> &leds;
};

/*****************************************************************************/

class UISwitch : public Switch {
public:
  UISwitch(int mcpId, uint8_t pin);

  void press(uint16_t captured);
};

/*****************************************************************************/

void initSwitches();
extern std::vector<std::shared_ptr<Switch> > switches;
