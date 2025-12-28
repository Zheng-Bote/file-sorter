/*
 * SPDX-FileCopyrightText: 2025 ZHENG Robert <robert@hase-zheng.net>
 * SPDX-License-Identifier: MIT
 *
 * @author ZHENG Robert
 * @version 1.1.0
 */
/**
 * @file FileSorter.cpp
 * @brief Implementation of the FileSorter class.
 */

#include "FileSorter.hpp"
#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>

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

void FileSorter::updateRules(const QList<Category> &categories) {
  m_currentCategories = categories;
  // Optional: Log that rules were updated
  emit logMessage("Regeln wurden aktualisiert.");
}

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

void FileSorter::sortDownloads(const QList<Category> &categories) {
  m_currentCategories = categories;

  QDir downloadDir(m_downloadPath);
  if (!downloadDir.exists()) {
    // Translate error texts
    emit logMessage(tr("Error: Download folder not found."));
    return;
  }

  bool wasWatching = !m_watcher->directories().isEmpty();
  if (wasWatching)
    m_watcher->removePath(m_downloadPath);

  const auto files =
      downloadDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);

  for (const auto &fileInfo : files) {
    QString extension = fileInfo.suffix().toLower();

    if (extension == "part" || extension == "crdownload" ||
        extension == "tmp") {
      continue;
    }

    for (const auto &cat : categories) {
      if (cat.extensions.contains(extension, Qt::CaseInsensitive)) {

        // Base path
        QString targetPath =
            m_downloadPath + QDir::separator() + cat.folderName;

        // --- NEW: Date Logic ---
        if (cat.useDatePath) {
          // We use lastModified() as this usually corresponds to the creation
          // time for downloads
          QDateTime fileDate = fileInfo.lastModified();

          // Format: YYYY/MM/dd
          // QDir::separator() automatically handles Backslash (Windows) or
          // Slash (Unix)
          QString dateSubPath = fileDate.toString("yyyy") + QDir::separator() +
                                fileDate.toString("MM") + QDir::separator() +
                                fileDate.toString("dd");

          targetPath += QDir::separator() + dateSubPath;
        }
        // -----------------------

        if (!QDir(targetPath).exists()) {
          // mkpath recursively creates all missing parent folders (e.g.
          // including year and month)
          QDir().mkpath(targetPath);
        }

        QString newFilePath =
            targetPath + QDir::separator() + fileInfo.fileName();

        if (QFile::exists(newFilePath)) {
          emit logMessage(tr("Skipped (exists): ") + fileInfo.fileName());
        } else {
          if (QFile::rename(fileInfo.absoluteFilePath(), newFilePath)) {
            emit logMessage(
                tr("Moved to %1: %2").arg(cat.folderName, fileInfo.fileName()));
          }
        }
        break;
      }
    }
  }

  if (wasWatching)
    m_watcher->addPath(m_downloadPath);
}
