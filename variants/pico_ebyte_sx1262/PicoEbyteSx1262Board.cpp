#include "PicoEbyteSx1262Board.h"

#include <Arduino.h>
#include <Wire.h>
#include "hardware/adc.h"

void PicoEbyteSx1262Board::begin() {
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
}

bool PicoEbyteSx1262Board::startOTAUpdate(const char *id, char reply[]) {
  return false;
}

float PicoEbyteSx1262Board::getMCUTemperature() {
    adc_select_input(4); // RP2040 internal temp sensor is tied to ADC channel 4

    // Read the 12-bit ADC value (0-4095)
    uint16_t raw = adc_read();

    // Convert raw reading to voltage (assuming 3.3V reference)
    float voltage = raw * (3.3f / 4095.0f);

    // Standard RP2040 Datasheet Formula: T = 27 - (Voltage - 0.706) / 0.001721
    float temperatureC = 27.0f - ((voltage - 0.706f) / 0.001721f);

    return temperatureC;
}
