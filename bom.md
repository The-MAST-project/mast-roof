# Bill of materials

| # | Qty | Part | Notes |
|---|-----|------|-------|
| 1 | 1 | Benewake TF02-Pro LiDAR, **UART/I2C variant** | Not the RS485 variant. 5–12 V, ≤200 mA avg, 300 mA peak. |
| 2 | 1 | Olimex ESP32-POE2 | WROVER module, LAN8720 PHY. Set the EXT PWR jumper to 12 V. |
| 3 | 1 | PoE switch or injector, 802.3af/at | Must supply ≥37 V. Passive 24 V injectors won't work. |
| 4 | 1 | Cat5e/6 Ethernet cable | Length to suit the installation. |
| 5 | 1 | Molex 51021-0400 housing (1.25 mm, 4-pin) + crimp terminals | Sensor-side connector. |
| 6 | — | 4-conductor hookup wire | Board header to sensor connector; keep under 2 m. |
| 7 | 1 | Olimex USB-ISO *(optional)* | Isolated USB for field debugging while on PoE. |

## Power budget

The ESP32-POE2 can supply up to 25 W total from PoE (12 V output: 1.5 A).
The TF02-Pro draws at most 300 mA at 12 V, well within that.
