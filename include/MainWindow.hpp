/*
 * SPDX-FileCopyrightText: 2025 ZHENG Robert <robert@hase-zheng.net>
 * SPDX-License-Identifier: MIT
 *
 * @author ZHENG Robert
 * @version 1.1.0
 */
#pragma once

#include <QCheckBox>
#include <QLabel>
/**
 * @file MainWindow.hpp
 * @brief Header file for the MainWindow class.
 *
 * Defines the main GUI window for the FileSorter application.
 */

#include <QMainWindow>
#include <QTableWidget>
#include <QTextEdit>

#include "FileSorter.hpp"

/**
 * @class MainWindow
 * @brief The main application window.
 *
 * Handles the user interface, including configuration of sorting categories,
 * log display, and manual/auto sorting controls.
 */
class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  /**
   * @brief Constructs the MainWindow.
   * @param parent The parent widget (default is nullptr).
   * @param startMinimized Whether the window should start minimized (default is
   * false).
   */
  explicit MainWindow(QWidget *parent = nullptr, bool startMinimized = false);

  /**
   * @brief Destructor.
   */
  ~MainWindow();

protected:
  /**
   * @brief Event filter to handle specific events.
   *
   * Used here to catch double-click events or other custom interactions.
   * @param obj The object the event is sent to.
   * @param event The event that occurred.
   * @return True if the event was filtered/handled, false otherwise.
   */
  bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
  void onAddCategory();    ///< Slot to add a new category.
  void onRemoveCategory(); ///< Slot to remove the selected category.
  void onStartSort();      ///< Manual trigger to start sorting.
  void
  onAutoSortToggled(bool checked); ///< Slot for the auto-sort checkbox toggle.
  void saveSettings();             ///< Saves current settings to storage.
  void loadSettings();             ///< Loads settings from storage.
  void showAboutDialog(); ///< Displays the dialog with application info.
  void onRulesModified(); ///< Called when sorting rules are modified.

private:
  void setupUi();        ///< Sets up the user interface components.
  void setupStatusBar(); ///< Initializes the status bar.

  /**
   * @brief Retrieves the list of categories from the UI table.
   * @return A list of Category objects configured in the UI.
   */
  QList<Category> getCategoriesFromUi() const;

  /**
   * @brief Updates the FileSorter instance with current rules.
   *
   * Helper function to avoid duplicating update logic.
   */
  void updateSorterInstance();

  QTableWidget
      *m_table; ///< Table widget for displaying and editing categories.
  QTextEdit *m_logOutput;     ///< Text area for log messages.
  QCheckBox *m_autoSortCheck; ///< Checkbox to toggle auto-sorting.
  FileSorter *m_sorter;       ///< Pointer to the FileSorter logic instance.
  QLabel *m_versionLabel;     ///< Label displaying the application version.
};
