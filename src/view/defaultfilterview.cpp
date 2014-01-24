#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QApplication>
#include <QtGui>


#include "defaultfilterview.hpp"
#include "../qtutil/accordion.hpp"
#include "../qtutil/util.hpp"

namespace cvv{ namespace view{


	DefaultFilterView::DefaultFilterView(std::vector<cv::Mat> images,QWidget *parent):FilterView{parent},images_(images){
		QHBoxLayout* layout = new QHBoxLayout{};
		qtutil::Accordion *accor = new qtutil::Accordion{};
		layout->addWidget(accor);
		QWidget *imwid = new QWidget{this};
		QHBoxLayout* imageLayout = new QHBoxLayout{};
		for(auto image:images_){
			QLabel* label = new QLabel{};
            label->setPixmap(qtutil::convertOpenCVMatToQPixmap(image));
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
