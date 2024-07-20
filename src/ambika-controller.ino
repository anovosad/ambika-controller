#include <SD.h>
#include <FS.h>
#include <SPI.h>
#include <MCP23017.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <TaskScheduler.h>
#include <list>
#include <cstdio>

#include "parameters.h"
#include "pins.h"
#include "pots.h"
#include "pages.h"
#include "multi.h"
#include "mcp.h"
#include "midi.h"
#include "switches.h"
#include "encoders.h"


void ioCallback();
void sdCallback();
void displayCallback();
void mcpCallback();
void midiCallback();

Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &SPI, 48, 47, 35);

Task io_task(5, TASK_FOREVER, &ioCallback);
Task display_task(50, TASK_FOREVER, &displayCallback);
Task sd_task(1000, TASK_ONCE, &sdCallback);
Task mcp_task(1, TASK_FOREVER, &mcpCallback);
Task midi_task(1, TASK_FOREVER, &midiCallback);

Scheduler runner;
uint8_t dmuxAddr = 0;

void setDmuxAddr(uint8_t addr) {
  digitalWrite(PIN_ADDR_A, (addr >> 0) & 1);
  digitalWrite(PIN_ADDR_B, (addr >> 1) & 1);
  digitalWrite(PIN_ADDR_C, (addr >> 2) & 1);
}

void ioCallback() {
  unsigned long start = micros();
  float weight = 0.5;

  for (int i = 0; i < 45; i++) {
    if (pots[i].getDmuxAddr() != dmuxAddr) {
      continue;
    }

    uint8_t pin = pots[i].getPin();
    ParameterID paramID = pots[i].getParamID();

    // read multiple times and calculate avg value
    float raw_value = 0.0;
    int num_reads = 5;

    for (int j = 0; j < num_reads; j++) {
      raw_value += analogRead(pin);
    }

    raw_value = 4095.0 - (raw_value / num_reads);

    float diff = abs(params[paramID].getRawValue() - raw_value);

    if (diff >= 8.0) {
      weight = diff > 16.0 ? 0.9 : 0.5;
      float filtered_value = weight * raw_value + (1.0 - weight) * params[paramID].getRawValue();
      params[paramID].setRawValue(filtered_value);
    }
    
  }

  dmuxAddr = (dmuxAddr + 1) % 8;
  setDmuxAddr(dmuxAddr);
}

void displayCallback() {
    getActivePage().draw(display);
}

void sdCallback() {

  unsigned long start = millis();

  listDir(SD, "/", 0);

  Serial.println(millis() - start);
}

void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.path(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void switchesPress(int mcpId, uint16_t captured) {
  for (auto &it : switches) {
      if (it->getMcpId() == mcpId) {
        it->press(captured);
      }
    }
}

void encodersRotate(int mcpIntPin, uint16_t captured) {
  for (auto &it : encoders) {
      if (it.getMcpIntPin() == mcpIntPin) {
        int result = it.process(captured);
        getActivePage().encoderEvent(it.getId(), result);
      }
    }
}

void mcpCallback() {
  if (!digitalRead(PIN_MCP_INT1)) {
    uint16_t captured = ~mcp[MCP1].getCapturedInterrupt();
    switchesPress(MCP1, captured);
    mcp[MCP1].clearInterrupts();
  } else {
    switchesPress(MCP1, 0);
  }

  if (!digitalRead(PIN_MCP_INT2)) {
    uint16_t captured = ~mcp[MCP2].getCapturedInterrupt();
    switchesPress(MCP2, captured);
    encodersRotate(PIN_MCP_INT2, captured);
    mcp[MCP2].clearInterrupts();
  } else {
    switchesPress(MCP2, 0);
    encodersRotate(PIN_MCP_INT2, 0);
  }

  if (!digitalRead(PIN_MCP_INT3)) {
    uint16_t captured = ~mcp[MCP2].getCapturedInterrupt();
    encodersRotate(PIN_MCP_INT3, captured);
    mcp[MCP2].clearInterrupts();
  } else {
    encodersRotate(PIN_MCP_INT3, 0);
  }
}

void midiCallback() {
  MIDI.read();

  for (int i = 0; i < PARAM_END; i++) {
    if (params[i].isDirty()) {
      if (params[i].getCC() > 0) {
        MIDI.sendControlChange(params[i].getCC(), params[i].getCCValue(), 1);
      } else if (params[i].getNRPN() > 0) {
        MIDI.beginNrpn(params[i].getNRPN(), 1);
        int8_t value = params[i].getNRPNValue();

        MIDI.sendNrpnValue((value >= 0) ? 0 : 0xff, value, 1);
      }

      auto it = std::find(changedParams.begin(), changedParams.end(), static_cast<ParameterID>(i));
      if (it != changedParams.end()) {
        changedParams.erase(it);
      }

      changedParams.push_front(static_cast<ParameterID>(i));
      while (changedParams.size() > 6) {
        changedParams.pop_back();
      }
      
      params[i].removeDirty();
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(PIN_ADDR_A, OUTPUT);
  pinMode(PIN_ADDR_B, OUTPUT);
  pinMode(PIN_ADDR_C, OUTPUT);

  if (!display.begin()) {
    Serial.println(F("SSD1306 allocation failed"));
  }

  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);

  if (!SD.begin(9, SPI, 1000000L)) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);



  MIDI.begin(1);

  pinMode(PIN_MCP_INT1, INPUT);
  pinMode(PIN_MCP_INT2, INPUT);
  pinMode(PIN_MCP_INT3, INPUT);

  initMCP();
  initSwitches();

  // scheduler
  runner.init();

  runner.addTask(display_task);
  runner.addTask(io_task);
  runner.addTask(sd_task);
  runner.addTask(mcp_task);
  runner.addTask(midi_task);

  display_task.enable();
  io_task.enable();
  // sd_task.enable();
  mcp_task.enable();
  midi_task.enable();

  analogReadResolution(12);
}

void loop() {
  runner.execute();
}
