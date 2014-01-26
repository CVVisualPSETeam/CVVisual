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
}

void OverviewPanel::initEngine(){
    //raw and description filter
    auto rawFilter = [](const QString& query, const OverviewTableCollumn& elem)
    {
        return elem.description() == query;
    };
    auto rawFilterPool = [](const OverviewTableCollumn& elem)
    {
        return elem.description();
    };
    queryEngine.setFilterFunc("raw", rawFilter);
    queryEngine.setFilterPoolFunc("raw", rawFilterPool);
    queryEngine.setFilterFunc("description", rawFilter);
    queryEngine.setFilterPoolFunc("description", rawFilterPool);

    //file filter
    queryEngine.setFilterCSFunc("file", [](const QStringList& args, const OverviewTableCollumn &elem)
    {
        return args.contains(elem.file());
    });
    queryEngine.setFilterCSPoolFunc("file", [](const OverviewTableCollumn &elem){
        return qtutil::createStringSet(elem.file());
    });

    //function filter
    queryEngine.setFilterCSFunc("function", [](const QStringList& args, const OverviewTableCollumn &elem)
    {
        return args.contains(elem.function());
    });
    queryEngine.setFilterCSPoolFunc("function", [](const OverviewTableCollumn &elem){
        return qtutil::createStringSet(elem.function());
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
