#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    ~Widget();

private:
    int m_taille = 800;
    float xmin, xmax, ymin, ymax, factor=1;
    QPainter *m_painter;
};
#endif // WIDGET_H
