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
    MainWindow(QWidget *parent = nullptr);
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

private:
    void setupUi();
    void setupStatusBar();
    QList<Category> getCategoriesFromUi() const;

    QTableWidget *m_table;
    QTextEdit *m_logOutput;
    QCheckBox *m_autoSortCheck; // Checkbox UI Element
    FileSorter *m_sorter;
    QLabel *m_versionLabel;
};
