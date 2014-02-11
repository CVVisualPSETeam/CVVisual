#ifndef CVVISUAL_RAWVIEW_HPP
#define	CVVISUAL_RAWVIEW_HPP

#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>

#include <QWidget>
#include <QString>
#include <QSlider>
#include <QLabel>
#include <QPushButton>

#include "../stfl/stfl_engine.hpp"
#include "../impl/call.hpp"
#include "../gui/rawview_table.hpp"
#include "../gui/rawview_table_row.hpp"
#include "../util/util.hpp"
#include "../controller/view_controller.hpp"
#include "match_view.hpp"

namespace cvv {

namespace controller {
    class ViewController;
}

namespace qtutil {
    class STFLQueryWidget;
}

namespace gui {
class RawviewTable;
class RawviewTableRow;
}

namespace view {

class Rawview : public MatchView
{

    Q_OBJECT

public:

    Rawview(util::Reference<controller::ViewController> controller,
			const std::vector<cv::KeyPoint>& keypoints1,
			const std::vector<cv::KeyPoint>& keypoints2,
			const std::vector<std::vector<cv::DMatch>>& matches);

signals:
	void updateLeftFooter(QString newText);

	void updateRightFooter(QString newText);
	
private slots:

    void filterQuery(QString query);

    void updateQuery(QString query);

    void requestSuggestions(QString query);

private:
    stfl::STFLEngine<gui::RawviewTableRow> queryEngine;
    qtutil::STFLQueryWidget *queryWidget;
    gui::RawviewTable *table;
    util::Reference<controller::ViewController> controller;

    void initEngine();

};

}}

#endif
