#include <QHBoxLayout>
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <QApplication>
#include <QtGui>

#include "../qtutil/accordion.hpp"
#include "../qtutil/filterselectorwidget.hpp"
#include "../qtutil/util.hpp"
#include "dual_filter_view.hpp"

namespace cvv
{
namespace view
{
	DualFilterView::DualFilterView(std::vector<cv::Mat> images, QWidget* parent)
		: FilterView{parent}, rawImages_(images)
	{
		QHBoxLayout* layout = new QHBoxLayout{};
		QHBoxLayout* imageLayout = new QHBoxLayout{};
		QWidget *imwid = new QWidget{};
		qtutil::Accordion *accor = new qtutil::Accordion{};

		auto filterWidget = util::make_unique<qtutil::FilterSelectorWidget<2,1>>();
		//TO DO: filter hinzufügen

		accor->insert("Filter selection", std::move(filterWidget));
		accor->setMinimumSize(150,0);
		layout->addWidget(accor);

		//add images
		for(auto image:rawImages_)
		{
			QLabel* label = new QLabel{};
			label->setMinimumSize(200,200);
			label->setPixmap(qtutil::convertMatToQPixmap(image).second.scaled(200,200,Qt::KeepAspectRatio));
			imageLayout->addWidget(label);
		}
		imwid->setLayout(imageLayout);
		layout->addWidget(imwid);

		setLayout(layout);
	}

}
} //namespaces
