#ifndef CVVISUAL_MATCH_VIEW_HPP
#define CVVISUAL_MATCH_VIEW_HPP

#include <QString>
#include <QWidget>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>

#include "../impl/match_call.hpp"

namespace cvv {
namespace view {

/**
 * @brief interface over visualizations of match operations.
 */
class MatchView: public QWidget
{
Q_OBJECT

signals:

	/**
	 * @brief update left Footer.
	 * Signal to update the left side of the footer with newText.
	 * @param newText to update the footer with.
	 */
	void updateLeftFooter(QString newText);

	/**
	 * @brief update right Footer.
	 * Signal to update the right side of the footer with newText.
	 * @param newText to update the footer with.
	 */
	void updateRightFoooter(QString newText);

	void setSelection(std::vector<cv::DMatch>);

public:
	/**
	 * @brief default constructor
	 */
	MatchView():MatchView{0}{}

	/**
	 * @brief default destructor.
	 */
	virtual ~MatchView() = default;

public slots:

	virtual void getSelection(std::vector<cv::DMatch>) {}

protected:
	/**
	 * @brief constructor of QWidget(parent).
	 * @param parent the parent of this view.
	 **/
	MatchView(QWidget *parent):QWidget{parent}{}
};

}} //namespaces

#endif
