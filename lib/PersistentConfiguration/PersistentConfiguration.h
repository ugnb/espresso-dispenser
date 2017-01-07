#ifndef PERSISTENT_CONFIGURATION_H
#define PERSISTENT_CONFIGURATION_H 1

#include <Arduino.h>
#include <FS.h>
#include <ArduinoJson.h>

#include <Logger.h>

class PersistentConfiguration {
private:
  String _fileName;
  Logger& _logger;
  StaticJsonBuffer<1024>* _jsonBuffer;
  JsonObject* _config;

  void _load();
  void _save();
public:
  PersistentConfiguration(String, Logger&);

  template<class T>
  void Set(String key, T value) {
    _config->set(key, value);
    _save();
  }

  template<class T>
  T Get(String key) {
    // _logger.Log("BEFORE LOAD");
    // _load();
    // _logger.Log("BEFORE GET");
    return _config->get<T>(key);
  }
};

#endif
