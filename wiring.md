# Wiring: TF02-Pro LiDAR on Olimex ESP32-POE2

A single Cat5e cable carries power and data: PoE powers the ESP32-POE2, and the
board's 12 V output powers the sensor. Readings are served as HTTP/JSON
(see [`tf02_http/tf02_http.ino`](tf02_http/tf02_http.ino)).

```
┌──────────────┐                 ┌───────────────────┐               ┌──────────────────┐
│  PoE switch  │   Cat5e/6       │ Olimex ESP32-POE2 │               │    TF02-Pro      │
│  802.3af/at  │═════════════════│                   │               │                  │
│   (≥37 V)    │  data + power   │  12V (EXT PWR) ───┼───────────────┼── Red    +12 V   │
└──────────────┘                 │  GND ─────────────┼───────────────┼── Black  GND     │
                                 │  GPIO36 (RX) ◄────┼───────────────┼── TX (sensor out)│
                                 │  GPIO4  (TX) ─────┼──────────────►┼── RX (sensor in) │
                                 │                   │    < 2 m      │                  │
                                 │ jumper EXT PWR=12V│               │ Molex 51021-0400 │
                                 └───────────────────┘               └──────────────────┘
```

| ESP32-POE2              | TF02-Pro wire | Function            |
|-------------------------|---------------|---------------------|
| 12V (EXT PWR, jumper 12V) | Red         | +12 V supply        |
| GND                     | Black         | Ground              |
| GPIO36 (RX, input-only) | TX            | Sensor → ESP32 data |
| GPIO4 (TX)              | RX            | ESP32 → sensor data |

- UART: 3.3 V TTL, 115200 8N1.
- Sensor connector: Molex 51021-0400, 1.25 mm pitch, 4-pin.
- Keep the UART lead under 2 m.

## Cautions

- **Check red/black before power-up.** The TF02-Pro has no polarity or
  over-voltage protection.
- The ESP32-POE2 is **not galvanically isolated**: keep the sensor housing
  floating from telescope/dome ground.
- Don't plug USB in while the board is on PoE; use an Olimex USB-ISO for field
  debugging.
- The PoE source must be 802.3af/at (≥37 V). Passive 24 V injectors won't work.
