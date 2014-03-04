#ifndef CVVISUAL_FILTER_CALL_TAB_HPP
#define CVVISUAL_FILTER_CALL_TAB_HPP

#include <QString>
#include <QWidget>

#include "multiview_call_tab.hpp"
#include "../view/filter_view.hpp"
#include "../impl/filter_call.hpp"
#include "../dbg/dbg.hpp"

namespace cvv
{
namespace gui
{

/** Filter Call Tab.
 * @brief Inner part of a tab, contains a FilterView.
 * The inner part of a tab or window
 * containing a FilterView.
 * Allows to switch views and to access the help.
 */
class FilterCallTab
    : public MultiViewCallTab<cvv::view::FilterView, cvv::impl::FilterCall>
{
	Q_OBJECT

      public:
	/**
	 * @brief Short constructor named after the Call, using the default
	 * view.
	 * Initializes the FilterCallTab with the default view and names it
	 * after the associated FilterCall.
	 * @param filterCall the FilterCall containing the information to be
	 * visualized.
	 */
	FilterCallTab(const cvv::impl::FilterCall &filterCall)
	    : MultiViewCallTab<cvv::view::FilterView, cvv::impl::FilterCall>(
	          filterCall)
	{
		TRACEPOINT;
		default_scope_ = QString{ "default_views" };
		default_key_ = QString{ "default_filter_view" };
		standard_default_ = QString{ "DefaultFilterView" };
		createGui();
		TRACEPOINT;
	}

	~FilterCallTab()
	{
		TRACEPOINT;
	}

	/**
	 * @brief Register the template class to the map of FilterViews.
	 * View needs to offer a constructor of the form View(const
	 * cvv::impl::FilterCall&, QWidget*).
	 * @param name to register the class under.
	 * @tparam View - Class to register.
	 * @return true when the view was registered and false when the name was
	 * already taken.
	 */
	template <class View>
	static bool registerFilterView(const QString &name)
	{
		TRACEPOINT;
		return registerView<View>(name);
	}
};
}
} // namespaces

#endif
