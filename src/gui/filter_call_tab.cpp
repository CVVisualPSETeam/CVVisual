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

#include "../dbg/dbg.hpp"

namespace cvv {
namespace gui {

FilterCallTab::FilterCallTab(const cvv::impl::FilterCall& fc, const cvv::controller::ViewController& vc):
	filterCall_{fc}, viewController_{vc}
{
    TRACEPOINT;
	setName(filterCall_->description());
	const QString scope{"default_views"};
	const QString key{"default_filter_view"};
	QString setting;
	try
	{
		setting = vc.getSetting(scope, key);
	} catch (std::invalid_argument)
	{
		setting = "DefaultFilterView";
	}
	filterViewId_ = setting;

	createGui();
    TRACEPOINT;
}

FilterCallTab::FilterCallTab(const QString& tabName, const cvv::impl::FilterCall& fc, const cvv::controller::ViewController& vc):
	filterCall_{fc}, viewController_{vc}
{
    TRACEPOINT;
	setName(tabName);
	const QString scope{"default_views"};
	const QString key{"default_filter_view"};
	QString setting;
	try
	{
		setting = vc.getSetting(scope, key);
	} catch (std::invalid_argument)
	{
		setting = "DefaultFilterView";
	}
	filterViewId_ = setting;

	createGui();
    TRACEPOINT;
}

FilterCallTab::FilterCallTab(const QString& tabName, const cvv::impl::FilterCall& fc, const cvv::controller::ViewController& vc, const QString& viewId):
	filterCall_{fc}, viewController_{vc}
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
	delete filterView_;
	setView(filterViewId_);
    TRACEPOINT;
}

void FilterCallTab::helpButtonClicked() const
{
    TRACEPOINT;
	viewController_->openHelpBrowser(filterViewId_);
    TRACEPOINT;
}

size_t FilterCallTab::getId() const
{
    TRACEPOINT;
	return filterCall_->getId();
    TRACEPOINT;
}

void FilterCallTab::addFilterViewToMap(const QString& filterViewId,
				       std::function<std::unique_ptr<cvv::view::FilterView>(std::vector<cv::Mat>, QWidget*)> fView)
{
    TRACEPOINT;
	cvv::qtutil::RegisterHelper<cvv::view::FilterView, std::vector<cv::Mat>, QWidget*>::registerElement(filterViewId, fView);
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
	try
	{
		auto fct = registeredElements_.at(viewId);
		std::vector<cv::Mat> images;
		images. push_back(filterCall_->original());
		images.push_back(filterCall_->result());
		filterView_ = (fct(images, this)).release();
		vlayout_->addWidget(filterView_);
    } catch (std::out_of_range&)
	{
        vlayout_->addWidget(new QLabel{"Error: View could not be set up."});
        throw;
	}
    TRACEPOINT;
}

}}//namespaces
