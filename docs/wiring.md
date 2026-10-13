# Wiring & Assembly

This document describes how the hardware is connected, the corrections applied
relative to the original Fritzing schematic, and the bill of materials.

- Board: **Arduino Uno (Rev3)**
- Reference files: [`hardware/IoT-Temp-Monitor_schematic.fzz`](../hardware/IoT-Temp-Monitor_schematic.fzz)
  (open with [Fritzing](https://fritzing.org/)) and
  [`hardware/IoT-Temp-Monitor_schematic.png`](../hardware/IoT-Temp-Monitor_schematic.png).

---

## 1. Pin map used by the firmware

| Signal                | Arduino pin | Peripheral                | Notes                         |
| --------------------- | :---------: | ------------------------- | ----------------------------- |
| LM34 output (`Vout`)  | **A0**      | LM34                      | Analog input                  |
| LM34 `+Vs`            | **5V**      | LM34                      | Power                         |
| LM34 `GND`            | **GND**     | LM34                      | Ground                        |
| I2C SDA               | **A2**      | OLED + (optional) DS2482  | Software I2C                  |
| I2C SCL               | **A3**      | OLED + (optional) DS2482  | Software I2C                  |
| OLED `VCC`            | **3.3V**    | SSD1306 OLED              | `VIN` accepts 3.3–5 V         |
| OLED `GND`            | **GND**     | SSD1306 OLED              | Ground                        |
| LED anode             | **D9**      | LED via 220 Ω             | PWM output                    |
| LED cathode           | **GND**     | LED                       | Ground                        |
| Buzzer `+`            | **D8**      | Passive buzzer            | Driven with `tone()`          |
| Buzzer `-`            | **GND**     | Passive buzzer            | Ground                        |

> All pin assignments are defined in
> [`firmware/IoT_Temperature_Monitor/config.h`](../firmware/IoT_Temperature_Monitor/config.h).

---

## 2. Bill of materials

| Qty | Part                                          | Notes                                  |
| :-: | --------------------------------------------- | -------------------------------------- |
|  1  | Arduino Uno Rev3                              | MCU board                              |
|  1  | LM34 precision temperature sensor             | Analog, 10 mV/°F                       |
|  1  | SSD1306 128×32 monochrome OLED (I2C)          | Address `0x3C`                         |
|  1  | Adafruit DS2482S-800 8-channel 1-Wire adapter | Optional — see §4                      |
|  1  | LED                                           | Alarm indicator                        |
|  1  | 220 Ω resistor                                | LED current limiting                   |
|  1  | Passive buzzer                                | Alarm sound                            |
|  —  | Breadboard + jumper wires                     |                                        |

---

## 3. Corrections applied to the original schematic

The original Fritzing schematic is a great starting point, but three items had
to be adjusted to make the project work. The firmware in this repository matches
the **corrected** wiring below.

### 3.1 LM34 must be read by the ADC, not by the DS2482

The schematic connects the LM34 output to **OW0** of the DS2482S-800. The
DS2482S-800 is a **1-Wire master** and can only communicate with digital 1-Wire
devices (e.g. DS18B20). It cannot measure an analog voltage, so the LM34 will
never produce a reading through it.

**Fix:** connect the LM34 output to an analog input, **A0**.

```
LM34 Vout  ->  Arduino A0
LM34 +Vs   ->  5V
LM34 GND   ->  GND
```

### 3.2 I2C is wired to A2/A3, which are not the Uno hardware-I2C pins

The schematic uses **A2/A3** for SDA/SCL. On the Arduino Uno the hardware I2C
bus is on **A4 (SDA)** / **A5 (SCL)** (also broken out on the dedicated
SDA/SCL header).

Two valid options:

- **Keep A2/A3** and use bit-banged *software* I2C — this is the default in the
  firmware (`USE_SOFT_I2C 1`) so it matches the drawing.
- **Move to A4/A5** and use *hardware* I2C — set `USE_SOFT_I2C 0` in
  `config.h`. This is more robust and is recommended if you also use the
  DS2482S-800 (see §4).

### 3.3 LED cathode must be grounded

The schematic shows the LED anode connected (through the 220 Ω resistor) to
D9, but the cathode was left floating, so the LED cannot light.

**Fix:** connect the LED cathode to **GND**.

```
Arduino D9  ->  220 Ω  ->  LED anode
LED cathode ->  GND
```

---

## 4. About the DS2482S-800 (optional)

The DS2482S-800 lets one I2C bus address **8 independent 1-Wire channels**, which
is ideal when you have several DS18B20 sensors and want to know which location
each one measures. It is **not** used by the main LM34 firmware because an LM34
is an analog sensor.

If you want to add 1-Wire sensors:

1. Connect the DS2482S-800 to **5 V**, **GND** and the I2C bus.
2. Use **hardware I2C (A4/A5)** for reliability — the Adafruit driver requires a
   `TwoWire` bus, and hardware I2C handles the DS2482's clock stretching.
3. Wire each DS18B20 to a channel `OW0`..`OW7`.
4. Flash the example sketch in
   [`examples/ds2482_8channel_scan`](../examples/ds2482_8channel_scan/ds2482_8channel_scan.ino)
   (requires the **Adafruit DS248x** library).

---

## 5. Notes on the I2C bus

- The Adafruit SSD1306 breakout includes on-board 10 kΩ pull-ups on SDA/SCL,
  which are enough for a short breadboard bus.
- If you add the DS2482S-800, keep the wires short and consider the module's own
  pull-ups as well.
- Software I2C runs noticeably slower than hardware I2C but is perfectly
  adequate for a 128×32 OLED refreshed a few times per second.
