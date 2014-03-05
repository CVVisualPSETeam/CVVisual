
#include <QVBoxLayout>


#include "matchselectionselector.hpp"
#include "../../util/util.hpp"

namespace cvv{ namespace qtutil{

MatchSelectionSelector::MatchSelectionSelector(const std::vector<cv::DMatch> &univers, QWidget *parent):
	MatchSelection{parent},
	univers_{univers}
{
	auto layout=util::make_unique<QVBoxLayout>();
	layout->addWidget(comboBox_);

	connect(&signalElementSelected(),SIGNAL(signal(QString)),this,SLOT(changeSelector()));
	setLayout(layout.release());
	changeSelector();
}


std::vector<cv::DMatch> cvv::qtutil::MatchSelectionSelector::select(const std::vector<cv::DMatch> &selection)
{
	return selection_->select(selection);
}

void MatchSelectionSelector::changeSelector()
{
	auto selection=(*this)()(univers_);
	layout_->removeWidget(selection_);
	selection_=selection.get();
	layout_->addWidget(selection.release());

}

}}
