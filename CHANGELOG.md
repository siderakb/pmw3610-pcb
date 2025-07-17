# Changelog

All notable changes to this project will be documented in this file. The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

Types of changes:

- `Added` for new features.
- `Changed` for changes in existing functionality.
- `Deprecated` for soon-to-be removed features.
- `Removed` for now removed features.
- `Fixed` for any bug fixes.
- `Security` in case of vulnerabilities.

## Unreleased

KiCad `v9.0.2`

### Changed

- `J1` pin header connector change from 1x07 to 1x08, add nRESET pin.
- Pin 8 of `J2` FFC/FPC connector changed from NC to nRESET.
- Change the footprint of `C1` and `C6` from 0603 to 0805 to make it easier to find large capacity capacitors with suitable specifications.
- `R1` nRESET pull up resistor is not longer DNI.

### Added

- Add the manufacturer and JLCPCB part number to each component.

### Removed

- Removed the test point for nRESET.

## 1.0.0 - 2024-01-28

KiCad `v7`

First release.
