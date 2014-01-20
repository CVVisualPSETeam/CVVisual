#ifndef CVVISUAL_OVERVIEWTABLECOLLUMN_HPP
#define	CVVISUAL_OVERVIEWTABLECOLLUMN_HPP

#include <QTableWidget>

#include "../impl/call.hpp"
#include "../util/util.hpp"

namespace cvv { namespace gui {

class OverviewTableCollumn
{
public:
	
	OverviewTableCollumn(const util::Reference<const impl::Call> call);
    
	/**
	 * @todo implement
     */	
	void addToTable(QTableWidget *table, size_t row, bool showImages);
	
	const util::Reference<const impl::Call> getCall();
	
private:
	util::Reference<const impl::Call> call;
};

}}

#endif

