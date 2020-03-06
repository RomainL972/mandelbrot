#include "widget.h"

#include <QPainter>
#include <QLabel>
#include <QDebug>
#include <QMouseEvent>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(m_taille, m_taille);
    xmin = -1.25, xmax=1.25,ymin=-1.25,ymax=1.25;
}

void Widget::paintEvent(QPaintEvent *event) {
    m_painter = new QPainter(this);
    int taille = m_taille, iterationmax = 200;
    float a=-0.5, b=0.6;

    for (int line = 0; line < taille; line++) {
        for (int col = 0; col < taille; col++) {
            QPoint point = QPoint(col, line);
            int i = 1;
            double x = xmin + col *  (xmax-xmin) / taille;
            double y = ymax - line * (ymax-ymin) / taille;

            while(i <= iterationmax && (x*x+y*y)<=4) {
                double temp = x;
                x = x*x - y*y + a;
                y = 2*temp*y+b;
                i += 1;
            }
            if(x*x+y*y <= 4) {
                m_painter->setPen(QPen(QColor(0, 0, 0)));
            }
            else {
                m_painter->setPen(QPen(QColor((4*i)%256,(2*i)%256,(6*i)%256)));
            }
            m_painter->drawPoint(point);
        }
    }
    delete m_painter;
}

void Widget::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        factor /= 2;
    }
    else if(event->button() == Qt::RightButton) {
        factor *= 2;
    }
    double x = xmin + event->x() * (xmax-xmin) / m_taille;
    double y = ymax - event->y() * (ymax-ymin) / m_taille;
    xmin = x - 1.25 * factor;
    xmax = x + 1.25 * factor;
    ymin = y - 1.25 * factor;
    ymax = y + 1.25 * factor;
    update();
}

Widget::~Widget()
{
}

