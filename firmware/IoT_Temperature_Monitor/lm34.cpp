/**
 * @file    lm34.cpp
 * @brief   Implementation of the LM34 analog temperature sensor driver.
 */

#include "lm34.h"
#include "config.h"

LM34::LM34(uint8_t pin, float vRef, uint8_t samples)
    : _pin(pin), _vRef(vRef), _samples(samples == 0 ? 1 : samples) {}

void LM34::begin() {
  pinMode(_pin, INPUT);
}

float LM34::readVoltage() {
  uint32_t accumulator = 0;

  for (uint8_t i = 0; i < _samples; i++) {
    accumulator += analogRead(_pin);
    delayMicroseconds(200);
  }

  const float raw = static_cast<float>(accumulator) / static_cast<float>(_samples);
  return raw * _vRef / 1023.0f;
}

float LM34::readFahrenheit() {
  const float volts = readVoltage();

  // 10 mV per degree Fahrenheit  ->  mV / (mV per F) = degrees F
  const float fahrenheit = volts * 1000.0f / LM34_MV_PER_F;
  return fahrenheit + LM34_CAL_OFFSET_F;
}

float LM34::readCelsius() {
  return toCelsius(readFahrenheit());
}

float LM34::toCelsius(float fahrenheit) {
  return (fahrenheit - 32.0f) * 5.0f / 9.0f;
}

float LM34::toFahrenheit(float celsius) {
  return celsius * 9.0f / 5.0f + 32.0f;
}
