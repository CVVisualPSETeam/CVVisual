#include "rawview_table_collumn.hpp"

#include <QTableWidgetItem>
#include <QImage>

#include "../qtutil/util.hpp"
#include "../stfl/stringutils.hpp"

namespace cvv { namespace gui {

RawviewTableCollumn::RawviewTableCollumn()
{
}

void RawviewTableCollumn::addToTable(QTableWidget *table, size_t row)
{
	(void)table;
	(void)row;
}

}}
