#include "rawview_table_row.hpp"

#include <QTableWidgetItem>
#include <QImage>

#include "../qtutil/util.hpp"
#include "../stfl/stringutils.hpp"

namespace cvv { namespace gui {

RawviewTableRow::RawviewTableRow(cv::DMatch match, cv::KeyPoint keyPoint1, cv::KeyPoint keyPoint2):
	match{match}, keyPoint1{keyPoint1}, keyPoint2{keyPoint2}
{
}

void RawviewTableRow::addToTable(QTableWidget *table, size_t row)
{
	std::vector<QString> items {
		QString::number(matchDistance()),
		QString::number(matchImgIdx()),
		QString::number(matchQueryIdx()),
		QString::number(matchTrainIdx()),
		QString::number(keyPoint1XCoord()),
		QString::number(keyPoint1YCoord()),
		QString::number(keyPoint1Size()),
		QString::number(keyPoint1Angle()),
		QString::number(keyPoint1Response()),
		QString::number(keyPoint1Octave()),
		QString::number(keyPoint1ClassId()),
		QString::number(keyPoint2XCoord()),
		QString::number(keyPoint2YCoord()),
		QString::number(keyPoint2Size()),
		QString::number(keyPoint2Angle()),
		QString::number(keyPoint2Response()),
		QString::number(keyPoint2Octave()),
		QString::number(keyPoint2ClassId())
	};
	for (size_t i = 0; i < items.size(); i++)
	{
		auto *item = new QTableWidgetItem(items[i]);
		item->setFlags(item->flags() ^ Qt::ItemIsEditable);
		table->setItem(row, i, item);
	}
}



QList<RawviewTableRow> createRawviewTableRows(const std::vector<cv::KeyPoint>& keyPoints1,
										const std::vector<cv::KeyPoint>& keyPoints2,
										const std::vector<std::vector<cv::DMatch>>& matches)
{
	auto retList = QList<RawviewTableRow>();
	for (auto &matchList : matches)
	{
		for (auto &match : matchList)
		{
			retList.append(RawviewTableRow(match, keyPoints1.at(match.queryIdx), keyPoints2.at(match.trainIdx)));
		}
	}
	return retList;
}

}}
