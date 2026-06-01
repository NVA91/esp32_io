#pragma once

#include <Arduino.h>

namespace Logger {

void begin(unsigned long baudRate = 115200);
void info(const String& message);
void warn(const String& message);
void error(const String& message);

}  // namespace Logger
