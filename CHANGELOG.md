<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
**Table of Contents**

- [Changelog](#changelog)
  - [[1.2.1] - 2026-02-16](#121---2026-02-16)
    - [Added](#added)
    - [Changed](#changed)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

# Changelog

All notable changes to this project will be documented in this file.

## [1.2.1] - 2026-02-16

### Added

- Created `CHANGELOG.md` file to track project changes.
- Generated new SBOM files for version 1.2.1 (CycloneDX and SPDX).
- Added `MACOSX_BUNDLE` configurations in `CMakeLists.txt` for native macOS application bundles.
- Added `app_icon.icns` for the macOS application icon.

### Changed

- Updated `create_cyclonedx_bom.sh` script to version 1.2.1.
- Adapted `create_dmg.sh` script to generate a drag-and-drop macOS `.dmg` installer with all Qt dependencies included.
