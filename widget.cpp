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

using namespace std;

std::pair<vector<QPoint>,vector<QColor>> Widget::paintWindow(int tailleMin, int tailleMax) {
    qDebug() << tailleMin << tailleMax;
    std::vector<QPoint> points;
    std::vector<QColor> colors;
    for(int line=tailleMin; line < tailleMax; line++) {
        for(int col = 0; col < m_taille; col++) {
            QPoint point = QPoint(col,line);
            points.push_back(point);
            int i = 1;
            double x = xmin + col *  (xmax-xmin) / m_taille;
            double y = ymax - line * (ymax-ymin) / m_taille;

            while(i <= iteration_max && (x*x+y*y)<=4) {
                double temp = x;
                x = x*x - y*y + a;
                y = 2*temp*y+b;
                i += 1;
            }
            if(x*x+y*y <= 4) {
                colors.push_back(QColor(0,0,0));
                //painter->setPen(QPen(QColor(0, 0, 0)));
            }
            else {
                colors.push_back(QColor((4*i)%256,(2*i)%256,(6*i)%256));
                //painter->setPen(QPen(QColor((4*i)%256,(2*i)%256,(6*i)%256)));
            }
        }
    }
    return std::pair<vector<QPoint>,vector<QColor>>(points, colors);
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(m_taille, m_taille);
    xmin = -1.25, xmax=1.25,ymin=-1.25,ymax=1.25;
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
    int threads = 8;
    int part = m_taille / threads;
    m_painter = new QPainter(this);
    std::vector<std::future<std::pair<vector<QPoint>,vector<QColor>>>> threadsArray;

    for (int i=0;i < 8;i++) {
        threadsArray.push_back(std::async(&Widget::paintWindow,this,part*i, part*(i+1)));
    }
    for (int i = 0; i < 8 ; i++) {
        auto pair = threadsArray[i].get();
        for (auto j = 0; j < pair.first.size();j++) {
            m_painter->setPen(QPen(pair.second[j]));
            m_painter->drawPoint(pair.first[j]);
        }
    }
    delete m_painter;
}

void Widget::resetZoom() {
    factor = 1;
    double x=0, y=0;
    xmin = x - 1.25 * factor;
    xmax = x + 1.25 * factor;
    ymin = y - 1.25 * factor;
    ymax = y + 1.25 * factor;
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
    double x = xmin + event->x() * (xmax-xmin) / m_taille;
    double y = ymax - event->y() * (ymax-ymin) / m_taille;
    xmin = x - 1.25 * factor;
    xmax = x + 1.25 * factor;
    ymin = y - 1.25 * factor;
    ymax = y + 1.25 * factor;
    update();
    m_zoomFactor->setText(tr("Zoom : X%n", "", 1/factor));
}

Widget::~Widget()
{
}

