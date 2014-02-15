#include "overview_panel.hpp"

#include <functional>
#include <math.h>

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

namespace cvv { namespace gui {

OverviewPanel::OverviewPanel(util::Reference<controller::ViewController> controller):
    controller{controller}
{
	TRACEPOINT;
    controller->setDefaultSetting("overview", "imgzoom", "20");
    queryWidget = new qtutil::STFLQueryWidget();
    table = new OverviewTable(util::makeRef(*controller), this);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(queryWidget);
    layout->addWidget(table);

    auto bottomArea = new QWidget{this};
    auto bottomLayout = new QHBoxLayout;
    imgSizeSliderLabel = new QLabel{"Zoom", bottomArea};
    imgSizeSliderLabel->setMaximumWidth(50);
    imgSizeSliderLabel->setAlignment(Qt::AlignRight);
    bottomLayout->addWidget(imgSizeSliderLabel);
    imgSizeSlider = new QSlider{Qt::Horizontal, bottomArea};
    imgSizeSlider->setMinimumWidth(50);
    imgSizeSlider->setMaximumWidth(200);
    imgSizeSlider->setMinimum(0);
    imgSizeSlider->setMaximum(100);
    imgSizeSlider->setSliderPosition(controller->getSetting("overview", "imgzoom").toInt());
    connect(imgSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(imgSizeSliderAction()));
    bottomLayout->addWidget(imgSizeSlider);
    showImgsButton = new QPushButton{};
    showImgsButton->setText("Hide images");
    //connect(showImgsButton, SIGNAL(released()), this, SLOT(toggleImages()));
    //bottomLayout->addWidget(showImgsButton);
    bottomArea->setLayout(bottomLayout);
    layout->addWidget(bottomArea);

    setLayout(layout);
    initEngine();
	connect(queryWidget, SIGNAL(showHelp(QString)), this, SLOT(showHelp(QString)));
    //connect(queryWidget, SIGNAL(userInputUpdate(QString)), this, SLOT(updateQuery(QString)));
    connect(queryWidget, SIGNAL(filterSignal(QString)), this, SLOT(filterQuery(QString)));
    connect(queryWidget, SIGNAL(requestSuggestions(QString)), this, SLOT(requestSuggestions(QString)));
    TRACEPOINT;
}

void OverviewPanel::initEngine(){
	TRACEPOINT;
    //raw and description filter
    auto rawFilter = [](const OverviewTableRow& elem)
    {
        return elem.description();
    };
    queryEngine.addStringCmdFunc("raw", rawFilter, false);
    queryEngine.addStringCmdFunc("description", rawFilter, false);

    //file filter
    queryEngine.addStringCmdFunc("file", [](const OverviewTableRow &elem)
    {
        return elem.file();
    });

    //function filter
    queryEngine.addStringCmdFunc("function", [](const OverviewTableRow &elem)
    {
        return elem.function();
    });

    //line filter
    queryEngine.addIntegerCmdFunc("line", [](const OverviewTableRow &elem)
    {
        return elem.line();
    });

    //id filter
    queryEngine.addIntegerCmdFunc("id", [](const OverviewTableRow &elem)
    {
        return elem.id();
    });

    //type filter
    queryEngine.addStringCmdFunc("type", [](const OverviewTableRow &elem)
    {
        return elem.type();
    });

    //"number of images" filter
    queryEngine.addIntegerCmdFunc("image_count", [](const OverviewTableRow &elem)
    {
        return elem.call()->matrixCount();
    });
    TRACEPOINT;
}

void OverviewPanel::addElement(const util::Reference<const impl::Call> newCall)
{
	TRACEPOINT;
    OverviewTableRow row(newCall);
    queryEngine.addNewElement(row);
    table->updateRowGroups(queryEngine.reexecuteLastQuery());
    TRACEPOINT;
}

void OverviewPanel::deleteElement(size_t id)
{
	TRACEPOINT;
    queryEngine.removeElements([id](OverviewTableRow elem) {return elem.id() == id;});
    TRACEPOINT;
    table->updateRowGroups(queryEngine.reexecuteLastQuery());
    TRACEPOINT;
}

void OverviewPanel::filterQuery(QString query)
{
	TRACEPOINT;
    table->updateRowGroups(queryEngine.query(query));
    TRACEPOINT;
}

void OverviewPanel::updateQuery(QString query)
{
	TRACEPOINT;
    filterQuery(query);
    TRACEPOINT;
}

void OverviewPanel::requestSuggestions(QString query)
{
	TRACEPOINT;
    queryWidget->showSuggestions(queryEngine.getSuggestions(query));
    TRACEPOINT;
}

void OverviewPanel::imgSizeSliderAction()
{
	TRACEPOINT;
    controller->setSetting("overview", "imgzoom", QString::number(imgSizeSlider->value()));
    table->updateUI();
    TRACEPOINT;
}

void OverviewPanel::toggleImages()
{
	TRACEPOINT;
    if (showImgsButton->text() == "Show images")
    {
        table->showImages();
        showImgsButton->setText("Hide images");
    }
    else
    {
        table->hideImages();
        showImgsButton->setText("Show images");
    }
    TRACEPOINT;
}

void OverviewPanel::showHelp(QString topic)
{
	TRACEPOINT;
	controller->openHelpBrowser(topic);
	TRACEPOINT;
}

}}
