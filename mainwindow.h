#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTextEdit>
#include <QDialog>
#include <QDebug>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QList>
#include <QComboBox>
#include <QScrollBar>

#include "serial.h"
#include "errorpage.h"

QT_CHARTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    Serial *serial;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void updateReceive();

private:
    bool flag;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
