#ifndef SERVOCONTROL_H
#define SERVOCONTROL_H 1

#include <Logger.h>
#include <Servo.h>

class ServoControl {
private:
  Servo& _servo;
  int _closedState;
  int _openState;
  Logger& _logger;

public:
  ServoControl(Servo&, int, int, Logger&);
  void Open();
  void Close();
  void MoveTo(int);
};

#endif
