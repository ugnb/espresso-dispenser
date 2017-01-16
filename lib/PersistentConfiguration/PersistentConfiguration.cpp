#include <PersistentConfiguration.h>

PersistentConfiguration::PersistentConfiguration(String name, Logger& logger)
: _logger(logger) {
  SPIFFS.begin();
  _fileName = String("/") + name + String(".json");
  _config = NULL;
  _jsonBuffer = NULL;
  _load();
}

void PersistentConfiguration::_load() {
  if(_jsonBuffer != NULL) {
    delete _jsonBuffer;
  }
  _jsonBuffer = new StaticJsonBuffer<1024>();

  File configFile = SPIFFS.open(_fileName, "r");
  if (configFile) {
    _logger.Log("Config file %s opened for reading", _fileName.c_str());
    String configJSONStr = configFile.readString();
    _config = &_jsonBuffer->parseObject(configJSONStr.c_str());
    configFile.close();
  }
  else {
    _logger.Log("Failed to read config file %s", _fileName.c_str());
    _config = &_jsonBuffer->createObject();
  }
}

void PersistentConfiguration::_save() {
  File configFile = SPIFFS.open(_fileName, "w");
  if (!configFile) {
    _logger.Log("Failed to open config file %s for writing", _fileName.c_str());
    return;
  }

  _config->printTo(configFile);
  configFile.close();
}
