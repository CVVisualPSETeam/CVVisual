#include <QString>
#include <QMap>
#include <QPushButton>
#include <QComboBox>

#include "match_call_tab.hpp"
#include "match_view.hpp"
#include "../controller/view_controller.hpp"
#include "match_call.hpp"
////////////////////////THIS IS A STUB//////////////////////////////////////////////////////////
namespace cvv {
namespace impl {

void MatchCallTab::currentIndexChanged()
{

}

void MatchCallTab::helpButtonClicked()
{

}

MatchCallTab::MatchCallTab(QString tabName, const cvv::impl::MatchCall& mc, const cvv::controller::ViewController& vc): matchCall{mc}, viewController{vc}
{
    this->setName(tabName);
	(void) mc;
	(void) vc;
}

MatchCallTab::MatchCallTab(QString tabName, const cvv::impl::MatchCall& mc, const cvv::controller::ViewController& vc, QString viewId): matchCall{mc}, viewController{vc}
{
    this->setName(tabName);
    (void) mc;
	(void) vc;
	(void) viewId;
}

size_t MatchCallTab::getId()
{
    return this->matchCall->getId();
}

void MatchCallTab::addMatchViewToMap(QString matchViewId, cvv::impl::MatchView matchView)
{
    (void) matchViewId;
    (void) matchView;
}

}} //namespaces
