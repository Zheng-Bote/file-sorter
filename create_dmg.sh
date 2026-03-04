#!/bin/bash
# =============================================================================
# SPDX-FileComment: Script for creating a macOS dmg installer package
# SPDX-FileType: SOURCE
# SPDX-FileContributor: ZHENG Robert
# SPDX-FileCopyrightText: 2026 ZHENG Robert
# SPDX-License-Identifier: MIT
#
# @file create_dmg.sh
# @brief Script to natively build and package the FileSorter application into a .dmg.
# @version <1.2.1>
# @date <2026-03-04>
#
# @author ZHENG Robert (robert@hase-zheng.net)
# @copyright Copyright (c) 2026 ZHENG Robert
#
# @/Users/zb_bamboo/Documents/DEV/CPP/file-sorter/license MIT License
# =============================================================================

# =============================================================================
# CONFIGURATION
# =============================================================================
APP_NAME="FileSorter"
PROJECT_DIR=$(pwd)
BUILD_DIR="$PROJECT_DIR/build_macos"

echo "--- Start macOS DMG Creation ---"

# =============================================================================
# 1. FIND QT PATH
# =============================================================================
if [ -d "$HOME/Qt/6.10.2/macos" ]; then
    QT_PATH="$HOME/Qt/6.10.2/macos"
elif command -v brew &> /dev/null && [ -d "$(brew --prefix qt)" ]; then
    QT_PATH="$(brew --prefix qt)"
else
    echo "ERROR: Qt6 installation not found. Please set CMAKE_PREFIX_PATH manually."
    exit 1
fi

echo "Using Qt Path: $QT_PATH"

# =============================================================================
# 2. PREPARE BUILD ENVIRONMENT
# =============================================================================
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# =============================================================================
# 3. COMPILE
# =============================================================================
echo "--- CMake Configuration ---"
cmake "$PROJECT_DIR" \
    -DCMAKE_PREFIX_PATH="$QT_PATH" \
    -DCMAKE_BUILD_TYPE=Release

echo "--- Build ---"
cmake --build . -j$(sysctl -n hw.ncpu)

if [ $? -ne 0 ]; then
    echo "ERROR: Build failed!"
    exit 1
fi

# =============================================================================
# 4. MACDEPLOYQT (ADD DEPENDENCIES)
# =============================================================================
echo "--- Collect Qt Dependencies ---"

MACDEPLOYQT="$QT_PATH/bin/macdeployqt"

if [ ! -f "$MACDEPLOYQT" ]; then
    echo "ERROR: macdeployqt not found at $MACDEPLOYQT"
    exit 1
fi

"$MACDEPLOYQT" "$APP_NAME.app"

if [ $? -ne 0 ]; then
    echo "ERROR: macdeployqt failed."
    exit 1
fi

# =============================================================================
# 5. GENERATE DMG (WITH CREATE-DMG)
# =============================================================================
echo "--- Generate Drag-and-Drop DMG ---"

if ! command -v create-dmg &> /dev/null; then
    echo "ERROR: create-dmg not found. Please install with 'brew install create-dmg'."
    exit 1
fi

rm -f "$APP_NAME.dmg"

create-dmg \
  --volname "$APP_NAME Installer" \
  --window-pos 200 120 \
  --window-size 600 400 \
  --icon-size 100 \
  --icon "$APP_NAME.app" 150 190 \
  --hide-extension "$APP_NAME.app" \
  --app-drop-link 450 190 \
  "$APP_NAME.dmg" \
  "$APP_NAME.app/"

if [ $? -eq 0 ]; then
    echo "------------------------------------------------"
    echo "SUCCESS! Drag-and-Drop DMG created:"
    ls -lh *.dmg
    echo "------------------------------------------------"
else
    echo "ERROR: create-dmg failed."
    exit 1
fi
