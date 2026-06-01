#include "Logger.h"

namespace Logger {

namespace {

void printLine(const char* level, const String& message) {
  Serial.printf("[%s] %s\n", level, message.c_str());
}

}  // namespace

void begin(unsigned long baudRate) {
  Serial.begin(baudRate);
  delay(200);
  printLine("INFO", "Logger initialisiert");
}

void info(const String& message) { printLine("INFO", message); }

void warn(const String& message) { printLine("WARN", message); }

void error(const String& message) { printLine("ERROR", message); }

}  // namespace Logger
