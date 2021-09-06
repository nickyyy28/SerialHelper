#ifndef ERRORPAGE_H
#define ERRORPAGE_H

#include <QDialog>
#include <QPushButton>

namespace Ui {
class errorPage;
}

class errorPage : public QDialog
{
    Q_OBJECT

public:
    explicit errorPage(QWidget *parent = nullptr);
    ~errorPage();

private:
    Ui::errorPage *ui;
};

#endif // ERRORPAGE_H
