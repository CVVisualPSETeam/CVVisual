#ifndef CVVISUAL_MAT_INFO_WIDGET
#define CVVISUAL_MAT_INFO_WIDGET

#include <QLabel>
#include <QString>
#include <QWidget>
#include <QDoubleSpinBox>

#include "opencv2/core/core.hpp"
#include "util.hpp"
#include "../dbg/dbg.hpp"

namespace cvv
{
namespace qtutil
{

class MatInfoWidget : public QWidget
{
	Q_OBJECT
      public:
	MatInfoWidget(cv::Mat, QWidget *parent = nullptr);

      public
slots:
	void updateMat(cv::Mat mat);
	void updateConvertStatus(ImageConversionResult result);
	void setZoom(QRectF, qreal);

signals:
	void getZoom(qreal zoomfac);

      private:
	QDoubleSpinBox *zoomSpin;
	QLabel *labelConvert;
	QLabel *labelDim;
	QLabel *labelType;
	QLabel *labelChannel;
	QLabel *labelSize;
	QLabel *labelDepth;
};
}
}

#endif
