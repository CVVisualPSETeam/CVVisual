#include <QVBoxLayout>
#include <QPushButton>

#include "zoomableimageoptpanel.hpp"
#include "../util/util.hpp"
#include "util.hpp"

namespace cvv{ namespace qtutil {

ZoomableOptPanel::ZoomableOptPanel(const ZoomableImage& zoomIm,QWidget *parent):QWidget{parent}
{
	TRACEPOINT;
	auto basicLayout 	= cvv::util::make_unique<QVBoxLayout>();

	auto zoomSpin		= cvv::util::make_unique<QDoubleSpinBox>();
	auto labelConvert 	= cvv::util::make_unique<QLabel>();
	auto labelDim 		= cvv::util::make_unique<QLabel>();
	auto labelType 		= cvv::util::make_unique<QLabel>();
	auto labelChannel 	= cvv::util::make_unique<QLabel>();
	auto labelSize 		= cvv::util::make_unique<QLabel>();
	auto labelDepth		= cvv::util::make_unique<QLabel>();
	auto buttonFullImage 	= cvv::util::make_unique<QPushButton>("show full Image");

	connect(&zoomIm,SIGNAL(updateConversionResult(ImageConversionResult,const cv::Mat&)),
		this,SLOT(updateConvertStatus(ImageConversionResult,const cv::Mat&)));

	connect(&zoomIm,SIGNAL(updateArea(QRectF,qreal)),this,SLOT(setZoom(QRectF,qreal)));
	
	connect(zoomSpin.get(),SIGNAL(valueChanged(double)),&zoomIm,SLOT(setZoom(qreal)));

	connect(buttonFullImage.get(),SIGNAL(clicked()),&zoomIm,SLOT(showFullImage()));

	zoomSpin->setMinimum(0.0);
	zoomSpin->setMaximum(2000.0);

	zoomSpin_		= zoomSpin.get();
	labelConvert_ 		= labelConvert.get();
	labelDim_		= labelDim.get();
	labelType_		= labelType.get();
	labelChannel_		= labelChannel.get();
	labelSize_		= labelSize.get();
	labelDepth_		= labelDepth.get();	

	basicLayout->addWidget(zoomSpin.release());
	basicLayout->addWidget(labelConvert.release());
	basicLayout->addWidget(labelDim.release());
	basicLayout->addWidget(labelType.release());
	basicLayout->addWidget(labelChannel.release());
	basicLayout->addWidget(labelSize.release());
	basicLayout->addWidget(labelDepth.release());
	basicLayout->addWidget(buttonFullImage.release());

	setLayout(basicLayout.release());

	updateMat(zoomIm.mat());
	TRACEPOINT;
}

void ZoomableOptPanel::updateConvertStatus(ImageConversionResult result,const cv::Mat& mat){
	TRACEPOINT;
	QString qs{"Convert Status: "};
	switch(result)
	{
		case ImageConversionResult::SUCCESS : 
			labelConvert_->setText(qs.append("SUCCES"));
			break;
		case ImageConversionResult::MAT_EMPTY:
			labelConvert_->setText(qs.append("Empty Mat"));
			break;
		case ImageConversionResult::MAT_NOT_2D:
			labelConvert_->setText(qs.append("Unsupported Dimension"));
			break;
		case ImageConversionResult::FLOAT_OUT_OF_0_TO_1:
			labelConvert_->setText(qs.append("Float values out of range [0,1]"));
			break;
		case ImageConversionResult::NUMBER_OF_CHANNELS_NOT_SUPPORTED:
			labelConvert_->setText(qs.append("Unsupported number of channels"));
			break;
		case ImageConversionResult::MAT_INVALID_SIZE:
			labelConvert_->setText(qs.append("Invalid Size"));
			break;
		case ImageConversionResult::MAT_UNSUPPORTED_DEPTH:
			labelConvert_->setText(qs.append("Unsupported Depth "));
			break;
		default : 
			labelConvert_->setText(qs.append("Unknown result from convert status"));
			break;
	}
	updateMat(mat);
	TRACEPOINT;
}

void ZoomableOptPanel::updateMat(cv::Mat mat){
	TRACEPOINT;
	if(mat.empty()){
		labelDim_->setText("empty");
		labelType_->setText("empty");
		labelChannel_->setText("empty");
		labelSize_->setText("empty");
		labelDepth_->setText("empty");
	}else
	{
		labelDim_->setText(QString("Dimension: %1").arg(mat.dims));
		labelChannel_->setText(QString("Number of Channels: %1").arg(mat.channels()));
		labelSize_->setText(QString("Size: %1/%2").arg(mat.rows).arg((mat.cols)));
		labelDepth_->setText(QString("Depth: %1").arg(mat.depth()));

		switch(mat.type())
		{
			case CV_8U: labelType_->setText("Type: CV_8U");
				break;
			case CV_8S: labelType_->setText("Type: CV_5U");
				break;
			case CV_16U: labelType_->setText("Type: CV_16U");
				break;
			case CV_16S: labelType_->setText("Type: CV_16S");
				break;
			case CV_32S: labelType_->setText("Type: CV_32S");
				break;
			case CV_32F: labelType_->setText("Type: CV_32F");
				break;
			case CV_64F: labelType_->setText("Type: CV_64S");
				break;
			default:
				labelType_->setText("Type: Unkown");
				break;
		}
	}
	TRACEPOINT;
}

void ZoomableOptPanel::setZoom(QRectF,qreal zoomfac)
{
	zoomSpin_->setValue(zoomfac);
}
}}
