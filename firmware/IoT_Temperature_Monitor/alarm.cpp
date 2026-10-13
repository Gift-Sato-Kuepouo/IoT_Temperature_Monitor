/**
 * @file    alarm.cpp
 * @brief   Implementation of the temperature alarm.
 */

#include "alarm.h"
#include "config.h"

namespace {
const unsigned long LED_BLINK_MS = 250;  // half period of the LED blink
const unsigned long BEEP_PERIOD_MS = 800;
const unsigned long BEEP_ON_MS = 200;
}  // namespace

Alarm::Alarm(uint8_t ledPin, uint8_t buzzerPin)
    : _ledPin(ledPin), _buzzerPin(buzzerPin), _state(ALARM_NONE),
      _buzzerOn(false) {}

void Alarm::begin() {
  pinMode(_ledPin, OUTPUT);
  pinMode(_buzzerPin, OUTPUT);
  digitalWrite(_ledPin, LOW);
  digitalWrite(_buzzerPin, LOW);
}

void Alarm::update(float tempC, bool enabled, unsigned long now) {
  // ---- state machine with hysteresis ------------------------------------
  if (!enabled) {
    _state = ALARM_NONE;
  } else {
    switch (_state) {
      case ALARM_NONE:
        if (tempC >= TEMP_HIGH_C) {
          _state = ALARM_HIGH;
        } else if (tempC <= TEMP_LOW_C) {
          _state = ALARM_LOW;
        }
        break;

      case ALARM_HIGH:
        if (tempC < TEMP_HIGH_C - TEMP_HYSTERESIS_C) {
          _state = ALARM_NONE;
        }
        break;

      case ALARM_LOW:
        if (tempC > TEMP_LOW_C + TEMP_HYSTERESIS_C) {
          _state = ALARM_NONE;
        }
        break;
    }
  }

  // ---- outputs ----------------------------------------------------------
  if (_state == ALARM_NONE) {
    digitalWrite(_ledPin, LOW);
    if (_buzzerOn) {
      noTone(_buzzerPin);
      _buzzerOn = false;
    }
    return;
  }

  // Blink the LED.
  const bool ledOn = ((now / LED_BLINK_MS) % 2) == 0;
  analogWrite(_ledPin, ledOn ? 255 : 0);

  // Intermittent beep (200 ms on / 600 ms off).
  if ((now % BEEP_PERIOD_MS) < BEEP_ON_MS) {
    if (!_buzzerOn) {
      tone(_buzzerPin, BUZZER_FREQ_HZ);
      _buzzerOn = true;
    }
  } else if (_buzzerOn) {
    noTone(_buzzerPin);
    _buzzerOn = false;
  }
}
