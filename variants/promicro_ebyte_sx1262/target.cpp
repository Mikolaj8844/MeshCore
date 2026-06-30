#include "target.h"

#include <Arduino.h>
#include <helpers/ArduinoHelpers.h>
#include <Wire.h>

ProMicroEbyteSx1262Board board;

RADIO_CLASS radio = new Module(PIN_SPI_NSS, P_LORA_DIO_1, P_LORA_RESET, P_LORA_BUSY, SPI);
WRAPPER_CLASS radio_driver(radio, board);

VolatileRTCClock fallback_clock;
AutoDiscoverRTCClock rtc_clock(fallback_clock);

#if ENV_INCLUDE_GPS
  #include <helpers/sensors/MicroNMEALocationProvider.h>
  MicroNMEALocationProvider nmea = MicroNMEALocationProvider(Serial1, &rtc_clock);
  EnvironmentSensorManager sensors = EnvironmentSensorManager(nmea);
#else
  EnvironmentSensorManager sensors;
#endif

#ifdef DISPLAY_CLASS
  DISPLAY_CLASS display;
#endif

MomentaryButton user_btn(PIN_BUTTON1, 1000, true, true);

bool radio_init() {
  rtc_clock.begin(Wire);

  if (!the_mesh.isDisplayEnabled()) {
    if (the_mesh.getNodePrefs()->ble_pin == 0) {
      the_mesh.getNodePrefs()->ble_pin = 864200; // Your exact 6-digit fixed PIN fallback
    }
  }

  pinMode(PIN_SPI_NSS, OUTPUT);
  digitalWrite(PIN_SPI_NSS, HIGH);

  SPI.begin();

  //passing NULL skips init of SPI
  return radio.std_init(NULL);
}

uint32_t radio_get_rng_seed() {
  return radio.random(0x7FFFFFFF);
}

void radio_set_params(float freq, float bw, uint8_t sf, uint8_t cr) {
  radio.setFrequency(freq);
  radio.setSpreadingFactor(sf);
  radio.setBandwidth(bw);
  radio.setCodingRate(cr);
}

void radio_set_tx_power(int8_t dbm) {
  radio.setOutputPower(dbm);
}

mesh::LocalIdentity radio_new_identity() {
  RadioNoiseListener rng(radio);
  return mesh::LocalIdentity(&rng); // create new random identity
}
