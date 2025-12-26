#include "MainWindow.hpp"
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QTranslator translator;

    // Systemsprache ermitteln
    QLocale systemLocale = QLocale::system();

    // Logik: Wenn Sprache Deutsch ist, lade die deutsche Datei.
    // Ansonsten (Englisch, Französisch, etc.) mache nichts -> Default ist Englisch (Source Code).
    if (systemLocale.language() == QLocale::German) {
        // Der Pfad ":/i18n/" wurde im CMakeLists.txt via RESOURCE_PREFIX definiert
        if (translator.load(":/i18n/FileSorter_de.qm")) {
            a.installTranslator(&translator);
        }
    }

    // Optional: Standard Qt-Dialog-Übersetzungen (z.B. für Standard-Buttons "Yes/No") laden
    if (systemLocale.language() == QLocale::German) {
        QTranslator qtTranslator;
        if (qtTranslator.load("qt_de", QLibraryInfo::path(QLibraryInfo::TranslationsPath))) {
            a.installTranslator(&qtTranslator);
        }
    }

    MainWindow w;
    w.show();

    return a.exec();
}
