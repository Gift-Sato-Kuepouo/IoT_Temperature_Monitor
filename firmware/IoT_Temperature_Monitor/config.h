/**
 * @file    config.h
 * @brief   Compile-time configuration for the IoT Temperature Monitor.
 *
 * All hardware mapping, thresholds and timing values live here so that the
 * firmware can be re-targeted to a different wiring without touching the
 * application logic.
 *
 * Target board: Arduino Uno (Rev3)
 */

#pragma once

/* ------------------------------------------------------------------------- */
/*  Pin map                                                                   */
/* ------------------------------------------------------------------------- */
#define PIN_LM34         A0   ///< LM34 analog temperature output
#define PIN_SDA          A2   ///< I2C data  (software I2C when USE_SOFT_I2C = 1)
#define PIN_SCL          A3   ///< I2C clock (software I2C when USE_SOFT_I2C = 1)
#define PIN_LED          9    ///< Alarm LED (PWM capable)
#define PIN_BUZZER       8    ///< Passive buzzer

/* ------------------------------------------------------------------------- */
/*  I2C backend                                                               */
/* ------------------------------------------------------------------------- */
/**
 * 1 = bit-banged software I2C on PIN_SDA / PIN_SCL (matches the schematic)
 * 0 = hardware I2C (Arduino Uno: SDA = A4, SCL = A5)
 *
 * !!! The schematic wires SDA/SCL to A2/A3 which are NOT the Uno hardware I2C
 *     pins, therefore software I2C is enabled by default to match the drawing.
 */
#define USE_SOFT_I2C        1

/* ------------------------------------------------------------------------- */
/*  OLED (SSD1306)                                                            */
/* ------------------------------------------------------------------------- */
#define OLED_WIDTH          128
#define OLED_HEIGHT         32
#define OLED_ADDRESS        0x3C   ///< 0x3C for the common mono 128x32 module

/* ------------------------------------------------------------------------- */
/*  LM34 temperature sensor                                                   */
/* ------------------------------------------------------------------------- */
#define ADC_VREF            5.0f    ///< Analog reference voltage (volts)
#define ADC_SAMPLES         10      ///< Samples per reading (averaged)
#define LM34_MV_PER_F       10.0f   ///< LM34 sensitivity: 10 mV per degree F
#define LM34_CAL_OFFSET_F   0.0f    ///< Calibration offset (degrees F)

/* ------------------------------------------------------------------------- */
/*  Alarm                                                                     */
/* ------------------------------------------------------------------------- */
#define ALARM_ENABLED        1
#define TEMP_HIGH_C          30.0f  ///< High-temperature threshold (C)
#define TEMP_LOW_C           10.0f  ///< Low-temperature threshold (C)
#define TEMP_HYSTERESIS_C     1.0f  ///< Hysteresis around the thresholds (C)
#define BUZZER_FREQ_HZ       2000   ///< Passive buzzer tone frequency (Hz)

/* ------------------------------------------------------------------------- */
/*  Timing / serial                                                           */
/* ------------------------------------------------------------------------- */
#define SAMPLE_INTERVAL_MS   1000UL
#define DISPLAY_INTERVAL_MS   250UL
#define SERIAL_INTERVAL_MS   1000UL
#define SERIAL_BAUD          115200
