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

Rawview::Rawview(
				 const std::vector<cv::KeyPoint>& keypoints1,
				 const std::vector<cv::KeyPoint>& keypoints2,
				 const std::vector<cv::DMatch>& matches)
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
	queryEngine.addFloatCmdFunc("match_distance", [](const gui::RawviewTableRow& row){
		return row.matchDistance();
	});
	queryEngine.addIntegerCmdFunc("img_idx", [](const gui::RawviewTableRow& row){
		return row.matchImgIdx();
	});
	queryEngine.addIntegerCmdFunc("query_idx", [](const gui::RawviewTableRow& row){
		return row.matchQueryIdx();
	});
	queryEngine.addIntegerCmdFunc("train_idx", [](const gui::RawviewTableRow& row){
		return row.matchTrainIdx();
	});
	queryEngine.addFloatCmdFunc ("x_1", [](const gui::RawviewTableRow& row){
		return row.keyPoint1XCoord();
	});
	queryEngine.addFloatCmdFunc ("y_1", [](const gui::RawviewTableRow& row){
		return row.keyPoint1YCoord();
	});
	queryEngine.addFloatCmdFunc ("size_1", [](const gui::RawviewTableRow& row){
		return row.keyPoint1Size();
	});
	queryEngine.addFloatCmdFunc ("angle_1", [](const gui::RawviewTableRow& row){
		return row.keyPoint1Angle();
	});
	queryEngine.addFloatCmdFunc ("response_1", [](const gui::RawviewTableRow& row){
		return row.keyPoint1Response();
	});
	queryEngine.addIntegerCmdFunc ("octave_1", [](const gui::RawviewTableRow& row){
		return row.keyPoint1Octave();
	});
	queryEngine.addFloatCmdFunc ("class_id_1", [](const gui::RawviewTableRow& row){
		return row.keyPoint1ClassId();
	});
	queryEngine.addFloatCmdFunc ("x_2", [](const gui::RawviewTableRow& row){
		return row.keyPoint2XCoord();
	});
	queryEngine.addFloatCmdFunc ("y_2", [](const gui::RawviewTableRow& row){
		return row.keyPoint2YCoord();
	});
	queryEngine.addFloatCmdFunc ("size_2", [](const gui::RawviewTableRow& row){
		return row.keyPoint2Size();
	});
	queryEngine.addFloatCmdFunc ("angle_2", [](const gui::RawviewTableRow& row){
		return row.keyPoint2Angle();
	});
	queryEngine.addFloatCmdFunc ("response_2", [](const gui::RawviewTableRow& row){
		return row.keyPoint2Response();
	});
	queryEngine.addIntegerCmdFunc ("octave_2", [](const gui::RawviewTableRow& row){
		return row.keyPoint2Octave();
	});
	queryEngine.addFloatCmdFunc ("class_id_2", [](const gui::RawviewTableRow& row){
		return row.keyPoint2ClassId();
	});
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
	qtutil::openHelpBrowser(topic);
	TRACEPOINT;
}

}}
