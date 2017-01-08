#include <Arduino.h>
#include <Servo.h>

#include <Wire.h>
#include <SSD1306.h>

#include "config.h"
#include "Logger.h"
#include "PersistentConfiguration.h"
#include "ServoControl.h"

Logger* logger = NULL;
ServoControl* servo = NULL;
PersistentConfiguration* config = NULL;
Servo myservo;

int it = 0;
int analog = 0;
int buttonState = 0;

SSD1306  display(0x3c, D5, D6);

void setup() {
  pinMode(BUTTON_PIN, INPUT);

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

  display.init();

  display.flipScreenVertically();

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
}

void loop() {
  analog = map(analogRead(POTENTIOMETER_PIN), 0, 1023, 0, 180);
  servo->MoveTo(analog);

  display.clear();
  display.drawString(0, 0, String("Position:"));
  display.drawString(0, 20, String(analog));

  buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == HIGH) {
    display.drawString(0, 40, String("Button ON"));
  }
  else {
    display.drawString(0, 40, String("Button OFF"));
  }

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
