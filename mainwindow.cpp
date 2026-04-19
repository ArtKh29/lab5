#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "carddialog.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <QTextStream>
#include <QRegularExpression>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Родительский класс — Craft");
    resize(1000, 700);

    auto* central = new QWidget(this);
    auto* layout = new QVBoxLayout(central);

    auto* loadBtn = new QPushButton("Загрузить файл", this);

    tableWidget = new QTableWidget(0, 5, this);
    tableWidget->setHorizontalHeaderLabels({
        "Имя",
        "Стихия/редкость",
        "Мана/урон",
        "хп",
        "броня"
    });

    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setAlternatingRowColors(true);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->verticalHeader()->setVisible(false);

    layout->addWidget(loadBtn);
    layout->addWidget(tableWidget);

    setCentralWidget(central);

    connect(loadBtn, &QPushButton::clicked, this, &MainWindow::loadFile);
    connect(tableWidget, &QTableWidget::doubleClicked, this, &MainWindow::onDoubleClick);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadFile()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Открыть файл с данными",
        "",
        "Текстовые файлы (*.txt);;Все файлы (*.*)"
        );

    if (fileName.isEmpty()) {
        return;
    }

    qDebug() << "Открываем файл:" << fileName;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл:\n" + fileName);
        return;
    }

    characters.clear();
    tableWidget->setRowCount(0);

    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);

    int lineNumber = 0;
    while (!stream.atEnd()) {
        QString line = stream.readLine().trimmed();
        lineNumber++;

        if (line.isEmpty() || line.startsWith("Скелет") || line.startsWith("Имя")) {
            continue;
        }

        parseLine(line);

        if (lineNumber % 10 == 0) {
            QApplication::processEvents();
        }
    }

    file.close();

}

void MainWindow::parseLine(const QString& line)
{
    static const QRegularExpression regex("\\s+", QRegularExpression::UseUnicodePropertiesOption);
    QStringList parts = line.split(regex, Qt::SkipEmptyParts);

    if (parts.size() < 5) return;

    QString name = parts[0];
    QString second = parts[1];
    int value = parts[2].toInt();
    int hp = parts[3].toInt();

    QString armorStr = parts[4];
    armorStr.remove('<').remove('>');

    QStringList armorParts = armorStr.split('/');
    int a0 = armorParts.size() > 0 ? armorParts[0].toInt() : 0;
    int a1 = armorParts.size() > 1 ? armorParts[1].toInt() : 0;
    int a2 = armorParts.size() > 2 ? armorParts[2].toInt() : 0;

    Parent p;
    bool isMag = false;

    if (second == "Огонь" || second == "вода") {
        p = Parent(name, "Маг", hp, hp, a0, a1, a2, second, "", value, 0);
        isMag = true;
    } else {
        p = Parent(name, "Враг", hp, hp, a0, a1, a2, "", second, 0, value);
        isMag = false;
    }

    characters.append(p);

    int row = tableWidget->rowCount();
    tableWidget->insertRow(row);

    tableWidget->setItem(row, 0, new QTableWidgetItem(p.name));

    QString elementOrRarity = isMag ? p.element : p.rarity;
    tableWidget->setItem(row, 1, new QTableWidgetItem(elementOrRarity));

    QString manaOrDamage = isMag ? QString::number(p.mana) : QString::number(p.damage);
    tableWidget->setItem(row, 2, new QTableWidgetItem(manaOrDamage));

    tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(p.health)));

    tableWidget->setItem(row, 4, new QTableWidgetItem(
                                     QString("%1/%2/%3").arg(p.armor[0]).arg(p.armor[1]).arg(p.armor[2])
                                     ));

    tableWidget->scrollToBottom();
}

void MainWindow::onDoubleClick(const QModelIndex& index)
{
    if (!index.isValid()) return;

    int row = index.row();
    if (row < 0 || row >= characters.size()) return;

    const Parent& p = characters[row];
    bool isMag = (p.type == "Маг");

    CardDialog dialog(p, isMag, this);
    dialog.exec();
}
