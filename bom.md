# Bill of materials

| # | Qty | Part | Notes |
|---|-----|------|-------|
| 1 | 1 | Benewake TF02-Pro LiDAR, **UART/I2C variant** | Not the RS485 variant. 5–12 V, ≤200 mA avg, 300 mA peak. |
| 2 | 1 | Olimex ESP32-POE2 | WROVER module, LAN8720 PHY. Set the EXT PWR jumper to 12 V. |
| 3 | 1 | PoE switch or injector, 802.3af/at | Must supply ≥37 V. Passive 24 V injectors won't work. |
| 4 | 1 | Cat5e/6 Ethernet cable | Length to suit the installation. |
| 5 | 1 | Molex 51021-0400 housing (1.25 mm, 4-pin) + Molex 50079-8000 crimp terminals | Sensor-side connector. Crimps take 26–28 AWG. May be unnecessary if the sensor ships with a pigtail. |
| 6 | — | 4-conductor hookup wire, 26–28 AWG | Board header to sensor connector; keep under 2 m. |
| 7 | 1 | Olimex USB-ISO *(optional)* | Isolated USB for field debugging while on PoE. |

## Suppliers

Prices are as listed on 2026-10-16 and will drift; check stock and regional
pricing (and import duties) before ordering.

| # | Supplier | Part number | Price | Alternative |
|---|----------|-------------|-------|-------------|
| 1 | [Mouser](https://www.mouser.com/c/?m=Benewake) | 234-TF02PROUART | ~US$122 | [Botland](https://botland.store/brand/285-benewake), [Mouser IL](https://www.mouser.co.il/en/ProductDetail/Benewake/TF02-Pro-Uart?qs=DPoM0jnrROW%252BoLJwOwmOVA%3D%3D) (EU), €94 |
| 2 | [Olimex](https://www.olimex.com/Products/IoT/ESP32/ESP32-POE2/open-source-hardware) | ESP32-POE2 | €20.95 (€18.86 @ 10+) | [The Pi Hut](https://thepihut.com/products/olimex-esp32-poe2-iot-board) (UK), £22.90 |
| 3 | Any electronics/networking retailer | e.g. TP-Link TL-POE150S | — | Any true 802.3af/at injector or PoE switch port |
| 4 | [Olimex](https://www.olimex.com/Products/IoT/ESP32/ESP32-POE2/open-source-hardware) | Ethernet-CABLE-1M | €2.00 | Local, for longer runs |
| 5 | [RS](https://uk.rs-online.com/web/p/wire-housings-plugs/1888793) | Molex 51021-0400; crimps [Molex 50079-8000](https://uk.rs-online.com/web/p/crimp-contacts/6706423) | crimps ~£0.02 ea (100s) | [Farnell](https://cpc.farnell.com/molex/51021-0400/crimp-housing-1-25mm-4way/dp/CN12434) |
| 6 | Local | — | — | — |
| 7 | [Olimex](https://www.olimex.com/Products/IoT/ESP32/ESP32-POE2/open-source-hardware) | USB-ISO | €17.95 (USB-ISO-HS €24.95) | [DigiKey](https://www.digikey.fr/en/products/detail/olimex-ltd/USB-ISO/3471376), ~€18 |

**Order consolidation:** items 2, 4 and 7 can come from one Olimex order.
Olimex also sells a BOX-ESP32-POE2 enclosure (€10) and a DIN-CLIP-ESP32-POE2
rail clip (€5.95), which may suit a roof installation.

**Mouser:** order part 234-TF02PROUART specifically; Mouser also lists an I2C
variant and the wiper-equipped TF02-Pro-W.

## Power budget

The ESP32-POE2 can supply up to 25 W total from PoE (12 V output: 1.5 A).
The TF02-Pro draws at most 300 mA at 12 V, well within that.
