/*
 * variant.h
 * Copyright (C) 2023 Seeed K.K.
 * MIT License
 */

 #pragma once

 #include "WVariant.h"
 
 ////////////////////////////////////////////////////////////////////////////////
 // Low frequency clock source 

#define VARIANT_MCK       (64000000ul)

//#define USE_LFXO      // 32.768 kHz crystal oscillator
#define USE_LFRC    // 32.768 kHz RC oscillator

////////////////////////////////////////////////////////////////////////////////
// Power

#define PIN_EXT_VCC           (13)
#define EXT_VCC               (PIN_EXT_VCC)

#define BATTERY_PIN           (4)

////////////////////////////////////////////////////////////////////////////////
// Number of pins

#define PINS_COUNT            (48)
#define NUM_DIGITAL_PINS      (48)
#define NUM_ANALOG_INPUTS     (8)
#define NUM_ANALOG_OUTPUTS    (0)

////////////////////////////////////////////////////////////////////////////////
// GPS
#define PIN_GPS_TX PIN_SERIAL1_TX
#define PIN_GPS_RX PIN_SERIAL1_RX

////////////////////////////////////////////////////////////////////////////////
// I2C pin definition

#define WIRE_INTERFACES_COUNT 1

////////////////////////////////////////////////////////////////////////////////
// SPI pin definition

#define SPI_INTERFACES_COUNT  1

////////////////////////////////////////////////////////////////////////////////
// Builtin LEDs

#ifdef PIN_SPI_TX_LED
  #define PIN_LED             PIN_SPI_TX_LED
#else
  #define PIN_LED             (15)
#endif

#define LED_PIN               PIN_LED
#define LED_BLUE              PIN_LED
#define LED_BUILTIN           PIN_LED
#define LED_STATE_ON          1

////////////////////////////////////////////////////////////////////////////////
// Builtin buttons

#ifdef PIN_BUTTON1
  #define BUTTON_PIN          PIN_BUTTON1
  #define PIN_USER_BTN        PIN_BUTTON1
#endif
