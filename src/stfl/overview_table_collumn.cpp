#include "overview_table_collumn.hpp"

namespace cvv { namespace stfl {

OverviewTableCollumn::OverviewTableCollumn(const util::Reference<const impl::Call> call): call{call} {}

void OverviewTableCollumn::addToTable(QTableWidget *table, size_t row, bool showImages)
{
	(void)table;
	(void)row;
	(void)showImages;
}
	
const util::Reference<const impl::Call> OverviewTableCollumn::getCall()
{
	return call;
}

}}
