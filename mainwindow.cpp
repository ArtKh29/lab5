#include "mainwindow.h"
#include "carddialog.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <QTextStream>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    setWindowTitle("Родительский класс — Craft");
    resize(1000, 700);

    auto* central = new QWidget(this);
    auto* layout = new QVBoxLayout(central);

    auto* loadBtn = new QPushButton("Загрузить файл");
    connect(loadBtn, &QPushButton::clicked, this, &MainWindow::loadFile);

    tableWidget = new QTableWidget(0, 5, this);
    tableWidget->setHorizontalHeaderLabels({"Имя", "Тип", "XP", "Здоровье", "Броня"});
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(tableWidget, &QTableWidget::doubleClicked, this, &MainWindow::onDoubleClick);

    layout->addWidget(loadBtn);
    layout->addWidget(tableWidget);

    setCentralWidget(central);
}

void MainWindow::loadFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл", "", "Text files (*.txt)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл!");
        return;
    }

    characters.clear();
    tableWidget->setRowCount(0);

    QTextStream stream(&file);
    while (!stream.atEnd()) {
        QString line = stream.readLine().trimmed();
        if (line.isEmpty() || line.startsWith("Скелет")) continue;
        parseLine(line);
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
    int hp = parts[2].toInt();
    int health = parts[3].toInt();

    QString armorStr = parts[4];
    armorStr.remove('<').remove('>');

    QStringList armorParts = armorStr.split('/');
    int a0 = armorParts.size() > 0 ? armorParts[0].toInt() : 0;
    int a1 = armorParts.size() > 1 ? armorParts[1].toInt() : 0;
    int a2 = armorParts.size() > 2 ? armorParts[2].toInt() : 0;

    Parent p;

    if (name == "Мэрлин" || second == "Огонь" || second == "вода" || second == "земля") {
        p = Parent(name, "Маг", hp, health, a0, a1, a2, second, "", 100, 0);
    } else {
        p = Parent(name, "Враг", hp, health, a0, a1, a2, "", second, 0, 85);
    }

    characters.append(p);

    int row = tableWidget->rowCount();
    tableWidget->insertRow(row);

    tableWidget->setItem(row, 0, new QTableWidgetItem(p.name));
    tableWidget->setItem(row, 1, new QTableWidgetItem(p.type));
    tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(p.hp)));
    tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(p.health)));
    tableWidget->setItem(row, 4, new QTableWidgetItem(QString("%1/%2/%3")
                                                          .arg(p.armor[0])
                                                          .arg(p.armor[1])
                                                          .arg(p.armor[2])));
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
