#include "overview_table_collumn.hpp"

#include <QTableWidgetItem>
#include <QImage>

#include "../qtutil/util.hpp"

namespace cvv { namespace gui {

OverviewTableCollumn::OverviewTableCollumn(util::Reference<const impl::Call> call): call_{call}
{
    QString idStr = QString::number(call_->getId());
    for (size_t i = 0; i < 2 && i < call->matrixCount(); i++)
    {
        QPixmap img = qtutil::convertOpenCVMatToQPixmap(call_->matrixAt(i));
        imgs.push_back(img);
    }
    description_ = QString(call_->description());
    if (call_->metaData().isKnown)
    {
        auto data = call_->metaData();
        lineStr = QString::number(data.line);
        fileStr = QString::fromStdString(data.file);
        functionStr = QString::fromStdString(data.function);
    }
    typeStr = QString(call_->type());
}

void OverviewTableCollumn::addToTable(QTableWidget *table, size_t row, bool showImages, size_t maxImages)
{
    auto *idItem = new QTableWidgetItem(idStr);
    table->setItem(row, 0, idItem);
    size_t imgNumber = 0;
    if (showImages)
    {
        for (size_t i = 0; i < imgs.size() && i < maxImages; i++)
        {
            QTableWidgetItem *imgWidget = new QTableWidgetItem{};
            imgWidget->setData(Qt::DecorationRole, imgs.at(i));
            table->setItem(row, i + 1, imgWidget);
        }
        imgNumber = maxImages;
    }
    table->setItem(row, 1 + imgNumber, new QTableWidgetItem(description_));
    table->setItem(row, 2 + imgNumber, new QTableWidgetItem(functionStr));
    table->setItem(row, 3 + imgNumber, new QTableWidgetItem(fileStr));
    table->setItem(row, 4 + imgNumber, new QTableWidgetItem(lineStr));
    table->setItem(row, 5 + imgNumber, new QTableWidgetItem(typeStr));
}

}}
