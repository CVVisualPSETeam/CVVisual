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

FilterCallTab::FilterCallTab(const cvv::impl::FilterCall& fc): filterCall_{fc}
{
	TRACEPOINT;
	FilterCallTab(filterCall_->description(), fc);
	TRACEPOINT;
}

FilterCallTab::FilterCallTab(const QString& tabName, const cvv::impl::FilterCall& fc):
	filterCall_{fc}
{
	TRACEPOINT;
	setName(tabName);
	const QString scope{"default_views"};
	const QString key{"default_filter_view"};
	qtutil::setDefaultSetting(scope, key, "DefaultFilterView"); 
	filterViewId_ = qtutil::getSetting(scope, key);
	createGui();
	TRACEPOINT;
}

FilterCallTab::FilterCallTab(const QString& tabName, const cvv::impl::FilterCall& fc, const QString& viewId):
	filterCall_{fc}
{
	TRACEPOINT;
	setName(tabName);
	filterViewId_ = viewId;
	createGui();
	TRACEPOINT;
}

void FilterCallTab::currentIndexChanged(const QString& text)
{
	TRACEPOINT;
	filterViewId_ = text;
	vlayout_->removeWidget(filterView_);
	filterView_->setVisible(false);
	setView(filterViewId_);
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
	qtutil::setDefaultSetting("default_views", "default_filter_view", filterViewId_);
	TRACEPOINT;
}

size_t FilterCallTab::getId() const
{
	TRACEPOINT;
	return filterCall_->getId();
}

void FilterCallTab::addFilterViewToMap(const QString& filterViewId,
				       std::function<std::unique_ptr<cvv::view::FilterView>(const std::vector<cv::Mat>&, QWidget*)> fView)
{
	TRACEPOINT;
	cvv::qtutil::RegisterHelper<cvv::view::FilterView, const std::vector<cv::Mat>&, QWidget*>::registerElement(filterViewId, fView);
	TRACEPOINT;
}

void FilterCallTab::createGui()
{
	TRACEPOINT;
	comboBox_->setCurrentText(filterViewId_);
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
	setView(filterViewId_);

	setLayout(vlayout_);
	connect(comboBox_, SIGNAL(currentTextChanged(QString)), this, SLOT(currentIndexChanged(QString)));
	TRACEPOINT;
}

void FilterCallTab::setView(const QString &viewId)
{
	TRACEPOINT;
	if (viewHistory_.count(viewId))
	{
		filterView_ = viewHistory_.at(viewId);
		vlayout_->addWidget(filterView_);
		filterView_->setVisible(true);
		return;
	} 
	if (registeredElements_.count(viewId) > 0)
	{
		auto fct = registeredElements_.at(viewId);
		std::vector<cv::Mat> images;
		images. push_back(filterCall_->original());
		images.push_back(filterCall_->result());
		viewHistory_.emplace(viewId, (fct(images, this).release()));
		filterView_ = viewHistory_.at(viewId);
		vlayout_->addWidget(filterView_);
		return;
	}
	
	vlayout_->addWidget(new QLabel{"Error: View could not be set up."});
	TRACEPOINT;
}

}}//namespaces
