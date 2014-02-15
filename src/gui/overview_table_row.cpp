#include "overview_table_row.hpp"

#include <QTableWidgetItem>
#include <QImage>

#include "../qtutil/util.hpp"
#include "../stfl/stringutils.hpp"
#include "../dbg/dbg.hpp"

namespace cvv { namespace gui {

OverviewTableRow::OverviewTableRow(util::Reference<const impl::Call> call): call_{call}
{
	TRACEPOINT;
	id_ = call_->getId();
	idStr = QString::number(call_->getId());
	for (size_t i = 0; i < 2 && i < call->matrixCount(); i++)
	{
		QPixmap img;
		std::tie(std::ignore, img) = qtutil::convertMatToQPixmap(call->matrixAt(i));
		imgs.push_back(std::move(img));
	}
	description_ = QString(call_->description());
	if (call_->metaData().isKnown)
	{
		const auto& data = call_->metaData();
		line_ = data.line;
		lineStr = QString::number(data.line);
		fileStr = data.file;
		functionStr = data.function;
	}
	typeStr = QString(call_->type());
	TRACEPOINT;
}

void OverviewTableRow::addToTable(QTableWidget *table, size_t row, bool showImages, size_t maxImages, int imgHeight, int imgWidth)
{
	TRACEPOINT;
	auto *idItem = new QTableWidgetItem(idStr);
	std::vector<QTableWidgetItem*> items{};
	items.push_back(idItem);
	if (showImages)
	{
		for (size_t i = 0; i < imgs.size() && i < maxImages; i++)
		{
			QTableWidgetItem *imgWidget = new QTableWidgetItem{};
            imgWidget->setData(Qt::DecorationRole, imgs.at(i).scaled(imgHeight, imgWidth, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            imgWidget->setTextAlignment(Qt::AlignHCenter);
			items.push_back(imgWidget);
		}
	}
	items.push_back(new QTableWidgetItem(description_));
	items.push_back(new QTableWidgetItem(functionStr, 30));
	items.push_back(new QTableWidgetItem(fileStr));
	items.push_back(new QTableWidgetItem(lineStr));
	items.push_back(new QTableWidgetItem(typeStr));
	for (size_t i = 0; i < items.size(); i++)
	{
		auto *item = items[i];
		item->setFlags(item->flags() ^ Qt::ItemIsEditable);
		table->setItem(row, i, item);
	}
	TRACEPOINT;
}

}}
