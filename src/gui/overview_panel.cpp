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

namespace cvv { namespace gui {

OverviewPanel::OverviewPanel(util::Reference<controller::ViewController> controller):
    controller{controller}
{
    controller->setDefaultSetting("overview", "imgzoom", "20");
    queryWidget = new qtutil::STFLQueryWidget(controller);
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

void OverviewPanel::deleteElement(size_t id)
{
    queryEngine.removeElements([id](OverviewTableCollumn elem) {return elem.id() == id;});
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

void OverviewPanel::imgSizeSliderAction()
{
    controller->setSetting("overview", "imgzoom", QString::number(imgSizeSlider->value()));
    table->updateUI();
}

void OverviewPanel::toggleImages()
{
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
}

}}
