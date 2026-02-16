/*
 * SPDX-FileCopyrightText: 2026 ZHENG Robert <robert@hase-zheng.net>
 * SPDX-License-Identifier: MIT
 *
 * @file main.cpp
 * @brief Main entry point for the FileSorter application.
 * @version 1.2.1
 * @date 2026-02-16
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 */

#include "MainWindow.hpp"
#include <QApplication>
#include <QCommandLineParser>
#include <QLibraryInfo>
#include <QLocale>
#include <QTranslator>

/**
 * @brief Main entry point.
 *
 * Initializes the Qt application, loads translations, parses command line
 * arguments, and launches the main window.
 *
 * @param argc Number of command line arguments.
 * @param argv Array of command line arguments.
 * @return int Execution return code.
 */
int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  QTranslator translator;

  // Determine system language
  QLocale systemLocale = QLocale::system();

  // Logic: If language is German, load the German file.
  // Otherwise (English, French, etc.) do nothing -> Default is English (Source
  // Code).
  if (systemLocale.language() == QLocale::German) {
    // The path ":/i18n/" was defined in CMakeLists.txt via RESOURCE_PREFIX
    if (translator.load(":/i18n/FileSorter_de.qm")) {
      a.installTranslator(&translator);
    }
  }

  // Optional: Load standard Qt dialog translations (e.g. for standard buttons
  // "Yes/No")
  if (systemLocale.language() == QLocale::German) {
    QTranslator qtTranslator;
    if (qtTranslator.load("qt_de",
                          QLibraryInfo::path(QLibraryInfo::TranslationsPath))) {
      a.installTranslator(&qtTranslator);
    }
  }

  // --- NEW: Command Line Parsing ---
  QCommandLineParser parser;
  parser.setApplicationDescription("FileSorter - Automatic file sorting");
  parser.addHelpOption();
  parser.addVersionOption();

  // Definition of the option -m / --minimized
  QCommandLineOption minimizedOption(
      QStringList() << "m" << "minimized",
      QCoreApplication::translate(
          "main", "Starts minimized and activates monitoring."));
  parser.addOption(minimizedOption);

  parser.process(a);

  // Check if the argument was set
  bool startMinimized = parser.isSet(minimizedOption);
  // ---------------------------------

  // Create MainWindow and pass argument
  MainWindow w(nullptr, startMinimized); // <--- Constructor adapted

  if (startMinimized) {
    w.showMinimized();
  } else {
    w.show();
  }
  return a.exec();
}
