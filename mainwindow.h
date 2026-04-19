#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QVector>
#include "parent.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadFile();
    void onDoubleClick(const QModelIndex &index);

private:
    void parseLine(const QString& line);

    Ui::MainWindow *ui;
    QTableWidget* tableWidget;
    QVector<Parent> characters;
};

#endif
