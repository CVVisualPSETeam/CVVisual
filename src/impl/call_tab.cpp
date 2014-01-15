#include <QString>

#include "call_tab.hpp"

namespace cvv {
namespace impl {

/* CallTab: */

QString CallTab::getName()
{
    return this->name;
}

void CallTab::setName(QString name)
{
    this->name = name;
}

}}//namespaces
