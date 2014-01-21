#include <QString>
#include <QMap>
#include <QPushButton>
#include <QComboBox>

#include "match_call_tab.hpp"
#include "../view/match_view.hpp"
#include "../controller/view_controller.hpp"
#include "../impl/match_call.hpp"

namespace cvv {
namespace gui {

void MatchCallTab::currentIndexChanged() const
{

}

void MatchCallTab::helpButtonClicked() const
{

}

MatchCallTab::MatchCallTab(const QString& tabName, const cvv::impl::MatchCall& mc, const cvv::controller::ViewController& vc):
	matchCall{mc}, viewController{vc}
{
	setName(tabName);
	const QString scope{"default_views"};
	const QString key{"default_match_view"};
	//QString setting = this->viewController->getSetting(scope, key);
QString setting = "PLACEHOLDER"; (void) scope; (void) key;
	matchViewId = setting;
}

MatchCallTab::MatchCallTab(const QString& tabName, const cvv::impl::MatchCall& mc, const cvv::controller::ViewController& vc, const QString& viewId):
	matchCall{mc}, viewController{vc}
{
	setName(tabName);
	matchViewId = viewId;
}

size_t MatchCallTab::getId() const
{
	return matchCall->getId();
}

void MatchCallTab::addMatchViewToMap(const QString& matchViewId, const cvv::view::MatchView& matchView)
{
	(void) matchViewId;
	(void) matchView;
}

}} //namespaces
