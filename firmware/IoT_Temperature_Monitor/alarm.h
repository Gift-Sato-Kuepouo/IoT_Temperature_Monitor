/**
 * @file    alarm.h
 * @brief   High/low temperature alarm driving an LED and a passive buzzer.
 *
 * The alarm is fully non-blocking: update() is expected to be called on every
 * pass of loop() and uses millis() to generate the blink and beep patterns.
 */

#pragma once

#include <Arduino.h>

enum : uint8_t {
  ALARM_NONE = 0,
  ALARM_HIGH = 1,
  ALARM_LOW  = 2
};

class Alarm {
public:
  Alarm(uint8_t ledPin, uint8_t buzzerPin);

  /** Configure the output pins. Call once from setup(). */
  void begin();

  /**
   * Update the alarm state and refresh the blink/beep outputs.
   *
   * @param tempC    Latest temperature in Celsius.
   * @param enabled  Allow the alarm to trigger.
   * @param now      Current millis() timestamp.
   */
  void update(float tempC, bool enabled, unsigned long now);

  /** @return ALARM_NONE, ALARM_HIGH or ALARM_LOW. */
  uint8_t state() const { return _state; }

  /** @return true while an alarm is active. */
  bool active() const { return _state != ALARM_NONE; }

private:
  uint8_t _ledPin;
  uint8_t _buzzerPin;
  uint8_t _state;
  bool    _buzzerOn;
};
