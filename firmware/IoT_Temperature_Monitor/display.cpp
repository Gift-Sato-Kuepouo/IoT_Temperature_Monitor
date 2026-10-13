/**
 * @file    display.cpp
 * @brief   SSD1306 OLED rendering.
 */

#include "display.h"
#include "config.h"

#include <stdio.h>

#include <U8g2lib.h>

#if USE_SOFT_I2C
// Bit-banged software I2C on arbitrary pins (matches the schematic).
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C oled(
    U8G2_R0, /* clock= */ PIN_SCL, /* data= */ PIN_SDA,
    /* reset= */ U8X8_PIN_NONE);
#else
// Hardware I2C (Uno: SDA = A4, SCL = A5).
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C oled(
    U8G2_R0, /* reset= */ U8X8_PIN_NONE);
#endif

void displayBegin() {
  oled.begin();
  oled.setContrast(180);
}

void displaySplash() {
  oled.clearBuffer();
  oled.setFont(u8g2_font_7x13_tf);
  oled.drawStr(0, 16, "IoT Temp");
  oled.setFont(u8g2_font_5x7_tf);
  oled.drawStr(0, 28, "Monitor  v1.0");
  oled.sendBuffer();
}

void displayShowReadings(float tempF, float tempC, uint8_t alarmState) {
  char buf[16];

  oled.clearBuffer();

  // ---- status line (top, small font, baseline at y = 7) -----------------
  oled.setFont(u8g2_font_5x7_tf);
  const char *status = "OK";
  if (alarmState == 1) {
    status = "HIGH";
  } else if (alarmState == 2) {
    status = "LOW";
  }
  oled.drawStr(0, 7, status);

  snprintf(buf, sizeof(buf), "%.1fC", tempC);
  oled.drawStr(OLED_WIDTH - oled.getStrWidth(buf), 7, buf);

  // ---- large Fahrenheit reading (baseline at y = 31) --------------------
  oled.setFont(u8g2_font_logisoso20_tf);
  snprintf(buf, sizeof(buf), "%.1f", tempF);
  oled.drawStr(0, 31, buf);

  int valueWidth = oled.getStrWidth(buf);
  oled.setFont(u8g2_font_6x10_tf);
  oled.drawStr(valueWidth + 2, 31, "F");

  oled.sendBuffer();
}
