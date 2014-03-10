#include <QVBoxLayout>
#include <QPushButton>

#include "showinrawviewwidget.hpp"
#include "../../util/util.hpp"

namespace  cvv {namespace qtutil{

ShowInRawView::ShowInRawView(const std::vector<cv::KeyPoint> left_key,
			     const std::vector<cv::KeyPoint> right_key,
			     const std::vector<cv::DMatch> matches,
			     MatchManagement* matchmnt,
			     KeyPointManagement* keymnt,
			     QWidget *parent):
	QWidget{parent},
	matchmnt_{matchmnt},
	keymnt_{keymnt},
	rawViewWindow_{new RawviewWindow{"Current Selection",left_key,right_key,matches}}
{
	rawViewWindow_->setVisible(false);
	auto layout=util::make_unique<QVBoxLayout>();
	auto showhidebutton=util::make_unique<QPushButton>("show/hide");

	connect(showhidebutton.get(),SIGNAL(clicked()),this,SLOT(showHideRawviewWindow()));

	connect(keymnt_,SIGNAL(updateSelection(std::vector<cv::KeyPoint>)),
		rawViewWindow_,SLOT(selectKeyPoints(std::vector<cv::KeyPoint>)));
	connect(matchmnt_,SIGNAL(updateSelection(std::vector<cv::DMatch>)),
		rawViewWindow_,SLOT(selectMatches(std::vector<cv::DMatch>)));

	connect(rawViewWindow_,SIGNAL(keyPointsSelected(std::vector<cv::KeyPoint>)),
		keymnt,SLOT(setSelection(std::vector<cv::KeyPoint>)));
	connect(rawViewWindow_,SIGNAL(matchesSelected(std::vector<cv::DMatch>)),
		matchmnt_,SLOT(setSelection(std::vector<cv::DMatch>)));


	layout->addWidget(showhidebutton.release());
	setLayout(layout.release());
}

}}
