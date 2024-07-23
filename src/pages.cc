#include "pages.h"
#include "pins.h"
#include "multi.h"
#include "sd.h"
#include <cstdio>

/*****************************************************************************/

void Page::encoderEvent(int encoder, int delta, bool pressed) {}
void Page::draw(Adafruit_SH1106G &display) {}
void Page::switchPressed() {}

/*****************************************************************************/

PageMatrix::PageMatrix(const std::vector<std::vector<ParameterID>> &m) {
  activeLine = 0;
  firstDisplayedLine = 0;
  lines = m;
  displayedLines = std::min(5, static_cast<int>(lines.size()));
}

void PageMatrix::encoderEvent(int encoder, int delta, bool pressed) {
  switch (encoder) {
    case 0:
      activeLine = max(min(activeLine - delta, static_cast<int>(lines.size()) - 1), 0);

      if (activeLine > firstDisplayedLine + displayedLines - 1) {
        firstDisplayedLine = activeLine - displayedLines + 1;
      } else if (activeLine < firstDisplayedLine) {
        firstDisplayedLine = activeLine;
      }
      break;

    case 1:
    case 2:
    case 3:
      Parameter &p = params[lines[activeLine][encoder - 1]];
      p.setNRPNValue(p.getNRPNValue() - delta);      
      break;
  }
}

void PageMatrix::draw(Adafruit_SH1106G &display) {
  char buffer[50];
  display.clearDisplay();

  display.setCursor(0, 0);
  display.print("P1   P2    P3    P4");
  display.setCursor(0, 8);

  display.print("--------------------");

  int y = 16;

  for (int i = 0; i < displayedLines; i++) {
    display.setCursor(0, y);
    int currentLine = i + firstDisplayedLine;

    std::sprintf(
      buffer, 
      "%s%-3d %-5s %-5s %-4s\n", 
      (currentLine == activeLine ? "*" : " "), 
      currentLine+1, 
      params[lines[currentLine][0]].getDisplayValue(),
      params[lines[currentLine][1]].getDisplayValue(),
      params[lines[currentLine][2]].getDisplayValue()
      );

    display.print(buffer);
    y += 10;
  }

  display.display();
}

/*****************************************************************************/

PageVoice::PageVoice(const std::vector<std::vector<ParameterID>> &m) {
  activeLine = 0;
  activeVoice = 0;
  firstDisplayedLine = 0;
  lines = m;
  displayedLines = std::min(5, static_cast<int>(lines.size()));
}

void PageVoice::encoderEvent(int encoder, int delta, bool pressed) {
  switch (encoder) {
    case 0:
      activeLine = max(min(activeLine - delta, static_cast<int>(lines.size()) - 1), 0);

      if (activeLine > firstDisplayedLine + displayedLines - 1) {
        firstDisplayedLine = activeLine - displayedLines + 1;
      } else if (activeLine < firstDisplayedLine) {
        firstDisplayedLine = activeLine;
      }
      break;

    case 1:
    case 2:
    case 3:
      {
        Parameter &p = params[lines[activeLine][encoder - 1]];
        p.setNRPNValue(p.getNRPNValue() - delta);  
        break;
      }

    case 4:
      if (pressed) {
        int16_t voiceValue = params[lines[activeLine][3]].getNRPNValue();
        voiceValue = voiceValue ^ ((int16_t) 1 << activeVoice);
        params[lines[activeLine][3]].setNRPNValue(voiceValue);

        // clear same voice in other parts
        for (int i = 0; i < 6; i++) {
          if (i != activeLine) {
            int16_t voiceValue = params[lines[i][3]].getNRPNValue();
            voiceValue = voiceValue & ~((int16_t) 1 << activeVoice);
            params[lines[i][3]].setNRPNValue(voiceValue);
          }
        }
      } else {
        activeVoice = std::max(std::min(activeVoice - delta, 5), 0);
      }
      break;
  }
}

void PageVoice::draw(Adafruit_SH1106G &display) {
  char buffer[50];

  display.clearDisplay();

  display.setCursor(0, 0);
  display.print("P1 P2 P3  P4  P5    ");
  display.setCursor(0, 8);
  display.print("--------------------");

  int y = 16;

  for (int i = 0; i < displayedLines; i++) {
    display.setCursor(0, y);
    int currentLine = i + firstDisplayedLine;

    std::sprintf(
      buffer, 
      "%s%-2d%-3s %-3s %-2s ", 
      (currentLine == activeLine ? "*" : " "), 
      currentLine+1, 
      params[lines[currentLine][0]].getDisplayValue(),
      params[lines[currentLine][1]].getDisplayValue(),
      params[lines[currentLine][2]].getDisplayValue()
    );

    display.print(buffer);
    int16_t voiceValue = params[lines[currentLine][3]].getNRPNValue();
    for (int i = 0; i < 6; i++) {
      if (currentLine == activeLine && activeVoice == i) {
        display.print((voiceValue & (1 << i)) ? "X" : "o");
      } else {
        display.print((voiceValue & (1 << i)) ? "x" : ".");
      }
    }

    y += 10;
  }

  display.display();
}

void PageVoice::switchPressed() {
  MultiData multiData;

  for (int i = 0; i < lines.size(); i++) {
    multiData.part_mapping_[i].midi_channel = params[lines[i][2]].getNRPNValue();
    multiData.part_mapping_[i].keyrange_low = params[lines[i][0]].getNRPNValue();
    multiData.part_mapping_[i].keyrange_high = params[lines[i][1]].getNRPNValue();
    multiData.part_mapping_[i].voice_allocation = params[lines[i][3]].getNRPNValue();
  }

  sendSysExDataToAmbika(MIDI, (uint8_t*) &multiData, sizeof(multiData), 0x04, 0x00);
}

/*****************************************************************************/

PageLoadPatch::PageLoadPatch(const std::vector<ParameterID> &m) {
  data = m;
  changed = false;
  loaded = false;
  uploading = false;
}

void PageLoadPatch::encoderEvent(int encoder, int delta, bool pressed) {
  switch (encoder) {
    case 0:
    case 1:
    case 2:
    case 3:
      Parameter &p = params[data[encoder]];
      p.setNRPNValue(p.getNRPNValue() - delta);    

      if (encoder == 2) break;

      changed = true;  
      lastChanged = millis();
      loaded = storage.loadName(
        SD, 
        params[data[1]].getDisplayValue().c_str(), 
        static_cast<uint8_t>(params[data[0]].getNRPNValue()),
        static_cast<uint8_t>(params[data[2]].getNRPNValue()),
        static_cast<uint8_t>(params[data[3]].getNRPNValue()),
        name
      );
      break;
  }
}

void PageLoadPatch::draw(Adafruit_SH1106G &display) {
  if (changed && loaded && millis() - lastChanged > 500) {
    uploading = true;
  }

  display.clearDisplay();

  display.setCursor(0, 0);
  display.print("Load ");
  display.print(params[data[3]].getDisplayValue());

  display.setCursor(0, 10);
  display.print("--------------------");

  display.setCursor(0, 20);
  display.print(params[data[1]].getDisplayValue());

  String id = params[data[0]].getDisplayValue();
  if (id.length() == 1) {
    display.print("00");
  } else if (id.length() == 2) {
    display.print("0");
  }
  display.print(id);

  if (loaded) {
    display.print(" ");
    display.print(name);
  } else {
    display.print(" <EMPTY>");
  }

  display.setCursor(0, 40);
  display.print("Part: ");
  display.print(params[data[2]].getDisplayValue());

  display.setCursor(0, 50);
  if (uploading) {
    display.print("Uploading...");
  }

  display.display();

  if (uploading) {
    // this is time consuming
    storage.loadAndSend(SD, static_cast<uint8_t>(params[data[2]].getNRPNValue()));
    changed = false;
    uploading = false;
  }
}

/*****************************************************************************/

PageMain::PageMain() {}

void PageMain::draw(Adafruit_SH1106G &display) {
  display.clearDisplay();

  unsigned long start = millis();
  display.setCursor(0, 0);
  display.print("Ambika Controller");
  display.setCursor(0, 8);
  display.print("--------------------");

  int y = 16;

  for (auto &it : changedParams) {
    if (start - params[it].getLastChanged() > 2000) {
      continue;
    }

    display.setCursor(0, y);
    display.print(params[it].getFullName());
    display.print(" ");
    display.print(params[it].getDisplayValue());
    y += 10;
  }

  display.display();
}

/*****************************************************************************/

PageMatrix pageMatrix({
  {PARAM_MTX1_SOURCE, PARAM_MTX1_DEST,  PARAM_MTX1_AMOUNT},
  {PARAM_MTX2_SOURCE, PARAM_MTX2_DEST,  PARAM_MTX2_AMOUNT},
  {PARAM_MTX3_SOURCE, PARAM_MTX3_DEST,  PARAM_MTX3_AMOUNT},
  {PARAM_MTX4_SOURCE, PARAM_MTX4_DEST,  PARAM_MTX4_AMOUNT},
  {PARAM_MTX5_SOURCE, PARAM_MTX5_DEST,  PARAM_MTX5_AMOUNT},
  {PARAM_MTX6_SOURCE, PARAM_MTX6_DEST,  PARAM_MTX6_AMOUNT},
  {PARAM_MTX7_SOURCE, PARAM_MTX7_DEST,  PARAM_MTX7_AMOUNT},
  {PARAM_MTX8_SOURCE, PARAM_MTX8_DEST,  PARAM_MTX8_AMOUNT},
  {PARAM_MTX9_SOURCE, PARAM_MTX9_DEST,  PARAM_MTX9_AMOUNT},
  {PARAM_MTX10_SOURCE, PARAM_MTX10_DEST,  PARAM_MTX10_AMOUNT},
  {PARAM_MTX11_SOURCE, PARAM_MTX11_DEST,  PARAM_MTX11_AMOUNT},
  {PARAM_MTX12_SOURCE, PARAM_MTX12_DEST,  PARAM_MTX12_AMOUNT},
  {PARAM_MTX13_SOURCE, PARAM_MTX13_DEST,  PARAM_MTX13_AMOUNT},
  {PARAM_MTX14_SOURCE, PARAM_MTX14_DEST,  PARAM_MTX14_AMOUNT}
});

PageMatrix pageOp({
  {PARAM_OP1_SOURCE1, PARAM_OP1_SOURCE2,  PARAM_OP1_OPERATION},
  {PARAM_OP2_SOURCE1, PARAM_OP2_SOURCE2,  PARAM_OP2_OPERATION},
  {PARAM_OP3_SOURCE1, PARAM_OP3_SOURCE2,  PARAM_OP3_OPERATION},
  {PARAM_OP4_SOURCE1, PARAM_OP4_SOURCE2,  PARAM_OP4_OPERATION}
});

PageVoice pageVoice({
  {PARAM_PART1_LOW_KEY, PARAM_PART1_HIGH_KEY, PARAM_PART1_CHANNEL, PARAM_PART1_VOICES},
  {PARAM_PART2_LOW_KEY, PARAM_PART2_HIGH_KEY, PARAM_PART2_CHANNEL, PARAM_PART2_VOICES},
  {PARAM_PART3_LOW_KEY, PARAM_PART3_HIGH_KEY, PARAM_PART3_CHANNEL, PARAM_PART3_VOICES},
  {PARAM_PART4_LOW_KEY, PARAM_PART4_HIGH_KEY, PARAM_PART4_CHANNEL, PARAM_PART4_VOICES},
  {PARAM_PART5_LOW_KEY, PARAM_PART5_HIGH_KEY, PARAM_PART5_CHANNEL, PARAM_PART5_VOICES},
  {PARAM_PART6_LOW_KEY, PARAM_PART6_HIGH_KEY, PARAM_PART6_CHANNEL, PARAM_PART6_VOICES},
});
 
PageLoadPatch pageLoadPatch({
  PARAM_LOAD_PATCH_INDEX, 
  PARAM_LOAD_PATCH_BANK, 
  PARAM_LOAD_PATCH_PART, 
  PARAM_LOAD_PATCH_TYPE,
});

PageMain pageMain;
PageID activePage = PAGE_MAIN;
std::list<ParameterID> changedParams;

Page& getActivePage() {
  switch (activePage) {
    case PAGE_MAIN:
      return pageMain;      

    case PAGE_EDIT_MATRIX:
      return pageMatrix;      

    case PAGE_EDIT_OP:
      return pageOp;      

    case PAGE_EDIT_VOICE:
      return pageVoice;  

    case PAGE_LOAD_PATCH:
      return pageLoadPatch;  
  }

  return pageMain;
}