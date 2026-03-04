#!/usr/bin/bash
# =============================================================================
# SPDX-FileComment: Script for creating a Linux AppImage package
# SPDX-FileType: SOURCE
# SPDX-FileContributor: ZHENG Robert
# SPDX-FileCopyrightText: 2026 ZHENG Robert
# SPDX-License-Identifier: MIT
#
# @file create_appimage.sh
# @brief Script to build and package FileSorter into an AppImage on Linux.
# @version <1.2.1>
# @date <2026-03-04>
#
# @author ZHENG Robert (robert@hase-zheng.net)
# @copyright Copyright (c) 2026 ZHENG Robert
#
# @/Users/zb_bamboo/Documents/DEV/CPP/file-sorter/license MIT License
# =============================================================================
set -e

# Konfiguration
PROJECT_NAME="FileSorter"
BUILD_DIR="build" # Dein CMake Build Ordner
APP_DIR="AppDir"

# 1. Tools herunterladen (falls nicht vorhanden)
if [ ! -f "linuxdeploy-x86_64.AppImage" ]; then
    wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
    chmod +x linuxdeploy-x86_64.AppImage
fi

if [ ! -f "linuxdeploy-plugin-qt-x86_64.AppImage" ]; then
    wget https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage
    chmod +x linuxdeploy-plugin-qt-x86_64.AppImage
fi

# 2. AppDir vorbereiten
rm -rf $APP_DIR
mkdir -p $APP_DIR

# 3. Installation ins AppDir simulieren (DESTDIR ist der Trick!)
# Wir gehen davon aus, dass 'make install' bereits konfiguriert ist
echo "Installiere Projekt in AppDir..."
DESTDIR=$(readlink -f $APP_DIR) cmake --install $BUILD_DIR --prefix /usr

# 4. AppImage erstellen
echo "Erstelle AppImage..."

# Umgebungsvariablen für das Qt Plugin
#export QMAKE=$(which qmake) # Sucht qmake im Pfad. Falls Qt manuell installiert ist: /pfad/zu/qt/bin/qmake angeben
export QMAKE=/opt/Qt/6.10.0/gcc_64/bin/qmake

# linuxdeploy aufrufen
./linuxdeploy-x86_64.AppImage \
    --appdir $APP_DIR \
    --plugin qt \
    --output appimage \
    --icon-file resources/app_icon.png \
    --desktop-file resources/FileSorter.desktop \
    --executable $APP_DIR/usr/bin/$PROJECT_NAME

echo "Fertig! Das AppImage liegt im aktuellen Ordner."