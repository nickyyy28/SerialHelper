#include "errorpage.h"
#include "ui_errorpage.h"
#include <QIcon>

errorPage::errorPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::errorPage)
{
    ui->setupUi(this);
    this->setFixedSize(QSize(500,300));
    ui->btnClose->setFixedSize(QSize(50,50));
    ui->btnClose->move((this->width() - ui->btnClose->width())/2, (this->height() - ui->btnClose->height())/2);

    this->setWindowIcon(QIcon(":/Image/error.png"));

    connect(ui->btnClose, &QPushButton::clicked, [&](){
        this->close();
    });
}

errorPage::~errorPage()
{
    delete ui;
}
