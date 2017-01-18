#ifndef CONFIG_H
#define CONFIG_H 1

#include <Arduino.h>

#define DEBUG true

#define I2C_SDA     D5
#define I2C_SCL     D6

#define SERVO_1     D0
#define SERVO_2     D1
#define SERVO_3     D2
#define SERVO_4     D3
#define SERVO_5     D4

#define POTENTIOMETER_PIN A0
#define BUTTON_PIN        D7

#define MQTT_HOST  "REPLACE_ME"
#define MQTT_PORT  16742
#define MQTT_USER  "REPLACE_ME"
#define MQTT_PASS  "REPLACE_ME"
#define MQTT_TOPIC "REPLACE_ME"

#define WIFI_SSID "REPLACE_ME"
#define WIFI_PASS "REPLACE_ME"

#endif
