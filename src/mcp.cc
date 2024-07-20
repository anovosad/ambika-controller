#include "mcp.h"
#include "pins.h"
#include <Arduino.h>

Adafruit_MCP23X17 mcp[3];

void initMCP() {
  if (!mcp[MCP1].begin_SPI(PIN_MCP1_CS)) {
    Serial.println("Error initializing MCP1");
    while (1);
  }

  if (!mcp[MCP2].begin_SPI(PIN_MCP2_CS)) {
    Serial.println("Error initializing MCP2");
    while (1);
  }

  if (!mcp[MCP3].begin_SPI(PIN_MCP3_CS)) {
    Serial.println("Error initializing MCP3");
    while (1);
  }

  mcp[MCP1].pinMode(PIN_LED_LFO3_EL, OUTPUT);
  mcp[MCP1].pinMode(PIN_LED_LFO3_LE, OUTPUT);
  mcp[MCP1].pinMode(PIN_LED_LFO1_FREE, OUTPUT);
  mcp[MCP1].pinMode(PIN_LED_LFO1_EL, OUTPUT);
  mcp[MCP1].pinMode(PIN_LED_LFO1_LE, OUTPUT);
  mcp[MCP1].pinMode(PIN_LED_LFO2_FREE, OUTPUT);
  mcp[MCP1].pinMode(PIN_LED_LFO2_EL, OUTPUT);
  mcp[MCP1].pinMode(PIN_LED_LFO2_LE, OUTPUT);
  mcp[MCP1].pinMode(PIN_SW_LFO2_TRG, INPUT_PULLUP);
  mcp[MCP1].pinMode(PIN_SW_EDIT_MATRIX, INPUT_PULLUP);
  mcp[MCP1].pinMode(PIN_SW_EDIT_OP, INPUT_PULLUP);
  mcp[MCP1].pinMode(PIN_SW_EDIT_VOICE, INPUT_PULLUP);
  mcp[MCP1].pinMode(PIN_SW_SAVE_PATCH, INPUT_PULLUP);
  mcp[MCP1].pinMode(PIN_SW_LOAD_PATCH, INPUT_PULLUP);
  mcp[MCP1].pinMode(PIN_SW_LFO3_TRG, INPUT_PULLUP);
  mcp[MCP1].pinMode(PIN_SW_LFO1_TRG, INPUT_PULLUP);

  mcp[MCP1].setupInterrupts(false, false, LOW);
  mcp[MCP1].setupInterruptPin(PIN_SW_LFO2_TRG, LOW);
  mcp[MCP1].setupInterruptPin(PIN_SW_EDIT_MATRIX, LOW);
  mcp[MCP1].setupInterruptPin(PIN_SW_EDIT_OP, LOW);
  mcp[MCP1].setupInterruptPin(PIN_SW_EDIT_VOICE, LOW);
  mcp[MCP1].setupInterruptPin(PIN_SW_SAVE_PATCH, LOW);
  mcp[MCP1].setupInterruptPin(PIN_SW_LOAD_PATCH, LOW);
  mcp[MCP1].setupInterruptPin(PIN_SW_LFO3_TRG, LOW);
  mcp[MCP1].setupInterruptPin(PIN_SW_LFO1_TRG, LOW);

  mcp[MCP2].pinMode(PIN_SW_VCF_MODE, INPUT_PULLUP);
  mcp[MCP2].pinMode(PIN_SW_SAVE, INPUT_PULLUP);
  mcp[MCP2].pinMode(PIN_SW_VOICE_POLY, INPUT_PULLUP);
  mcp[MCP2].pinMode(PIN_SW_VOICE_OCT, INPUT_PULLUP);
  mcp[MCP2].pinMode(PIN_SW_VOICE_LEGATO, INPUT_PULLUP);
  mcp[MCP2].pinMode(PIN_ENC1_CW, INPUT_PULLUP);
  mcp[MCP2].pinMode(PIN_ENC1_CCW, INPUT_PULLUP);
  mcp[MCP2].pinMode(PIN_ENC2_CW, INPUT_PULLUP);
  mcp[MCP2].pinMode(PIN_ENC2_CCW, INPUT_PULLUP);
  mcp[MCP2].pinMode(PIN_ENC3_CW, INPUT_PULLUP);
  mcp[MCP2].pinMode(PIN_ENC3_CCW, INPUT_PULLUP);
  mcp[MCP2].pinMode(PIN_ENC4_CW, INPUT_PULLUP);
  mcp[MCP2].pinMode(PIN_ENC4_CCW, INPUT_PULLUP);
  mcp[MCP2].pinMode(PIN_ENC5_CW, INPUT_PULLUP);
  mcp[MCP2].pinMode(PIN_ENC5_CCW, INPUT_PULLUP);
  mcp[MCP2].pinMode(PIN_ENC5_BTN, INPUT_PULLUP);

  mcp[MCP2].setupInterrupts(false, false, LOW);
  mcp[MCP2].setupInterruptPin(PIN_SW_VCF_MODE, LOW);
  mcp[MCP2].setupInterruptPin(PIN_SW_SAVE, LOW);
  mcp[MCP2].setupInterruptPin(PIN_SW_VOICE_POLY, LOW);
  mcp[MCP2].setupInterruptPin(PIN_SW_VOICE_OCT, LOW);
  mcp[MCP2].setupInterruptPin(PIN_SW_VOICE_LEGATO, LOW);
  mcp[MCP2].setupInterruptPin(PIN_ENC1_CW, LOW);
  mcp[MCP2].setupInterruptPin(PIN_ENC1_CCW, LOW);
  mcp[MCP2].setupInterruptPin(PIN_ENC2_CW, LOW);
  mcp[MCP2].setupInterruptPin(PIN_ENC2_CCW, LOW);
  mcp[MCP2].setupInterruptPin(PIN_ENC3_CW, LOW);
  mcp[MCP2].setupInterruptPin(PIN_ENC3_CCW, LOW);
  mcp[MCP2].setupInterruptPin(PIN_ENC4_CW, LOW);
  mcp[MCP2].setupInterruptPin(PIN_ENC4_CCW, LOW);
  mcp[MCP2].setupInterruptPin(PIN_ENC5_CW, LOW);
  mcp[MCP2].setupInterruptPin(PIN_ENC5_CCW, LOW);
  mcp[MCP2].setupInterruptPin(PIN_ENC5_BTN, LOW);

  mcp[MCP3].pinMode(PIN_LED_VCF_LP, OUTPUT);
  mcp[MCP3].pinMode(PIN_LED_VCF_BP, OUTPUT);
  mcp[MCP3].pinMode(PIN_LED_VCF_HP, OUTPUT);
  mcp[MCP3].pinMode(PIN_LED_MODE_MONO, OUTPUT);
  mcp[MCP3].pinMode(PIN_LED_MODE_POLY, OUTPUT);
  mcp[MCP3].pinMode(PIN_LED_MODE_2X, OUTPUT);
  mcp[MCP3].pinMode(PIN_LED_MODE_CYCLIC, OUTPUT);
  mcp[MCP3].pinMode(PIN_LED_MODE_CHAIN, OUTPUT);
  mcp[MCP3].pinMode(PIN_LED_LFO3_FREE, OUTPUT);
  mcp[MCP3].pinMode(PIN_LED_OCT_PLUS2, OUTPUT);
  mcp[MCP3].pinMode(PIN_LED_OCT_PLUS1, OUTPUT);
  mcp[MCP3].pinMode(PIN_LED_OCT_0, OUTPUT);
  mcp[MCP3].pinMode(PIN_LED_OCT_MINUS1, OUTPUT);
  mcp[MCP3].pinMode(PIN_LED_OCT_MINUS2, OUTPUT);
  mcp[MCP3].pinMode(PIN_LED_LEGATO, OUTPUT);

  mcp[MCP1].digitalWrite(PIN_LED_LFO3_EL, LOW);
  mcp[MCP1].digitalWrite(PIN_LED_LFO3_LE, LOW);
  mcp[MCP1].digitalWrite(PIN_LED_LFO1_FREE, LOW);
  mcp[MCP1].digitalWrite(PIN_LED_LFO1_EL, LOW);
  mcp[MCP1].digitalWrite(PIN_LED_LFO1_LE, LOW);
  mcp[MCP1].digitalWrite(PIN_LED_LFO2_FREE, LOW);
  mcp[MCP1].digitalWrite(PIN_LED_LFO2_EL, LOW);
  mcp[MCP1].digitalWrite(PIN_LED_LFO2_LE, LOW);

  mcp[MCP3].digitalWrite(PIN_LED_VCF_LP, LOW);
  mcp[MCP3].digitalWrite(PIN_LED_VCF_BP, LOW);
  mcp[MCP3].digitalWrite(PIN_LED_VCF_HP, LOW);
  mcp[MCP3].digitalWrite(PIN_LED_MODE_MONO, LOW);
  mcp[MCP3].digitalWrite(PIN_LED_MODE_POLY, LOW);
  mcp[MCP3].digitalWrite(PIN_LED_MODE_2X, LOW);
  mcp[MCP3].digitalWrite(PIN_LED_MODE_CYCLIC, LOW);
  mcp[MCP3].digitalWrite(PIN_LED_MODE_CHAIN, LOW);
  mcp[MCP3].digitalWrite(PIN_LED_LFO3_FREE, LOW);
  mcp[MCP3].digitalWrite(PIN_LED_OCT_PLUS2, LOW);
  mcp[MCP3].digitalWrite(PIN_LED_OCT_PLUS1, LOW);
  mcp[MCP3].digitalWrite(PIN_LED_OCT_0, LOW);
  mcp[MCP3].digitalWrite(PIN_LED_OCT_MINUS1, LOW);
  mcp[MCP3].digitalWrite(PIN_LED_OCT_MINUS2, LOW);
  mcp[MCP3].digitalWrite(PIN_LED_LEGATO, LOW);
}
