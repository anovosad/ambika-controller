#pragma once

#include "pins.h"
#include "mcp.h"

struct LED {
  Adafruit_MCP23X17 *mcp;
  uint8_t pin;
};

const std::vector<LED> leds_lfo1_trg = {
  LED{&mcp[MCP1], PIN_LED_LFO1_FREE},
  LED{&mcp[MCP1], PIN_LED_LFO1_EL},
  LED{&mcp[MCP1], PIN_LED_LFO1_LE},
};

const std::vector<LED> leds_lfo2_trg = {
  LED{&mcp[MCP1], PIN_LED_LFO2_FREE},
  LED{&mcp[MCP1], PIN_LED_LFO2_EL},
  LED{&mcp[MCP1], PIN_LED_LFO2_LE},
};

const std::vector<LED> leds_lfo3_trg = {
  LED{&mcp[MCP3], PIN_LED_LFO3_FREE},
  LED{&mcp[MCP1], PIN_LED_LFO3_EL},
  LED{&mcp[MCP1], PIN_LED_LFO3_LE},
};

const std::vector<LED> leds_vcf_mode = {
  LED{&mcp[MCP3], PIN_LED_VCF_LP},
  LED{&mcp[MCP3], PIN_LED_VCF_BP},
  LED{&mcp[MCP3], PIN_LED_VCF_HP},
};

const std::vector<LED> leds_poly_mode = {
  LED{&mcp[MCP3], PIN_LED_MODE_MONO},
  LED{&mcp[MCP3], PIN_LED_MODE_POLY},
  LED{&mcp[MCP3], PIN_LED_MODE_2X},
  LED{&mcp[MCP3], PIN_LED_MODE_CYCLIC},
  LED{&mcp[MCP3], PIN_LED_MODE_CHAIN},
};

const std::vector<LED> leds_oct = {
  LED{&mcp[MCP3], PIN_LED_OCT_MINUS2},
  LED{&mcp[MCP3], PIN_LED_OCT_MINUS1},
  LED{&mcp[MCP3], PIN_LED_OCT_0},
  LED{&mcp[MCP3], PIN_LED_OCT_PLUS1},
  LED{&mcp[MCP3], PIN_LED_OCT_PLUS2},
};

const std::vector<LED> leds_legato = {
  LED{&mcp[MCP3], PIN_LED_LEGATO},
};