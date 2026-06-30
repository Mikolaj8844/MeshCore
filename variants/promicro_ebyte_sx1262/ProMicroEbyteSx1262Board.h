#pragma once

#include <Arduino.h>
#include <MeshCore.h>
#include <helpers/NRF52Board.h>

#define BATTERY_SAMPLES          8

class ProMicroEbyteSx1262Board : public NRF52BoardDCDC {
protected:
  float adc_mult = ADC_MULTIPLIER;
  uint8_t btn_prev_state;

public:
  void begin();
  ProMicroEbyteSx1262Board() : NRF52Board("ProMicro_Nice_Nano_OTA") {}
  bool is_headless = false; 

  uint16_t getBattMilliVolts() override {
    analogReadResolution(12);

    uint32_t raw = 0;
    for (int i = 0; i < BATTERY_SAMPLES; i++) {
      raw += analogRead(BATTERY_PIN);
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
    return "ProMicroEbyteSx1262 nRF52840-LoRa";
  }

  // Interrupt for the display?
  int buttonStateChanged() {
    #ifdef PIN_BUTTON1
      uint8_t v = digitalRead(PIN_BUTTON1);
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
