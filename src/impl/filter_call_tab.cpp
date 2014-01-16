#include <QString>
#include <QMap>
#include <QPushButton>
#include <QComboBox>

#include "filter_call_tab.hpp"
#include "filter_view.hpp"
#include "../controller/view_controller.hpp"
#include "filter_call.hpp"

namespace cvv {
namespace impl {

void FilterCallTab::currentIndexChanged(const QString& text)
{
    filterViewId = text;
    //filterView = filterViewMap[filterViewId].createFilterView(filterCall->original(), filterCall->result());
}

void FilterCallTab::helpButtonClicked()
{
    //this->viewController->openHelpBrowser(filterViewId);
}

FilterCallTab::FilterCallTab(QString tabName, const cvv::impl::FilterCall& fc, const cvv::controller::ViewController& vc):
    filterCall{fc}, viewController{vc}
{
    this->setName(tabName);
    const QString scope{"default_views"};
    const QString key{"default_filter_view"};
    //QString setting = vc.getSetting(scope, key);
QString setting = "PLACEHOLDER"; (void) scope; (void) key;
    filterViewId = setting;
}

FilterCallTab::FilterCallTab(QString tabName, const cvv::impl::FilterCall& fc, const cvv::controller::ViewController& vc, QString viewId):
    filterCall{fc}, viewController{vc}
{
    this->setName(tabName);
    filterViewId = viewId;
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
