#include "sd.h"
#include "midi.h"
#include "parameters.h"
#include "part.h"
#include "patch.h"
#include "multi.h"
#include <Arduino.h>
#include <cstring>
#include <cctype>

static const uint8_t kRiffTag[4] = {'R', 'I', 'F', 'F'};
static const uint8_t kFormatTag[4] = {'M', 'B', 'K', 'S'};
static const uint8_t kNameTag[4] = {'n', 'a', 'm', 'e'};
static const uint8_t kObjectTag[4] = {'o', 'b', 'j', ' '}; 

#pragma pack(push, 1)

// union StorageData {
//   Patch patch;
//   PartData part;
//   MultiData multi;
// };

struct StorageObject {
  uint8_t command;
  uint8_t argument;
  uint16_t padding;
  uint8_t data[1024];
};

#pragma pack(pop)


Storage storage;

void sendSysExDataToAmbika(midi::MidiInterface<midi::SerialMIDI<HardwareSerial> > &MIDI, uint8_t *data, size_t size, uint8_t command, uint8_t part) {
  uint8_t sysexMessage[512];

  uint8_t startSysex[7] = { 0x00, 0x21, 0x02, // (Manufacturer ID for Mutable Instruments)
      0x00,  0x04, // (Product ID for Ambika)
      command, // Command
      part  // Part
  };
  memcpy(sysexMessage, startSysex, 7);
  int index = 7;
  int checkSum = 0;

  for (int s = 0; s < size; s++) {
      sysexMessage[index++] = data[s] >> 4;
      sysexMessage[index++] = data[s] & 0xf;

      checkSum = (checkSum + data[s]) % 256;
  }

  sysexMessage[index++] = checkSum >> 4;
  sysexMessage[index++] = checkSum & 0xf;

  MIDI.sendSysEx(index, sysexMessage);
}

inline void appendString(char **dest, const char *src) {
  while (char c = *src) {
    *(*dest) = c;
    src++;
    (*dest)++;
  }
}

inline void appendString(char **dest, char src) {
    *(*dest) = src;
    (*dest)++;
}

void Storage::setFilePath(const char* bank, uint8_t index, uint8_t type) {
  filePathSet = false;
  char *filePathIndex = filePathBuffer;

  switch (type) {
    case PATCH_TYPE_PROGRAM: 
      appendString(&filePathIndex, "/PROGRAM/BANK/");
      break;

    case PATCH_TYPE_MULTI: 
      appendString(&filePathIndex, "/MULTI/BANK/");
      break;

    case PATCH_TYPE_PATCH: 
      appendString(&filePathIndex, "/PATCH/BANK/");
      break;

    case PATCH_TYPE_SEQUENCE: 
      appendString(&filePathIndex, "/SEQUENCE/BANK/");
      break;
  }

  appendString(&filePathIndex, bank);
  appendString(&filePathIndex, '/');

  if (index < 10) {
    appendString(&filePathIndex, "00");
  } else if (index < 100) {
    appendString(&filePathIndex, "0");
  }

  itoa(index, filePathIndex, 10);

  if (index < 10) {
     filePathIndex += 1;
  } else if (index < 100) {
     filePathIndex += 2;
  } else {
     filePathIndex += 3;
  }

  appendString(&filePathIndex, '.');

  std::memcpy(filePathIndex, filePathBuffer + 1, 3);
  filePathIndex += 3;  

  appendString(&filePathIndex, '\0');
  filePathSet = true;
}

bool Storage::loadName(fs::FS &fs, const char* bank, uint8_t index, uint8_t part, uint8_t type, char *name) {
  uint8_t readBuffer[16];
  StorageObject object;

  setFilePath(bank, index, type);

  File file = fs.open(filePathBuffer);
  if (!file) {
    return false;
  }

  if (file.isDirectory()) {
    return false;
  }

  size_t readBytes = 0;

  readBytes = file.read(readBuffer, 8);
  if (readBytes != 8) return false;
  if (std::memcmp(readBuffer, kRiffTag, 4) != 0) return false;

  readBytes = file.read(readBuffer, 4);
  if (readBytes != 4) return false;
  if (std::memcmp(readBuffer, kFormatTag, 4) != 0) return false;

  readBytes = file.read(readBuffer, 8);
  if (readBytes != 8) return false;

  if (std::memcmp(readBuffer, kNameTag, 4) == 0) {
    uint32_t size = *reinterpret_cast<uint32_t*>(&readBuffer[4]);
    readBytes = file.read((uint8_t *) name, size);
    if (readBytes != size) return false;
    return true;
  }

  return false;
}

void Storage::loadAndSend(fs::FS &fs, uint8_t part) {
  if (!filePathSet) return;

  uint8_t readBuffer[16];
  StorageObject object;

  File file = fs.open(filePathBuffer);
  if (!file) {
    return;
  }

  if (file.isDirectory()) {
    return;
  }

  size_t readBytes = 0;

  readBytes = file.read(readBuffer, 8);
  if (readBytes != 8) return;
  if (std::memcmp(readBuffer, kRiffTag, 4) != 0) return;

  readBytes = file.read(readBuffer, 4);
  if (readBytes != 4) return;
  if (std::memcmp(readBuffer, kFormatTag, 4) != 0) return;

  while (1) {
    readBytes = file.read(readBuffer, 8);
    if (readBytes != 8) return;

    if (std::memcmp(readBuffer, kNameTag, 4) == 0) {
      uint32_t size = *reinterpret_cast<uint32_t*>(&readBuffer[4]);
      readBytes = file.read((uint8_t *) readBuffer, size);
      if (readBytes != size) return;
    } else if (std::memcmp(readBuffer, kObjectTag, 4) == 0) {
      uint32_t size = *reinterpret_cast<uint32_t*>(&readBuffer[4]);
      readBytes = file.read((uint8_t *) &object, size);
      if (readBytes != size) return;

      sendSysExDataToAmbika(
        MIDI, 
        (uint8_t *) &object.data, 
        size - 4, 
        static_cast<uint8_t>(object.command), 
        static_cast<uint8_t>(object.argument)
        // params[PARAM_LOAD_PATCH_PART].getNRPNValue()
      );
    };

  }
}
