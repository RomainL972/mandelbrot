#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <utility>
#include <vector>
#include <QLineEdit>
#include <QCheckBox>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    std::pair<std::vector<QPoint>,std::vector<QColor>> paintWindow(int tailleMin, int tailleMax);
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    ~Widget();

public slots:
    void resetZoom();
    void saveSettings();

private:
    int m_long = 1200;
    int m_larg = (double)m_long / 3.0 * 2;
    int iteration_max = 200;
    double xmin, xmax, ymin, ymax, factor=1, zoomFactorChange=2;
    int threads = 10;
    QPainter *m_painter;
    QDialog *m_subwindow;
    QVBoxLayout *m_sublayout;
    QLabel *m_zoomFactor;
    QLineEdit *m_longueurField;
    QLineEdit *m_zoomFactorField;
    QLineEdit *m_iterationMaxField;
    QLineEdit *m_threadsField;
    QCheckBox *m_resetViewField;
};
#endif // WIDGET_H
