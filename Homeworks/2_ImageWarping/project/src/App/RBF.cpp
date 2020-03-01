#include "RBF.h"
#include<iostream>
using namespace std;

RBF::RBF()
{
    d_ = nullptr;
    r_ = nullptr;
    _size = _u = 0;
}

RBF::RBF(vector<QPoint>& p_, vector<QPoint>& q_, QPoint start, int u = -1)
{
    set_start(start);

    set_p(p_);
    set_q(q_);
    _u = u;
    _size = p_.size();
    d_ = new double[_size * _size];
    r_ = new double[_size];

    cal_distance_martix();
    calculate_ri();
    calculate_ai();
}

RBF::~RBF()
{
    delete d_;
    delete r_;
}

double RBF::get_distance(QPoint p1, QPoint p2)
{
    return qSqrt(((_int64)p1.x() - (_int64)p2.x()) * ((_int64)p1.x() - (_int64)p2.x())
        + ((_int64)p1.y() - (_int64)p2.y()) * ((_int64)p1.y() - (_int64)p2.y()));
}
void RBF::cal_distance_martix()
{
    for (size_t i = 0; i < _size; i++)
    {
        d_[i * _size + i] = 0;
        for (size_t j = i + 1; j < _size; j++)
        {
            d_[i * _size + j] = d_[j * _size + i] = get_distance(p_[i], p_[j]);
        }
    }
}

void RBF::calculate_ri()
{
    double min;


    for (size_t i = 0; i < _size; i++)
    {
        min = INT_MAX;
        for (size_t j = 0; j < _size; j++)
        {
            if (min > d_[i * _size + j] && i != j)
                min = d_[i * _size + j];
        }
        r_[i] = min;
    }
}

void RBF::calculate_ai()
{
    Matrix<double, Dynamic, Dynamic> K(_size, _size);
    VectorXd Bx(_size);
    VectorXd By(_size);
    for (size_t i = 0; i < _size; i++)
    {
        Bx(i) = (_int64)q_[i].x() - (_int64)p_[i].x();
        By(i) = (_int64)q_[i].y() - (_int64)p_[i].y();
        for (size_t j = 0; j < _size; j++)
        {
            K(i, j) = pow(d_[j * _size + i] + r_[j], _u);
        }
    }
    A_x = K.colPivHouseholderQr().solve(Bx);
    A_y = K.colPivHouseholderQr().solve(By);
}

QPoint RBF::get_output_point(QPoint p)
{
    double x = p.x(), y = p.y(), t;
    for (int i = 0; i < _size; i++)
    {
        t = pow(get_distance(p, p_[i]) + r_[i], _u);
        x += A_x(i) * t;
        y += A_y(i) * t;
    }
    return QPoint(x, y);
}

void RBF::set_start(QPoint s)
{
    this->start_point = s;
}

void RBF::set_q(vector<QPoint> q_)
{
    for (int i = 0; i < q_.size(); i++)
    {
        QPoint temp;
        temp = q_[i];
        this->q_.push_back(QPoint(temp.x() - start_point.x(), temp.y() - start_point.y()));
    }

}

void RBF::set_p(vector<QPoint> p_)
{
    for (int i = 0; i < p_.size(); i++)
    {
        QPoint temp;
        temp = p_[i];
        this->p_.push_back(QPoint(temp.x() - start_point.x(), temp.y() - start_point.y()));
    }

}

QImage RBF::get_final_image_RBF(QImage src)
{
    QImage ouput_image_ = QImage(src.width(), src.height(), QImage::Format_ARGB32);
    QPoint output_point_;
    int** filled_ = new int* [src.height()];
    for (int j = 0; j < src.height(); j++)
        filled_[j] = new int[src.width()];

    for (int i = 0; i < src.height(); i++)
        for (int j = 0; j < src.width(); j++)
            filled_[i][j] = 0;

    for (int i = 0; i < src.width(); i++)
    {
        for (int j = 0; j < src.height(); j++)
        {
            output_point_ = this->get_output_point(QPoint(i, j));
            if (output_point_.x() > 0 && output_point_.y() > 0 && output_point_.x() < src.width() && output_point_.y() < src.height())
            {
                filled_[output_point_.y()][output_point_.x()] = 1;
                ouput_image_.setPixelColor(output_point_.x(), output_point_.y(), src.pixelColor(QPoint(i, j)));
            }
        }
    }

    for (int i = 1; i < src.height() - 1; i++)
    {
        for (int j = 1; j < src.width() - 1; j++)
        {
            if (filled_[i][j] == 0)
            {
                if (filled_[i + 1][j] != 0)
                    ouput_image_.setPixelColor(j, i, ouput_image_.pixelColor(QPoint(j, i + 1)));
                else if (filled_[i - 1][j] != 0)
                    ouput_image_.setPixelColor(j, i, ouput_image_.pixelColor(QPoint(j, i - 1)));
                else if (filled_[i][j + 1] != 0)
                    ouput_image_.setPixelColor(j, i, ouput_image_.pixelColor(QPoint(j + 1, i)));
                else if (filled_[i][j - 1] != 0)
                    ouput_image_.setPixelColor(j, i, ouput_image_.pixelColor(QPoint(j - 1, i)));

                if (filled_[i + 1][j + 1] != 0)
                    ouput_image_.setPixelColor(j, i, ouput_image_.pixelColor(QPoint(j + 1, i + 1)));
                else if (filled_[i - 1][j - 1] != 0)
                    ouput_image_.setPixelColor(j, i, ouput_image_.pixelColor(QPoint(j - 1, i - 1)));
                else if (filled_[i + 1][j - 1] != 0)
                    ouput_image_.setPixelColor(j, i, ouput_image_.pixelColor(QPoint(j - 1, i + 1)));
                else if (filled_[i - 1][j + 1] != 0)
                    ouput_image_.setPixelColor(j, i, ouput_image_.pixelColor(QPoint(j + 1, i - 1)));
            }

        }
    }
    
    for (int j = 0; j < src.height(); j++)
        delete[] filled_[j];
    delete[] filled_;
    return ouput_image_;
}
