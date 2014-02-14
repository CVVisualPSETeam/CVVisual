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

class MatchView: public QWidget
{
Q_OBJECT

signals:

	/**
	 * @brief update left footer
	 * Signal to update the left side of the footer with newText.
	 * @param newText
	 */
	void updateLeftFooter(QString newText);

	/**
	 * @brief update right footer
	 * Signal to update the right side of the footer with newText.
	 * @param newText
	 */
	void updateRightFoooter(QString newText);
	
	/**
	 * @brief Show help to the given topic
	 * @param topic given topic
	 */
	void showHelp(QString topic);

	/**
	 * @todo Hä?!?!?!
	 * @brief output internal selection
	 * @param matchCalls
	 */
	void outputInternalSelection(const std::vector<cvv::impl::MatchCall>& matchCalls);

public:

public:
	/**
	 * @brief default constructor
	 **/
	MatchView():MatchView{0}{};

	virtual ~MatchView() = default;

protected:
	/**
	 * @brief constructor of QWidget(parent)
	 * @param parent the parent of this view
	 **/
	MatchView(QWidget *parent):QWidget{parent}{};
};

}} //namespaces

#endif
