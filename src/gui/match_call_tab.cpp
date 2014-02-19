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

MatchCallTab::MatchCallTab(const cvv::impl::MatchCall& fc): matchCall_{fc}
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

MatchCallTab::MatchCallTab(const QString& tabName, const cvv::impl::MatchCall& fc):
	matchCall_{fc}
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

MatchCallTab::MatchCallTab(const QString& tabName, const cvv::impl::MatchCall& fc, const QString& viewId):
	matchCall_{fc}
{
	TRACEPOINT;
	setName(tabName);
	matchViewId_ = viewId;

	createGui();
	TRACEPOINT;
}

void MatchCallTab::currentIndexChanged(const QString& text)
{
	TRACEPOINT;
	matchViewId_ = text;
	vlayout_->removeWidget(matchView_);
	matchView_->setVisible(false);
	setView(matchViewId_);
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
	qtutil::setDefaultSetting("default_views", "default_match_view", matchViewId_);
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
	comboBox_->setCurrentText(matchViewId_);
	hlayout_ = new QHBoxLayout{this};
	hlayout_->setAlignment(Qt::AlignTop);
	hlayout_->addWidget(new QLabel{"View:"});
	hlayout_->addWidget(comboBox_);
	setAsDefaultButton_ = new QPushButton{"Set as default", this};
	hlayout_->addWidget(setAsDefaultButton_);
	connect(setAsDefaultButton_, SIGNAL(clicked()), this, SLOT(setAsDefaultButtonClicked()));
	helpButton_ = new QPushButton{"Help", this};
	hlayout_->addWidget(helpButton_);
	connect(helpButton_, SIGNAL(clicked()), this, SLOT(helpButtonClicked()));

	upperBar_ = new QWidget{this};
	upperBar_->setLayout(hlayout_);

	vlayout_ = new QVBoxLayout{this};

	vlayout_->addWidget(upperBar_);
	setView(matchViewId_);

	setLayout(vlayout_);
	connect(comboBox_, SIGNAL(currentTextChanged(QString)), this, SLOT(currentIndexChanged(QString)));
	TRACEPOINT;
}

void MatchCallTab::setView(const QString &viewId)
{
	TRACEPOINT;
	if (viewHistory_.count(viewId) > 0)
	{
		matchView_ = viewHistory_.at(viewId);
		vlayout_->addWidget(matchView_);
		matchView_->setVisible(true);
		return;
	}
	if (registeredElements_.count(viewId) > 0)
	{
		auto fct = registeredElements_.at(viewId);
		viewHistory_.emplace(viewId, (fct(matchCall_->img1(), matchCall_->keyPoints1(), matchCall_->img2(),
						  matchCall_->keyPoints2(), matchCall_->matches(), this).release()));
		matchView_ = viewHistory_.at(viewId);
		vlayout_->addWidget(matchView_);
		return;
	}
	vlayout_->addWidget(new QLabel{"Error: View could not be set up."});
	TRACEPOINT;

}

}}//namespaces
