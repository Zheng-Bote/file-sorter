/*
 * SPDX-FileCopyrightText: 2025 ZHENG Robert <robert@hase-zheng.net>
 * SPDX-License-Identifier: MIT
 *
 * @author ZHENG Robert
 * @version 1.1.0
 */
#pragma once

/**
 * @file FileSorter.hpp
 * @brief Header file for the FileSorter class and related structures.
 *
 * Defines the core logic for monitoring and sorting files.
 */

#include <QFileSystemWatcher>
#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QTimer>

/**
 * @struct Category
 * @brief Represents a sorting category/rule.
 */
struct Category {
  QString folderName; ///< Name of the target folder.
  QStringList
      extensions;   ///< List of file extensions belonging to this category.
  bool useDatePath; ///< Whether to include the current date in the path.
};

/**
 * @class FileSorter
 * @brief Handles the sorting logic for files.
 *
 * This class monitors a directory and moves files into subdirectories based on
 * defined categories and rules.
 */
class FileSorter : public QObject {
  Q_OBJECT

public:
  /**
   * @brief Constructs the FileSorter object.
   * @param parent The parent object (default is nullptr).
   */
  explicit FileSorter(QObject *parent = nullptr);

  /**
   * @brief Main function to sort downloads based on the provided categories.
   * @param categories A list of categories/rules for sorting files.
   */
  void sortDownloads(const QList<Category> &categories);

  /**
   * @brief Enables or disables automatic directory monitoring.
   * @param enable True to enable monitoring, false to disable.
   */
  void setMonitoring(bool enable);

  /**
   * @brief Updates the sorting rules with new categories.
   * @param categories The new list of categories.
   */
  void updateRules(const QList<Category> &categories);

public slots:
  /**
   * @brief Slot called when the monitored directory changes.
   * @param path The path of the directory that changed.
   */
  void onDirectoryChanged(const QString &path);

signals:
  /**
   * @brief Signal emitted to log a message.
   * @param message The message string to log.
   */
  void logMessage(const QString &message);

private:
  QString m_downloadPath; ///< Path to the directory being monitored.
  QFileSystemWatcher
      *m_watcher;          ///< Watcher implementation for directory monitoring.
  QTimer *m_debounceTimer; ///< Timer to wait for file operations to complete
                           ///< (debounce).
  QList<Category>
      m_currentCategories; ///< Stores current rules for auto-sorting.
};
