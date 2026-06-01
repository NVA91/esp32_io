#include "FileSystemService.h"

#include <LittleFS.h>

#include "Logger.h"

bool FileSystemService::begin() {
  const bool mounted = LittleFS.begin(true);
  if (mounted) {
    Logger::info("LittleFS bereit");
  } else {
    Logger::error("LittleFS konnte nicht gemountet werden");
  }
  return mounted;
}

bool FileSystemService::hasFile(const char* path) const { return LittleFS.exists(path); }

String FileSystemService::readFile(const char* path) const {
  File file = LittleFS.open(path, "r");
  if (!file) {
    return String();
  }

  String content = file.readString();
  file.close();
  return content;
}
