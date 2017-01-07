#include <Arduino.h>
#include <Servo.h>

#include "config.h"
#include "Logger.h"
#include "PersistentConfiguration.h"
#include "ServoControl.h"

Logger* logger = NULL;
ServoControl* servo = NULL;
PersistentConfiguration* config = NULL;
Servo myservo;

int it = 0;

void setup() {
  Stream *s = NULL;
  if(DEBUG) {
    Serial.begin(9600);
    s = &Serial;
  }
  logger = new Logger(s);

  config = new PersistentConfiguration(String("espresso-dispenser"), *logger);

  myservo.attach(SERVO_1);
  servo = new ServoControl(myservo, 0, 180, *logger);

  it = config->Get<int>(String("iteration"));
}

void loop() {
  logger->Log("Iteration: %d", it);

  servo->Open();
  delay(500);

  servo->Close();
  delay(500);

  servo->MoveTo(90);
  delay(1500);

  config->Set<int>(String("iteration"), ++it);
  logger->Log("Iteration: %d", config->Get<int>(String("iteration")));
}
