#ifndef CVVISUAL_FILTER_CALL_TAB_HPP
#define CVVISUAL_FILTER_CALL_TAB_HPP

#include <QString>
#include <QMap>
#include <QPushButton>
#include <QComboBox>
#include <QWidget>

#include "call_tab.hpp"
#include "filter_view.hpp"
#include "../controller/view_controller.hpp"
#include "filter_call.hpp"

#include "../util/util.hpp"

namespace cvv {
namespace impl {

class FilterCallTab: public CallTab
{
Q_OBJECT
private:
    static thread_local QMap<QString, cvv::impl::FilterView> filterViewMap;
    util::Reference<const cvv::impl::FilterCall> filterCall;
    util::Reference<const cvv::controller::ViewController> viewController;
    QString filterViewId;
    cvv::impl::FilterView filterView;

	QPushButton helpButton;
	QComboBox filterViewSelection;
public slots:
	void currentIndexChanged();
	void helpButtonClicked();
public:
    FilterCallTab(QString tabName, const cvv::impl::FilterCall& fc, const cvv::controller::ViewController& vc);
    FilterCallTab(QString tabName, const cvv::impl::FilterCall& fc, const cvv::controller::ViewController& vc, QString viewId);
	size_t getId();
    static thread_local void addFilterViewToMap(QString filterViewId, cvv::impl::FilterView filterView);
};

}}//namespaces

#endif
