#include "histogram.hpp"

#include <QHBoxLayout>

#include <iostream>

namespace cvv
{
namespace qtutil
{

Histogram::Histogram(const cv::Mat& mat, QWidget* parent)
  :QWidget{parent},
   histSize_(512, 200),
   histLineWidth_(2),
   histBackgroundColor_(255, 255, 255)
{
  setMat(mat);

  zoomableImage = new ZoomableImage();
  auto layout = new QHBoxLayout();
  layout->addWidget(zoomableImage);
  setLayout(layout);
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

  //printf("cv::Rect(%lf, %lf, %lf, %lf)\n", x1, y1, width, height);
  return cv::Rect(x1, y1, width, height);
}

void
Histogram::setArea(QRectF rect, qreal zoom)
{
  (void)zoom;

  /*
  double x1, y1, x2, y2;
  rect.getCoords(&x1, &y1, &x2, &y2);
  printf("New area (%lf, %lf) - (%lf, %lf)\n", x1, y1, x2, y2);
  */

  channelHists_ = calcHist(mat_, qrect2cvrect(mat_, rect));
  histMat_ = drawHist(channelHists_, histSize_, histLineWidth_, histBackgroundColor_);

  zoomableImage->setMat(histMat_);
  zoomableImage->showFullImage();
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

cv::Mat
Histogram::drawHist(const std::vector<cv::Mat>& channelHists, cv::Size histSize, int lineWidth, const cv::Scalar& backgroundColor)
{
  int binCount = channelHists[0].rows;
  int binWidth = cvRound(double(histSize.width)/binCount);
  std::vector<cv::Scalar> colors{cv::Scalar(255, 0, 0), cv::Scalar(0, 255, 0), cv::Scalar(0, 0, 255), cv::Scalar(0, 0, 0)}; // BGR

  cv::Mat histMat(histSize, CV_8UC3, backgroundColor);

  double maxVal = 0;
  for (auto& hist : channelHists) {
    double tmpMaxVal;
    cv::minMaxLoc(hist, NULL, &tmpMaxVal);
    maxVal = std::max(maxVal, tmpMaxVal);
  }

  double valScale = histSize.height / maxVal;
  for (size_t channel = 0; channel < channelHists.size(); channel++) {
    auto& hist = channelHists[channel];
    auto& color = colors[channel];

    for (int bin = 1; bin < binCount; bin++) {
      //printf("%zd:%d=%f\n", channel, bin, hist.at<float>(bin));
      auto pt1 = cv::Point(binWidth * (bin-1), histSize.height - cvRound(hist.at<float>(bin-1) * valScale));
      auto pt2 = cv::Point(binWidth * bin, histSize.height - cvRound(hist.at<float>(bin) * valScale));
      cv::line(histMat, pt1, pt2, color, lineWidth);
    }
  }

  return histMat;
}

}
}
