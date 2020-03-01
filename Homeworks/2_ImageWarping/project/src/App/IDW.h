#pragma once
#ifndef IDW_H
#define IDW_H

#include <QPoint>
#include <vector>
#include <QtMath>
#include <QDebug>
#include <QImage>

using namespace std;

class IDW
{
public:
    IDW();
    ~IDW();

    double get_distanse( QPoint p1,  QPoint p2);
    QMap<int, double> get_input_control_point_weight( QPoint input_point_);
    QPoint get_output_point( QPoint input_point_);
    QImage get_image_deal_with_IDW( QImage src);

    void set_m_u(double m_u);
    double get_m_u(void);

    void set_p( vector<QPoint> p_);
    vector<QPoint> get_p(void);

    void set_q( vector<QPoint> q_);
    vector<QPoint> get_q(void);

    void set_start(QPoint s);
    void set_end(QPoint e);

private:
    double m_u;
    vector<QPoint> p_;
    vector<QPoint> q_;

    QPoint start_point;
    QPoint end_point;
};
#endif
