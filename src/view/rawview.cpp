#include "rawview.hpp"

#include <functional>

#include <QMap>
#include <QSet>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>
#include <QScrollArea>

#include "../controller/view_controller.hpp"
#include "../qtutil/stfl_query_widget.hpp"
#include "../qtutil/util.hpp"
#include "../dbg/dbg.hpp"

namespace cvv { namespace view {

Rawview::Rawview(util::Reference<controller::ViewController> controller,
				 const std::vector<cv::KeyPoint>& keypoints1,
				 const std::vector<cv::KeyPoint>& keypoints2,
				 const std::vector<cv::DMatch>& matches):
				 controller{controller}
{
	TRACEPOINT;
    queryWidget = new qtutil::STFLQueryWidget();
    table = new gui::RawviewTable(this);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(queryWidget);
    layout->addWidget(table);

    setLayout(layout);
    initEngine();
	connect(queryWidget, SIGNAL(showHelp(QString)), this, SLOT(showHelp(QString)));
    connect(queryWidget, SIGNAL(userInputUpdate(QString)), this, SLOT(updateQuery(QString)));
    connect(queryWidget, SIGNAL(filterSignal(QString)), this, SLOT(filterQuery(QString)));
    connect(queryWidget, SIGNAL(requestSuggestions(QString)), this, SLOT(requestSuggestions(QString)));

	queryEngine.addElements(gui::createRawviewTableRows(keypoints1, keypoints2, matches));
	table->updateRowGroups(queryEngine.query(""));
	TRACEPOINT;
}

void Rawview::initEngine(){
   
}

void Rawview::filterQuery(QString query)
{
	TRACEPOINT;
    table->updateRowGroups(queryEngine.query(query));
    TRACEPOINT;
}

void Rawview::updateQuery(QString query)
{
	TRACEPOINT;
    requestSuggestions(query);
    TRACEPOINT;
}

void Rawview::requestSuggestions(QString query)
{
	TRACEPOINT;
    queryWidget->showSuggestions(queryEngine.getSuggestions(query));
    TRACEPOINT;
}

void Rawview::showHelp(QString topic)
{
	TRACEPOINT;
	controller->openHelpBrowser(topic);
	TRACEPOINT;
}

}}
