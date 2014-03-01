#ifndef CVVISUAL_FILTER_CALL_TAB_HPP
#define CVVISUAL_FILTER_CALL_TAB_HPP

#include <memory>

#include <QString>
#include <QMap>
#include <QPushButton>
#include <QComboBox>
#include <QWidget>

#include "call_tab.hpp"
#include "../view/filter_view.hpp"
#include "../impl/filter_call.hpp"
#include "../util/util.hpp"
#include "../qtutil/registerhelper.hpp"
#include "../qtutil/signalslot.hpp"
#include "../dbg/dbg.hpp"

namespace cvv {
namespace gui {

/** Filter Call Tab.
 * @brief Inner part of a tab, contains a FilterView.
 * The inner part of a tab or window
 * containing a FilterView.
 * Allows to switch views and to access the help.
 */
class FilterCallTab:
		public CallTab, public cvv::qtutil::RegisterHelper<cvv::view::FilterView, const std::vector<cv::Mat>&, QWidget*>
{
Q_OBJECT

public:

	/**
	 * @brief Short constructor named after the Call, using the default view.
	 * Initializes the FilterCallTab with the default view and names it after the associated FilterCall.
	 * @param filterCall the FilterCall containing the information to be visualized.
	 */
	FilterCallTab(const cvv::impl::FilterCall& filterCall);

	/**
	 * @brief Constructor using default view.
	 * Short constructor which initialises the Call Tab with default view from settings.
	 * @param tabName.
	 * @param filterCall the FilterCall containing the information to be visualized.
	 */
	FilterCallTab(const QString& tabName, const cvv::impl::FilterCall& filterCalll);

	/**
	 * @brief Constructor with specific view.
	 * Constructor initialising the Call Tab.
	 * @param tabName.
	 * @param filterCall the FilterCall containing the information to be visualized.
	 * @param viewId the ID of the view to be shown inside this CallTab.
	 */
	FilterCallTab(const QString& tabName, const cvv::impl::FilterCall& filterCall, const QString& viewId);
	
	~FilterCallTab(){TRACEPOINT;}
	
	/**
	 * @brief get ID.
	 * @return the ID of the CallTab.
	 * (ID is equal to the ID of the associated call).
	 * Overrides CallTab's getId.
	 */
	size_t getId() const override;

	/**
	 * @brief Register the template class to the map of FilterViews.
	 * View needs to offer a constructor of the form View(const std::vector<cv::Mat>&, QWidget*).
	 * @param name to register the class under.
	 * @tparam View - Class to register.
	 * @return true when the view was registered and false when the name was already taken.
	 */
	template<class View>
	static bool registerFilterView(const QString& name)
	{
		TRACEPOINT;
		return gui::FilterCallTab::registerElement(name,
			[](const std::vector<cv::Mat>& mat, QWidget* parent){
				TRACEPOINT;
				return cvv::util::make_unique<View>(mat,parent);
			}
		);
	}

	/**
	 * @brief adds FilterView to map of all.
	 * Adds a FilterView with a name to the thread local map of all FilterViews.
	 * @param filterViewId the ID or name of the FilterView.
	 * @param fView: function returning a unique_ptr on a FilterView.
	 */
	static void addFilterViewToMap(const QString& filterViewId, std::function<std::unique_ptr<cvv::view::FilterView>(const std::vector<cv::Mat>&, QWidget*)>);

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
	 * @brief sets up the View currently selected in the ComboBox inherited from RegisterHelper.
	 */
	void setView();

	util::Reference<const cvv::impl::FilterCall> filterCall_;
	QString filterViewId_;
	cvv::view::FilterView* filterView_;
	std::map<QString, cvv::view::FilterView*> viewHistory_;

	QPushButton* helpButton_;
	QPushButton* setAsDefaultButton_;
	QHBoxLayout* hlayout_;
	QVBoxLayout* vlayout_;
	QWidget* upperBar_;

};

}}//namespaces

#endif
