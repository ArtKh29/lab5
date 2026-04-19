#include "carddialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QFont>
#include <QFileInfo>
#include <QCoreApplication>
#include <QFile>

CardDialog::CardDialog(const Parent& p, bool isMag, QWidget* parent)
    : QDialog(parent), obj(p), isMag(isMag)
{
    setWindowTitle(isMag ? "Карточка мага" : "Карточка врага");
    setFixedSize(400, 650);

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(12);

    auto* imageLabel = new QLabel(this);
    imageLabel->setFixedSize(200, 200);
    imageLabel->setAlignment(Qt::AlignCenter);

    QString imagePath;
    QString charName = p.name.toLower();
    bool isApache = charName.contains("апач", Qt::CaseInsensitive) ||
                    charName.contains("апац", Qt::CaseInsensitive);

    QString basePath = QCoreApplication::applicationDirPath();
    QString imagesPath = basePath + "/images";

    if (isApache) {
        imagePath = imagesPath + "/апац.jpg";
        if (!QFile::exists(imagePath)) imagePath = imagesPath + "/апач.png";
    } else if (charName.contains("тагила", Qt::CaseInsensitive)) {
        imagePath = imagesPath + "/тагила.jpg";
        if (!QFile::exists(imagePath)) imagePath = imagesPath + "/тагила.png";
    } else if (charName.contains("ультрамарин", Qt::CaseInsensitive)) {
        imagePath = imagesPath + "/ультрамарин.jpg";
        if (!QFile::exists(imagePath)) imagePath = imagesPath + "/ультрамарин.png";
    }

    QString nameColor = "#2c3e50";

    if (isMag || isApache) {
        if (p.element == "Огонь") { nameColor = "#e74c3c"; }
        else if (p.element == "вода") { nameColor = "#3498db"; }
    } else {
        if (p.rarity == "Легенда" || p.rarity == "Легендарный") { nameColor = "#f1c40f"; }
        else if (p.rarity == "Редкий") { nameColor = "#9b59b6"; }
    }

    auto* nameValueLabel = new QLabel(p.name);
    nameValueLabel->setAlignment(Qt::AlignCenter);
    nameValueLabel->setStyleSheet("color: " + nameColor + "; font-size: 26px; font-weight: bold;");
    mainLayout->addWidget(nameValueLabel);

    mainLayout->addSpacing(10);

    if (isMag || isApache) {
        auto* statsRow = new QHBoxLayout;
        statsRow->setSpacing(20);

        auto* hpContainer = new QVBoxLayout;
        auto* hpLabel = new QLabel("ХП");
        hpLabel->setAlignment(Qt::AlignCenter);
        hpLabel->setStyleSheet("color: #27ae60; font-size: 16px; font-weight: bold;");

        auto* hpValueLabel = new QLabel(QString::number(p.hp));
        hpValueLabel->setAlignment(Qt::AlignCenter);
        hpValueLabel->setStyleSheet("color: #27ae60; font-size: 28px; font-weight: bold;");

        hpContainer->addWidget(hpLabel);
        hpContainer->addWidget(hpValueLabel);

        auto* manaContainer = new QVBoxLayout;
        auto* manaLabel = new QLabel("Мана");
        manaLabel->setAlignment(Qt::AlignCenter);
        manaLabel->setStyleSheet("color: #2980b9; font-size: 16px; font-weight: bold;");

        auto* manaValueLabel = new QLabel(QString::number(p.mana));
        manaValueLabel->setAlignment(Qt::AlignCenter);
        manaValueLabel->setStyleSheet("color: #2980b9; font-size: 28px; font-weight: bold;");

        manaContainer->addWidget(manaLabel);
        manaContainer->addWidget(manaValueLabel);

        statsRow->addStretch();
        statsRow->addLayout(hpContainer);
        statsRow->addSpacing(40);
        statsRow->addLayout(manaContainer);
        statsRow->addStretch();

        mainLayout->addLayout(statsRow);
    } else {
        auto* hpLabel = new QLabel("ХП");
        hpLabel->setAlignment(Qt::AlignCenter);
        hpLabel->setStyleSheet("color: #27ae60; font-size: 16px; font-weight: bold;");
        mainLayout->addWidget(hpLabel);

        auto* hpValueLabel = new QLabel(QString::number(p.hp));
        hpValueLabel->setAlignment(Qt::AlignCenter);
        hpValueLabel->setStyleSheet("color: #27ae60; font-size: 28px; font-weight: bold;");
        mainLayout->addWidget(hpValueLabel);
    }

    if (!isApache) {
        mainLayout->addSpacing(10);

        auto* armorLabel = new QLabel("Броня");
        armorLabel->setAlignment(Qt::AlignCenter);
        armorLabel->setStyleSheet("color: #95a5a6; font-size: 16px; font-weight: bold;");
        mainLayout->addWidget(armorLabel);

        auto* armorValueLabel = new QLabel(
            QString("<%1/%2/%3>").arg(p.armor[0]).arg(p.armor[1]).arg(p.armor[2])
            );
        armorValueLabel->setAlignment(Qt::AlignCenter);
        armorValueLabel->setStyleSheet("color: #95a5a6; font-size: 20px; font-weight: bold;");
        mainLayout->addWidget(armorValueLabel);
    }

    mainLayout->addStretch();

    auto* buttonsLayout = new QHBoxLayout;
    buttonsLayout->setSpacing(15);
    buttonsLayout->addStretch();

    auto* printBtn = new QPushButton("Печать");
    printBtn->setFixedSize(120, 50);
    printBtn->setStyleSheet(
        "QPushButton { background-color: #3498db; color: white; font-size: 16px; font-weight: bold; border: none; border-radius: 8px; }"
        "QPushButton:hover { background-color: #2980b9; }"
        "QPushButton:pressed { background-color: #1a5276; }"
        );
    connect(printBtn, &QPushButton::clicked, this, &CardDialog::onPrintClicked);

    auto* cancelBtn = new QPushButton("Отмена");
    cancelBtn->setFixedSize(120, 50);
    cancelBtn->setStyleSheet(
        "QPushButton { background-color: #e74c3c; color: white; font-size: 16px; font-weight: bold; border: none; border-radius: 8px; }"
        "QPushButton:hover { background-color: #c0392b; }"
        "QPushButton:pressed { background-color: #922b21; }"
        );
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

    buttonsLayout->addWidget(printBtn);
    buttonsLayout->addWidget(cancelBtn);
    buttonsLayout->addStretch();

    mainLayout->addLayout(buttonsLayout);
}

CardDialog::~CardDialog()
{
}

void CardDialog::onPrintClicked()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPageSize::A4);
    printer.setPageOrientation(QPageLayout::Portrait);

    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        bool isApache = obj.name.toLower().contains("апач", Qt::CaseInsensitive) ||
                        obj.name.toLower().contains("апац", Qt::CaseInsensitive);

        painter.setFont(QFont("Arial", 26, QFont::Bold));
        painter.setPen(Qt::black);
        painter.drawText(150, 60, obj.name);

        painter.setFont(QFont("Arial", 14));
        int y = 100;

        painter.drawText(150, y, "ХП: " + QString::number(obj.hp));
        y += 30;

        if (isMag || isApache) {
            painter.drawText(150, y, "Мана: " + QString::number(obj.mana));
            y += 30;
        }

        if (!isApache) {
            painter.drawText(150, y, "Броня: <" + QString("%1/%2/%3").arg(obj.armor[0]).arg(obj.armor[1]).arg(obj.armor[2]) + ">");
        }

        painter.end();
    }
}
