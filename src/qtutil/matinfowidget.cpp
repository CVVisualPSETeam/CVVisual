#include <QVBoxLayout>

#include "matinfowidget.hpp"

		 labelConvert = new QLabel{};
namespace cvv{ namespace qtutil {

	MatInfoWidget::MatInfoWidget(cv::Mat mat,QWidget *parent):QWidget{parent}{
		labelDim = new QLabel{};
		labelType = new QLabel{};
		labelChannel = new QLabel{};
		labelSize = new QLabel{};
		labelDepth = new QLabel{};

		QVBoxLayout *basicLayout=new QVBoxLayout{};

		basicLayout->addWidget(labelConvert);
		basicLayout->addWidget(labelDim);
		basicLayout->addWidget(labelType);
		basicLayout->addWidget(labelChannel);
		basicLayout->addWidget(labelSize);
		basicLayout->addWidget(labelDepth);

	setLayout(basicLayout);
		updateMat(mat);
	}

	void MatInfoWidget::updateConvertStatus(QString str){
		labelConvert->setText(QString("Convert Status").append(str));
	}

	void MatInfoWidget::updateMat(cv::Mat mat){
		if(mat.empty()){
			labelDim->setText("empty");
			labelType->setText("empty");
			labelChannel->setText("empty");
			labelSize->setText("empty");
			labelDepth->setText("empty");
		}else{
			labelDim->setText(QString("Dimension: %1").arg(mat.dims));
			labelChannel->setText(QString("Number of Channels: %1").arg(mat.channels()));
			labelSize->setText(QString("Size: %1/%2").arg(mat.rows).arg((mat.cols)));
			labelDepth->setText(QString("Depth: %1").arg(mat.depth()));

			switch(mat.type()){
			case CV_8U: labelType->setText("Type: CV_8U");
				break;
			 case CV_8S: labelType->setText("Type: CV_5U");
				break;
			 case CV_16U: labelType->setText("Type: CV_16U");
				break;
			 case CV_16S: labelType->setText("Type: CV_16S");
				break;
			 case CV_32S: labelType->setText("Type: CV_32S");
				break;
			 case CV_32F: labelType->setText("Type: CV_32F");
				break;
			 case CV_64F: labelType->setText("Type: CV_64S");
				break;
			 default:
				labelType->setText("Type: Unkown");
				break;
			}
		}
	}

}}
