#ifndef CVVISUAL_CALL_TAB_HPP
#define CVVISUAL_CALL_TAB_HPP

#include <QString>
#include <QWidget>

#include "../util/util.hpp"

namespace cvv {
namespace impl {

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
    QString getName();

    /**
     * @brief setName
     * @param name
     */
    void setName(QString name);

private:
    QString name;
};

}}//namespaces

#endif
