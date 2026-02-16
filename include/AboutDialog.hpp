/*
 * SPDX-FileCopyrightText: 2026 ZHENG Robert <robert@hase-zheng.net>
 * SPDX-License-Identifier: MIT
 *
 * @file AboutDialog.hpp
 * @brief Header file for the AboutDialog class.
 * @version 1.2.1
 * @date 2026-02-16
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 */
#pragma once

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
