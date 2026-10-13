# Hardware

Design files for the IoT Temperature Monitor.

| File                              | Description                                             |
| --------------------------------- | ------------------------------------------------------- |
| `IoT-Temp-Monitor_schematic.fzz`  | Fritzing source file (breadboard / schematic / PCB).    |
| `IoT-Temp-Monitor_schematic.png`  | Exported schematic image for quick reference.           |

Open the `.fzz` file with [Fritzing](https://fritzing.org/) (free, open source).

> **Note:** the schematic contains three connections that were corrected in the
> final build. See [`../docs/wiring.md`](../docs/wiring.md#3-corrections-applied-to-the-original-schematic)
> for details:
>
> 1. The LM34 output must go to an analog pin (A0), not to the DS2482 OW0.
> 2. The Uno's hardware I2C is on A4/A5, not A2/A3 (firmware defaults to
>    software I2C on A2/A3 to match the drawing).
> 3. The LED cathode must be connected to GND.
