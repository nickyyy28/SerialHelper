#include "errorpage_pid.h"
#include "ui_errorpage_pid.h"

errorpage_pid::errorpage_pid(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::errorpage_pid)
{
    ui->setupUi(this);

    ui->row->setReadOnly(true);
    ui->column->setReadOnly(true);

    connect(ui->btn_close, &QPushButton::clicked, [this](){
        this->close();
    });
}

errorpage_pid::~errorpage_pid()
{
    delete ui;
}

void errorpage_pid::setRowAndColumn(int row, int column)
{
    this->ui->row->setText(QString("%1").arg(row));
    this->ui->column->setText(QString("%1").arg(column));
}
