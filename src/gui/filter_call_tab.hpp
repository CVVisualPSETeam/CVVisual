#ifndef CVVISUAL_FILTER_CALL_TAB_HPP
#define CVVISUAL_FILTER_CALL_TAB_HPP

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
class FilterCallTab: public CallTab
{
Q_OBJECT

public:

	/**
	 * @brief Constructor using default view.
	 * Short constructor which initialises the Call Tab with default view from settings.
	 * @param tabName
	 * @param fc the FilterCall containing the information to be visualized.
	 * @param vc the ViewController this CallTab belongs to.
	 */
	FilterCallTab(const QString& tabName, const cvv::impl::FilterCall& fc, const cvv::controller::ViewController& vc);

	/**
	 * @brief Constructor with specific view.
	 * Constructor initialising the Call Tab.
	 * @param tabName
	 * @param fc the FilterCall containing the information to be visualized.
	 * @param vc the ViewController this CallTab belongs to
	 * @param viewId the ID of the view to be shown inside this CallTab.
	 */
	FilterCallTab(const QString& tabName, const cvv::impl::FilterCall& fc, const cvv::controller::ViewController& vc, const QString& viewId);

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
	static void addFilterViewToMap(const QString& filterViewId, const cvv::view::FilterView& filterView);

private slots:

	/**
	 * @brief View selection change
	 * Called when the index of the view selection changes.
	 * @param text of the current selection in the view selection.
	 */
	void currentIndexChanged(const QString& text) const;

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

	//thread_local static QMap<QString, std::unique_ptr<cvv::view::FilterView>> filterViewMap;
	util::Reference<const cvv::impl::FilterCall> filterCall;
	util::Reference<const cvv::controller::ViewController> viewController;
	QString filterViewId;
	std::unique_ptr<cvv::view::FilterView> filterView;

	QPushButton* helpButton;
	QComboBox* filterViewSelection;	// Will eventually be replaced with the register helper's combo box (below)

	static cvv::qtutil::RegisterHelper<std::unique_ptr<cvv::view::FilterView>>* filterViewMap;
};

}}//namespaces

#endif
