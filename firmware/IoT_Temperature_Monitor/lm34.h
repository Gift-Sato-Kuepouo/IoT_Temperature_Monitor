/**
 * @file    lm34.h
 * @brief   Driver for the LM34 analog (Fahrenheit) temperature sensor.
 *
 * The LM34 outputs 10 mV per degree Fahrenheit, referenced to its own ground.
 * The output is sampled with the ATmega328P ADC and averaged to reduce noise.
 */

#pragma once

#include <Arduino.h>

class LM34 {
public:
  /**
   * @param pin      Analog input connected to the LM34 output.
   * @param vRef     ADC reference voltage in volts (defaults to 5 V).
   * @param samples  Number of ADC samples averaged per reading.
   */
  LM34(uint8_t pin, float vRef = 5.0f, uint8_t samples = 10);

  /** Configure the analog pin. Call once from setup(). */
  void begin();

  /** @return Temperature in degrees Fahrenheit. */
  float readFahrenheit();

  /** @return Temperature in degrees Celsius. */
  float readCelsius();

  /** Convert Fahrenheit to Celsius. */
  static float toCelsius(float fahrenheit);

  /** Convert Celsius to Fahrenheit. */
  static float toFahrenheit(float celsius);

private:
  float readVoltage();

  uint8_t _pin;
  float   _vRef;
  uint8_t _samples;
};
