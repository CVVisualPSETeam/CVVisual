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

#include "../view/defaultfilterview.hpp"

namespace cvv {
namespace gui {

FilterCallTab::FilterCallTab(const cvv::impl::FilterCall& fc, const cvv::controller::ViewController& vc):
	filterCall{fc}, viewController{vc}
{
	setName(filterCall->description());
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

	//QString setting = "PLACEHOLDER"; (void) scope; (void) key;
	filterViewId = setting;
	// TODO set filterView

	createGui();
}

FilterCallTab::FilterCallTab(const QString& tabName, const cvv::impl::FilterCall& fc, const cvv::controller::ViewController& vc):
	filterCall{fc}, viewController{vc}
{
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

	//QString setting = "PLACEHOLDER"; (void) scope; (void) key;
	filterViewId = setting;
	// TODO set filterView

	createGui();
}

FilterCallTab::FilterCallTab(const QString& tabName, const cvv::impl::FilterCall& fc, const cvv::controller::ViewController& vc, const QString& viewId):
	filterCall{fc}, viewController{vc}
{
	setName(tabName);
	filterViewId = viewId;
	// TODO set filterView

	createGui();
}

void FilterCallTab::currentIndexChanged(const QString& text)
{
	delete filterView;
	auto fct = registeredElements_.at(text);
	std::vector<cv::Mat> images;
	images. push_back(filterCall->original());
	images.push_back(filterCall->result());
	filterView = (fct(images, this)).release();
	vlayout->addWidget(filterView);
}

void FilterCallTab::helpButtonClicked() const
{
	viewController->openHelpBrowser(filterViewId);
/* Only for testing: */	helpButton->setText("Connect successful");
}

size_t FilterCallTab::getId() const
{
	return filterCall->getId();
}

void FilterCallTab::addFilterViewToMap(const QString& filterViewId,
				       std::function<std::unique_ptr<cvv::view::FilterView>(std::vector<cv::Mat>, QWidget*)> fView)
{
	cvv::qtutil::RegisterHelper<cvv::view::FilterView, std::vector<cv::Mat>, QWidget*>::registerElement(filterViewId, fView);
}

void FilterCallTab::createGui()
{
	hlayout = new QHBoxLayout;
	hlayout->setAlignment(Qt::AlignTop);
	QLabel* selectionLabel = new QLabel{"View:"};
	hlayout->addWidget(selectionLabel);
	//filterViewSelection = new QComboBox{};
	hlayout->addWidget(comboBox_);
	//hlayout->addWidget(filterViewMap);
	helpButton = new QPushButton{"Help"};
	hlayout->addWidget(helpButton);
	connect(helpButton, SIGNAL(clicked()), this, SLOT(helpButtonClicked()));

	QWidget* upperBar = new QWidget;
	upperBar->setLayout(hlayout);

	vlayout = new QVBoxLayout;

	connect(comboBox_, SIGNAL(currentTextChanged(QString)), this, SLOT(currentIndexChanged(QString)));
	//QLabel* viewDummy = new QLabel{"There will be a view here."};

/* For testing: */
	std::vector<cv::Mat> images;
	images. push_back(filterCall->original());
	images.push_back(filterCall->result());
	filterView = new cvv::view::DefaultFilterView{images, this};

	vlayout->addWidget(upperBar);
	//vlayout->addWidget(viewDummy);
	vlayout->addWidget(filterView);

	setLayout(vlayout);
}

}}//namespaces
