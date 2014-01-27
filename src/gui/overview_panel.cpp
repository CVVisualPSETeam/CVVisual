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
    queryEngine.setFilterCSFunc("line", [](const QStringList& args, const OverviewTableCollumn &elem)
    {
        return args.contains(elem.line());
    });
    queryEngine.setFilterCSPoolFunc("line", [](const OverviewTableCollumn &elem){
        return qtutil::createStringSet(elem.line());
    });

    //id filter
    queryEngine.setFilterCSFunc("id", [](const QStringList& args, const OverviewTableCollumn &elem)
    {
        return args.contains(elem.id());
    });
    queryEngine.setFilterCSPoolFunc("file", [](const OverviewTableCollumn &elem){
        return qtutil::createStringSet(elem.id());
    });

    //type filter
    queryEngine.setFilterCSFunc("type", [](const QStringList& args, const OverviewTableCollumn &elem)
    {
        return args.contains(elem.type());
    });
    queryEngine.setFilterCSPoolFunc("type", [](const OverviewTableCollumn &elem){
        return qtutil::createStringSet(elem.type());
    });

    //"number of images" filter
    queryEngine.setFilterCSFunc("img_count", [](const QStringList& args, const OverviewTableCollumn &elem)
    {
        return args.contains(QString::number(elem.call()->matrixCount()));
    });
    queryEngine.setFilterCSPoolFunc("img_count", [](const OverviewTableCollumn &elem){
        return qtutil::createStringSet(QString::number(elem.call()->matrixCount()));
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
