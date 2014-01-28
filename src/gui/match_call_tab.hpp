#ifndef CVVISUAL_MATCH_CALL_TAB_HPP
#define CVVISUAL_MATCH_CALL_TAB_HPP

#include <QString>
#include <QMap>
#include <QPushButton>
#include <QComboBox>
#include <QWidget>

#include "call_tab.hpp"
#include "../view/match_view.hpp"
#include "../controller/view_controller.hpp"
#include "../impl/match_call.hpp"
#include "../util/util.hpp"

namespace cvv {
namespace gui {

/** Match Call Tab
 * @brief Inner part of a tab, contains a MatchView
 * The inner part of a tab or window
 * containing a MatchView.
 * Allows to switch views and to access the help.
 */
class MatchCallTab: public CallTab
{
Q_OBJECT

public:

	/**
	 * @brief Constructor with default view.
	 * Short constructor which initialises the Call Tab with default view from settings.
	 * @param tabName
	 * @param fc the MatchCall containing the information to be visualized.
	 * @param vc the ViewController this CallTab belongs to.
	 */
	MatchCallTab(const QString& tabName, const cvv::impl::MatchCall& mc, const cvv::controller::ViewController& vc);

	/**
	 * @brief Constructor with specific view.
	 * Constructor initialising the Call Tab.
	 * @param tabName
	 * @param fc the MatchCall containing the information to be visualized.
	 * @param vc the ViewController this CallTab belongs to
	 * @param viewId the ID of the view to be shown inside this CallTab.
	 */
	MatchCallTab(const QString& name, const cvv::impl::MatchCall& mc, const cvv::controller::ViewController& vc, const QString& viewId);

	/**
	 * @brief get ID
	 * @return the ID of the CallTab
	 * (ID is equal to the ID of the associated call)
	 * Overrides CallTab's getId.
	 */
	size_t getId() const override;

	/**
	 * @brief Add MatchView to map of all.
	 * Adds a MatchView with a name to the thread local map of all MatchViews.
	 * @param MatchViewId the Id or name of the FilterView.
	 */
	static void addMatchViewToMap(const QString& matchViewId, const cvv::view::MatchView& matchView);

private slots:

	/**
	 * @brief View selection changed.
	 * Called when the index of the view selection changes.
	 */
	void currentIndexChanged() const;

	/**
	 * @brief Help button clicked.
	 * Called when the help button is clicked.
	 */
	void helpButtonClicked() const;

private:

	thread_local static QMap<QString, cvv::view::MatchView> matchViewMap;
	util::Reference<const cvv::impl::MatchCall> matchCall;
	util::Reference<const cvv::controller::ViewController> viewController;
	QString matchViewId;
	cvv::view::MatchView matchView;

	QPushButton helpButton;
	QComboBox matchViewSelection;
};

}} //namespaces

#endif
