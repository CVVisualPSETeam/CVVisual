
#include <QVBoxLayout>


#include "matchselectionselector.hpp"
#include "../../util/util.hpp"

namespace cvv{ namespace qtutil{

MatchSelectionSelector::MatchSelectionSelector(const std::vector<cv::DMatch> &univers, QWidget *parent):
	MatchSelection{parent},
	RegisterHelper<MatchSelection,std::vector<cv::DMatch>>{},
	univers_{univers}
{
	auto layout=util::make_unique<QVBoxLayout>();

	connect(&signalElementSelected(),SIGNAL(signal(QString)),this,SLOT(changeSelector()));

	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(comboBox_);

	layout_=layout.get();

	setLayout(layout.release());

	if(this->has(this->selection())){
		changeSelector();
	}
}

std::vector<cv::DMatch> cvv::qtutil::MatchSelectionSelector::select(const std::vector<cv::DMatch> &selection)
{
	return selection_->select(selection);
}

void MatchSelectionSelector::changeSelector()
{
	auto selection=(*this)()(univers_);

	if(selection){
		if(selection_){
			layout_->removeWidget(selection_);
			disconnect(selection_,SIGNAL(settingsChanged()),this,SIGNAL(settingsChanged()));
		}

		selection_->deleteLater();
		selection_ = selection.get();
		connect(selection.get(),SIGNAL(settingsChanged()),this,SIGNAL(settingsChanged()));
		layout_->addWidget(selection.release());
	}
}

}}
