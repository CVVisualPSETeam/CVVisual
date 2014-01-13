#include <QString>
#include <QMap>
#include <QPushButton>
#include <QComboBox>

#include "filter_call_tab.hpp"
#include "filter_view.hpp"
#include "../controller/view_controller.hpp"
#include "filter_call.hpp"
////////////////////////THIS IS A STUB//////////////////////////////////////////////////////////
namespace cvv {
namespace impl {

void FilterCallTab::currentIndexChanged()
{

}

void FilterCallTab::helpButtonClicked() {

}

FilterCallTab::FilterCallTab(QString tabName, const cvv::impl::FilterCall& fc, const cvv::controller::ViewController& vc): filterCall{fc}, viewController{vc}
{
    this->setName(tabName);
}

FilterCallTab::FilterCallTab(QString tabName, const cvv::impl::FilterCall& fc, const cvv::controller::ViewController& vc, QString viewId): filterCall{fc}, viewController{vc}
{
    this->setName(tabName);
    (void) fc;
	(void) vc;
	(void) viewId;
}

size_t FilterCallTab::getId()
{
    return this->filterCall->getId();
}

void FilterCallTab::addFilterViewToMap(QString filterViewId, cvv::impl::FilterView filterView)
{
    (void) filterViewId;
    (void) filterView;
}

}}//namespaces
