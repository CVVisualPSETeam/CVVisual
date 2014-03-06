
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
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(comboBox_);
	connect(&signalElementSelected(),SIGNAL(signal(QString)),this,SLOT(changeSelector()));
	layout_=layout.get();
	setLayout(layout.release());
	TRACEPOINT;
	if(this->has(this->selection())){
		TRACEPOINT;
		changeSelector();
	}
	TRACEPOINT;
}

std::vector<cv::DMatch> cvv::qtutil::MatchSelectionSelector::select(const std::vector<cv::DMatch> &selection)
{
	return selection_->select(selection);
}

void MatchSelectionSelector::changeSelector()
{
	TRACEPOINT;
	auto selection=(*this)()(univers_);
	TRACEPOINT;
	if(selection){
		TRACEPOINT;
		if(selection_){
			TRACEPOINT;
			layout_->removeWidget(selection_);
		}
		TRACEPOINT;
		selection_->deleteLater();
		TRACEPOINT;
		selection_=selection.get();
		TRACEPOINT;
		layout_->addWidget(selection.release());
		TRACEPOINT;
	}
}

}}
