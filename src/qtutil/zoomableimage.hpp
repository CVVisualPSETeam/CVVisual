#ifndef CVVISUAL_ZOOMABLEIMAGE
#define CVVISUAL_ZOOMABLEIMAGE

#include <QLabel>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QRectF>

#include "opencv2/core/core.hpp"

#include "util.hpp"
#include "../util/util.hpp"

namespace cvv{ namespace qtutil{

class ZoomableImage:public QWidget
{
Q_OBJECT
public:
	ZoomableImage(const cv::Mat& mat=cv::Mat{},QWidget* parent = nullptr);

	const cv::Mat& mat() const
		{return mat_;}

	QRectF visibleArea() const;

	qreal zoom() const
		{return zoom_;}

signals:
	void updateConversionResult(ImageConversionResult);
	void updateArea(QRectF);

public slots:
	void updateMat(cv::Mat mat);
	void updateZoom(qreal factor);

private slots:
	void viewScrolled()
		{emit updateArea(visibleArea());}
private:
	cv::Mat mat_;

	QGraphicsView* view_;
	std::unique_ptr<QGraphicsScene> scene_;
	qreal zoom_;
};

}}
#endif
