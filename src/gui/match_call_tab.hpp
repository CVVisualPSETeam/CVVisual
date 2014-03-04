#ifndef CVVISUAL_MATCH_CALL_TAB_HPP
#define CVVISUAL_MATCH_CALL_TAB_HPP

#include <memory>

#include <QString>
#include <QWidget>

#include "multiview_call_tab.hpp"
#include "../view/match_view.hpp"
#include "../impl/match_call.hpp"
#include "../util/util.hpp"

namespace cvv {
namespace gui {

/** Match Call Tab.
 * @brief Inner part of a tab, contains a MatchView.
 * The inner part of a tab or window
 * containing a MatchView.
 * Allows to switch views and to access the help.
 */
class MatchCallTab:
		public MultiViewCallTab<cvv::view::MatchView, cvv::impl::MatchCall>
{
	Q_OBJECT

public:

	/**
	 * @brief Short constructor named after Call and using the default view.
	 * Initializes the MatchCallTab with the default view and names it after the associated MatchCall.
	 * @param matchCall the MatchCall containing the information to be visualized.
	 */
	MatchCallTab(const cvv::impl::MatchCall& matchCall):
		MultiViewCallTab<cvv::view::MatchView, cvv::impl::MatchCall>{matchCall}
	{
		TRACEPOINT;
		default_scope_ = QString{"default_views"};
		default_key_ = QString{"default_match_view"};
		standard_default_ = QString{"LineMatchView"};
		createGui();
		TRACEPOINT;
	}

	~MatchCallTab(){TRACEPOINT;}

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
		return registerView<View>(name);
	}

};

}}//namespaces

#endif
