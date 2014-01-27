#include "overview_table_collumn.hpp"

#include <QTableWidgetItem>
#include <QImage>

#include "../qtutil/util.hpp"
#include "../stfl/stringutils.hpp"

namespace cvv { namespace gui {

OverviewTableCollumn::OverviewTableCollumn(util::Reference<const impl::Call> call): call_{call}
{
    id_ = call_->getId();
    idStr = QString::number(call_->getId());
    for (size_t i = 0; i < 2 && i < call->matrixCount(); i++)
    {
        QPixmap img = qtutil::convertOpenCVMatToQPixmap(call_->matrixAt(i));
        imgs.push_back(img);
    }
    description_ = QString(call_->description());
    if (call_->metaData().isKnown)
    {
        auto data = call_->metaData();
        line_ = data.line;
        lineStr = QString::number(data.line);
        fileStr = QString::fromStdString(data.file);
        functionStr = QString::fromStdString(data.function);
    }
    typeStr = QString(call_->type());
}

void OverviewTableCollumn::addToTable(QTableWidget *table, size_t row, bool showImages, size_t maxImages)
{
    auto *idItem = new QTableWidgetItem(idStr);
    std::vector<QTableWidgetItem*> items{};
    items.push_back(idItem);
    if (showImages)
    {
        for (size_t i = 0; i < imgs.size() && i < maxImages; i++)
        {
            QTableWidgetItem *imgWidget = new QTableWidgetItem{};
            imgWidget->setData(Qt::DecorationRole, imgs.at(i));
            imgWidget->setSizeHint(QSize(100, 100));
            imgWidget->setTextAlignment(Qt::AlignHCenter);
            items.push_back(imgWidget);
        }
    }
    items.push_back(new QTableWidgetItem(description_));
    items.push_back(new QTableWidgetItem(stfl::shortenString(functionStr, 30)));
    items.push_back(new QTableWidgetItem(stfl::shortenString(fileStr, 30, false)));
    items.push_back(new QTableWidgetItem(lineStr));
    items.push_back(new QTableWidgetItem(typeStr));
    for (size_t i = 0; i < items.size(); i++)
    {
        auto *item = items[i];
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        table->setItem(row, i, item);
    }
}

}}
