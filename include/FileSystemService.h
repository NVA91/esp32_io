#pragma once

#include <Arduino.h>

class FileSystemService {
 public:
  bool begin();
  bool hasFile(const char* path) const;
  String readFile(const char* path) const;
  bool writeFile(const char* path, const String& content) const;
};
