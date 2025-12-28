/*
 * SPDX-FileCopyrightText: 2025 ZHENG Robert <robert@hase-zheng.net>
 * SPDX-License-Identifier: MIT
 *
 * @author ZHENG Robert
 * @version 1.1.0
 */
/**
 * @file main.cpp
 * @brief Main entry point for the FileSorter application.
 */

#include "MainWindow.hpp"
#include <QApplication>
#include <QCommandLineParser>
#include <QLibraryInfo>
#include <QLocale>
#include <QTranslator>

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
  parser.setApplicationDescription("FileSorter - Automatische Dateisortierung");
  parser.addHelpOption();
  parser.addVersionOption();

  // Definition of the option -m / --minimized
  QCommandLineOption minimizedOption(
      QStringList() << "m" << "minimized",
      QCoreApplication::translate(
          "main", "Startet minimiert und aktiviert die Überwachung."));
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
