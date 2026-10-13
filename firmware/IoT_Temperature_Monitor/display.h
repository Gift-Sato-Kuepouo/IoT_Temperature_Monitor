/**
 * @file    display.h
 * @brief   SSD1306 OLED (128x32) output for the temperature monitor.
 *
 * Uses the U8g2 library which supports both hardware and bit-banged software
 * I2C. The backend is selected with USE_SOFT_I2C in config.h.
 */

#pragma once

#include <Arduino.h>

/** Initialise the OLED. Call once from setup(). */
void displayBegin();

/** Draw the power-on splash screen. */
void displaySplash();

/**
 * Render the current reading.
 *
 * @param tempF       Temperature in Fahrenheit (shown large).
 * @param tempC       Temperature in Celsius  (shown small).
 * @param alarmState  0 = normal, 1 = high, 2 = low.
 */
void displayShowReadings(float tempF, float tempC, uint8_t alarmState);
