
#include <opencv2/core/core.hpp>
#include <QHBoxLayout>
#include <QWidget>

#include "dummyfilterview.hpp"
#include "../qtutil/accordion.hpp"

namespace cvv{ namespace view
{
	DummyFilterView::DummyFilterView(QWidget *parent) : FilterView{parent}
	{
		QHBoxLayout* layout = new QHBoxLayout{};
		qtutil::Accordion *accor = new qtutil::Accordion{this};
		layout->addWidget(accor);
		layout->addWidget(new QWidget{});
		setLayout(layout);
	}
/*
	static DummyFilterView* create(QWidget *parent)
	{
		(void) parent;
		return 0;
	}
*/
}}//namespaces
