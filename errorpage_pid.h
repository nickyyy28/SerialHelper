#ifndef ERRORPAGE_PID_H
#define ERRORPAGE_PID_H

#include <QDialog>

namespace Ui {
class errorpage_pid;
}

class errorpage_pid : public QDialog
{
    Q_OBJECT

public:
    explicit errorpage_pid(QDialog *parent = nullptr);
    ~errorpage_pid();
public slots:
    void setRowAndColumn(int row, int column);
private:
    Ui::errorpage_pid *ui;
};

#endif // ERRORPAGE_PID_H
