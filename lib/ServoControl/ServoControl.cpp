#include "ServoControl.h"

ServoControl::ServoControl(Servo& servo, int closedState, int openState, Logger& logger)
  : _servo(servo),
    _closedState(closedState),
    _openState(openState),
    _logger(logger) {}

void ServoControl::Close() {
  _logger.Log("Moving to closed state %d", _closedState);
  _servo.write(_closedState);
}

void ServoControl::Open() {
  _logger.Log("Moving to open state %d", _openState);
  _servo.write(_openState);
}

void ServoControl::MoveTo(int position) {
  _logger.Log("Moving to %d", position);
  _servo.write(position);
}
