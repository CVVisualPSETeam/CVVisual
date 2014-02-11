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
#include "../controller/view_controller.hpp"
#include "../impl/filter_call.hpp"
#include "../util/util.hpp"
#include "../qtutil/registerhelper.hpp"

namespace cvv {
namespace gui {

/** Filter Call Tab
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
	 * @brief Short constructor using name from Call and default view.
	 * Initializes the FilterCallTab with the default view and names it after the associated FilterCall.
	 * @param fc the FilterCall containing the information to be visualized.
	 * @param vc the ViewController this CallTab belongs to.
	 */
	FilterCallTab(const cvv::impl::FilterCall& fc, cvv::controller::ViewController& vc);

	/**
	 * @brief Constructor using default view.
	 * Short constructor which initialises the Call Tab with default view from settings.
	 * @param tabName
	 * @param fc the FilterCall containing the information to be visualized.
	 * @param vc the ViewController this CallTab belongs to.
	 * @attention might be deleted
	 */
	FilterCallTab(const QString& tabName, const cvv::impl::FilterCall& fc, cvv::controller::ViewController& vc);

	/**
	 * @brief Constructor with specific view.
	 * Constructor initialising the Call Tab.
	 * @param tabName
	 * @param fc the FilterCall containing the information to be visualized.
	 * @param vc the ViewController this CallTab belongs to
	 * @param viewId the ID of the view to be shown inside this CallTab.
	 * @attention might be deleted
	 */
	FilterCallTab(const QString& tabName, const cvv::impl::FilterCall& fc, cvv::controller::ViewController& vc, const QString& viewId);

	/**
	 * @brief get ID
	 * @return the ID of the CallTab
	 * (ID is equal to the ID of the associated call)
	 * Overrides CallTab's getId.
	 */
	size_t getId() const override;

	/**
	 * @brief adds FilterView to map of all
	 * Adds a FilterView with a name to the thread local map of all FilterViews.
	 * @param filterViewId the Id or name of the FilterView.
	 */
	static void addFilterViewToMap(const QString& filterViewId, std::function<std::unique_ptr<cvv::view::FilterView>(const std::vector<cv::Mat>&, QWidget*)>);

private slots:

	/**
	 * @brief View selection change.
	 * Called when the index of the view selection changes.
	 * @param text of the current selection in the view selection.
	 */
	void currentIndexChanged(const QString& text);

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
	 * @throw std::out_of_range if no view named viewId was registered.
	 */
	void setView(const QString& viewId);

	util::Reference<const cvv::impl::FilterCall> filterCall_;
	util::Reference<cvv::controller::ViewController> viewController_;
	QString filterViewId_;
	cvv::view::FilterView* filterView_;

	QPushButton* helpButton_;
	QPushButton* setAsDefaultButton_;
	QHBoxLayout* hlayout_;
	QVBoxLayout* vlayout_;
	QWidget* upperBar_;

};

}}//namespaces

#endif
