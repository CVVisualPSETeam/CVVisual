#ifndef CVVISUAL_MATCH_CALL_TAB_HPP
#define CVVISUAL_MATCH_CALL_TAB_HPP

#include <QString>
#include <QMap>
#include <QPushButton>
#include <QComboBox>
#include <QWidget>

#include "call_tab.hpp"
#include "match_view.hpp"
#include "../controller/view_controller.hpp"
#include "match_call.hpp"

#include "../util/util.hpp"

namespace cvv {
namespace impl {

class MatchCallTab: public CallTab
{
Q_OBJECT
private:
    static thread_local QMap<QString, cvv::impl::MatchView> matchViewMap;
    util::Reference<const cvv::impl::MatchCall> matchCall;
    util::Reference<const cvv::controller::ViewController> viewController;
    QString matchViewId;
    cvv::impl::MatchView matchView;

	QPushButton helpButton;
	QComboBox matchViewSelection;
public slots:
	void currentIndexChanged();
	void helpButtonClicked();
public:
    MatchCallTab(QString tabName, const cvv::impl::MatchCall& mc, const cvv::controller::ViewController& vc);
    MatchCallTab(QString name, const cvv::impl::MatchCall& mc, const cvv::controller::ViewController& vc, QString viewId);
	size_t getId();
    static thread_local void addMatchViewToMap(QString matchViewId, cvv::impl::MatchView matchView);
};
}} //namespaces

#endif
