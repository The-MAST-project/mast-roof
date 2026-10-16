# Bill of materials

| # | Qty | Part | Notes |
|---|-----|------|-------|
| 1 | 1 | Benewake TF02-Pro LiDAR, **UART/I2C variant** | Not the RS485 variant. 5–12 V, ≤200 mA avg, 300 mA peak. |
| 2 | 1 | Olimex ESP32-POE2 | WROVER module, LAN8720 PHY. Set the EXT PWR jumper to 12 V. |
| 3 | 1 | PoE switch or injector, 802.3af/at | Must supply ≥37 V. Passive 24 V injectors won't work. |
| 4 | 1 | Cat5e/6 Ethernet cable | Length to suit the installation. |
| 5 | 1 | Molex 51021-0400 housing (1.25 mm, 4-pin) + Molex 50079-8000 crimp terminals | Sensor-side connector. Crimps take 26–28 AWG. May be unnecessary if the sensor ships with a pigtail. |
| 6a | 1 | Olimex UEXT cable with female jumper wires, 30 cm | Sensor GND, TX and RX from the UEXT connector; crimp the used wires into item 5. See [wiring.md](wiring.md). |
| 6b | 1 pack | Female–female jumper wires, 200 mm | One wire for +12 V from the EXT PWR header to the sensor's red wire. Keep the whole lead under 2 m. |
| 7 | 1 | Olimex BOX-ESP32-POE2 plastic enclosure | |
| 8 | 1 | Olimex DIN-CLIP-ESP32-POE2 *(optional)* | For DIN-rail mounting. |
| 9 | 1 | USB-A to USB-C cable | For programming on the bench. |
| 10 | 1 | Olimex USB-ISO *(optional)* | Isolated USB for field debugging while on PoE. |

## Suppliers

Prices are as listed on 2026-10-16 and will drift; check stock and regional
pricing (and import duties) before ordering.

| # | Supplier | Part number | Price | Alternative |
|---|----------|-------------|-------|-------------|
| 1 | [Mouser](https://www.mouser.com/c/?m=Benewake) | 234-TF02PROUART | ~US$122 | [Botland](https://botland.store/brand/285-benewake) (EU), €94 |
| 2 | [Olimex](https://www.olimex.com/Products/IoT/ESP32/ESP32-POE2/open-source-hardware) | ESP32-POE2 | €20.95 (€18.86 @ 10+) | [The Pi Hut](https://thepihut.com/products/olimex-esp32-poe2-iot-board) (UK), £22.90 |
| 3 | Any electronics/networking retailer | e.g. TP-Link TL-POE150S | — | Any true 802.3af/at injector or PoE switch port |
| 4 | [Olimex](https://www.olimex.com/Products/Components/Cables/) | Ethernet-CABLE-1M | €2.00 | Local, for longer runs |
| 5 | [RS](https://uk.rs-online.com/web/p/wire-housings-plugs/1888793) | Molex 51021-0400; crimps [Molex 50079-8000](https://uk.rs-online.com/web/p/crimp-contacts/6706423) | crimps ~£0.02 ea (100s) | [Farnell](https://cpc.farnell.com/molex/51021-0400/crimp-housing-1-25mm-4way/dp/CN12434) |
| 6a | [Olimex](https://www.olimex.com/Products/Components/Cables/) | CABLE-UEXT-JWF | €3.00 | — |
| 6b | [Olimex](https://www.olimex.com/Products/Components/Cables/) | JW-200x10-FF | €0.95 (10 pcs) | JW-200x40-FF, €3.00 (40 pcs) |
| 7 | [Olimex](https://www.olimex.com/Products/IoT/ESP32/ESP32-POE2/open-source-hardware) | BOX-ESP32-POE2 | €10.00 | — |
| 8 | [Olimex](https://www.olimex.com/Products/IoT/ESP32/ESP32-POE2/open-source-hardware) | DIN-CLIP-ESP32-POE2 | €5.95 | — |
| 9 | [Olimex](https://www.olimex.com/Products/Components/Cables/) | USB-CABLE-A-TO-C-1M | €2.40 | Local |
| 10 | [Olimex](https://www.olimex.com/Products/IoT/ESP32/ESP32-POE2/open-source-hardware) | USB-ISO | €17.95 (USB-ISO-HS €24.95) | [DigiKey](https://www.digikey.fr/en/products/detail/olimex-ltd/USB-ISO/3471376), ~€18 |

**Olimex order:** items 2, 4 and 6–10 come from one Olimex order, about
€63 before shipping and VAT (all optional items included). Olimex doesn't
sell the sensor, a PoE injector or PicoBlade connectors.

**Mouser:** order part 234-TF02PROUART specifically; Mouser also lists an I2C
variant and the wiper-equipped TF02-Pro-W.

## Power budget

The ESP32-POE2 can supply up to 25 W total from PoE (12 V output: 1.5 A).
The TF02-Pro draws at most 300 mA at 12 V, well within that.
