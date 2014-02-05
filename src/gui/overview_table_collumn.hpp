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
    void addToTable(QTableWidget *table, size_t row, bool showImages, size_t maxImages, int imgHeight = 100, int imgWidth = 100);
	
    util::Reference<const impl::Call> call() const { return call_; }

    QString description() const { return description_; }

    size_t id() const { return id_; }

    QString function() const { return functionStr; }

    QString file() const { return fileStr; }

    size_t line() const { return line_; }

    QString type() const { return typeStr; }

private:
    util::Reference<const impl::Call> call_;
    size_t id_ = 0;
    size_t line_ = 0;
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

