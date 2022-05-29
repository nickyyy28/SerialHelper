#include "mywaveplot.h"
#include <QDebug>

MyWavePlot::MyWavePlot(QWidget *parent) : QCustomPlot(parent)
{
    setMouseTracking(true);
}

float MyWavePlot::getYAxis() const
{
    return m_yAxis;
}

float MyWavePlot::offset() const
{
    return m_offset;
}

void MyWavePlot::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0) {
//        qDebug() << "大于0";
        if (m_yAxis > 0.01) {
            m_yAxis *= 0.8;
        }

    } else if (event->delta() < 0) {
        if (m_yAxis < 10000000) {
            m_yAxis *= 1.2;
        }
    }
}

void MyWavePlot::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        if (event->pos().y() - lastYPos > 0) {
            m_offset += m_yAxis * 0.01;
        } else if (event->pos().y() - lastYPos < 0) {
            m_offset -= m_yAxis * 0.01;
        }
    }
    lastYPos = event->pos().y();
}
