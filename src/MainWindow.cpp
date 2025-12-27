#include "MainWindow.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QHeaderView>
#include <QSettings>
#include <QLabel>
#include <QStatusBar>
#include <QEvent>

#include "AboutDialog.hpp"
#include "rz_config.hpp"

MainWindow::MainWindow(QWidget *parent, bool startMinimized)
    : QMainWindow(parent), m_sorter(new FileSorter(this)) {
    
    setupUi();
    setupStatusBar();
    loadSettings(); // Lädt auch den Status der Checkbox

    // CLI logic
    if (startMinimized) {
        // Erzwinge Aktivierung der Checkbox -> löst onAutoSortToggled aus
        // -> startet Überwachung mit aktuellen Regeln
        if (!m_autoSortCheck->isChecked()) {
            m_autoSortCheck->setChecked(true);
            m_logOutput->append(tr("Gestartet im minimierten Modus (Überwachung erzwungen)."));
        }
    }

    connect(m_sorter, &FileSorter::logMessage, this, [this](const QString &msg){
        m_logOutput->append(msg);
        // Optional: Nachricht auch kurz in der Statusleiste anzeigen (für 3 Sekunden)
        statusBar()->showMessage(msg, 3000);
    });

    // Wenn der User in der Tabelle tippt oder Checkboxen ändert:
    connect(m_table, &QTableWidget::itemChanged, this, &MainWindow::onRulesModified);
}

MainWindow::~MainWindow() {
    saveSettings();
}

void MainWindow::setupUi() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Tabelle
    m_table = new QTableWidget(0, 3, this);
    m_table->setHorizontalHeaderLabels({tr("Folder Name"), tr("Extensions"),tr("use Date?")});
    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents); // Checkbox braucht wenig Platz

    mainLayout->addWidget(new QLabel(tr("Rules:"), this));
    mainLayout->addWidget(m_table);

    // Buttons & Checkbox
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *btnAdd = new QPushButton("+", this);
    QPushButton *btnRemove = new QPushButton("-", this);
    
    // Checkbox für Automatisierung
    m_autoSortCheck = new QCheckBox(tr("Automatic Monitoring"), this);
    
    QPushButton *btnSort = new QPushButton(tr("Sort Manually"), this);
    
    btnLayout->addWidget(btnAdd);
    btnLayout->addWidget(btnRemove);
    btnLayout->addWidget(m_autoSortCheck); // Einfügen
    btnLayout->addStretch();
    btnLayout->addWidget(btnSort);
    mainLayout->addLayout(btnLayout);

    m_logOutput = new QTextEdit(this);
    m_logOutput->setReadOnly(true);
    mainLayout->addWidget(m_logOutput);

    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onAddCategory);
    connect(btnRemove, &QPushButton::clicked, this, &MainWindow::onRemoveCategory);
    connect(btnSort, &QPushButton::clicked, this, &MainWindow::onStartSort);
    connect(m_autoSortCheck, &QCheckBox::toggled, this, &MainWindow::onAutoSortToggled);

    setWindowTitle("FileSorter");
    resize(600, 500);
}

void MainWindow::onAddCategory() {
    int row = m_table->rowCount();
    m_table->insertRow(row);
    m_table->setItem(row, 0, new QTableWidgetItem("Documents"));
    m_table->setItem(row, 1, new QTableWidgetItem("pdf, docx, odt"));

    // Checkbox Item erstellen
    QTableWidgetItem *checkItem = new QTableWidgetItem();
    checkItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    checkItem->setCheckState(Qt::Unchecked); // Standardmäßig aus
    m_table->setItem(row, 2, checkItem);

    onRulesModified();
}

void MainWindow::onRemoveCategory() {
    int row = m_table->currentRow();
    if (row >= 0) {
        m_table->removeRow(row);
        // NEU: Nach Löschen sofort Update triggern
        onRulesModified();
    }
}

void MainWindow::onStartSort() {
    // Manuelles Sortieren updated auch die Regeln im Sorter
    m_sorter->sortDownloads(getCategoriesFromUi());
}

void MainWindow::onRulesModified() {
    // 1. Einstellungen speichern (optional, aber praktisch)
    // saveSettings();

    // 2. Wenn Überwachung aktiv ist, müssen wir dem Sorter die neuen Regeln geben
    if (m_autoSortCheck->isChecked()) {
        // Wir holen die frischen Daten aus der UI
        auto newCategories = getCategoriesFromUi();

        // Update im Sorter
        m_sorter->updateRules(newCategories);

        // Optional: Kurzes Feedback im Log
        // m_logOutput->append(tr("Konfiguration geändert: Regeln aktualisiert."));
    }
}

void MainWindow::onAutoSortToggled(bool checked) {
    // Wenn wir aktivieren, müssen wir dem Sorter auch die aktuellen Regeln geben
    if (checked) {
        m_sorter->sortDownloads(getCategoriesFromUi()); // Einmal initial prüfen + Regeln setzen
    }
    m_sorter->setMonitoring(checked);
}

QList<Category> MainWindow::getCategoriesFromUi() const {
    QList<Category> categories;
    for(int i = 0; i < m_table->rowCount(); ++i) {
        auto itemFolder = m_table->item(i, 0);
        auto itemExt = m_table->item(i, 1);
        auto itemDate = m_table->item(i, 2); // Das Checkbox Item holen

        if (!itemFolder || !itemExt) continue;

        QString folder = itemFolder->text().trimmed();
        QStringList exts = itemExt->text().split(',', Qt::SkipEmptyParts);
        for(auto &e : exts) e = e.trimmed().toLower();

        // Checkbox Status auslesen
        bool useDate = (itemDate->checkState() == Qt::Checked);

        if(!folder.isEmpty() && !exts.isEmpty()) {
            categories.append({folder, exts, useDate}); // <--- useDate übergeben
        }
    }
    return categories;
}

void MainWindow::saveSettings() {
    QSettings settings("ZhengRobert", "FileSorter");

    settings.setValue("autoSort", m_autoSortCheck->isChecked());

    settings.beginWriteArray("rules");
    for (int i = 0; i < m_table->rowCount(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("folder", m_table->item(i, 0)->text());
        settings.setValue("extensions", m_table->item(i, 1)->text());

        // Status der Checkbox speichern
        bool isDateSort = (m_table->item(i, 2)->checkState() == Qt::Checked);
        settings.setValue("useDate", isDateSort);
    }
    settings.endArray();
}

void MainWindow::loadSettings() {
    QSettings settings("ZhengRobert", "FileSorter");

    bool autoSort = settings.value("autoSort", false).toBool();
    m_autoSortCheck->setChecked(autoSort);

    int size = settings.beginReadArray("rules");
    if (size == 0) {
        settings.endArray();
        onAddCategory();
        return;
    }

    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        m_table->insertRow(i);
        m_table->setItem(i, 0, new QTableWidgetItem(settings.value("folder").toString()));
        m_table->setItem(i, 1, new QTableWidgetItem(settings.value("extensions").toString()));

        // Checkbox wiederherstellen
        bool useDate = settings.value("useDate", false).toBool();
        QTableWidgetItem *checkItem = new QTableWidgetItem();
        checkItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        checkItem->setCheckState(useDate ? Qt::Checked : Qt::Unchecked);
        m_table->setItem(i, 2, checkItem);
    }
    settings.endArray();
}

void MainWindow::setupStatusBar() {
    // Label erstellen mit Version
    QString versionText = QString("v%1").arg(QString::fromStdString(PROJECT_VERSION));
    m_versionLabel = new QLabel(versionText, this);

    // Style: Ein bisschen Padding und Cursor als Hand, um Klickbarkeit anzudeuten
    m_versionLabel->setStyleSheet("padding: 0 5px; color: #555;");
    m_versionLabel->setCursor(Qt::PointingHandCursor);
    m_versionLabel->setToolTip(tr("Double-click for more information"));

    // EventFilter installieren, um Doppelklicks abzufangen
    m_versionLabel->installEventFilter(this);

    // Widget rechts (Permanent) zur Statusleiste hinzufügen
    statusBar()->addPermanentWidget(m_versionLabel);
}

// EventFilter Implementierung
bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    // Prüfen, ob das Event vom VersionLabel kommt UND ein Doppelklick ist
    if (obj == m_versionLabel && event->type() == QEvent::MouseButtonDblClick) {
        showAboutDialog();
        return true; // Event verbraucht
    }

    // Standard-Verarbeitung für alles andere
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::showAboutDialog() {
    AboutDialog dlg(this);
    dlg.exec();
}

