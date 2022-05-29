#ifndef MYWAVEPLOT_H
#define MYWAVEPLOT_H

#include <QObject>
#include "qcustomplot.h"
#include <QMouseEvent>

class MyWavePlot : public QCustomPlot
{
    Q_OBJECT
public:
    explicit MyWavePlot(QWidget *parent = nullptr);

    float getYAxis() const;
    float offset() const;

protected:
    void wheelEvent(QWheelEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;


private:
    float m_yAxis = 10;
    float m_offset = 0;

    float lastYPos = 0;

signals:

};

#endif // MYWAVEPLOT_H
