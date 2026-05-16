#include "target.h"

#include <Arduino.h>
#include <helpers/ArduinoHelpers.h>

PicoEbyteSx1262Board board;

RADIO_CLASS radio = new Module(P_LORA_NSS, P_LORA_DIO_1, P_LORA_RESET, P_LORA_BUSY, SPI1);

WRAPPER_CLASS radio_driver(radio, board);

VolatileRTCClock fallback_clock;
AutoDiscoverRTCClock rtc_clock(fallback_clock);
SensorManager sensors;

// bool radio_init() {
//   rtc_clock.begin(Wire);

//   SPI1.setSCK(P_LORA_SCLK);
//   SPI1.setTX(P_LORA_MOSI);
//   SPI1.setRX(P_LORA_MISO);


//   SPI1.begin(P_LORA_SCLK, P_LORA_MISO, P_LORA_MOSI, P_LORA_NSS);

//  // HARD RESET SX1262 (important)
//   pinMode(P_LORA_RESET, OUTPUT);
//   digitalWrite(P_LORA_RESET, LOW);
//   delay(10);
//   digitalWrite(P_LORA_RESET, HIGH);
//   delay(50);

//   pinMode(P_LORA_NSS, OUTPUT);
//   digitalWrite(P_LORA_NSS, HIGH);
//   // Serial.println("RESET PIN TOGGLING");


// // pinMode(P_LORA_RESET, OUTPUT);
// // digitalWrite(P_LORA_RESET, LOW);
// // delay(20);
// // digitalWrite(P_LORA_RESET, HIGH);
// // delay(50);

// // Serial.println("SPI READY, INIT RADIO...");
// // bool ok = radio.std_init(NULL);

// // Serial.print("RADIO INIT: ");
// // Serial.println(ok);

//   //passing NULL skips init of SPI
//   // return radio.std_init(NULL);
// // IMPORTANT: create module AFTER SPI init
//   radio = new Module(P_LORA_NSS, P_LORA_DIO_1, P_LORA_RESET, P_LORA_BUSY, SPI1);

//   bool ok = radio->std_init(NULL);

//   Serial.print("SX1262 INIT: ");
//   Serial.println(ok);

//   return ok;
// }

bool radio_init() {
  Serial.println("[RADIO] init start");

  rtc_clock.begin(Wire);

  SPI1.setSCK(P_LORA_SCLK);
  SPI1.setTX(P_LORA_MOSI);
  SPI1.setRX(P_LORA_MISO);

  pinMode(P_LORA_NSS, OUTPUT);
  pinMode(P_LORA_RESET, OUTPUT);
  pinMode(P_LORA_BUSY, INPUT);

  digitalWrite(P_LORA_NSS, HIGH);
  digitalWrite(P_LORA_RESET, HIGH);

  SPI1.begin();

  delay(50);

  bool ok = radio.std_init(NULL);

  Serial.printf("[RADIO] init result: %d\n", ok);

  return ok;
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

