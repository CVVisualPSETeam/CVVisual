#include <vector>
#include <memory>

#include <QString>
#include <QMap>
#include <QPushButton>
#include <QComboBox>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

#include "filter_call_tab.hpp"
#include "../view/filter_view.hpp"
#include "../controller/view_controller.hpp"
#include "../impl/filter_call.hpp"
#include "../qtutil/util.hpp"

#include "../dbg/dbg.hpp"

namespace cvv {
namespace gui {

FilterCallTab::FilterCallTab(const cvv::impl::FilterCall& filterCall):
	FilterCallTab{filterCall.description(), filterCall}
{
	TRACEPOINT;
}

FilterCallTab::FilterCallTab(const QString& tabName, const cvv::impl::FilterCall& filterCall):
	filterCall_{filterCall}
{
	TRACEPOINT;
	setName(tabName);
	const QString scope{"default_views"};
	const QString key{"default_filter_view"};
	// Sets DefaultFilterView as default in case no other default is set:
	qtutil::setDefaultSetting(scope, key, "DefaultFilterView");
	filterViewId_ = qtutil::getSetting(scope, key);
	createGui();
	TRACEPOINT;
}

FilterCallTab::FilterCallTab(const QString& tabName, const cvv::impl::FilterCall& filterCall, const QString& viewId):
	filterCall_{filterCall}
{
	TRACEPOINT;
	setName(tabName);
	filterViewId_ = viewId;
	createGui();
	TRACEPOINT;
}

void FilterCallTab::currentIndexChanged()
{
	TRACEPOINT;
	vlayout_->removeWidget(filterView_);
	filterView_->setVisible(false);
	setView();
	TRACEPOINT;
}

void FilterCallTab::helpButtonClicked() const
{
	TRACEPOINT;
	qtutil::openHelpBrowser(filterViewId_);
	TRACEPOINT;
}

void FilterCallTab::setAsDefaultButtonClicked()
{
	TRACEPOINT;
	qtutil::setSetting("default_views", "default_filter_view", filterViewId_);
	TRACEPOINT;
}

size_t FilterCallTab::getId() const
{
	TRACEPOINT;
	return filterCall_->getId();
}

void FilterCallTab::createGui()
{
	TRACEPOINT;
	if(!select(filterViewId_))
	{
		select("DefaultFilterView");
		filterViewId_ = selection();
		setAsDefaultButtonClicked();	// Set as default.
		/* If filterViewId_ does not name a valid View, it will be attempted to set DefaultFilterView.
		 * If that was not registered either, the current selection of the ComboBox will be used automatically.
		 * Whichever was chosen will be set as the new default. */
	}
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

void FilterCallTab::setView()
{
	TRACEPOINT;
	filterViewId_ = selection();
	if (viewHistory_.count(selection()))
	{
		filterView_ = viewHistory_.at(selection());
		vlayout_->addWidget(filterView_);
		filterView_->setVisible(true);
	} else
	{
		std::vector<cv::Mat> images;
		images.push_back(filterCall_->original());
		images.push_back(filterCall_->result());
		viewHistory_.emplace(selection(), ((*this)()(images, this).release()));
		filterView_ = viewHistory_.at(selection());
		vlayout_->addWidget(filterView_);
	}
	TRACEPOINT;
}

}}//namespaces
