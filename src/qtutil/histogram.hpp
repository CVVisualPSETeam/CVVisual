#ifndef CVVISUAL_HISTOGRAM_HPP
#define CVVISUAL_HISTOGRAM_HPP

#include <QWidget>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>


namespace cvv
{
namespace qtutil
{

class Histogram : public QWidget
{
  Q_OBJECT
  public:
    Histogram(const cv::Mat& mat = cv::Mat{}, QWidget* parent = nullptr);

    void setMat(const cv::Mat& mat);
    cv::Mat calcHist(cv::Mat mat, cv::Rect rect, int bins = 256, float rangeMin = 0.0, float rangeMax = 256.0);

  public slots:
    void setArea(QRectF, qreal);

  private:
    cv::Rect qrect2cvrect(const cv::Mat& mat, QRectF qrect);

    cv::Mat mat_;
    cv::Mat histMat_;
};

}
}

#endif // CVVISUAL_HISTOGRAM_HPP
