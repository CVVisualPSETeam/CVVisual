
#include <QHBoxLayout>

#include "zoomableimage.hpp"
#include "util.hpp"

namespace cvv{ namespace qtutil{
	ZoomableImage::ZoomableImage(const cv::Mat& mat,QWidget* parent):
		QWidget{parent},mat_(mat)
	{
		label_ = new QLabel{};
		QHBoxLayout *layout=new QHBoxLayout{};
		setLayout(layout) ;
		layout->addWidget(label_);
		updateMat(mat_);
	}

	void ZoomableImage::updateMat(const cv::Mat& mat)
	{
		this->mat_ = mat;
		auto result = convertMatToQPixmap(mat_);
		switch(result.first)
		{
			case ImageConversionResult::SUCCESS : 
				label_->setPixmap(result.second); 
				emit updateInfo("SUCCESS");
				break;
			case ImageConversionResult::MAT_EMPTY:
				emit updateInfo("Empty Mat");
				break;
			case ImageConversionResult::MAT_NOT_2D:
				emit updateInfo("Unsupported Dimension");
				break;
			case ImageConversionResult::FLOAT_OUT_OF_0_TO_1:
				emit updateInfo("Float values out of range [0,1]");
				break;
			case ImageConversionResult::NUMBER_OF_CHANNELS_NOT_SUPPORTED:
				emit updateInfo("Unsupported number of channels");
				break;
			case ImageConversionResult::MAT_INVALID_SIZE:
				emit updateInfo("Invalid Size");
				break;
			case ImageConversionResult::MAT_UNSUPPORTED_DEPTH:
				emit updateInfo("Unsupported Depth ");
				break;
			default : 
				emit updateInfo("Unknown result from convert function");
				break;
		}

	}
}}
