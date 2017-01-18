#include <Arduino.h>
#include <Servo.h>
#include <Ticker.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <Wire.h>
#include <SSD1306.h>

#include "config.h"
#include "Logger.h"
#include "PersistentConfiguration.h"
#include "ServoControl.h"

Logger* logger = NULL;

int selectedServo = 0;
int isBusy = 0;

void openServo();
void closeServo();

void callback(const char* topic, byte* payload, unsigned int length) {
    logger->Log("Got message on topic %s", topic);
    int servo = String((char)payload[0]).toInt();
    logger->Log("Target servo %d", servo);
    if(servo > 0 && servo < 6 && isBusy == 0) {
      isBusy = 1;
      selectedServo = servo - 1;
      openServo();
    }
}

WiFiClient espClient;
const char* mqtt_server = MQTT_HOST;
PubSubClient client(espClient);


ServoControl* servoControl[5];
PersistentConfiguration* config = NULL;
Servo servo[5];

int it = 0;
int analog = 0;
int buttonState = 0;

Ticker closeTimer;

bool NETWORK_CONNECTED = false;
bool ED_MQTT_CONNECTED = false;

SSD1306  display(0x3c, D5, D6);

void closeServo() {
  servoControl[selectedServo]->Close();
  closeTimer.detach();
  isBusy = 0;
}

void openServo() {
  servoControl[selectedServo]->Open();
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

  for(int i = 0; i < 5; i++) {
    servoControl[i]->Close();
  }

  it = config->Get<int>(String("iteration"));

  display.init();

  display.flipScreenVertically();

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_24);

  attachInterrupt(BUTTON_PIN, openServo, RISING);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while(WiFi.status() != WL_CONNECTED) {
    logger->Log("WiFi waiting...");
    delay(500);
  }

  client.setServer(mqtt_server, MQTT_PORT);
  client.setCallback(callback);
}

void loop() {
  NETWORK_CONNECTED = (WiFi.status() == WL_CONNECTED);

  if (!client.connected()) {
    ED_MQTT_CONNECTED = false;
    logger->Log("Connecting to MQTT server");
    if (
      client.connect(
        MQTT_TOPIC,
        MQTT_USER,
        MQTT_PASS)
    ) {
      logger->Log("Connected to MQTT server");
      client.subscribe("nespresso-alpha");
      ED_MQTT_CONNECTED = true;
    }
    else {
      logger->Log("Could not connect to MQTT server");
    }
  } else {
    ED_MQTT_CONNECTED = true;
  }
  client.loop();

  // selectedServo = map(analogRead(POTENTIOMETER_PIN), 0, 1023, 0, 5);

  display.clear();
  display.drawString(0, 0, String("WiFi:"));
  display.drawString(80, 0, String(NETWORK_CONNECTED ? "OK" : "FAIL"));
  display.drawString(0, 30, String("MQTT:"));
  display.drawString(80, 30, String(ED_MQTT_CONNECTED ? "OK" : "FAIL"));

  display.display();
  delay(20);
}
