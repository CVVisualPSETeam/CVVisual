#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QApplication>
#include <QtGui>

#include "defaultfilterview.hpp"
#include "../qtutil/accordion.hpp"
#include "../qtutil/util.hpp"

namespace cvv{ namespace view{

	DefaultFilterView::DefaultFilterView(std::vector<cv::Mat> images,QWidget *parent):FilterView{parent},images_(images)
	{
		QHBoxLayout* layout = new QHBoxLayout{};
		qtutil::Accordion *accor = new qtutil::Accordion{};
		accor->insert("this is a accordion",*(new QWidget{}));
		accor->setMinimumSize(150,0);
		QWidget *imwid = new QWidget{};
		QHBoxLayout* imageLayout = new QHBoxLayout{};
		layout->addWidget(accor);

		for(auto image:images_)
		{
			QLabel* label = new QLabel{};
			label->setMinimumSize(200,200);
			QPixmap pix = qtutil::convertOpenCVMatToQPixmap(image);
			label->setPixmap(pix.scaled(200,200,Qt::KeepAspectRatio));
			imageLayout->addWidget(label);
		}
		imwid->setLayout(imageLayout);
		layout->addWidget(imwid);

		setLayout(layout);
	}

/*	static DefaultFilterView create(const std::vecotr<InputArray>& input)
	{
		return DefaultFilterView{input};
	};
*/	
}}//namespaces
