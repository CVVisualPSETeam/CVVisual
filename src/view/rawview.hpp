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

/**
 * @brief A view displaying the raw data of a match call (aka the matches).
 */
class Rawview : public MatchView
{

    Q_OBJECT

public:

	/**
	 * @brief Constructor of this class.
	 * @param keypoints1 left keypoints
	 * @param keypoints2 right keypoints
	 * @param matches matches between the left and the right keypoints.
	 */
    Rawview(const std::vector<cv::KeyPoint>& keypoints1,
			const std::vector<cv::KeyPoint>& keypoints2,
			const std::vector<cv::DMatch>& matches);
	/**
	 * @todo remove.
	 */
	Rawview(const std::vector<cv::KeyPoint>& keypoints1,
			const std::vector<cv::KeyPoint>& keypoints2,
			const std::vector<std::vector<cv::DMatch>>& matches);

signals:
	/**
	 * @brief 
	 */
	void updateLeftFooter(QString newText);

	void updateRightFooter(QString newText);
	
	void showHelp(QString topic);
	
private slots:

    void filterQuery(QString query);

    void updateQuery(QString query);

    void requestSuggestions(QString query);

private:
    stfl::STFLEngine<gui::RawviewTableRow> queryEngine;
    qtutil::STFLQueryWidget *queryWidget;
    gui::RawviewTable *table;

    void initEngine();

};

}}

#endif
