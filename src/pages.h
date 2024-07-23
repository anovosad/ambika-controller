#pragma once

#include <vector>
#include "parameters.h"
#include "midi.h"
#include <Adafruit_SH110X.h>
#include <list>


/*****************************************************************************/

enum PageID {
  PAGE_MAIN = 0,
  PAGE_EDIT_MATRIX,
  PAGE_EDIT_OP,
  PAGE_EDIT_VOICE,
  PAGE_LOAD_PATCH,
};

/*****************************************************************************/

class Page {
public:
  virtual void encoderEvent(int encoder, int delta) = 0;
  virtual void draw(Adafruit_SH1106G &display) = 0;
  virtual void task(Adafruit_SH1106G &display) = 0;
};

/*****************************************************************************/

class PageMatrix : public Page {
public:
  PageMatrix(const std::vector<std::vector<ParameterID>> &m);

  void encoderEvent(int encoder, int delta);
  void draw(Adafruit_SH1106G &display);
  void task(Adafruit_SH1106G &display);

private:
  std::vector<std::vector<ParameterID>> lines;
  int activeLine;
  int firstDisplayedLine;
  unsigned int displayedLines;
};

/*****************************************************************************/

class PageVoice : public Page {
public:
  PageVoice(const std::vector<std::vector<ParameterID>> &m);

  void encoderEvent(int encoder, int delta);
  void draw(Adafruit_SH1106G &display);
  void task(Adafruit_SH1106G &display);

  void pressEnc();
  void sendMultiDataToAmbika(midi::MidiInterface<midi::SerialMIDI<HardwareSerial> > &MIDI);

private:
  std::vector<std::vector<ParameterID>> lines;
  int activeLine;
  int activeVoice;
  int firstDisplayedLine;
  unsigned int displayedLines;
};

/*****************************************************************************/

class PageLoadPatch : public Page {
public:
  PageLoadPatch(const std::vector<ParameterID> &m);

  void encoderEvent(int encoder, int delta);
  void draw(Adafruit_SH1106G &display);
  void task(Adafruit_SH1106G &display);
  void setChanged();

private:
  std::vector<ParameterID> data;
  char name[64];
  bool loaded;
  bool changed;
  bool uploading;
  unsigned long lastChanged;
};

/*****************************************************************************/

class PageMain : public Page {
public:
  PageMain();

  void encoderEvent(int encoder, int delta);
  void draw(Adafruit_SH1106G &display);
  void task(Adafruit_SH1106G &display);

};

/*****************************************************************************/

extern PageMatrix pageMatrix;
extern PageMatrix pageOp;
extern PageVoice pageVoice;
extern PageLoadPatch pageLoadPatch;
extern PageMain pageMain;

extern PageID activePage;
extern std::list<ParameterID> changedParams;

Page& getActivePage();
