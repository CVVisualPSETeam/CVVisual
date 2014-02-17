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
#include "overview_table_row.hpp"
#include "../util/util.hpp"
#include "../dbg/dbg.hpp"
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
class OverviewTableRow;

/**
 * @brief The overview showing a filterable table displaying the different calls.
 */
class OverviewPanel : public QWidget
{

    Q_OBJECT

public:

	/**
	 * @brief Contructs an OverviewPanel.
	 * @param controller ViewController that inherits this overview
	 */
    OverviewPanel(util::Reference<controller::ViewController> controller);

	~OverviewPanel() { TRACEPOINT; }
	
	/**
	 * @brief Adds the given calll to the shown overview table.
	 * @param newCall given call
	 */
    void addElement(const util::Reference<const impl::Call> newCall);

    /**
     * @brief Changes the "Resume program execution" button label to "Exit Application."
     */
    void showExitApplicationButton();

    /**
     * @brief Removes and deletes the element with the given id.
     * @param id given element id
     */
    void removeElement(size_t id);

private slots:

    void filterQuery(QString query);

    void updateQuery(QString query);

    void requestSuggestions(QString query);

    void imgSizeSliderAction();

    void toggleImages();
	
	void showHelp(QString topic);

private:
    stfl::STFLEngine<OverviewTableRow> queryEngine;
    qtutil::STFLQueryWidget *queryWidget;
    OverviewTable *table;
    util::Reference<controller::ViewController> controller;
    QLabel *imgSizeSliderLabel;
    QSlider *imgSizeSlider;
    QPushButton *showImgsButton;

    void initEngine();

};

}}

#endif
