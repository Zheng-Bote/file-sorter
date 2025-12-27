#include "FileSorter.hpp"
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include <QDateTime>

FileSorter::FileSorter(QObject *parent) : QObject(parent) {
    m_downloadPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    
    m_watcher = new QFileSystemWatcher(this);
    m_debounceTimer = new QTimer(this);
    
    // Timer konfigurieren: SingleShot, z.B. 2 Sekunden warten nach letzter Änderung
    m_debounceTimer->setSingleShot(true);
    m_debounceTimer->setInterval(2000); 

    connect(m_watcher, &QFileSystemWatcher::directoryChanged, this, &FileSorter::onDirectoryChanged);
    
    // Wenn der Timer abläuft, führen wir die Sortierung durch
    connect(m_debounceTimer, &QTimer::timeout, this, [this]() {
        if (!m_currentCategories.isEmpty()) {
            emit logMessage("Automatische Sortierung gestartet...");
            sortDownloads(m_currentCategories);
        }
    });
}

void FileSorter::setMonitoring(bool enable) {
    if (enable) {
        if (!m_watcher->directories().contains(m_downloadPath)) {
            m_watcher->addPath(m_downloadPath);
            emit logMessage("Überwachung aktiviert für: " + m_downloadPath);
        }
    } else {
        if (m_watcher->directories().contains(m_downloadPath)) {
            m_watcher->removePath(m_downloadPath);
            emit logMessage("Überwachung deaktiviert.");
        }
    }
}

void FileSorter::updateRules(const QList<Category>& categories) {
    m_currentCategories = categories;
    // Optional: Loggen, dass Regeln aktualisiert wurden
    emit logMessage("Regeln wurden aktualisiert.");
}

void FileSorter::onDirectoryChanged(const QString &path) {
    Q_UNUSED(path);
    // Timer neu starten (Debounce). Solange Änderungen reinkommen, wird die Sortierung verzögert.
    if (m_debounceTimer->isActive()) {
        m_debounceTimer->start(); 
    } else {
        m_debounceTimer->start();
        emit logMessage("Änderung erkannt. Warte auf Abschluss der Schreibvorgänge...");
    }
}

void FileSorter::sortDownloads(const QList<Category>& categories) {
    m_currentCategories = categories;

    QDir downloadDir(m_downloadPath);
    if (!downloadDir.exists()) {
        // Fehlertexte übersetzen
        emit logMessage(tr("Error: Download folder not found."));
        return;
    }

    bool wasWatching = !m_watcher->directories().isEmpty();
    if (wasWatching) m_watcher->removePath(m_downloadPath);

    const auto files = downloadDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);

    for (const auto& fileInfo : files) {
        QString extension = fileInfo.suffix().toLower();

        if (extension == "part" || extension == "crdownload" || extension == "tmp") {
            continue;
        }

        for (const auto& cat : categories) {
            if (cat.extensions.contains(extension, Qt::CaseInsensitive)) {

                // Basis-Pfad
                QString targetPath = m_downloadPath + QDir::separator() + cat.folderName;

                // --- NEU: Datums-Logik ---
                if (cat.useDatePath) {
                    // Wir nehmen lastModified(), da dies bei Downloads meist dem Erstellzeitpunkt entspricht
                    QDateTime fileDate = fileInfo.lastModified();

                    // Format: YYYY/MM/dd
                    // QDir::separator() sorgt automatisch für Backslash (Windows) oder Slash (Unix)
                    QString dateSubPath = fileDate.toString("yyyy") + QDir::separator() +
                                          fileDate.toString("MM") + QDir::separator() +
                                          fileDate.toString("dd");

                    targetPath += QDir::separator() + dateSubPath;
                }
                // -------------------------

                if (!QDir(targetPath).exists()) {
                    // mkpath erstellt rekursiv alle fehlenden Elternordner (z.B. auch Jahr und Monat)
                    QDir().mkpath(targetPath);
                }

                QString newFilePath = targetPath + QDir::separator() + fileInfo.fileName();

                if (QFile::exists(newFilePath)) {
                    emit logMessage(tr("Skipped (exists): ") + fileInfo.fileName());
                } else {
                    if (QFile::rename(fileInfo.absoluteFilePath(), newFilePath)) {
                        emit logMessage(tr("Moved to %1: %2").arg(cat.folderName, fileInfo.fileName()));
                    }
                }
                break;
            }
        }
    }

    if (wasWatching) m_watcher->addPath(m_downloadPath);
}
