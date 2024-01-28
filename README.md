# PMW3610 PCB

<img src="https://i.imgur.com/gLDkCWS.jpeg" width="450">

[PixArt PMW3610DM-SUDU](https://www.pixart.com/products-detail/21/PMW3610DM-SUDU) low power laser mouse sensor breakout board, can be operated with either a 1.8V or 3.3V voltage level. Alternative to [PMW3360 PCB](https://github.com/siderakb/pmw3360-pcb).

[Demo video](https://youtu.be/nyLZ6U8h7f0?t=17)

## PCB

Online preview avaliable [here](https://kicanvas.org/?github=https%3A%2F%2Fgithub.com%2Fsiderakb%2Fpmw3610-pcb), powered by [KiCanvas](https://github.com/theacodes/kicanvas).

<img src="https://i.imgur.com/sCI6LZP.png" width="450">

### BOM

|Ref|Value|Footprint|
|-|-|-|
|C1|3.3uF/16V|SMD 0603|
|C2, C3|100nF|SMD 0603|
|C4|100nF X7R|SMD 0603|
|C5|10nF|SMD 0603|
|C6|10uF X7R|SMD 0603|
|C7|10nF X7R|SMD 0603|
|C8, C9|1uF|SMD 0603|
|C10| DNI |SMD 0603|
|R1|10k|SMD 0603|
|U1|PMW3610|16 Pin|
|U2|TLV74318|SOT-23-5|
|J1|2.54mm Pin header||
|J2|8P 0.5mm FFC/FPC|AFC01-S08FCA-00|
- DNI: don't install.
- SMD 0603 (Imperial) aka 1608 Metric.
- PMW3610 needs to be used with LM18-LSI lens.
- `R1` is optional/additional RESET pull up resistor, RESET pin of PMW3610 has a built in weak pull up circuit.
- `U2` LDO can be replace by RT9193-18GB, change `C10` to 22nF.
- The wiring order of `J1` and `J2` is different.

### Board Characteristics

- Copper layer count: 2
- Board thickness: 1.6 mm
- Board overall dimensions: 23.5 x 31.5 mm
- Min track/spacing: 0.15 mm / 0.15 mm
- Min hole diameter: 0.3 mm
- Castellated pads: No
- Edge card connectors: No
- Plated board edge: No

## Power

### Voltage level config

- 3.3V logic
  - VIN: 3.3V
  - JP1: `[1-2 3]`
- 1.8V logic
  - VIN: (VDD+0.2V) \~ 5.5V
  - JP1: `[1 2-3]`

|JP1||Description|
|-|-|-|
|`[1-2 3]`|2 and 1 close, 3 NC| VIO = VIN |
|`[1 2-3]`|2 and 3 close, 1 NC| VIO = VDD |
|`[1-2-3]`|1, 2 and 3 close| VIO = VDD = VIN (LDO bypass) |

### PMW3610 Spec

- VDD: 1.7V\~2.1V, 1.8V Typical.
- VDDIO: 1.7V\~3.3V, 1.8V Typical.

## License

Available under the [CERN-OHL-P v2](/LICENSE) permissive license.
