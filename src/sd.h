#include <SD.h>
#include <FS.h>
#include <MIDI.h>

class Storage {
public:
  bool loadName(fs::FS &fs, const char* bank, uint8_t index, uint8_t part, uint8_t type, char *name);
  void loadAndSend(fs::FS &fs, uint8_t part);

private:
  void setFilePath(const char* bank, uint8_t index, uint8_t type);

  char filePathBuffer[128];
  bool filePathSet;
};

void sendSysExDataToAmbika(midi::MidiInterface<midi::SerialMIDI<HardwareSerial> > &MIDI, uint8_t *data, size_t size, uint8_t command, uint8_t part);

extern Storage storage;

