/*
 * SPDX-FileCopyrightText: 2026 ZHENG Robert <robert@hase-zheng.net>
 * SPDX-License-Identifier: MIT
 *
 * @file MainWindow.cpp
 * @brief Implementation of the MainWindow class.
 * @version 1.2.0
 * @date 2026-02-09
 *
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @copyright Copyright (c) 2026 ZHENG Robert
 */

#include "MainWindow.hpp"
#include <QEvent>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QStatusBar>
#include <QVBoxLayout>

#include "AboutDialog.hpp"
#include "rz_config.hpp"

/**
 * @brief Constructs a new MainWindow object.
 *
 * Initializes the UI, status bar, and loads saved settings.
 * If startMinimized is true, the application starts minimized to the tray (if
 * applicable) or just minimized window state, and monitoring is enforced.
 *
 * @param parent The parent widget.
 * @param startMinimized Flag to indicate if the application should start
 * minimized.
 */
MainWindow::MainWindow(QWidget *parent, bool startMinimized)
    : QMainWindow(parent), m_sorter(new FileSorter(this)) {

  setupUi();
  setupStatusBar();
  loadSettings(); // Loads the checkbox status as well

  // CLI logic
  if (startMinimized) {
    // Enforce activation of the checkbox -> triggers onAutoSortToggled
    // -> starts monitoring with current rules
    if (!m_autoSortCheck->isChecked()) {
      m_autoSortCheck->setChecked(true);
      m_logOutput->append(
          tr("Started in minimized mode (monitoring enforced)."));
    }
  }

  connect(m_sorter, &FileSorter::logMessage, this, [this](const QString &msg) {
    m_logOutput->append(msg);
    // Optional: Show message briefly in the status bar (for 3 seconds)
    statusBar()->showMessage(msg, 3000);
  });

  // When the user types in the table or changes checkboxes:
  connect(m_table, &QTableWidget::itemChanged, this,
          &MainWindow::onRulesModified);
}

/**
 * @brief Destroys the MainWindow object.
 *
 * Saves the current settings before destruction.
 */
MainWindow::~MainWindow() { saveSettings(); }

/**
 * @brief Sets up the user interface.
 *
 * Creates and arranges widgets, layouts, and connects signals and slots.
 */
void MainWindow::setupUi() {
  QWidget *centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);
  QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

  // Table
  m_table = new QTableWidget(0, 3, this);
  m_table->setHorizontalHeaderLabels(
      {tr("Folder Name"), tr("Extensions"), tr("use Date?")});
  m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
  m_table->horizontalHeader()->setSectionResizeMode(
      2, QHeaderView::ResizeToContents); // Checkbox needs little space

  mainLayout->addWidget(new QLabel(tr("Rules:"), this));
  mainLayout->addWidget(m_table);

  // Buttons & Checkbox
  QHBoxLayout *btnLayout = new QHBoxLayout();
  QPushButton *btnAdd = new QPushButton("+", this);
  QPushButton *btnRemove = new QPushButton("-", this);

  // Checkbox for automation
  m_autoSortCheck = new QCheckBox(tr("Automatic Monitoring"), this);

  QPushButton *btnSort = new QPushButton(tr("Sort Manually"), this);

  btnLayout->addWidget(btnAdd);
  btnLayout->addWidget(btnRemove);
  btnLayout->addWidget(m_autoSortCheck); // Insert
  btnLayout->addStretch();
  btnLayout->addWidget(btnSort);
  mainLayout->addLayout(btnLayout);

  m_logOutput = new QTextEdit(this);
  m_logOutput->setReadOnly(true);
  mainLayout->addWidget(m_logOutput);

  connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onAddCategory);
  connect(btnRemove, &QPushButton::clicked, this,
          &MainWindow::onRemoveCategory);
  connect(btnSort, &QPushButton::clicked, this, &MainWindow::onStartSort);
  connect(m_autoSortCheck, &QCheckBox::toggled, this,
          &MainWindow::onAutoSortToggled);

  setWindowTitle("FileSorter");
  resize(600, 500);
}

/**
 * @brief Adds a new category row to the table.
 *
 * Inserts a new row with default values for folder name and extensions.
 */
void MainWindow::onAddCategory() {
    m_table->blockSignals(true); // Signale temporär deaktivieren

    int row = m_table->rowCount();
    m_table->insertRow(row);

    // Standardwerte setzen
    m_table->setItem(row, 0, new QTableWidgetItem("Documents"));
    m_table->setItem(row, 1, new QTableWidgetItem("pdf, docx, odt"));

    QTableWidgetItem *checkItem = new QTableWidgetItem();
    checkItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    checkItem->setCheckState(Qt::Unchecked);
    m_table->setItem(row, 2, checkItem);

    m_table->blockSignals(false); // Signale wieder aktivieren

    onRulesModified(); // Jetzt einmalig manuell triggern, da alles bereit ist
}

/**
 * @brief Removes the currently selected category from the table.
 *
 * If a row is selected, it is removed and the rules are updated.
 */
void MainWindow::onRemoveCategory() {
  int row = m_table->currentRow();
  if (row >= 0) {
    m_table->removeRow(row);
    // NEW: Trigger update immediately after deletion
    onRulesModified();
  }
}

/**
 * @brief Manually triggers the sorting process.
 *
 * Reads the current configuration from the UI and initiates the sort.
 */
void MainWindow::onStartSort() {
  // Manual sorting also updates the rules in the sorter
  m_sorter->sortDownloads(getCategoriesFromUi());
}

/**
 * @brief Handles changes to the sorting rules.
 *
 * Called when the user modifies the table. If auto-sorting is active,
 * the new rules are immediately applied to the sorter.
 */
void MainWindow::onRulesModified() {
  // 1. Save settings (optional, but handy)
  // saveSettings();

  // 2. If monitoring is active, we must give the sorter the new rules
  if (m_autoSortCheck->isChecked()) {
    // We get the fresh data from the UI
    auto newCategories = getCategoriesFromUi();

    // Update in Sorter
    m_sorter->updateRules(newCategories);

    // Optional: Short feedback in log
    // m_logOutput->append(tr("Konfiguration geändert: Regeln aktualisiert."));
  }
}

/**
 * @brief Toggles automatic sorting monitoring.
 *
 * @param checked True if monitoring should be enabled, false otherwise.
 */
void MainWindow::onAutoSortToggled(bool checked) {
  // If we activate, we must also give the sorter the current rules
  if (checked) {
    m_sorter->sortDownloads(
        getCategoriesFromUi()); // Check once initially + set rules
  }
  m_sorter->setMonitoring(checked);
}

/**
 * @brief Retrieves the sorting categories from the UI table.
 *
 * Parses the table rows to create a list of Category objects.
 *
 * @return QList<Category> A list of configured categories.
 */
QList<Category> MainWindow::getCategoriesFromUi() const {
    QList<Category> categories;
    for (int i = 0; i < m_table->rowCount(); ++i) {
        auto itemFolder = m_table->item(i, 0);
        auto itemExt = m_table->item(i, 1);
        auto itemDate = m_table->item(i, 2);

        // Validierung: Falls ein Item noch nicht existiert (während des Hinzufügens)
        // oder der Ordnername leer ist, überspringen wir diese Zeile.
        if (!itemFolder || !itemExt || !itemDate)
            continue;

        QString folder = itemFolder->text().trimmed();
        if (folder.isEmpty())
            continue;

        // Extraktion der Extensions (erlaubt Komma, Semikolon oder Leerzeichen)
        QString rawExts = itemExt->text();
        QStringList exts = rawExts.split(QRegularExpression("[,;\\s]+"), Qt::SkipEmptyParts);

        for (auto &e : exts)
            e = e.trimmed().toLower().remove('.'); // Entfernt Punkte, falls User ".pdf" statt "pdf" schreibt

        if (exts.isEmpty())
            continue;

        bool useDate = (itemDate->checkState() == Qt::Checked);
        categories.append({folder, exts, useDate});
    }
    return categories;
}

/**
 * @brief Saves the current application settings.
 *
 * Stores the window state, auto-sort preference, and category rules to
 * persistent storage.
 */
void MainWindow::saveSettings() {
  QSettings settings("ZhengRobert", "FileSorter");

  settings.setValue("autoSort", m_autoSortCheck->isChecked());

  settings.beginWriteArray("rules");
  for (int i = 0; i < m_table->rowCount(); ++i) {
    settings.setArrayIndex(i);
    settings.setValue("folder", m_table->item(i, 0)->text());
    settings.setValue("extensions", m_table->item(i, 1)->text());

    // Save status of the checkbox
    bool isDateSort = (m_table->item(i, 2)->checkState() == Qt::Checked);
    settings.setValue("useDate", isDateSort);
  }
  settings.endArray();
}

/**
 * @brief Loads the application settings.
 *
 * Restores the window state, auto-sort preference, and category rules from
 * persistent storage.
 */
void MainWindow::loadSettings() {
  QSettings settings("ZhengRobert", "FileSorter");

  bool autoSort = settings.value("autoSort", false).toBool();
  // Bug: 1.1.0: m_autoSortCheck->setChecked(autoSort);

  int size = settings.beginReadArray("rules");
  if (size == 0) {
    settings.endArray();
    onAddCategory();
    return;
  }

  for (int i = 0; i < size; ++i) {
    settings.setArrayIndex(i);
    m_table->insertRow(i);
    m_table->setItem(i, 0,
                     new QTableWidgetItem(settings.value("folder").toString()));
    m_table->setItem(
        i, 1, new QTableWidgetItem(settings.value("extensions").toString()));

    // Restore checkbox
    bool useDate = settings.value("useDate", false).toBool();
    QTableWidgetItem *checkItem = new QTableWidgetItem();
    checkItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    checkItem->setCheckState(useDate ? Qt::Checked : Qt::Unchecked);
    m_table->setItem(i, 2, checkItem);
  }
  settings.endArray();

  // Bugfix: 1.1.0: m_autoSortCheck->setChecked(autoSort);
  m_autoSortCheck->setChecked(autoSort);
}

/**
 * @brief Initializes the status bar.
 *
 * Adds a version label to the status bar that can be double-clicked to show the
 * About dialog.
 */
void MainWindow::setupStatusBar() {
  // Create label with version
  QString versionText =
      QString("v%1").arg(QString::fromStdString(PROJECT_VERSION));
  m_versionLabel = new QLabel(versionText, this);

  // Style: A bit of padding and cursor as hand to indicate searchability
  m_versionLabel->setStyleSheet("padding: 0 5px; color: #555;");
  m_versionLabel->setCursor(Qt::PointingHandCursor);
  m_versionLabel->setToolTip(tr("Double-click for more information"));

  // Install EventFilter to catch double clicks
  m_versionLabel->installEventFilter(this);

  // Add widget right (Permanent) to status bar
  statusBar()->addPermanentWidget(m_versionLabel);
}

// EventFilter Implementation
/**
 * @brief Filters events for the MainWindow and its children.
 *
 * Specifically handles double-click events on the version label in the status
 * bar.
 *
 * @param obj The object the event is sent to.
 * @param event The event that occurred.
 * @return true if the event was handled, false otherwise.
 */
bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
  // Check if the event comes from the VersionLabel AND is a double click
  if (obj == m_versionLabel && event->type() == QEvent::MouseButtonDblClick) {
    showAboutDialog();
    return true; // Event consumed
  }

  // Standard processing for everything else
  return QMainWindow::eventFilter(obj, event);
}

/**
 * @brief Shows the About dialog.
 */
void MainWindow::showAboutDialog() {
  AboutDialog dlg(this);
  dlg.exec();
}
