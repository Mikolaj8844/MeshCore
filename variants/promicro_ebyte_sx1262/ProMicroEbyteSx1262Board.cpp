#include "ProMicroEbyteSx1262Board.h"

#include "Adafruit_LittleFS.h"

#include <Arduino.h>
#include <Wire.h>

void ProMicroEbyteSx1262Board::begin() {
  NRF52Board::begin();

  // 1. Force the Pico's USB Serial stack to initialize at boot
  Serial.begin(115200);

  btn_prev_state = HIGH;

  pinMode(BATTERY_PIN, INPUT);

#ifdef P_LORA_TX_LED
  pinMode(P_LORA_TX_LED, OUTPUT);
#endif

#ifdef PIN_BUTTON1
  pinMode(PIN_BUTTON1, INPUT_PULLUP);
#endif

#if defined(PIN_WIRE_SDA) && defined(PIN_WIRE_SCL)
  Wire.setPins(PIN_WIRE_SDA, PIN_WIRE_SCL);
#endif

  Wire.begin();

  pinMode(PIN_VCC_ON, OUTPUT);
  digitalWrite(PIN_VCC_ON, HIGH);
  delay(10); // give sx1262 some time to power up

  // Forces the Pico flash layout to format itself if blank
  if (!LittleFS.begin()) {
    LittleFS.format();
    LittleFS.begin();
  }
}
