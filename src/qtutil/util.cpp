#include "util.hpp"

#include <opencv/highgui.h>

namespace cvv { namespace qtutil {

QImage convertOpenCVMatToQImage(const cv::Mat &mat)
{
    CvMat* image2Draw_mat;
    image2Draw_mat = cvCreateMat(mat.rows, mat.cols, CV_8UC3);
    QImage image2Draw_qt = QImage(image2Draw_mat->data.ptr, image2Draw_mat->cols, image2Draw_mat->rows, image2Draw_mat->step, QImage::Format_RGB888);
    return image2Draw_qt;
}

QPixmap convertOpenCVMatToQPixmap(const cv::Mat &mat)
{
    return QPixmap::fromImage(convertOpenCVMatToQImage(mat));
}

QSet<QString> createStringSet(QString string)
{
    QSet<QString> set;
    set.insert(string);
    return set;
}
}}
