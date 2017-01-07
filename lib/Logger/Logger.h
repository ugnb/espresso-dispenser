#ifndef LOGGER_H
#define LOGGER_H 1

#include <Arduino.h>

class Logger {
private:
    Stream *_logger;
public:
    Logger(Stream*);

    template<class T>
    void Log(T);

    template<typename ... T>
    void Log(const char*, T ...);
};

template<typename ... T>
void Logger::Log(const char *format, T ... args) {
    if(_logger != NULL) {
        _logger->print(">>> ");
        _logger->printf(format, args...);
        _logger->print("\n");
    }
}

template<class T>
void Logger::Log(T line) {
    if(_logger != NULL) {
        _logger->print(">>> ");
        _logger->println(line);
    }
}

#endif
