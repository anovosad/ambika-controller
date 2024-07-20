#pragma once

#include <Adafruit_MCP23X17.h>

#define MCP1 0
#define MCP2 1
#define MCP3 2

void initMCP();

extern Adafruit_MCP23X17 mcp[3];

