#pragma once

#include <Arduino.h>
#include <MeshCore.h>
#include "hardware/adc.h"

// LoRa radio module pins for Waveshare RP2040-LoRa-HF/LF
// https://files.waveshare.com/wiki/RP2040-LoRa/Rp2040-lora-sch.pdf

/*
 * This board has no built-in way to read battery voltage.
 * Nevertheless it's very easy to make it work, you only require two 1% resistors.
 *
 *    BAT+ -----+
 *              |
 *       VSYS --+ -/\/\/\/\- --+
 *                   200k      |
 *                             +-- GPIO28
 *                             |
 *        GND --+ -/\/\/\/\- --+
 *              |    100k
 *    BAT- -----+
 */
#define PIN_VBAT_READ            28
#define BATTERY_SAMPLES          8
#define ADC_MULTIPLIER           (3.0f * 3.3f * 1000)

class PicoEbyteSx1262Board : public mesh::MainBoard {
protected:
  uint8_t startup_reason;

public:
  void begin();
  uint8_t getStartupReason() const override { return startup_reason; }

#ifdef P_LORA_TX_LED
  void onBeforeTransmit() override { digitalWrite(P_LORA_TX_LED, HIGH); }
  void onAfterTransmit() override { digitalWrite(P_LORA_TX_LED, LOW); }
#endif

  uint16_t getBattMilliVolts() override {
#if defined(PIN_VBAT_READ) && defined(ADC_MULTIPLIER)
    analogReadResolution(12);

    uint32_t raw = 0;
    for (int i = 0; i < BATTERY_SAMPLES; i++) {
      raw += analogRead(PIN_VBAT_READ);
    }
    raw = raw / BATTERY_SAMPLES;

    return (ADC_MULTIPLIER * raw) / 4096;
#else
    return 0;
#endif

  // Add this right inside the public: section of your class definition
  float getMCUTemperature() override {
      // 1. Point the ADC mux to channel 4 (connected to the internal sensor)
      adc_select_input(4); 
    
      // 2. Read the raw 12-bit value (0 - 4095)
      uint16_t raw = adc_read();
    
      // 3. Convert to voltage based on a standard 3.3V reference
      float voltage = raw * (3.3f / 4095.0f);
    
      // 4. Calculate Celsius using the official RP2040 datasheet formula
      float temperatureC = 27.0f - ((voltage - 0.706f) / 0.001721f);
    
      return temperatureC;
  }

  }

  const char *getManufacturerName() const override { return "PicoEbyteSx1262 RP2040-LoRa"; }

  void reboot() override { rp2040.reboot(); }

  bool startOTAUpdate(const char *id, char reply[]) override;
};
