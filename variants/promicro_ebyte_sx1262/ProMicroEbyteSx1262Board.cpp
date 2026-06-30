#include "ProMicroEbyteSx1262Board.h"

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
  Serial1.begin(9600);

  bool screen_found = false;

  // Probe primary display address (0x3C)
  Wire.beginTransmission(0x3C);
  if (Wire.endTransmission() == 0) {
    screen_found = true;
  } else {
    // Fallback: Probe secondary display address (0x3D)
    Wire.beginTransmission(0x3D);
    if (Wire.endTransmission() == 0) {
      screen_found = true;
    }
  }

  if (!screen_found) {
    is_headless = true; 
    Serial.println("[I2C Scan] No display found at 0x3C or 0x3D. Headless mode active.");
  } else {
    is_headless = false;
    Serial.println("[I2C Scan] Display detected! Standard randomized security active.");
  }

  pinMode(PIN_VCC_ON, OUTPUT);
  digitalWrite(PIN_VCC_ON, HIGH);
  delay(10); // give sx1262 some time to power up
}
