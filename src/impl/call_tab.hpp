#ifndef CVVISUAL_CALL_TAB_HPP
#define CVVISUAL_CALL_TAB_HPP

#include <QString>
#include <QWidget>

#include "../util/util.hpp"

namespace cvv {
namespace impl {

class CallTab: public QWidget
{
Q_OBJECT
private:
	QString name;
public:
    QString getName();
    void setName(QString name);
};

}}//namespaces

#endif
