#include "widget.h"

#include <QPainter>
#include <QLabel>
#include <QDebug>
#include <QMouseEvent>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <thread>
#include <vector>
#include <future>
#include <utility>
#include <complex>
#include <iostream>
#include <QApplication>

using namespace std;

std::pair<vector<QPoint>,vector<QColor>> Widget::paintWindow(int tailleMin, int tailleMax) {
    std::vector<QPoint> points;
    std::vector<QColor> colors;
    for(int line=tailleMin; line < tailleMax; line++) {
        for(int col = 0; col < m_long; col++) {
            QPoint point = QPoint(col,line);
            points.push_back(point);

            std::complex<double> c(xmin + ((double)col / m_long) * (xmax - xmin),
                                   ymin + ((double)line / m_larg) * (ymax - ymin));

            std::complex<double> z(0,0);
            int n;
            for (n = 0; std::abs(z) <= 2 && n < iteration_max; n++) {
                z = z*z + c;
            }

            double m;
            if (n == iteration_max) {
                m = iteration_max;
            }
            else {
                m = n + 1 - std::log(std::log2(std::abs(z)));
            }

            int teinte = (int)(255 * m / iteration_max);
            int satu = 255;
            int valeur;
            if (m < iteration_max) {
                valeur = 255;
            }
            else {
                valeur = 0;
            }

            colors.push_back(QColor::fromHsv(teinte, satu, valeur));
        }
    }
    return std::pair<vector<QPoint>,vector<QColor>>(points, colors);
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(m_long, m_larg);
    xmin = -2, xmax=1,ymin=-1,ymax=1;
    m_subwindow = new QDialog(this);
    m_subwindow->show();
    m_sublayout = new QVBoxLayout;
    m_subwindow->setLayout(m_sublayout);

    m_zoomFactor = new QLabel("Zoom : X1");
    QPushButton *button = new QPushButton("Réinitialiser la vue");

    m_sublayout->addWidget(m_zoomFactor);
    m_sublayout->addWidget(button);

    connect(button, &QPushButton::clicked, this, &Widget::resetZoom);
    connect(m_subwindow, &QDialog::finished, m_subwindow, &QDialog::show);
}

void Widget::paintEvent(QPaintEvent *event) {
    int threads = 20;
    int part = m_larg / threads;
    m_painter = new QPainter(this);
    std::vector<std::future<std::pair<vector<QPoint>,vector<QColor>>>> threadsArray;

    for (int i=0;i < threads;i++) {
        threadsArray.push_back(std::async(&Widget::paintWindow,this,part*i, part*(i+1)));
    }
    for (int i = 0; i < threads ; i++) {
        auto pair = threadsArray[i].get();
        for (auto j = 0; j < pair.first.size();j++) {
            m_painter->setPen(QPen(pair.second[j]));
            m_painter->drawPoint(pair.first[j]);
        }
    }
    delete m_painter;
//    QApplication::quit();
}

void Widget::resetZoom() {
    factor = 1;
    double x=0, y=0;
    xmin = x - 2 * factor;
    xmax = x + 1 * factor;
    ymin = y - 1 * factor;
    ymax = y + 1 * factor;
    update();
    m_zoomFactor->setText(tr("Zoom : X%n", "", 1/factor));
}

void Widget::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        factor /= 2;
    }
    else if(event->button() == Qt::RightButton) {
        factor *= 2;
    }
    double x = xmin + event->x() * (xmax-xmin) / m_long;
    double y = ymin + event->y() * (ymax-ymin) / m_larg;
    xmin = x - 1.5 * factor;
    xmax = x + 1.5 * factor;
    ymin = y - 1 * factor;
    ymax = y + 1 * factor;
    update();
    m_zoomFactor->setText(tr("Zoom : X%n", "", 1/factor));
}

Widget::~Widget()
{
}

