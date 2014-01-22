#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QApplication>
#include <QtGui>


#include "defaultfilterview.hpp"
#include "../qtutil/accordion.hpp"

namespace cvv{ namespace view{


	DefaultFilterView::DefaultFilterView(std::vector<cv::Mat> images,QWidget *parent):FilterView{parent},images_(images){
		QHBoxLayout* layout = new QHBoxLayout{};
		qtutil::Accordion *accor = new qtutil::Accordion{};
		layout->addWidget(accor);
		QWidget *imwid = new QWidget{this};
		QHBoxLayout* imageLayout = new QHBoxLayout{};
		for(auto image:images_){
			QLabel* label = new QLabel{};
			label->setPixmap(convertCV2OT(image));
			imageLayout->addWidget(label);
		}
		imwid->setLayout(imageLayout);
		layout->addWidget(imwid);
		setLayout(layout);
	}

//hässlich
QImage::Format opencvToQtImageFormat(int opencvFormat)
{
	QImage::Format qtFormat;
	switch (opencvFormat) {
	case CV_8U:
		qtFormat = QImage::Format_Indexed8;
		break;
	case CV_8UC3:
		qtFormat = QImage::Format_RGB888;
		break;
	default:
		qtFormat = QImage::Format_Invalid;
		break;
	}

	return qtFormat;
}

QImage convertCV2OT(cv::Mat m_displayOpencvImg)
{
	QImage::Format qtFormat = opencvToQtImageFormat(m_displayOpencvImg.type());
	QImage myqimage = QImage(m_displayOpencvImg.ptr(),
	m_displayOpencvImg.size().width, m_displayOpencvImg.size().height,m_displayOpencvImg.step[0],qtFormat);

	if (qtFormat == QImage::Format_Indexed8) {
		QVector<QRgb> colorTable; // am besten in konstruktor einmalig initialisieren
		for (int i = 0; i < 256; i++) {
			colorTable.push_back(qRgb(i, i, i));
		}
		myqimage.setColorTable(colorTable);
	} else if (qtFormat == QImage::Format_RGB888) {
		return QPixmap{}.toImage();
 	}
	return myqimage;
}

/*	static DefaultFilterView create(const std::vecotr<InputArray>& input)
	{
		return DefaultFilterView{input};
	};
*/	
}}//namespaces
