# HID Mouse

This is a NCS (ZephyrRTOS) sample using the PMW3610 motion sensor with the nRF52840-DK. Refer to [pixart,pmw3610 (on spi bus) — Zephyr Project Documentation](https://docs.zephyrproject.org/latest/build/dts/api/bindings/input/pixart%2Cpmw3610.html).


Pin mapping:

| PMW3610 PCB | nRF52840 |
| :---------: | :------: |
|    SDIO     | `P0.31`  |
|    SCLK     | `P0.30`  |
|     CS      | `P0.29`  |
|   MOTION    | `P0.28`  |
|    RESET    |    NC    |

RTT logging backend. Example log:

```log
*** Booting nRF Connect SDK v3.1.0-6c6e5b32496e ***
*** Using Zephyr OS v4.1.99-1612683d4010 ***
[00:00:00.419,250] <inf> main: Starting PMW3610 example
[00:00:00.419,433] <inf> main: USB HID Initialized
[00:00:07.137,878] <inf> main: Motion X: 2
[00:00:07.137,908] <inf> main: Motion Y: -4
[00:00:07.141,143] <inf> main: Motion X: 4
[00:00:07.141,143] <inf> main: Motion Y: -10
[00:00:07.145,874] <inf> main: Motion X: 5
[00:00:07.145,874] <inf> main: Motion Y: -11
[00:00:07.149,139] <inf> main: Motion X: 7
[00:00:07.149,169] <inf> main: Motion Y: -14
[00:00:07.153,869] <inf> main: Motion X: 8
[00:00:07.153,900] <inf> main: Motion Y: -18
[00:00:07.157,897] <inf> main: Motion X: 8
[00:00:07.157,897] <inf> main: Motion Y: -17
[00:00:07.161,163] <inf> main: Motion X: 9
[00:00:07.161,193] <inf> main: Motion Y: -20
[00:00:07.165,893] <inf> main: Motion X: 8
[00:00:07.165,924] <inf> main: Motion Y: -19
[00:00:07.323,974] <inf> main: Motion X: 2
```
