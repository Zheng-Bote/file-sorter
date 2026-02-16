/*
 * SPDX-FileCopyrightText: 2026 ZHENG Robert <robert@hase-zheng.net>
 * SPDX-License-Identifier: MIT
 *
 * @file AboutDialog.cpp
 * @brief Implementation of the AboutDialog class.
 * @version 1.2.1
 * @date 2026-02-16
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 */

#include "AboutDialog.hpp"
#include "rz_config.hpp" // Here we get the data

#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

/**
 * @brief Constructs the AboutDialog object.
 *
 * Sets up the UI elements to display application information including version,
 * description, copyright, and technical details.
 *
 * @param parent The parent widget.
 */
AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent) {
  setWindowTitle(tr("About") + " " + QString::fromStdString(PROG_LONGNAME));
  setModal(true);
  resize(400, 300);

  QVBoxLayout *layout = new QVBoxLayout(this);

  // Title & Version
  QLabel *titleLabel = new QLabel(
      QString("<h2>%1</h2>").arg(QString::fromStdString(PROG_LONGNAME)));
  titleLabel->setAlignment(Qt::AlignCenter);

  QLabel *versionLabel = new QLabel(
      tr("Version: %1").arg(QString::fromStdString(PROJECT_VERSION)));
  versionLabel->setAlignment(Qt::AlignCenter);

  // Description
  QLabel *descLabel = new QLabel(QString::fromStdString(PROJECT_DESCRIPTION));
  descLabel->setWordWrap(true);
  descLabel->setAlignment(Qt::AlignCenter);

  // Copyright & Author
  QLabel *copyLabel =
      new QLabel(QString("© %1 %2").arg(QString::fromStdString(PROG_CREATED),
                                        QString::fromStdString(PROG_AUTHOR)));
  copyLabel->setAlignment(Qt::AlignCenter);

  // Link (GitHub)
  // We use HTML <a> tags. setOpenExternalLinks(true) is crucial.
  QString linkHtml = QString("<a href='%1'>%1</a>")
                         .arg(QString::fromStdString(PROJECT_HOMEPAGE_URL));
  QLabel *linkLabel = new QLabel(linkHtml);
  linkLabel->setOpenExternalLinks(true); // Opens default browser
  linkLabel->setAlignment(Qt::AlignCenter);

  // Technical Info (Optional, but cool)
  QLabel *techLabel =
      new QLabel(QString("<small>Built with Qt %1 (%2) on %3</small>")
                     .arg(QString::fromStdString(CMAKE_QT_VERSION))
                     .arg(QString::fromStdString(CMAKE_CXX_COMPILER))
                     .arg(QString::fromStdString(CMAKE_CXX_STANDARD)));
  techLabel->setAlignment(Qt::AlignCenter);
  techLabel->setStyleSheet("color: gray;");

  // Close Button
  QPushButton *btnClose = new QPushButton(tr("Close"));
  connect(btnClose, &QPushButton::clicked, this, &QDialog::accept);

  // Add widgets
  layout->addWidget(titleLabel);
  layout->addWidget(versionLabel);
  layout->addSpacing(10);
  layout->addWidget(descLabel);
  layout->addSpacing(10);
  layout->addWidget(linkLabel);
  layout->addSpacing(10);
  layout->addWidget(copyLabel);
  layout->addWidget(techLabel);
  layout->addStretch();
  layout->addWidget(btnClose);
}
