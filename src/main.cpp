#include <Arduino.h>
#include <Servo.h>
#include <Ticker.h>

#include <Wire.h>
#include <SSD1306.h>

#include "config.h"
#include "Logger.h"
#include "PersistentConfiguration.h"
#include "ServoControl.h"

Logger* logger = NULL;
ServoControl* servoControl[5];
PersistentConfiguration* config = NULL;
Servo servo[5];

int it = 0;
int analog = 0;
int buttonState = 0;
int selectedServo = 0;

Ticker closeTimer;

SSD1306  display(0x3c, D5, D6);

void closeServo() {
  servoControl[selectedServo]->MoveTo(10);
  closeTimer.detach();
}

void openServo() {
  servoControl[selectedServo]->MoveTo(170);
  closeTimer.attach(0.3, closeServo);
}

void setup() {
  pinMode(BUTTON_PIN, INPUT);

  Stream *s = NULL;
  if(DEBUG) {
    Serial.begin(9600);
    s = &Serial;
  }
  logger = new Logger(s);

  config = new PersistentConfiguration(String("espresso-dispenser"), *logger);

  servo[0].attach(SERVO_1);
  servo[1].attach(SERVO_2);
  servo[2].attach(SERVO_3);
  servo[3].attach(SERVO_4);
  servo[4].attach(SERVO_5);

  servoControl[0] = new ServoControl(servo[0], 0, 180, *logger);
  servoControl[1] = new ServoControl(servo[1], 0, 180, *logger);
  servoControl[2] = new ServoControl(servo[2], 0, 180, *logger);
  servoControl[3] = new ServoControl(servo[3], 0, 180, *logger);
  servoControl[4] = new ServoControl(servo[4], 0, 180, *logger);

  it = config->Get<int>(String("iteration"));

  display.init();

  display.flipScreenVertically();

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_24);

  attachInterrupt(BUTTON_PIN, openServo, RISING);
}

void loop() {
  selectedServo = map(analogRead(POTENTIOMETER_PIN), 0, 1023, 0, 5);

  display.clear();
  display.drawString(0, 0, String("Servo:"));
  display.drawString(80, 0, String(selectedServo + 1));

  // buttonState = digitalRead(BUTTON_PIN);

  // if (buttonState == HIGH) {
  //   display.drawString(0, 40, String("Servo: "));
  //   display.drawString(64, 40, String(analog));
  // }
  // else {
  //   display.drawString(0, 40, String("Button OFF"));
  // }

  display.display();
  delay(20);
  // logger->Log("Iteration: %d", it);
  //
  // servo->Open();
  // delay(500);
  //
  // servo->Close();
  // delay(500);
  //
  // servo->MoveTo(90);
  // delay(1500);
  //
  // config->Set<int>(String("iteration"), ++it);
  // logger->Log("Iteration: %d", config->Get<int>(String("iteration")));
}
