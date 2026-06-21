#pragma once

#include <MeshCore.h>
#include <Arduino.h>
#include <helpers/NRF52Board.h>

// --- Pull values from platformio.ini rather than overriding them ---
#ifndef P_LORA_NSS
  #define P_LORA_NSS 13
#endif
#ifndef P_LORA_DIO_1
  #define P_LORA_DIO_1 11
#endif
#ifndef P_LORA_RESET
  #define P_LORA_RESET 10
#endif
#ifndef P_LORA_BUSY
  #define P_LORA_BUSY 16
#endif

// --- Handle standard SPI Fallbacks cleanly ---
#ifndef P_LORA_MISO
  #define P_LORA_MISO PIN_SPI_MISO
#endif
#ifndef P_LORA_SCLK
  #define P_LORA_SCLK PIN_SPI_SCLK
#endif
#ifndef P_LORA_MOSI
  #define P_LORA_MOSI PIN_SPI_MOSI
#endif

#ifndef SX126X_POWER_EN
  #define SX126X_POWER_EN PIN_VCC_ON
#endif

#ifndef SX126X_RXEN
  #define SX126X_RXEN 2
#endif
#ifndef SX126X_TXEN
  #define SX126X_TXEN RADIOLIB_NC
#endif

#define SX126X_DIO3_TCXO_VOLTAGE (1.8f)

#define PIN_VBAT_READ 17
#define ADC_MULTIPLIER (1.815f) // dependent on voltage divider resistors. TODO: more accurate battery tracking

class ProMicroEbyteSx1262Board : public NRF52BoardDCDC {
protected:
  uint8_t btn_prev_state;
  float adc_mult = ADC_MULTIPLIER;

public:
  ProMicroEbyteSx1262Board() : NRF52Board("ProMicro_OTA") {}
  void begin();

  #define BATTERY_SAMPLES 8

  uint16_t getBattMilliVolts() override {
    analogReadResolution(12);

    uint32_t raw = 0;
    for (int i = 0; i < BATTERY_SAMPLES; i++) {
      raw += analogRead(PIN_VBAT_READ);
    }
    raw = raw / BATTERY_SAMPLES;
    return (adc_mult * raw);
  }

  bool setAdcMultiplier(float multiplier) override {
    if (multiplier == 0.0f) {
      adc_mult = ADC_MULTIPLIER;}
    else {
      adc_mult = multiplier;
    }
    return true;
  }
  float getAdcMultiplier() const override {
    if (adc_mult == 0.0f) {
      return ADC_MULTIPLIER;
    } else {
      return adc_mult;
    }
  }

  const char* getManufacturerName() const override {
    return "ProMicroEbyteSx1262 RP2040-LoRa";
  }

  int buttonStateChanged() {
    #ifdef BUTTON_PIN
      uint8_t v = digitalRead(BUTTON_PIN);
      if (v != btn_prev_state) {
        btn_prev_state = v;
        return (v == LOW) ? 1 : -1;
      }
    #endif
      return 0;
  }

  void powerOff() override {
    sd_power_system_off();
  }
};
