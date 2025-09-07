# Changelog

All notable changes to this project will be documented in this file. The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

Types of changes:

- `Added` for new features.
- `Changed` for changes in existing functionality.
- `Deprecated` for soon-to-be removed features.
- `Removed` for now removed features.
- `Fixed` for any bug fixes.
- `Security` in case of vulnerabilities.

<!-- ## Unreleased -->

## 2.0.0 - 2025-09-07

KiCad `v9.0.2`

### Changed

- `J1` pin header connector changed from 1x07 to 1x08, adding the nRESET pin.
- Pin 8 of `J2` FFC/FPC connector changed from NC to nRESET.
- Changed the `J2` FFC/FPC connector to the top layer,
- Changed the footprint of `C1` and `C6` from 0603 to 0805 to make it easier to find large-capacity capacitors with recommended specifications.
- `R1` nRESET pull-up resistor is no longer DNI
- Updated PMW3610 footprint.
- Increased the copper-to-board edge clearance to 0.4mm for better V-cut panelization.

### Added

- Added the manufacturer and JLCPCB part number to each component for PCBA.

### Removed

- Removed the test point for nRESET.

## 1.0.0 - 2024-01-28

KiCad `v7`

First release.
