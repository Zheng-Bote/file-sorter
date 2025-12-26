#include "AboutDialog.hpp"
#include "rz_config.hpp" // Hier holen wir die Daten

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QIcon>

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle(tr("About") + " " + QString::fromStdString(PROG_LONGNAME));
    setModal(true);
    resize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(this);

    // Titel & Version
    QLabel *titleLabel = new QLabel(QString("<h2>%1</h2>").arg(QString::fromStdString(PROG_LONGNAME)));
    titleLabel->setAlignment(Qt::AlignCenter);

    QLabel *versionLabel = new QLabel(tr("Version: %1").arg(QString::fromStdString(PROJECT_VERSION)));
    versionLabel->setAlignment(Qt::AlignCenter);

    // Beschreibung
    QLabel *descLabel = new QLabel(QString::fromStdString(PROJECT_DESCRIPTION));
    descLabel->setWordWrap(true);
    descLabel->setAlignment(Qt::AlignCenter);

    // Copyright & Autor
    QLabel *copyLabel = new QLabel(QString("© %1 %2").arg(QString::fromStdString(PROG_CREATED), QString::fromStdString(PROG_AUTHOR)));
    copyLabel->setAlignment(Qt::AlignCenter);

    // Link (GitHub)
    // Wir nutzen HTML <a> Tags. setOpenExternalLinks(true) ist entscheidend.
    QString linkHtml = QString("<a href='%1'>%1</a>").arg(QString::fromStdString(PROJECT_HOMEPAGE_URL));
    QLabel *linkLabel = new QLabel(linkHtml);
    linkLabel->setOpenExternalLinks(true); // Öffnet Standard-Browser
    linkLabel->setAlignment(Qt::AlignCenter);

    // Technische Infos (Optional, aber cool)
    QLabel *techLabel = new QLabel(QString("<small>Built with Qt %1 (%2) on %3</small>")
                                       .arg(QString::fromStdString(CMAKE_QT_VERSION))
                                       .arg(QString::fromStdString(CMAKE_CXX_COMPILER))
                                       .arg(QString::fromStdString(CMAKE_CXX_STANDARD)));
    techLabel->setAlignment(Qt::AlignCenter);
    techLabel->setStyleSheet("color: gray;");

    // Button zum Schließen
    QPushButton *btnClose = new QPushButton(tr("Close"));
    connect(btnClose, &QPushButton::clicked, this, &QDialog::accept);

    // Widgets hinzufügen
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
