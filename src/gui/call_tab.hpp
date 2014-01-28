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
	 * @brief get name
	 * @return name
	 */
	const QString getName() const { return name; }

	/**
	 * @brief set name
	 * @param name
	 */
	void setName(const QString& newName) { name = newName; }

	/**
	 * @brief get ID
	 * @return the ID of the CallTab
	 * (ID is equal to the ID of the associated call in derived classes)
	 */
	virtual size_t getId() const { return 0; }

private:
	QString name;
};

}}//namespaces

#endif
