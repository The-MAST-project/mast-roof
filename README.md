# mast-roof

Benewake TF02-Pro LiDAR on an Olimex ESP32-POE2, serving distance readings as
HTTP/JSON. A single Cat5e cable carries everything: PoE powers the board, and
the board powers the sensor.

> **Status:** not yet compiled or tested on hardware.

## Contents

| Path | What |
|------|------|
| [`tf02_http/tf02_http.ino`](tf02_http/tf02_http.ino) | Firmware (Arduino sketch) |
| [`wiring.md`](wiring.md) | Wiring diagram, pinout and cautions |
| [`bom.md`](bom.md) | Bill of materials |

## Building

- Arduino-ESP32 core **3.x** (the `ETH.begin` signature differs from 2.x).
- Board: **OLIMEX ESP32-PoE**, with **PSRAM enabled**.
- Open `tf02_http/tf02_http.ino` and upload. The Ethernet settings match
  Olimex's own [ESP32-POE2 example](https://github.com/OLIMEX/ESP32-POE2/tree/main/SOFTWARE/ARDUINO/LAN8720-POE2).

At boot the firmware sets the sensor to 20 Hz and saves the setting.

## HTTP API

The board takes its address from DHCP and asks for the hostname `tf02`.

### `GET /`

```json
{"distance_cm":523,"strength":2140,"temp_c":31.5,"valid":true,"stale":false,
 "age_ms":12,"frames_ok":18234,"frames_bad":0}
```

- Returns **503** if no sensor frame has arrived for 500 ms (`stale: true`).
- `valid` is `false` when the reading is stale or strength is below 100 or equals 65535.

### `GET /health`

```json
{"eth_up":true,"ip":"192.168.1.50","uptime_s":3600}
```

## Safety

See [`wiring.md`](wiring.md#cautions) before powering up. In short: check
polarity (the sensor has no protection), and the ESP32-POE2 is not
galvanically isolated, so keep the sensor housing off telescope/dome ground and
don't plug in USB while on PoE.
