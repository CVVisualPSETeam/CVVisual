#ifndef CVVISUAL_ZOOMABLEIMAGE_HPP
#define CVVISUAL_ZOOMABLEIMAGE_HPP

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QRectF>
#include <QGraphicsPixmapItem>

#include "opencv2/core/core.hpp"

#include "util.hpp"
#include "../util/util.hpp"
#include "../dbg/dbg.hpp"

namespace cvv{ namespace qtutil{

class ZoomableImage:public QWidget
{
Q_OBJECT
public:
	ZoomableImage(const cv::Mat& mat=cv::Mat{}, QWidget* parent = nullptr);

	const cv::Mat& mat() const
		{TRACEPOINT;return mat_;}

	QRectF visibleArea() const;

	qreal zoom() const
		{TRACEPOINT;return zoom_;}

	QPointF mapImagePointToParent(QPointF);

	qreal threshold() const
		{TRACEPOINT;return threshold_;}

	virtual void resizeEvent(QResizeEvent*) override
		{TRACEPOINT;emit updateArea(visibleArea(),zoom_);TRACEPOINT;}

	int imageWidth() const
		{TRACEPOINT;return mat_.cols;}

	int imageHeight() const
		{TRACEPOINT;return mat_.rows;}

	bool valuesVisible() const
		{TRACEPOINT;return valuesVisible_;}

	bool autoShowValues() const
		{TRACEPOINT;return autoShowValues_;}

signals:
	void updateConversionResult(ImageConversionResult);
	void updateArea(QRectF,qreal);

public slots:
	void updateMat(cv::Mat mat);
	void updateZoom(qreal factor = 1);
	void setAutoShowValues(bool enable = true)
		{TRACEPOINT;autoShowValues_=enable;TRACEPOINT;}
	void setThreshold(qreal threshold = 60)
		{TRACEPOINT;threshold_=threshold;TRACEPOINT;}
	void showFullImage();


private slots:
	void viewScrolled()
		{TRACEPOINT;emit updateArea(visibleArea(),zoom_);TRACEPOINT;}
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
