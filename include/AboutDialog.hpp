/*
 * SPDX-FileCopyrightText: 2025 ZHENG Robert <robert@hase-zheng.net>
 * SPDX-License-Identifier: MIT
 *
 * @author ZHENG Robert
 * @version 1.1.0
 */
#pragma once

/**
 * @file AboutDialog.hpp
 * @brief Header file for the AboutDialog class.
 *
 * Defines the dialog window that displays application information.
 */

#include <QDialog>

/**
 * @class AboutDialog
 * @brief A dialog that shows information about the application.
 *
 * This class inherits from QDialog and displays details such as the
 * application name, version, and author.
 */
class AboutDialog : public QDialog {
  Q_OBJECT

public:
  /**
   * @brief Constructs the AboutDialog.
   * @param parent The parent widget (default is nullptr).
   */
  explicit AboutDialog(QWidget *parent = nullptr);
};
