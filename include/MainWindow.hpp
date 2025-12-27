#pragma once

#include <QMainWindow>
#include <QTableWidget>
#include <QTextEdit>
#include <QCheckBox>
#include <QLabel>

#include "FileSorter.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, bool startMinimized = false);
    ~MainWindow();

protected:
    // EventFilter fängt den Doppelklick ab
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void onAddCategory();
    void onRemoveCategory();
    void onStartSort(); // Manueller Trigger
    void onAutoSortToggled(bool checked); // Checkbox Slot
    void saveSettings();
    void loadSettings();
    void showAboutDialog();
    void onRulesModified();

private:
    void setupUi();
    void setupStatusBar();
    QList<Category> getCategoriesFromUi() const;
    // Hilfsfunktion, um Logik nicht zu duplizieren
    void updateSorterInstance();

    QTableWidget *m_table;
    QTextEdit *m_logOutput;
    QCheckBox *m_autoSortCheck; // Checkbox UI Element
    FileSorter *m_sorter;
    QLabel *m_versionLabel;
};
