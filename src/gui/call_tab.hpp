#ifndef CVVISUAL_CALL_TAB_HPP
#define CVVISUAL_CALL_TAB_HPP

#include <QString>
#include <QWidget>

#include "../util/util.hpp"

namespace cvv {
namespace gui {

/** A call tab.
 * The inner part of a tab or a window.
 * Super class for actual call tabs containing views.
 */
class CallTab: public QWidget
{
Q_OBJECT
public:

	/**
	 * @brief getName
	 * @return name
	 */
	const QString getName() { return name; }

	/**
	 * @brief setName
	 * @param name
	 */
	void setName(QString newName) { name = newName; }

private:
	QString name;
};

}}//namespaces

#endif
