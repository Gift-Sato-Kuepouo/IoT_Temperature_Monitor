# Changelog

All notable changes to this project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2026-10-13

### Added
- Complete Arduino firmware for the LM34 temperature monitor:
  - `config.h` compile-time configuration (pins, thresholds, timing).
  - `LM34` analog temperature driver with averaging and calibration.
  - `Display` module driving the SSD1306 128x32 OLED over software I2C.
  - Non-blocking `Alarm` module (PWM LED + passive buzzer).
  - Main sketch with non-blocking sampling, display, alarm and serial logging.
- Optional `ds2482_8channel_scan` example for the DS2482S-800 + DS18B20 sensors.
- `docs/wiring.md` with the full pin map and assembly notes.
- Project documentation: README, CHANGELOG and `hardware/` reference files.
- Fritzing schematic (`.fzz`) and schematic image (`.png`).
