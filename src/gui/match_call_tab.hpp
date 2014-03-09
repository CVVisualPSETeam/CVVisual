#ifndef CVVISUAL_MATCH_CALL_TAB_HPP
#define CVVISUAL_MATCH_CALL_TAB_HPP

#include <memory>

#include <QString>
#include <QWidget>

#include "multiview_call_tab.hpp"
#include "../view/match_view.hpp"
#include "../impl/match_call.hpp"
#include "../util/util.hpp"

namespace cvv
{
namespace gui
{

/** Match Call Tab.
 * @brief Inner part of a tab, contains a MatchView.
 * The inner part of a tab or window
 * containing a MatchView.
 * Allows to switch views and to access the help.
 */
class MatchCallTab
    : public MultiViewCallTab<cvv::view::MatchView, cvv::impl::MatchCall>
{
	Q_OBJECT

      public:
	/**
	 * @brief Short constructor named after Call and using the default view.
	 * Initializes the MatchCallTab with the default view and names it after
	 * the associated MatchCall.
	 * @param matchCall - the MatchCall containing the information to be
	 * visualized.
	 */
	MatchCallTab(const cvv::impl::MatchCall &matchCall)
	    : MultiViewCallTab<cvv::view::MatchView, cvv::impl::MatchCall>{
		      matchCall, QString{ "default_match_view" }, QString{ "LineMatchView" }
	      }
	{
		TRACEPOINT;
		oldView_ = view_;
		connect(&this->viewSet, SIGNAL(signal()), this, SLOT(viewChanged()));
		TRACEPOINT;
	}

	/**
	 * @brief Constructor with possibility to select view.
	 * Note that the default view is still created first.
	 * @param matchCall - the MatchCall containing the information to be
	 * visualized.
	 * @param matchViewId - ID of the View to be set up. If a view of this name does
	 * not exist, the default view will be used.
	 */
	MatchCallTab(const cvv::impl::MatchCall& matchCall, const QString& matchViewId)
		: MultiViewCallTab<cvv::view::MatchView, cvv::impl::MatchCall>{
			  matchCall, matchViewId, QString{ "default_match_view" }, QString{ "LineMatchView" }
		  }
	{
		TRACEPOINT;
		oldView_ = view_;
		connect(&this->viewSet, SIGNAL(signal()), this, SLOT(viewChanged()));
		TRACEPOINT;
	}

	~MatchCallTab()
	{
		TRACEPOINT;
	}

	/**
	 * @brief Register the template class to the map of MatchViews.
	 * View needs to offer a constructor of the form View(const
	 * cvv::impl::MatchCall&, QWidget*).
	 * @param name to register the class under.
	 * @tparam View - Class to register.
	 * @return true when the view was registered and false when the name was
	 * already taken.
	 */
	template <class View> static bool registerMatchView(const QString &name)
	{
		TRACEPOINT;
		return registerView<View>(name);
	}

private slots:

	/**
	 * @brief Slot called when the view has completely changed.
	 */
	void viewChanged()
	{
		TRACEPOINT;
		if(oldView_ != nullptr)
		{
			view_->setKeyPointSelection(oldView_->getKeyPointSelection());
			view_->setMatchSelection(oldView_->getMatchSelection());
		}
		oldView_ = view_;
	}

private:

	/**
	 * @brief usually equal to view_, but not immediately changed when view_ is changed.
	 */
	cvv::view::MatchView* oldView_;

};
}
} // namespaces

#endif
