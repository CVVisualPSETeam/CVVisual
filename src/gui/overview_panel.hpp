#ifndef CVVISUAL_OVERVIEWPANEL_HPP
#define	CVVISUAL_OVERVIEWPANEL_HPP

#include <QWidget>
#include <QString>
#include <QSlider>
#include <QLabel>
#include <QPushButton>

#include "../stfl/stfl_engine.hpp"
#include "../impl/call.hpp"
#include "overview_table.hpp"
#include "overview_table_collumn.hpp"
#include "../util/util.hpp"
#include "../controller/view_controller.hpp"



namespace cvv {

namespace controller {
    class ViewController;
}

namespace qtutil {
    class STFLQueryWidget;
}

namespace gui {

class OverviewTable;
class OverviewTableCollumn;

class OverviewPanel : public QWidget
{

    Q_OBJECT

public:

    OverviewPanel(controller::ViewController *controller);

    void addElement(const util::Reference<const impl::Call> newCall);

    /**
     * @brief Changes the "Resume program execution" button label to "Exit Application"
     */
    void showExitApplicationButton();

    /**
     * @brief Removes and deletes the element with the given id.
     * @param id element id
     */
    void deleteElement(size_t id);

private slots:

    void filterQuery(QString query);

    void updateQuery(QString query);

    void requestSuggestions(QString query);

    void imgSizeSliderAction();

    void toggleImages();

private:
    stfl::STFLEngine<OverviewTableCollumn> queryEngine;
    qtutil::STFLQueryWidget *queryWidget;
    OverviewTable *table;
    controller::ViewController *controller;
    QLabel *imgSizeSliderLabel;
    QSlider *imgSizeSlider;
    QPushButton *showImgsButton;

    void initEngine();

};

}}

#endif
