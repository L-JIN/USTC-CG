#include "IDW.h"
#include <iostream>

IDW::IDW()
{
    m_u = 2.0;

}
IDW::~IDW()
{

}

double IDW::get_distanse(QPoint p1, QPoint p2)
{
    return qSqrt((p1.x() - p2.x())*(p1.x() - p2.x()) + (p1.y() - p2.y())*(p1.y() - p2.y()));
}

QMap<int, double> IDW::get_input_control_point_weight(QPoint input_point_)
{
    QMap<int, double> point_correspond_control_point_weight_map;
    double sum_weight_ = 0;

    for (int i = 0; i < p_.size(); i++)
    {
        double temp_ = 0;
        temp_ = 1 / this->get_distanse(p_.at(i), input_point_);
        temp_ = qPow(temp_, m_u);
        sum_weight_ = sum_weight_ + temp_;
    }

    for (int i = 0; i < p_.size(); i++)
    {
        double temp_ = 0;
        temp_ = 1 / this->get_distanse(p_.at(i), input_point_);
        temp_ = qPow(temp_, m_u);
        temp_ = temp_ / sum_weight_;
        point_correspond_control_point_weight_map.insert(i, temp_);
    }
    return point_correspond_control_point_weight_map;
}

QPoint IDW::get_output_point(QPoint input_point_)
{
    double offset_x_ = 0;
    double offset_y_ = 0;
    double x = 0, y = 0;
    QMap<int, double> tem_map_ = get_input_control_point_weight(input_point_);
    for (int i = 0; i < p_.size(); i++)
    {
        offset_x_ = q_.at(i).x() - p_.at(i).x();
        offset_y_ = q_.at(i).y() - p_.at(i).y();
        x += offset_x_*tem_map_.value(i);
        y += offset_y_*tem_map_.value(i);

    }

    input_point_.rx() = input_point_.x() + x;
    input_point_.ry() = input_point_.y() + y;
    return input_point_;
}

QImage IDW::get_image_deal_with_IDW(QImage src)
{
    QImage ouput_image_ = QImage(src.width(), src.height(), QImage::Format_ARGB32);
    QPoint output_point_;
    int **filled_;
    filled_ = new int*[src.height()];
    for (int j = 0; j < src.height(); j++)
        filled_[j] = new int[src.width()];
    for (int i = 0; i < src.height(); i++)
        for (int j = 0; j < src.width(); j++)
            filled_[i][j] = 0;

    for (int i = 0; i <src.width(); i++)
    {
        for (int j = 0; j <src.height(); j++)
        {
            output_point_ = this->get_output_point(QPoint(i, j));
            if (output_point_.x() > 0 && output_point_.y() > 0&& output_point_.x()<src.width() && output_point_.y() <src.height())
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
                    ouput_image_.setPixelColor(j, i , ouput_image_.pixelColor(QPoint(j, i - 1)));
                else if (filled_[i][j + 1] != 0)
                    ouput_image_.setPixelColor(j, i, ouput_image_.pixelColor(QPoint(j + 1, i)));
                else if (filled_[i][j - 1] != 0)
                    ouput_image_.setPixelColor(j, i, ouput_image_.pixelColor(QPoint(j - 1, i)));

                if (filled_[i + 1][j+1] != 0)
                    ouput_image_.setPixelColor(j, i, ouput_image_.pixelColor(QPoint(j + 1, i + 1)));
                else if (filled_[i - 1][j-1] != 0)
                    ouput_image_.setPixelColor(j, i, ouput_image_.pixelColor(QPoint(j-1, i - 1)));
                else if (filled_[i+1][j - 1] != 0)
                    ouput_image_.setPixelColor(j, i, ouput_image_.pixelColor(QPoint(j - 1, i+1)));
                else if (filled_[i-1][j + 1] != 0)
                    ouput_image_.setPixelColor(j, i, ouput_image_.pixelColor(QPoint(j + 1, i-1)));
            }

        }
    }

    for (int j = 0; j < src.height(); j++)
        delete[] filled_[j];
    delete[] filled_;

    return ouput_image_;
}

void IDW::set_m_u(double m_u)
{
    this->m_u = m_u;
}

double IDW::get_m_u(void)
{
    return m_u;
}

void IDW::set_p(vector<QPoint> p_)
{
    for(int i=0;i<p_.size();i++)
    {
        QPoint temp;
        temp=p_[i];
        this->p_.push_back(QPoint(temp.x()-start_point.x(),temp.y()-start_point.y()));
    }

}

vector<QPoint> IDW::get_p(void)
{
    return p_;
}

void IDW::set_q(vector<QPoint> q_)
{
    for(int i=0;i<q_.size();i++)
    {
        QPoint temp;
        temp=q_[i];
        this->q_.push_back(QPoint(temp.x()-start_point.x(),temp.y()-start_point.y()));
    }

}

vector<QPoint> IDW::get_q(void)
{
    return q_;
}

void IDW::set_start(QPoint s)
{
    this->start_point=s;
}
void IDW::set_end(QPoint e)
{
    this->end_point=e;
}

