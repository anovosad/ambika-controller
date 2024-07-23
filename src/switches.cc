#pragma once

#include "switches.h"
#include "pages.h"
#include "mcp.h"
#include "midi.h"
#include "sd.h"

/*****************************************************************************/

Switch::Switch(int mcpId, uint8_t pin) : 
  mcpId(mcpId), pin(pin), isPressed(false) {
  pinMask = 1 << pin;
}

void Switch::press(uint16_t captured) {}

int Switch::getMcpId() const {
  return mcpId;
}

/*****************************************************************************/

LEDSwitch::LEDSwitch(ParameterID paramId, int mcpId, uint8_t pin, const std::vector<LED> &leds)
  : Switch(mcpId, pin), paramId(paramId), leds(leds) {
    for (auto &it : leds) {
      it.mcp->digitalWrite(it.pin, LOW);
    }

    uint8_t value = getValueFromParam();
    leds[value].mcp->digitalWrite(leds[value].pin, HIGH);
}

void LEDSwitch::press(uint16_t captured) {
  bool pressed = captured & pinMask;
  if (pressed && isPressed) return;

  if (!pressed) {
    isPressed = false;
    return;
  }

  if (pressed && !isPressed) {
    isPressed = true;

    if (leds.size() == 1) {
      int8_t oldValue = params[paramId].getCCValue();
      if (oldValue < 63) {
        params[paramId].setRawValue(4095.0);
        leds[0].mcp->digitalWrite(leds[0].pin, HIGH);
      } else {
        params[paramId].setRawValue(0.0);
        leds[0].mcp->digitalWrite(leds[0].pin, LOW);
      }

      return;
    }

    uint8_t oldValue = getValueFromParam();
    uint8_t newValue = (oldValue + 1) % leds.size();
    float boundary = 4095.0 / leds.size();
    params[paramId].setRawValue((boundary / 2) + (boundary * newValue));

    leds[oldValue].mcp->digitalWrite(leds[oldValue].pin, LOW);
    leds[newValue].mcp->digitalWrite(leds[newValue].pin, HIGH);
  }
}

uint8_t LEDSwitch::getValueFromParam() {
    float boundary = 127.0 / leds.size();
    int8_t value = floor(params[paramId].getCCValue() / boundary);
    return min((int8_t) leds.size(), value);
  }

/*****************************************************************************/

UISwitch::UISwitch(int mcpId, uint8_t pin) : Switch(mcpId, pin) {}

void UISwitch::press(uint16_t captured) {
  bool pressed = captured & pinMask;

  if (pressed && isPressed) return;

  if (!pressed) {
    isPressed = false;
    return;
  }

  if (pressed && !isPressed) {
    isPressed = true;

    switch (pin) {
      case PIN_SW_MAIN_PAGE:
        activePage = PAGE_MAIN;
        break;

      case PIN_SW_EDIT_MATRIX_OP:
        activePage = (activePage == PAGE_EDIT_MATRIX) ? PAGE_EDIT_OP : PAGE_EDIT_MATRIX;
        break;

      case PIN_SW_EDIT_VOICE:
        activePage = PAGE_EDIT_VOICE;
        break;

      case PIN_SW_LOAD_PATCH:
        pageLoadPatch.encoderEvent(0, 0);
        activePage = PAGE_LOAD_PATCH;
        break;

      case PIN_SW_SAVE_PATCH:
        break;

      case PIN_SW_SAVE:
        if (activePage == PAGE_EDIT_VOICE) {
          pageVoice.sendMultiDataToAmbika(MIDI);
        } else if (activePage == PAGE_LOAD_PATCH) {
        }
        break;

      case PIN_SW_ENC5:
        if (activePage == PAGE_EDIT_VOICE) {
          pageVoice.pressEnc();
        } else if (activePage == PAGE_LOAD_PATCH) {
          pageLoadPatch.encoderEvent(0, 0);
        }
        break;
    }
  }
}

/*****************************************************************************/

std::vector<std::shared_ptr<Switch> > switches;

void initSwitches() {
  switches.emplace_back(new LEDSwitch(PARAM_LFO1_TRIGGER, MCP1, PIN_SW_LFO1_TRG, leds_lfo1_trg));
  switches.emplace_back(new LEDSwitch(PARAM_LFO2_TRIGGER, MCP1, PIN_SW_LFO2_TRG, leds_lfo2_trg));
  switches.emplace_back(new LEDSwitch(PARAM_LFO3_TRIGGER, MCP1, PIN_SW_LFO3_TRG, leds_lfo3_trg));
  switches.emplace_back(new LEDSwitch(PARAM_FILTER_MODE, MCP2, PIN_SW_VCF_MODE, leds_vcf_mode));
  switches.emplace_back(new LEDSwitch(PARAM_TUNING_POLY, MCP2, PIN_SW_VOICE_POLY, leds_poly_mode));
  switches.emplace_back(new LEDSwitch(PARAM_TUNING_OCTAVE, MCP2, PIN_SW_VOICE_OCT, leds_oct));
  switches.emplace_back(new LEDSwitch(PARAM_TUNING_LEGATO, MCP2, PIN_SW_VOICE_LEGATO, leds_legato));
  switches.emplace_back(new UISwitch(MCP1, PIN_SW_MAIN_PAGE));
  switches.emplace_back(new UISwitch(MCP1, PIN_SW_EDIT_MATRIX_OP));
  switches.emplace_back(new UISwitch(MCP1, PIN_SW_EDIT_VOICE));
  switches.emplace_back(new UISwitch(MCP1, PIN_SW_LOAD_PATCH));
  switches.emplace_back(new UISwitch(MCP1, PIN_SW_SAVE_PATCH));
  switches.emplace_back(new UISwitch(MCP2, PIN_SW_SAVE));
  switches.emplace_back(new UISwitch(MCP2, PIN_SW_ENC5));
}

  
