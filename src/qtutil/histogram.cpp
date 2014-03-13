#include "histogram.hpp"

#include <iostream>

namespace cvv
{
namespace qtutil
{

Histogram::Histogram(const cv::Mat& mat, QWidget* parent)
  :QWidget{parent}
{
  setMat(mat);
}

void
Histogram::setMat(const cv::Mat& mat)
{
  mat_ = mat;
}

cv::Rect
Histogram::qrect2cvrect(const cv::Mat& mat, QRectF qrect)
{
  double x1, y1, x2, y2;
  qrect.getCoords(&x1, &y1, &x2, &y2);
  x1 = std::max(0.0, x1);
  y1 = std::max(0.0, y1);
  x2 = std::min(static_cast<double>(mat.size().width), x2);
  y2 = std::min(static_cast<double>(mat.size().height), y2);
  double width = x2 - x1;
  double height = y2 - y1;

  printf("cv::Rect(%lf, %lf, %lf, %lf)\n", x1, y1, width, height);
  return cv::Rect(x1, y1, width, height);
}

void
Histogram::setArea(QRectF rect, qreal zoom)
{
  (void)zoom;

  double x1, y1, x2, y2;
  rect.getCoords(&x1, &y1, &x2, &y2);
  printf("New area (%lf, %lf) - (%lf, %lf)\n", x1, y1, x2, y2);

  channelHists_ = calcHist(mat_, qrect2cvrect(mat_, rect));
}

std::vector<cv::Mat>
Histogram::calcHist(cv::Mat mat, cv::Rect rect, int bins, float rangeMin, float rangeMax)
{
  cv::Mat rectMat(mat, rect);
  cv::Mat histMat;

  std::vector<cv::Mat> channelPlanes;
  cv::split(rectMat, channelPlanes);

  int histSize = bins;
  float range[] = {rangeMin, rangeMax};
  const float* histRange = {range};
  bool uniform = true;
  bool accumulate = false;

  std::vector<cv::Mat> channelHists(channelPlanes.size());

  for (size_t chan = 0; chan < channelPlanes.size(); chan++) {
    cv::calcHist(&channelPlanes[chan], 1, 0, cv::Mat(), channelHists[chan], 1, &histSize, &histRange, uniform, accumulate);
  }

  return channelHists;
}

}
}
