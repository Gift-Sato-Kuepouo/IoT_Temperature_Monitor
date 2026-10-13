/**
 * @file    IoT_Temperature_Monitor.ino
 * @brief   IoT temperature monitor with an LM34 sensor, SSD1306 OLED and a
 *          temperature alarm (LED + passive buzzer).
 *
 * Hardware (Arduino Uno Rev3):
 *   - LM34 precision Fahrenheit temperature sensor   -> A0
 *   - SSD1306 128x32 mono OLED                       -> A2 (SDA) / A3 (SCL)
 *   - Alarm LED (through 220 ohm)                    -> D9  (PWM)
 *   - Passive buzzer                                  -> D8
 *
 * The sketch samples the LM34 once per second, shows the reading on the OLED,
 * logs it over serial and drives the LED/buzzer whenever the temperature
 * leaves the configured range. Register that `loop()` never blocks.
 *
 * Author: Gift Sato Kuepouo
 * License: MIT
 */

#include "config.h"

#include "alarm.h"
#include "display.h"
#include "lm34.h"

// --------------------------------------------------------------------------
// Module instances
// --------------------------------------------------------------------------
LM34  sensor(PIN_LM34, ADC_VREF, ADC_SAMPLES);
Alarm alarm(PIN_LED, PIN_BUZZER);

// --------------------------------------------------------------------------
// Runtime state
// --------------------------------------------------------------------------
static float         latestF   = 0.0f;
static float         latestC   = 0.0f;
static float         minC      =  1e9f;
static float         maxC      = -1e9f;

static unsigned long lastSampleMs  = 0;
static unsigned long lastDisplayMs = 0;
static unsigned long lastSerialMs  = 0;

// --------------------------------------------------------------------------
// Arduino lifecycle
// --------------------------------------------------------------------------
void setup() {
  Serial.begin(SERIAL_BAUD);

  sensor.begin();
  alarm.begin();

  displayBegin();
  displaySplash();

  Serial.println(F("IoT Temperature Monitor"));
  Serial.println(F("======================="));
}

void loop() {
  const unsigned long now = millis();

  // Sample the sensor.
  if (now - lastSampleMs >= SAMPLE_INTERVAL_MS) {
    lastSampleMs = now;

    latestF = sensor.readFahrenheit();
    latestC = LM34::toCelsius(latestF);

    if (latestC < minC) minC = latestC;
    if (latestC > maxC) maxC = latestC;
  }

  // Refresh the alarm outputs on every pass (non-blocking).
  alarm.update(latestC, ALARM_ENABLED, now);

  // Refresh the OLED.
  if (now - lastDisplayMs >= DISPLAY_INTERVAL_MS) {
    lastDisplayMs = now;
    displayShowReadings(latestF, latestC, alarm.state());
  }

  // Log to the serial monitor.
  if (now - lastSerialMs >= SERIAL_INTERVAL_MS) {
    lastSerialMs = now;

    Serial.print(F("Temp: "));
    Serial.print(latestF, 1);
    Serial.print(F(" F / "));
    Serial.print(latestC, 1);
    Serial.print(F(" C   [min "));
    Serial.print(minC, 1);
    Serial.print(F(" C, max "));
    Serial.print(maxC, 1);
    Serial.print(F(" C]   "));

    switch (alarm.state()) {
      case ALARM_HIGH: Serial.println(F("ALARM: HIGH")); break;
      case ALARM_LOW:  Serial.println(F("ALARM: LOW"));  break;
      default:         Serial.println(F("OK"));          break;
    }
  }
}
