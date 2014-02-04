#ifndef CVVISUAL_ZOOMABLEIMAGE
#define CVVISUAL_ZOOMABLEIMAGE

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QRectF>
#include <QGraphicsPixmapItem>

#include "opencv2/core/core.hpp"

#include "util.hpp"
#include "../util/util.hpp"

namespace cvv{ namespace qtutil{

class ZoomableImage:public QWidget
{
Q_OBJECT
public:
	ZoomableImage(const cv::Mat& mat=cv::Mat{}, QWidget* parent = nullptr);

	const cv::Mat& mat() const
		{return mat_;}

	QRectF visibleArea() const;

	qreal zoom() const
		{return zoom_;}
	QPointF mapImagePointToParent(QPointF);

signals:
	void updateConversionResult(ImageConversionResult);
	void updateArea(QRectF,qreal);

public slots:
	void updateMat(cv::Mat mat);
	void updateZoom(qreal factor);
	void showValues(bool show);
	void autoShowValues(bool enable)
		{autoShowValues_=enable;}

private slots:
	void viewScrolled()
		{emit updateArea(visibleArea(),zoom_);}
	void drawValues();
private:

	cv::Mat mat_;

	QGraphicsPixmapItem* pixmap_;
	QGraphicsView* view_;
	QGraphicsScene* scene_;
	qreal zoom_;

	qreal threshold_;
	bool autoShowValues_;
	bool valuesVisible_;
	std::vector<QGraphicsTextItem*> values_;
};

}}
#endif
