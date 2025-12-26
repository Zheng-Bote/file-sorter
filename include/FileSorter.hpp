#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QList>
#include <QFileSystemWatcher>
#include <QTimer>

struct Category {
    QString folderName;
    QStringList extensions;
    bool useDatePath;
};

class FileSorter : public QObject {
    Q_OBJECT

public:
    explicit FileSorter(QObject *parent = nullptr);

    // Hauptfunktion zum Sortieren
    void sortDownloads(const QList<Category>& categories);

    // Aktiviert/Deaktiviert die automatische Überwachung
    void setMonitoring(bool enable);

public slots:
    // Slot, der aufgerufen wird, wenn sich im Ordner was tut
    void onDirectoryChanged(const QString &path);

signals:
    void logMessage(const QString &message);

private:
    QString m_downloadPath;
    QFileSystemWatcher *m_watcher;
    QTimer *m_debounceTimer; // Wartet kurz, bis Schreibvorgänge fertig sind
    QList<Category> m_currentCategories; // Speichert aktuelle Regeln für den Auto-Sort
};
