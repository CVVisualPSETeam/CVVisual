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

namespace cvv {
namespace gui {

MatchCallTab::MatchCallTab(const cvv::impl::MatchCall& fc, const cvv::controller::ViewController& vc):
	matchCall_{fc}, viewController_{vc}
{
	setName(matchCall_->description());
	const QString scope{"default_views"};
	const QString key{"default_match_view"};
	QString setting;
	try
	{
		setting = vc.getSetting(scope, key);
	} catch (std::invalid_argument)
	{
		setting = "DefaultMatchView";
	}
	matchViewId_ = setting;

	createGui();
}

MatchCallTab::MatchCallTab(const QString& tabName, const cvv::impl::MatchCall& fc, const cvv::controller::ViewController& vc):
	matchCall_{fc}, viewController_{vc}
{
	setName(tabName);
	const QString scope{"default_views"};
	const QString key{"default_match_view"};
	QString setting;
	try
	{
		setting = vc.getSetting(scope, key);
	} catch (std::invalid_argument)
	{
		setting = "DefaultMatchView";
	}
	matchViewId_ = setting;

	createGui();
}

MatchCallTab::MatchCallTab(const QString& tabName, const cvv::impl::MatchCall& fc, const cvv::controller::ViewController& vc, const QString& viewId):
	matchCall_{fc}, viewController_{vc}
{
	setName(tabName);
	matchViewId_ = viewId;

	createGui();
}

void MatchCallTab::currentIndexChanged(const QString& text)
{
	matchViewId_ = text;
	delete matchView_;
	setView(matchViewId_);
}

void MatchCallTab::helpButtonClicked() const
{
	viewController_->openHelpBrowser(matchViewId_);
}

size_t MatchCallTab::getId() const
{
	return matchCall_->getId();
}

void MatchCallTab::addMatchViewToMap(const QString& matchViewId,
				       std::function<std::unique_ptr<cvv::view::MatchView>(std::vector<cv::Mat>, QWidget*)> fView)
{
	cvv::qtutil::RegisterHelper<cvv::view::MatchView, std::vector<cv::Mat>, QWidget*>::registerElement(matchViewId, fView);
}

void MatchCallTab::createGui()
{
	comboBox_->setCurrentText(matchViewId_);
	hlayout_ = new QHBoxLayout{this};
	hlayout_->setAlignment(Qt::AlignTop);
	hlayout_->addWidget(new QLabel{"View:"});
	hlayout_->addWidget(comboBox_);
	helpButton_ = new QPushButton{"Help", this};
	hlayout_->addWidget(helpButton_);
	connect(helpButton_, SIGNAL(clicked()), this, SLOT(helpButtonClicked()));

	QWidget* upperBar = new QWidget{this};
	upperBar->setLayout(hlayout_);

	vlayout_ = new QVBoxLayout{this};

	vlayout_->addWidget(upperBar);
	setView(matchViewId_);

	setLayout(vlayout_);
	connect(comboBox_, SIGNAL(currentTextChanged(QString)), this, SLOT(currentIndexChanged(QString)));
}

void MatchCallTab::setView(const QString &viewId)
{
	try
	{
		auto fct = registeredElements_.at(viewId);
		std::vector<cv::Mat> images;
		/*images. push_back(matchCall_->original());
		images.push_back(matchCall_->result());*/
		matchView_ = (fct(images, this)).release();
		vlayout_->addWidget(matchView_);
	} catch (std::out_of_range)
	{
		vlayout_->addWidget(new QLabel{"Error: View could not be set up."});
	}

}

}}//namespaces
