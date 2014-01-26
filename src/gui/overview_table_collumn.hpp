#ifndef CVVISUAL_OVERVIEWTABLECOLLUMN_HPP
#define	CVVISUAL_OVERVIEWTABLECOLLUMN_HPP

#include <vector>

#include <QTableWidget>
#include <QString>
#include <QPixmap>

#include "../impl/call.hpp"
#include "../util/util.hpp"

namespace cvv { namespace gui {

class OverviewTableCollumn
{
public:
	
    OverviewTableCollumn(util::Reference<const impl::Call> call);
    
	/**
	 * @todo implement
     */	
    void addToTable(QTableWidget *table, size_t row, bool showImages, size_t maxImages);
	
    util::Reference<const impl::Call> call() const { return call_; }

    QString description() const { return description_; }

    QString id() const { return idStr; }

    QString function() const { return functionStr; }

    QString file() const { return fileStr; }

    QString line() const { return lineStr; }

    QString type() const { return typeStr; }

private:
    util::Reference<const impl::Call> call_;
    QString idStr = "";
    QString description_ = "";
    std::vector<QPixmap> imgs{};
    QString functionStr = "";
    QString fileStr = "";
    QString lineStr = "";
    QString typeStr = "";
};

}}

#endif

