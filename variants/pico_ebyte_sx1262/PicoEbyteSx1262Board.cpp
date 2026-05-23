#include "PicoEbyteSx1262Board.h"

#include <Arduino.h>
#include <Wire.h>
#include "LittleFS.h"

void PicoEbyteSx1262Board::begin() {
  // 1. Force the Pico's USB Serial stack to initialize at boot
  Serial.begin(115200);

  // for future use, sub-classes SHOULD call this from their begin()
  startup_reason = BD_STARTUP_NORMAL;

#ifdef P_LORA_TX_LED
  pinMode(P_LORA_TX_LED, OUTPUT);
#endif

#ifdef PIN_VBAT_READ
  pinMode(PIN_VBAT_READ, INPUT);
#endif

#if defined(PIN_BOARD_SDA) && defined(PIN_BOARD_SCL)
  Wire.setSDA(PIN_BOARD_SDA);
  Wire.setSCL(PIN_BOARD_SCL);
#endif

  Wire.begin();

  delay(10); // give sx1262 some time to power up

  adc_init();
  adc_set_temp_sensor_enabled(true);

  // Forces the Pico flash layout to format itself if blank
  if (!LittleFS.begin()) {
    LittleFS.format();
    LittleFS.begin();
  }
}

bool PicoEbyteSx1262Board::startOTAUpdate(const char *id, char reply[]) {
  return false;
}
