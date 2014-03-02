#ifndef CVVISUAL_MATCH_CALL_TAB_HPP
#define CVVISUAL_MATCH_CALL_TAB_HPP

#include <memory>

#include <QString>
#include <QMap>
#include <QPushButton>
#include <QComboBox>
#include <QWidget>

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"

#include "call_tab.hpp"
#include "../view/match_view.hpp"
#include "../impl/match_call.hpp"
#include "../util/util.hpp"
#include "../qtutil/registerhelper.hpp"

namespace cvv {
namespace gui {

/** Match Call Tab.
 * @brief Inner part of a tab, contains a MatchView.
 * The inner part of a tab or window
 * containing a MatchView.
 * Allows to switch views and to access the help.
 */
class MatchCallTab:
		public CallTab, public cvv::qtutil::RegisterHelper<
			cvv::view::MatchView, const cvv::impl::MatchCall&, QWidget*>
{
	Q_OBJECT

public:
	
	using MatchViewBuilder = std::function<std::unique_ptr<cvv::view::MatchView>(
			const cvv::impl::MatchCall&, QWidget*)>;

	/**
	 * @brief Short constructor named after Call and using the default view.
	 * Initializes the MatchCallTab with the default view and names it after the associated MatchCall.
	 * @param matchCall the MatchCall containing the information to be visualized.
	 */
	MatchCallTab(const cvv::impl::MatchCall& matchCall);

	/**
	 * @brief Constructor using default view.
	 * Short constructor which initialises the Call Tab with default view from settings.
	 * @param tabName.
	 * @param matchCall the MatchCall containing the information to be visualized.
	 */
	MatchCallTab(const QString& tabName, const cvv::impl::MatchCall& matchCall);

	/**
	 * @brief Constructor with specific view.
	 * Constructor initialising the Call Tab.
	 * @param tabName.
	 * @param matchCall the MatchCall containing the information to be visualized.
	 * @param viewId the ID of the view to be shown inside this CallTab.
	 */
	MatchCallTab(const QString& tabName, const cvv::impl::MatchCall& matchCall, const QString& viewId);

	/**
	 * @brief get ID.
	 * @return the ID of the CallTab.
	 * (ID is equal to the ID of the associated call).
	 * Overrides CallTab's getId.
	 */
	size_t getId() const override;

	/**
	 * @brief Register the template class to the map of MatchViews.
	 * View needs to offer a constructor of the form View(const cvv::impl::MatchCall&, QWidget*).
	 * @param name to register the class under.
	 * @tparam View - Class to register.
	 * @return true when the view was registered and false when the name was already taken.
	 */
	template<class View>
	static bool registerMatchView(const QString& name)
	{
		TRACEPOINT;
		return gui::MatchCallTab::registerElement(name,
			[](const cvv::impl::MatchCall& call, QWidget* parent){
				TRACEPOINT;
				return cvv::util::make_unique<View>(call,parent);
			}
		);
	}

	/**
	 * @brief adds MatchView to map of all.
	 * Adds a MatchView with a name to the thread local map of all MatchViews.
	 * @param matchViewId the ID or name of the MatchView.
	 * @param mView: function returning a unique_ptr to a MatchView.
	 * @attention: Use of registerMatchView recommended.
	 */
	static void addMatchViewToMap(const QString& matchViewId, MatchViewBuilder mView);

private slots:

	/**
	 * @brief View selection change.
	 * Called when the index of the view selection changes.
	 */
	void currentIndexChanged();

	/**
	 * @brief Help Button clicked.
	 * Called when the help button is clicked.
	 */
	void helpButtonClicked() const;

	/**
	 * @brief setAsDefaultButton clicked.
	 * Called when the setAsDefaultButton,which sets the current view as default, is clicked.
	 */
	void setAsDefaultButtonClicked();

private:

	/**
	 * @brief Sets up the visible parts.
	 * Called by the constructors.
	 */
	void createGui();

	/**
	 * @brief sets up View referred to by viewId.
	 * @param viewId ID of the view to be set.
	 */
	void setView();

	util::Reference<const cvv::impl::MatchCall> matchCall_;
	QString matchViewId_;
	cvv::view::MatchView* matchView_;
	std::map<QString, cvv::view::MatchView*> viewHistory_;

	QPushButton* setAsDefaultButton_;
	QPushButton* helpButton_;
	QHBoxLayout* hlayout_;
	QVBoxLayout* vlayout_;
	QWidget* upperBar_;

};

}}//namespaces

#endif
