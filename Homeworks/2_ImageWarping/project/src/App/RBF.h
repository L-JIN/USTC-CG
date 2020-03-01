#pragma once
#include <vector>
#include <QPoint>
#include <Eigen/Dense>
#include <QImage>
#include <QtMath>
using namespace std;
using namespace Eigen;

class RBF
{
public:
    RBF();
    RBF(vector<QPoint> &p_points, vector<QPoint> &q_points, QPoint start, int u);
    ~RBF();

    QPoint get_output_point(QPoint p);
    double get_distance(QPoint p, QPoint q);

    void cal_distance_martix();
    void calculate_ri();
    void calculate_ai();

    QImage get_final_image_RBF(const QImage src);

    void set_start(QPoint s);
    void set_p(vector<QPoint> p_);
    void set_q(vector<QPoint> q_);


private:
    double _u;
    size_t _size;
    vector<QPoint> p_;
    vector<QPoint> q_;
    double *d_;
    double *r_;
    VectorXd A_x;
    VectorXd A_y;

    QPoint start_point;
};
