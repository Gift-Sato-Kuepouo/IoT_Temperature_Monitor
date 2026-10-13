/**
 * @file    ds2482_8channel_scan.ino
 * @brief   Optional example: read up to eight DS18B20 1-Wire sensors through
 *          the Adafruit DS2482S-800 8-channel I2C-to-1-Wire adapter.
 *
 * ---------------------------------------------------------------------------
 * IMPORTANT
 * ---------------------------------------------------------------------------
 * This sketch is provided for the DS2482S-800 that appears in the project
 * bill of materials. A DS2482S-800 is a 1-Wire *master*: it can only talk to
 * digital 1-Wire devices such as the DS18B20. It CANNOT read the analog LM34
 * used by the main firmware.
 *
 * Use this sketch only if you replace/add the analog LM34 with DS18B20
 * sensors (one per channel, OW0..OW7).
 *
 * Wiring (Arduino Uno):
 *   DS2482S-800 VCC  -> 5V
 *   DS2482S-800 GND  -> GND
 *   DS2482S-800 SDA  -> SDA (A4 on the Uno)   <-- hardware I2C
 *   DS2482S-800 SCL  -> SCL (A5 on the Uno)   <-- hardware I2C
 *   DS18B20 DQ       -> channel OW0..OW7 (with the on-board pull-up)
 *
 * This example uses the hardware I2C bus. The Adafruit_DS248x library requires
 * a TwoWire instance, which is why it is not part of the LM34 firmware (that
 * uses bit-banged software I2C on A2/A3 to match the schematic).
 *
 * Required library: "Adafruit DS248x" by Adafruit.
 */

#include <Wire.h>
#include <Adafruit_DS248x.h>

#define DS18B20_CMD_SKIP_ROM         0xCC
#define DS18B20_CMD_CONVERT_T        0x44
#define DS18B20_CMD_READ_SCRATCHPAD  0xBE

Adafruit_DS248x ds248x;

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 2000) {
    delay(10);
  }

  Serial.println(F("DS2482S-800 8-channel 1-Wire scan"));

  Wire.begin();

  if (!ds248x.begin(&Wire, DS248X_ADDRESS)) {
    Serial.println(F("DS2482S-800 init failed - check wiring."));
    while (true) {
      delay(10);
    }
  }
  Serial.println(F("DS2482S-800 ready."));
}

void loop() {
  for (uint8_t channel = 0; channel < 8; channel++) {
    const float celsius = readTemperature(channel);

    Serial.print(F("Channel "));
    Serial.print(channel);
    Serial.print(F(": "));

    if (isnan(celsius)) {
      Serial.println(F("no sensor / read error"));
    } else {
      Serial.print(celsius, 2);
      Serial.print(F(" C  ("));
      Serial.print(celsius * 9.0f / 5.0f + 32.0f, 2);
      Serial.println(F(" F)"));
    }
  }

  Serial.println();
  delay(2000);
}

/**
 * Trigger a conversion and read the scratchpad of a single DS18B20 on the
 * given channel. Returns NAN on error.
 */
float readTemperature(uint8_t channel) {
  if (!ds248x.selectChannel(channel)) {
    return NAN;
  }

  // Start conversion.
  ds248x.OneWireReset();
  ds248x.OneWireWriteByte(DS18B20_CMD_SKIP_ROM);
  ds248x.OneWireWriteByte(DS18B20_CMD_CONVERT_T);
  delay(750);  // 750 ms for 12-bit resolution

  // Read scratchpad.
  ds248x.OneWireReset();
  ds248x.OneWireWriteByte(DS18B20_CMD_SKIP_ROM);
  ds248x.OneWireWriteByte(DS18B20_CMD_READ_SCRATCHPAD);

  uint8_t data[9];
  for (uint8_t i = 0; i < 9; i++) {
    ds248x.OneWireReadByte(&data[i]);
  }

  const int16_t raw = (data[1] << 8) | data[0];
  return static_cast<float>(raw) / 16.0f;
}
