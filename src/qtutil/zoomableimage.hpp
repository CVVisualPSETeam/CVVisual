#ifndef CVVISUAL_ZOOMABLEIMAGE_HPP
#define CVVISUAL_ZOOMABLEIMAGE_HPP

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QRectF>
#include <QGraphicsPixmapItem>
#include <QWheelEvent>
#include <QApplication>

#include "opencv2/core/core.hpp"

#include "util.hpp"
#include "../util/util.hpp"
#include "../dbg/dbg.hpp"

namespace cvv{ namespace qtutil{
namespace structures {
/**
 * @brief A graphics view with overwritten event handlers.
 */
class GraphicsView:public QGraphicsView
{
public:
	/**
	 * @brief Constructor
	 */
	GraphicsView():QGraphicsView{}
	{
		TRACEPOINT;
	}
protected:
	/**
	 * @brief Ignores the wheel event if ctrl is pressed.
	 * @param event The event.
	 */
	virtual void wheelEvent(QWheelEvent * event) override;
};
}


/**
 * @brief The ZoomableImage class shows an image and adds zoom functionality.
 */
class ZoomableImage:public QWidget
{
Q_OBJECT
public:
	/**
	 * @brief Constructor
	 * @param mat The image to display
	 * @param parent The parent widget.
	 */
	ZoomableImage(const cv::Mat& mat=cv::Mat{}, QWidget* parent = nullptr);

	/**
	 * @brief Returns the current image.
	 * @return The current image.
	 */
	const cv::Mat& mat() const
	{
		TRACEPOINT;
		return mat_;
	}

	/**
	 * @brief Returns the current image.
	 * @return The current image.
	 */
	cv::Mat& mat()
	{
		TRACEPOINT;
		return mat_;
	}

	/**
	 * @brief Returns the visible area of the image.
	 * @return The visible area of the image.
	 */
	QRectF visibleArea() const;

	/**
	 * @brief Returns the zoom factor.
	 * @return The zoom factor.
	 */
	qreal zoom() const
	{
		TRACEPOINT;
		return zoom_;
	}

	QPointF mapImagePointToParent(QPointF) const;

	/**
	 * @brief Returns the threshold that determines wheather the pixelvalues are shown.
	 * @return
	 */
	qreal threshold() const
		{TRACEPOINT;return threshold_;}

	/**
	 * @brief The overridden resize event (from QWidget).
	 */
	virtual void resizeEvent(QResizeEvent*) override
	{
		TRACEPOINT;
		emit updateArea(visibleArea(),zoom_);
		TRACEPOINT;
	}

	/**
	 * @brief Returns the width of the image.
	 * @return The width of the image.
	 */
	int imageWidth() const
	{
		TRACEPOINT;
		return mat_.cols;
	}

	/**
	 * @brief Returns the height of the image.
	 * @return The height of the image.
	 */
	int imageHeight() const
	{
		TRACEPOINT;
		return mat_.rows;
	}

	/**
	 * @brief Returns weather pixel values are shown depending on threshold.
	 * @return Weather pixel values are shown depending on threshold.
	 */
	bool autoShowValues() const
	{
		TRACEPOINT;
		return autoShowValues_;
	}

	/**
	 * @brief Returns the current scroll factor for CTRL+scroll.
	 * @return The current scroll factor for CTRL+scroll.
	 */
	qreal scrollFactorCTRL() const
	{
		TRACEPOINT;
		return scrollFactorCTRL_;
	}

	/**
	 * @brief Returns the current scroll factor for CTRL+shift+scroll.
	 * @return The current scroll factor for CTRL+shift+scroll.
	 */
	qreal scrollFactorCTRLShift() const
	{
		TRACEPOINT;
		return scrollFactorCTRLShift_;
	}
signals:
	/**
	 * @brief Emmited whenever the image is updated. It passes the conversion result.
	 */
	void updateConversionResult(ImageConversionResult);

	/**
	 *@brief Emitted whenever the visible area changes. Passes the visible area and zoom factor.
	 */
	void updateArea(QRectF,qreal);

public slots:
	/**
	 * @brief Sets the curent visible area (the center) and zoom factor.
	 * @param rect The area.
	 * @param zoom The zoom.
	 */
	void setArea(QRectF rect,qreal zoom);

	/**
	 * @brief Updates the image to display.
	 * @param mat The new image to display.
	 */
	void setMat(cv::Mat mat);

	/**
	 * @brief Updates the zoom factor.
	 * @param factor The zoom factor.
	 */
	void setZoom(qreal factor = 1);

	/**
	 * @brief Sets weather pixel values are shown depending on threshold.
	 * @param enable If true pixel values are shown depending on threshold.
	 */
	void setAutoShowValues(bool enable = true)
	{
		TRACEPOINT;
		autoShowValues_=enable;
		TRACEPOINT;
	}

	/**
	 * @brief Sets the threshold that determines wheather the pixelvalues are shown.
	 * @param threshold The threshold.
	 */
	void setThreshold(qreal threshold = 60)
	{
		TRACEPOINT;
		threshold_=threshold;
		TRACEPOINT;
	}

	/**
	 * @brief Resizes the image so it is fully visible.
	 */
	void showFullImage();

	/**
	 * @brief Returns the current CTRL+scroll zoom factor.
	 * @return The current scroll factor.
	 */
	void setCTRLZoomFactor(qreal factor = 1.025)
	{
		TRACEPOINT;
		scrollFactorCTRL_=factor;
		TRACEPOINT;
	}

	/**
	 * @brief Returns the current CTRL+ shift+scroll zoom factor.
	 * @return The current scroll factor.
	 */
	void setCTRLShiftZoomFactor(qreal factor = 1.01)
	{
		TRACEPOINT;
		scrollFactorCTRLShift_=factor;
		TRACEPOINT;
	}

protected:
	/**
	 * @brief The overridden wheel event (from QWidget).
	 * @param event The event.
	 */
	virtual void wheelEvent(QWheelEvent * event) override;

private slots:
	/**
	 * @brief Called when the graphic view is scrolled.
	 */
	void viewScrolled()
	{
		TRACEPOINT;
		emit updateArea(visibleArea(),zoom_);
		TRACEPOINT;
	}

	/**
	 * @brief Draws the pixel value for all visible pixels.
	 */
	void drawValues();
private:
	/**
	 * @brief The image to display.
	 */
	cv::Mat mat_;

	/**
	 * @brief The pixmap containing the converted image.
	 */
	QGraphicsPixmapItem* pixmap_;
	/**
	 * @brief The graphics view showing the scene.
	 */
	structures::GraphicsView* view_;
	/**
	 * @brief The scene containing the pixmap.
	 */
	QGraphicsScene* scene_;
	/**
	 * @brief The current zoom factor.
	 */
	qreal zoom_;

	/**
	 * @brief The current threshold.
	 */
	qreal threshold_;
	/**
	 * @brief Weather pixel values are shown depending on threshold.
	 */
	bool autoShowValues_;
	/**
	 * @brief The values on the graphics scene.
	 */
	std::vector<QGraphicsTextItem*> values_;
	/**
	 * @brief The factor multiplied to the number of scrolled pixels.
	 */
	qreal scrollFactorCTRL_;

	/**
	 * @brief The factor multiplied to the number of scrolled pixels.
	 */
	qreal scrollFactorCTRLShift_;
};

}}
#endif
