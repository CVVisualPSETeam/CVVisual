#include "overview_panel.hpp"

#include <functional>

#include <QMap>
#include <QSet>
#include <QString>
#include <QVBoxLayout>

#include "../qtutil/stfl_query_widget.hpp"
#include "../qtutil/util.hpp"

namespace cvv { namespace gui {

OverviewPanel::OverviewPanel(controller::ViewController *controller):
    controller{controller}
{
    queryWidget = new qtutil::STFLQueryWidget();
    table = new OverviewTable(util::makeRef(*controller), this);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(queryWidget);
    layout->addWidget(table);
    setLayout(layout);
    initEngine();
	connect(queryWidget, SIGNAL(userInputUpdate(QString)), this, SLOT(updateQuery(QString)));
    connect(queryWidget, SIGNAL(filterSignal(QString)), this, SLOT(filterQuery(QString)));
    connect(queryWidget, SIGNAL(requestSuggestions(QString)), this, SLOT(requestSuggestions(QString)));
}

void OverviewPanel::initEngine(){
    //raw and description filter
    auto rawFilter = [](const OverviewTableCollumn& elem)
    {
        return elem.description();
    };
    queryEngine.addStringCmdFunc("raw", rawFilter, false);
    queryEngine.addStringCmdFunc("description", rawFilter, false);

    //file filter
    queryEngine.addStringCmdFunc("file", [](const OverviewTableCollumn &elem)
    {
        return elem.file();
    });

    //function filter
    queryEngine.addStringCmdFunc("function", [](const OverviewTableCollumn &elem)
    {
        return elem.function();
    });

    //line filter
    queryEngine.addIntegerCmdFunc("line", [](const OverviewTableCollumn &elem)
    {
        return elem.line();
    });

    //id filter
    queryEngine.addIntegerCmdFunc("id", [](const OverviewTableCollumn &elem)
    {
        return elem.id();
    });

    //type filter
    queryEngine.addStringCmdFunc("type", [](const OverviewTableCollumn &elem)
    {
        return elem.type();
    });

    //"number of images" filter
    queryEngine.addIntegerCmdFunc("image_count", [](const OverviewTableCollumn &elem)
    {
        return elem.call()->matrixCount();
    });
}

void OverviewPanel::addElement(const util::Reference<const impl::Call> newCall)
{
	OverviewTableCollumn col(newCall);
    queryEngine.addNewElement(col);
    table->updateCollumnGroups(queryEngine.reexecuteLastQuery());
}

void OverviewPanel::filterQuery(QString query)
{
    table->updateCollumnGroups(queryEngine.query(query));
}

void OverviewPanel::updateQuery(QString query)
{
    filterQuery(query);
}

void OverviewPanel::requestSuggestions(QString query)
{
    queryWidget->showSuggestions(queryEngine.getSuggestions(query));
}

}}
