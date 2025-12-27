#include "MainWindow.hpp"
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QCommandLineParser>

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

    // --- NEU: Command Line Parsing ---
    QCommandLineParser parser;
    parser.setApplicationDescription("FileSorter - Automatische Dateisortierung");
    parser.addHelpOption();
    parser.addVersionOption();

    // Definition der Option -m / --minimized
    QCommandLineOption minimizedOption(QStringList() << "m" << "minimized",
                                       QCoreApplication::translate("main", "Startet minimiert und aktiviert die Überwachung."));
    parser.addOption(minimizedOption);

    parser.process(a);

    // Prüfen, ob das Argument gesetzt wurde
    bool startMinimized = parser.isSet(minimizedOption);
    // ---------------------------------

    // MainWindow erstellen und Argument übergeben
    MainWindow w(nullptr, startMinimized); // <--- Konstruktor angepasst

    if (startMinimized) {
        w.showMinimized();
    } else {
        w.show();
    }
    return a.exec();
}
