# Wiring: TF02-Pro LiDAR on Olimex ESP32-POE2

A single Cat5e cable carries power and data: PoE powers the ESP32-POE2, and the
board's 12 V output powers the sensor. Readings are served as HTTP/JSON
(see [`tf02_http/tf02_http.ino`](tf02_http/tf02_http.ino)).

The sensor's data lines and ground come from the board's **UEXT** connector
(Olimex CABLE-UEXT-JWF); the sensor's +12 V comes from the **EXT PWR** header
on a separate wire.

```
┌──────────────┐               ┌──────────────────────┐                ┌──────────────────┐
│  PoE switch  │   Cat5e/6     │  Olimex ESP32-POE2   │                │    TF02-Pro      │
│  802.3af/at  │═══════════════│                      │                │                  │
│   (≥37 V)    │ data + power  │ EXT PWR 12V ─────────┼────────────────┼── Red    +12 V   │
└──────────────┘               │                      │                │                  │
                               │ UEXT 2  GND ─────────┼────────────────┼── Black  GND     │
                               │ UEXT 4  RX (GPIO36)◄─┼────────────────┼── TX (sensor out)│
                               │ UEXT 3  TX (GPIO4) ──┼───────────────►┼── RX (sensor in) │
                               │                      │     < 2 m      │                  │
                               │ jumper EXT PWR = 12V │                │ Molex 51021-0400 │
                               └──────────────────────┘                └──────────────────┘
```

| ESP32-POE2                     | TF02-Pro wire | Function            |
|--------------------------------|---------------|---------------------|
| EXT PWR 12V (jumper set to 12V) | Red          | +12 V supply        |
| UEXT pin 2 (GND)               | Black         | Ground              |
| UEXT pin 4 (GPIO36, U1RXD)     | TX            | Sensor → ESP32 data |
| UEXT pin 3 (GPIO4, U1TXD)      | RX            | ESP32 → sensor data |

Leave UEXT pin 1 (+3.3 V) and pins 5–10 unconnected.

- UART: 3.3 V TTL, 115200 8N1.
- Sensor connector: Molex 51021-0400, 1.25 mm pitch, 4-pin.
- Keep the sensor lead under 2 m.

## UEXT connector (ESP32-POE2 Rev B)

From Olimex's
[Rev B schematic](https://github.com/OLIMEX/ESP32-POE2/tree/main/HARDWARE/ESP32-PoE2_Rev_B).
Check the revision printed on your board.

| Pin | Signal           | Pin | Signal              |
|-----|------------------|-----|---------------------|
| 1   | +3.3V            | 2   | GND                 |
| 3   | GPIO4 / U1TXD    | 4   | GPIO36 / U1RXD (via diode) |
| 5   | GPIO16 / I2C-SCL | 6   | GPIO13 / I2C-SDA    |
| 7   | GPIO15           | 8   | GPIO2               |
| 9   | GPIO14           | 10  | GPIO5               |

**RX goes through a diode.** UEXT pin 4 reaches GPIO36 through a Schottky
diode (D6, 1N5819, cathode on the UEXT side), and R50 (10 kΩ) pulls GPIO36 up
to 3.3 V. The sensor can only pull the line low; the resistor pulls it back
high. That works for the TF02-Pro's 3.3 V output at 115200 baud: a low reads
about 0.4–0.5 V (under the ESP32's ~0.8 V threshold), and the line rises in
about 0.1 µs against an 8.7 µs bit time.

UEXT pin 5 is labelled GPIO16 / I2C-SCL, but GPIO16 is used by PSRAM on the
WROVER module. It's unused here; don't use UEXT I2C on this board without
checking.

## Cautions

- **Check red/black before power-up.** The TF02-Pro has no polarity or
  over-voltage protection. Don't connect the sensor's red wire to UEXT.
- The ESP32-POE2 is **not galvanically isolated**: keep the sensor housing
  floating from telescope/dome ground.
- Don't plug USB in while the board is on PoE; use an Olimex USB-ISO for field
  debugging.
- The PoE source must be 802.3af/at (≥37 V). Passive 24 V injectors won't work.
