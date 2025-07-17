# PMW3610 PCB

[![license](https://img.shields.io/badge/License-CERN--OHL--P_v2-limegreen)](https://github.com/siderakb/pmw3610-pcb/blob/main/LICENSE)
[![kicad](https://img.shields.io/badge/KiCad-v9-orange)](https://github.com/siderakb/pmw3610-pcb/blob/main/pmw3610_pcb.kicad_pro)

<img src="https://i.imgur.com/gLDkCWS.jpeg" width="450">

Breakout board for the [PixArt PMW3610DM-SUDU](https://www.pixart.com/products-detail/21/PMW3610DM-SUDU), a low-power laser mouse sensor. An alternative to the [PMW3360 PCB](https://github.com/siderakb/pmw3360-pcb) for low-power applications.

Features:

- Operates at either 1.8V or 3.3V voltage levels.
- Supports both 2.54mm pin headers and 0.5mm-pitch FFC/FPC connectors.
- The optical center of the sensor is located in the center of the PCB.
- Compatible with [ZMK](https://zmk.dev/) projects. [Demo video](https://youtu.be/nyLZ6U8h7f0?t=17)

## PCB

Online preview available [here](https://kicanvas.org/?github=https%3A%2F%2Fgithub.com%2Fsiderakb%2Fpmw3610-pcb), powered by [KiCanvas](https://github.com/theacodes/kicanvas).

<a href="https://raw.githubusercontent.com/siderakb/pmw3610-pcb/refs/heads/main/fig/schematic.webp" target="_blank">
  <img src="/fig/schematic.webp" alt="schematic" style="height: 350px">
</a>
<a href="https://raw.githubusercontent.com/siderakb/pmw3610-pcb/refs/heads/main/fig/layout.webp" target="_blank">
  <img src="/fig/layout.webp" alt="layout" style="height: 350px">
</a>

### BOM

| Ref        | Value             | Footprint       |
| ---------- | ----------------- | --------------- |
| C1         | 3.3uF/16V         | SMD 0805        |
| C2, C3, C4 | 100nF             | SMD 0603        |
| C5, C7     | 10nF              | SMD 0603        |
| C6         | 10uF              | SMD 0805        |
| C8, C9     | 1uF               | SMD 0603        |
| C10        | *DNI*             | SMD 0603        |
| R1         | 10k               | SMD 0603        |
| U1         | PMW3610           | DIP-16          |
| U2         | TLV74318          | SOT-23-5        |
| J1         | 2.54mm Pin header | 2.54mm 1x07     |
| J2         | 8P 0.5mm FFC/FPC  | AFC01-S08FCA-00 |

- *DNI*: Do Not Install.
- SMD 0603 (Imperial) aka 1608 Metric; SMD 0805 (Imperial) aka 2012 Metric.
- The PMW3610 must be used with an LM18-LSI lens.
- The `U2` LDO (TLV74318) can be replaced with an RT9193-18GB. If using this replacement, set `C10` to 22nF.
- Recommended capacitor specifications:
  - Dielectric material: X7R
  - Tolerance: +/- 10%
  - Rated working voltage: 6.3 \~ 16V

### Board Characteristics

- Copper layer count: 2
- Board thickness: 1.6 mm
- Board overall dimensions: 23.5 x 31.5 mm
- Min track/spacing: 0.15 mm / 0.15 mm
- Min hole diameter: 0.3 mm
- Castellated pads: No
- Edge card connectors: No
- Plated board edge: No
- Mounting hole: M3 x4

## Connector

|  Pin  | J1 PinHeader | J2 FFC/FPC |
| :---: | :----------- | :--------- |
|   1   | VIN          | GND        |
|   2   | GND          | VIN        |
|   3   | SDIO         | nCS        |
|   4   | SCLK         | --         |
|   5   | nCS          | SDIO       |
|   6   | MOTION       | SCLK       |
|   7   | nRESET       | MOTION     |
|   8   | --           | nRESET     |

## Power

### Voltage Level Config

This circuit is designed with a VDD of 1.8V. VDDIO is the voltage used by the IO pin (e.g. SCLK, SDIO), and can be configured for either 3.3V or 1.8V logic.

Most of the current MCU IOs are 3.3V, please use ***3.3V Logic*** config for this MCU. This board does NOT support 5V logic. If using a 5V MCU, please add a level shifter such as the TXS0108E.

| Config           |    JP1    | VIN Min. | VIN Max. |
| :--------------- | :-------: | :------- | :------- |
| ***3.3V Logic*** | `[1-2 3]` | 3.3V     | 3.3V     |
| ***1.8V Logic*** | `[1 2-3]` | 2.1V\*   | 5.5V     |
| ***LDO Bypass*** | `[1-2-3]` | 1.7V     | 2.1V     |

*2.1V = 1.8V + 0.3V, where 0.3V is the dropout voltage of the TLV74318 (at Tj ≤ 125ºC).*

### Jumper

| JP1       | Description             | Effect                         |
| --------- | ----------------------- | ------------------------------ |
| `[1-2 3]` | 2 and 1 shorted; 3 *NC* | VDDIO = VIN                    |
| `[1 2-3]` | 2 and 3 shorted; 1 *NC* | VDDIO = VDD = 1.8V             |
| `[1-2-3]` | 1, 2 and 3 shorted      | VDDIO = VDD = VIN (LDO bypass) |

*NC: Not Connected.*

## PMW3610 Spec

| Parameter                                    | Min.  | Typ.  | Max.  | Unit |
| :------------------------------------------- | :---: | :---: | :---: | :--- |
| VDD                                          |  1.7  |  1.8  |  2.1  | V    |
| VDDIO                                        |  1.7  |  1.8  |  3.3  | V    |
| SCLK Frequency                               |       |       |   2   | MHz  |
| Distance from lens reference plan to surface |  2.2  |  2.4  |  2.6  | mm   |
| Operating Temperature                        |   0   |       |  40   | ºC   |
| Resolution                                   |  200  |       | 3200  | cpi  |

*The above table is for reference only.*

## License

Available under the [CERN-OHL-P v2](/LICENSE) permissive license.
