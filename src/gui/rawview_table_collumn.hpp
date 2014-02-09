#ifndef CVVISUAL_RAWVIEWTABLECOLLUMN_HPP
#define	CVVISUAL_RAWVIEWTABLECOLLUMN_HPP

#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>

#include <QTableWidget>
#include <QString>
#include <QList>

namespace cvv { namespace gui {

class RawviewTableCollumn
{
public:
	
    RawviewTableCollumn();
    
	/**
	 * @todo implement
     */	
    void addToTable(QTableWidget *table, size_t row);
	
private:

};

}}

#endif

