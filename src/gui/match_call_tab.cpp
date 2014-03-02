#include <vector>
#include <memory>

#include <QString>
#include <QMap>
#include <QPushButton>
#include <QComboBox>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

#include "match_call_tab.hpp"
#include "../view/match_view.hpp"
#include "../controller/view_controller.hpp"
#include "../impl/match_call.hpp"
#include "../dbg/dbg.hpp"
#include "../qtutil/util.hpp"

namespace cvv {
namespace gui {

MatchCallTab::MatchCallTab(const cvv::impl::MatchCall& matchCall): matchCall_{matchCall}
{
	TRACEPOINT;
	setName(matchCall_->description());
	const QString scope{"default_views"};
	const QString key{"default_match_view"};
	qtutil::setDefaultSetting(scope, key, "LineMatchView");
	matchViewId_ = qtutil::getSetting(scope, key);
	createGui();
	TRACEPOINT;
}

MatchCallTab::MatchCallTab(const QString& tabName, const cvv::impl::MatchCall& matchCall):
	matchCall_{matchCall}
{
	TRACEPOINT;
	setName(tabName);
	const QString scope{"default_views"};
	const QString key{"default_match_view"};
	qtutil::setDefaultSetting(scope, key, "LineMatchView");
	matchViewId_ = qtutil::getSetting(scope, key);
	createGui();
	TRACEPOINT;
}

MatchCallTab::MatchCallTab(const QString& tabName, const cvv::impl::MatchCall& matchCall, const QString& viewId):
	matchCall_{matchCall}
{
	TRACEPOINT;
	setName(tabName);
	matchViewId_ = viewId;
	createGui();
	TRACEPOINT;
}

void MatchCallTab::currentIndexChanged()
{
	TRACEPOINT;
	vlayout_->removeWidget(matchView_);
	matchView_->setVisible(false);
	setView();
	TRACEPOINT;
}

void MatchCallTab::helpButtonClicked() const
{
	TRACEPOINT;
	qtutil::openHelpBrowser(matchViewId_);
	TRACEPOINT;
}

void MatchCallTab::setAsDefaultButtonClicked()
{
	TRACEPOINT;
	qtutil::setSetting("default_views", "default_match_view", matchViewId_);
	TRACEPOINT;
}

size_t MatchCallTab::getId() const
{
	TRACEPOINT;
	return matchCall_->getId();
}

void MatchCallTab::addMatchViewToMap(const QString& matchViewId, MatchViewBuilder mView)
{
	TRACEPOINT;
	cvv::qtutil::RegisterHelper<cvv::view::MatchView, const cv::Mat&,
			const std::vector<cv::KeyPoint>&,
			const cv::Mat&, const std::vector<cv::KeyPoint>&,
			const std::vector<cv::DMatch>&, QWidget*>::registerElement(matchViewId, mView);
	TRACEPOINT;
}

void MatchCallTab::createGui()
{
	TRACEPOINT;
	if(!select(matchViewId_))
	{
		select("LineMatchView");
		matchViewId_ = selection();
		setAsDefaultButtonClicked();	// Set as default.
		/* If matchViewId_ does not name a valid View, it will be attempted to set LineMatchView.
		 * If that was not registered either, the current selection of the ComboBox will be used automatically.
		 * Whichever was chosen will be set as the new default. */
	}
	comboBox_->setCurrentText(matchViewId_);
	hlayout_ = new QHBoxLayout{};
	hlayout_->setAlignment(Qt::AlignTop);
	hlayout_->addWidget(new QLabel{"View:"});
	hlayout_->addWidget(comboBox_);
	setAsDefaultButton_ = new QPushButton{"Set as default", this};
	hlayout_->addWidget(setAsDefaultButton_);
	helpButton_ = new QPushButton{"Help", this};
	hlayout_->addWidget(helpButton_);
	upperBar_ = new QWidget{this};
	upperBar_->setLayout(hlayout_);

	vlayout_ = new QVBoxLayout{};

	vlayout_->addWidget(upperBar_);
	setView();

	setLayout(vlayout_);

	connect(setAsDefaultButton_, SIGNAL(clicked()), this, SLOT(setAsDefaultButtonClicked()));
	connect(helpButton_, SIGNAL(clicked()), this, SLOT(helpButtonClicked()));
	connect(&signalElementSelected(), SIGNAL(signal(QString)), this, SLOT(currentIndexChanged()));
	TRACEPOINT;
}

void MatchCallTab::setView()
{
	TRACEPOINT;
	matchViewId_ = selection();
	if (viewHistory_.count(selection()))
	{
		matchView_ = viewHistory_.at(selection());
		vlayout_->addWidget(matchView_);
		matchView_->setVisible(true);
	} else
	{
		viewHistory_.emplace(selection(), ((*this)()(matchCall_->img1(), matchCall_->keyPoints1(),
							     matchCall_->img2(), matchCall_->keyPoints2(),
							     matchCall_->matches(), this).release()));
		matchView_ = viewHistory_.at(selection());
		vlayout_->addWidget(matchView_);
	}
	TRACEPOINT;

}

}}//namespaces
