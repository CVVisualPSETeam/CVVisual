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

  histMat_ = calcHist(mat_, qrect2cvrect(mat_, rect));
}

cv::Mat
Histogram::calcHist(cv::Mat mat, cv::Rect rect, int bins, float rangeMin, float rangeMax)
{
  cv::Mat rectMat(mat, rect);
  cv::Mat histMat;
  std::vector<int> channels(rectMat.channels());
  for (int ch = 0; ch < rectMat.channels(); ch++) {
    channels[ch] = ch;
  }
  std::vector<int> histSize(rectMat.channels(), bins);
  float oneChannelRanges[] = { rangeMin, rangeMax };
  const float** ranges = new const float*[rectMat.channels()];
  for (int ch = 0; ch < rectMat.channels(); ch++) {
    ranges[ch] = oneChannelRanges;
  }
  cv::calcHist(&rectMat, 1, channels.data(), cv::Mat(), histMat, rectMat.channels(), histSize.data(), ranges);
  delete[] ranges;

  return histMat;
}

}
}
