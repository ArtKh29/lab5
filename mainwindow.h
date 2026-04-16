#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QVector>
#include "parent.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void loadFile();
    void onDoubleClick(const QModelIndex& index);

private:
    QTableWidget* tableWidget = nullptr;
    QVector<Parent> characters;

    void parseLine(const QString& line);
};

#endif // MAINWINDOW_H
