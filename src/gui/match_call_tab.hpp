#ifndef CVVISUAL_MATCH_CALL_TAB_HPP
#define CVVISUAL_MATCH_CALL_TAB_HPP

#include <memory>

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
#include "../qtutil/registerhelper.hpp"

namespace cvv {
namespace gui {

/** Match Call Tab
 * @brief Inner part of a tab, contains a MatchView.
 * The inner part of a tab or window
 * containing a MatchView.
 * Allows to switch views and to access the help.
 */
class MatchCallTab:
		public CallTab, public cvv::qtutil::RegisterHelper<cvv::view::MatchView, std::vector<cv::Mat>, QWidget*>
{
Q_OBJECT

public:

	/**
	 * @brief Short constructor using name from Call and default view.
	 * Initializes the MatchCallTab with the default view and names it after the associated MatchCall.
	 * @param fc the MatchCall containing the information to be visualized.
	 * @param vc the ViewController this CallTab belongs to.
	 */
	MatchCallTab(const cvv::impl::MatchCall& fc, const cvv::controller::ViewController& vc);

	/**
	 * @brief Constructor using default view.
	 * Short constructor which initialises the Call Tab with default view from settings.
	 * @param tabName
	 * @param fc the MatchCall containing the information to be visualized.
	 * @param vc the ViewController this CallTab belongs to.
	 * @attention might be deleted
	 */
	MatchCallTab(const QString& tabName, const cvv::impl::MatchCall& fc, const cvv::controller::ViewController& vc);

	/**
	 * @brief Constructor with specific view.
	 * Constructor initialising the Call Tab.
	 * @param tabName
	 * @param fc the MatchCall containing the information to be visualized.
	 * @param vc the ViewController this CallTab belongs to
	 * @param viewId the ID of the view to be shown inside this CallTab.
	 * @attention might be deleted
	 */
	MatchCallTab(const QString& tabName, const cvv::impl::MatchCall& fc, const cvv::controller::ViewController& vc, const QString& viewId);

	/**
	 * @brief get ID
	 * @return the ID of the CallTab
	 * (ID is equal to the ID of the associated call)
	 * Overrides CallTab's getId.
	 */
	size_t getId() const override;

	/**
	 * @brief adds MatchView to map of all
	 * Adds a MatchView with a name to the thread local map of all MatchViews.
	 * @param matchViewId the Id or name of the MatchView.
	 */
	static void addMatchViewToMap(const QString& matchViewId, std::function<std::unique_ptr<cvv::view::MatchView>(std::vector<cv::Mat>, QWidget*)>);

private slots:

	/**
	 * @brief View selection change
	 * Called when the index of the view selection changes.
	 * @param text of the current selection in the view selection.
	 */
	void currentIndexChanged(const QString& text);

	/**
	 * @brief Help Button clicked.
	 * Called when the help button is clicked.
	 */
	void helpButtonClicked() const;

private:

	/**
	 * @brief Sets up the visible parts.
	 * Called by the constructors.
	 */
	void createGui();

	/**
	 * @brief sets up View referred to by viewId
	 * @param viewId ID of the view to be set.
	 */
	void setView(const QString& viewId);

	util::Reference<const cvv::impl::MatchCall> matchCall_;
	util::Reference<const cvv::controller::ViewController> viewController_;
	QString matchViewId_;
	cvv::view::MatchView* matchView_;

	QPushButton* helpButton_;
	QHBoxLayout* hlayout_;
	QVBoxLayout* vlayout_;

};

}}//namespaces

#endif
