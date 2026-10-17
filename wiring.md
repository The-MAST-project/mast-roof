# Wiring: TF02-Pro LiDAR on Olimex ESP32-POE2

A single Cat5e cable carries power and data: PoE powers the ESP32-POE2, and the
board's 12 V output powers the sensor. Readings are served as HTTP/JSON
(see [`tf02_http/tf02_http.ino`](tf02_http/tf02_http.ino)).

The sensor's data lines and ground come from the board's **UEXT** connector
(Olimex CABLE-UEXT-JWF); the sensor's +12 V comes from the **EXT PWR** header
on a separate wire.

```
┌──────────────┐               ┌──────────────────────┐                ┌──────────────────┐
│  PoE source  │   Cat5e/6     │  Olimex ESP32-POE2   │                │    TF02-Pro      │
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
| 5   | GPIO33 / I2C-SCL | 6   | GPIO13 / I2C-SDA    |
| 7   | GPIO15           | 8   | GPIO2               |
| 9   | GPIO14           | 10  | GPIO5               |

**RX goes through a diode.** UEXT pin 4 reaches GPIO36 through a Schottky
diode (D6, 1N5819, cathode on the UEXT side), and R50 (10 kΩ) pulls GPIO36 up
to 3.3 V. The sensor can only pull the line low; the resistor pulls it back
high. That works for the TF02-Pro's 3.3 V output at 115200 baud: a low reads
about 0.4–0.5 V (under the ESP32's ~0.8 V threshold), and the line rises in
about 0.1 µs against an 8.7 µs bit time.

UEXT pin 5 is GPIO33. The schematic net is still named "GPIO16\I2C-SCL" from
the original ESP32-POE, but on the POE2 it reaches GPIO33 through R19 (10 Ω),
because GPIO16 belongs to the WROVER's PSRAM. Olimex's ESPHome config for the
POE2 also uses GPIO33 for UEXT SCL.

## Installation

The PoE injector sits in the network cabinet; Cat6 in the walls carries data
and power to the mast.

```
        network cabinet                                         mast
┌────────┐  patch  ┌───────────────────┐  Cat6 in walls  ┌───────────┐  patch  ┌────────────┐
│ switch ├─────────┤ DATA IN   PoE OUT ├═════════════════╡ wall jack ╞═════════╡ ESP32-POE2 │
└────────┘         │   PoE injector    │  data + 48 V    └───────────┘         └────────────┘
                   └───────────────────┘
```

- Switch goes to the injector's **data in** port; **PoE out** goes to the
  wall. Don't swap them.
- 100 m maximum from switch port to board, patch cords included.
- The injector must be 802.3af/at with its output isolated from mains (≥1500 V;
  check the datasheet). The board's ground then floats, so the mast end is the
  only place it can pick up earth.

### Isolation at the mast

The ESP32-POE2 is not isolated: board ground is at PoE potential. Nothing on
it may touch anything earthed.

- **Sensor housing:** before mounting, check with a multimeter whether the
  housing connects to the sensor's black wire. If it does, mount the sensor on
  an insulating plate with nylon screws.
- **Board:** keep it in the plastic enclosure. No metal standoffs to a grounded
  plate; check what a DIN rail is fixed to.
- **Cable shield:** if the in-wall Cat6 is shielded and bonded in the cabinet,
  use an unshielded patch cord at the mast end so the shield doesn't reach the
  board's RJ45.
- **USB:** never connect a grounded PC while on PoE (use a USB-ISO).
- **Verify:** after installation, with PoE unplugged, measure between UEXT
  pin 2 (GND) and the mast structure. It must read open (>1 MΩ). Repeat after
  any maintenance, and label the enclosure "keep isolated from ground".

### Surge protection

If the cable leaves the building to reach the roof, fit a PoE-rated Ethernet
surge protector where it exits, bonded to building earth there, not at the
mast.

## Cautions

- **Check red/black before power-up.** The TF02-Pro has no polarity or
  over-voltage protection. Don't connect the sensor's red wire to UEXT.
- The ESP32-POE2 is **not galvanically isolated**: keep the board and sensor
  housing floating from telescope/dome ground (see
  [Isolation at the mast](#isolation-at-the-mast)).
- Don't plug USB in while the board is on PoE; use an Olimex USB-ISO for field
  debugging.
- The PoE source must be 802.3af/at (≥37 V). Passive 24 V injectors won't work.
