/*
 * SPDX-FileCopyrightText: 2026 ZHENG Robert <robert@hase-zheng.net>
 * SPDX-License-Identifier: MIT
 *
 * @file FileSorter.cpp
 * @brief Implementation of the FileSorter class.
 * @version 1.2.0
 * @date 2026-02-09
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 */

#include "FileSorter.hpp"
#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>

/**
 * @brief Constructs the FileSorter object.
 *
 * Initializes the download path, file system watcher, and debounce timer.
 * Connects signals for directory changes and timer timeouts.
 *
 * @param parent The parent object.
 */
FileSorter::FileSorter(QObject *parent) : QObject(parent) {
  m_downloadPath =
      QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);

  m_watcher = new QFileSystemWatcher(this);
  m_debounceTimer = new QTimer(this);

  // Configure timer: SingleShot, e.g., wait 2 seconds after last change
  m_debounceTimer->setSingleShot(true);
  m_debounceTimer->setInterval(2000);

  connect(m_watcher, &QFileSystemWatcher::directoryChanged, this,
          &FileSorter::onDirectoryChanged);

  // When the timer elapses, we perform the sorting
  connect(m_debounceTimer, &QTimer::timeout, this, [this]() {
    if (!m_currentCategories.isEmpty()) {
      emit logMessage("Automatische Sortierung gestartet...");
      sortDownloads(m_currentCategories);
    }
  });
}

/**
 * @brief Enables or disables directory monitoring.
 *
 * Adds or removes the download path from the file system watcher.
 *
 * @param enable True to enable monitoring, false to disable.
 */
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

/**
 * @brief Updates the sorting rules.
 *
 * Stores the provided categories as the current ruleset.
 *
 * @param categories New list of categories to apply.
 */
void FileSorter::updateRules(const QList<Category> &categories) {
  m_currentCategories = categories;
  // Optional: Log that rules were updated
  emit logMessage("Regeln wurden aktualisiert.");
}

/**
 * @brief Handles directory change events.
 *
 * Starts or restarts the debounce timer to prevent excessive sorting operations
 * during rapid file changes.
 *
 * @param path The path that changed (unused, as we monitor a specific path).
 */
void FileSorter::onDirectoryChanged(const QString &path) {
  Q_UNUSED(path);
  // Restart timer (Debounce). As long as changes come in, sorting is delayed.
  if (m_debounceTimer->isActive()) {
    m_debounceTimer->start();
  } else {
    m_debounceTimer->start();
    emit logMessage(
        "Änderung erkannt. Warte auf Abschluss der Schreibvorgänge...");
  }
}

/**
 * @brief Sorts files in the download directory according to rules.
 *
 * Iterates through files in the download folder, matches them against
 * categories, and moves them to the corresponding target directories. handles
 * duplicates by renaming.
 *
 * @param categories The list of categories to use for sorting.
 */
void FileSorter::sortDownloads(const QList<Category> &categories) {
  // Lokalen Status aktualisieren
  m_currentCategories = categories;

  QDir downloadDir(m_downloadPath);
  if (!downloadDir.exists()) {
    emit logMessage(tr("Error: Download folder not found."));
    return;
  }

  // Überwachung pausieren, um Endlosschleifen durch das Verschieben zu
  // verhindern
  bool wasWatching = !m_watcher->directories().isEmpty();
  if (wasWatching)
    m_watcher->removePath(m_downloadPath);

  const auto files =
      downloadDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);

  for (const auto &fileInfo : files) {
    QString extension = fileInfo.suffix().toLower();

    // Temporäre Browser-Dateien ignorieren
    if (extension == "part" || extension == "crdownload" ||
        extension == "tmp") {
      continue;
    }

    for (const auto &cat : categories) {
      if (cat.extensions.contains(extension, Qt::CaseInsensitive)) {

        // 1. Basis-Zielpfad bestimmen
        QString targetPath =
            m_downloadPath + QDir::separator() + cat.folderName;

        // 2. Optional: Unterordner nach Datum (Jahr/Monat/Tag)
        if (cat.useDatePath) {
          QDateTime fileDate = fileInfo.lastModified();
          QString dateSubPath = fileDate.toString("yyyy") + QDir::separator() +
                                fileDate.toString("MM") + QDir::separator() +
                                fileDate.toString("dd");

          targetPath += QDir::separator() + dateSubPath;
        }

        // 3. Zielordner erstellen, falls nicht vorhanden
        if (!QDir(targetPath).exists()) {
          QDir().mkpath(targetPath);
        }

        // 4. Ziel-Dateinamen bestimmen
        QString finalFileName = fileInfo.fileName();
        QString finalFilePath = targetPath + QDir::separator() + finalFileName;

        // --- NEU: Konfliktlösung (Umbenennen statt Überspringen) ---
        if (QFile::exists(finalFilePath)) {
          // Zeitstempel generieren: YYYY-MM-DD_hhmmss_
          QString timestampPrefix =
              QDateTime::currentDateTime().toString("yyyy-MM-dd_hhmmss_");

          // Neuen Namen zusammensetzen
          finalFileName = timestampPrefix + fileInfo.fileName();
          finalFilePath = targetPath + QDir::separator() + finalFileName;

          emit logMessage(tr("Duplicate detected. Renamed to: ") +
                          finalFileName);
        }
        // -----------------------------------------------------------

        // 5. Verschieben versuchen
        if (QFile::rename(fileInfo.absoluteFilePath(), finalFilePath)) {
          emit logMessage(
              tr("Moved to %1: %2").arg(cat.folderName, finalFileName));
        } else {
          // Falls es trotz Umbenennung fehlschlägt (z.B. Datei
          // geöffnet/gesperrt)
          emit logMessage(tr("Error moving: ") + fileInfo.fileName());
        }

        // Datei wurde einer Kategorie zugeordnet -> Schleife für diese Datei
        // beenden
        break;
      }
    }
  }

  // Überwachung wieder aktivieren, falls sie vorher an war
  if (wasWatching)
    m_watcher->addPath(m_downloadPath);
}