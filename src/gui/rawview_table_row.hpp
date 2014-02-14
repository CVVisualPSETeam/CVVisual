#ifndef CVVISUAL_RAWVIEWTABLEROW_HPP
#define	CVVISUAL_RAWVIEWTABLEROW_HPP

#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>

#include <QTableWidget>
#include <QString>
#include <QList>

namespace cvv { namespace gui {

/**
 * @brief A simple container wrapper for the cv::DMatch and cv::KeyPoint class.
 * See the opencv documentation for more information on the getter methods.
 */
class RawviewTableRow
{
public:
	
    RawviewTableRow(cv::DMatch match, cv::KeyPoint keyPoint1, cv::KeyPoint keyPoint2);

    void addToTable(QTableWidget *table, size_t row);
	
	
	float matchDistance() const { return match.distance; }
	
	int matchImgIdx() const { return match.imgIdx; }
	
	int matchQueryIdx() const { return match.queryIdx; }
	
	int matchTrainIdx() const { return match.trainIdx; }
	

	float keyPoint1XCoord() const { return keyPoint1.pt.x; }

	float keyPoint1YCoord() const { return keyPoint1.pt.y; }
	
	cv::Point2f keyPoint1Coords() const { return keyPoint1.pt; }
	
	float keyPoint1Size() const { return keyPoint1.size; }
	
	float keyPoint1Angle() const { return keyPoint1.angle; }
	
	float keyPoint1Response() const { return keyPoint1.response; }
	
	int keyPoint1Octave() const { return keyPoint1.octave; }
	
	int keyPoint1ClassId() const { return keyPoint1.class_id; }
	
	
	float keyPoint2XCoord() const { return keyPoint2.pt.x; }

	float keyPoint2YCoord() const { return keyPoint2.pt.y; }

	cv::Point2f keyPoint2Coords() const { return keyPoint2.pt; }
	
	float keyPoint2Size() const { return keyPoint2.size; }
	
	float keyPoint2Angle() const { return keyPoint2.angle; }
	
	float keyPoint2Response() const { return keyPoint2.response; }
	
	int keyPoint2Octave() const { return keyPoint2.octave; }
	
	int keyPoint2ClassId() const { return keyPoint2.class_id; }
	
	/**
	 * @brief Serealizes the given rows into a single block of text.
	 * The currently supported formats are:
	 *  - `CSV`: Valid RFC 4180 CSV, with the same columns like the table.
	 *  - `JSON`: Valid JSON (each row is an object consisting of three sub objects:
	 * 			  `match`, `keypoint 1` and `keypoint 2`).
	 *  - `PYTHON`: Valid python code (see JSON).
	 *  - `RUBY`: Valid ruby code (see JSON).
	 *  @param rows given rows
	 *  @param format the format of the resulting representation (see above)
	 *  @return block representation of the given rows. 
	 */
	static QString rowsToText(const std::vector<RawviewTableRow> &rows, const QString format);
	
	/**
	 * @brief Returns the currently available text formats for the rowsToText method.
	 * @return {"CSV", "JSON", "PYTHON", "RUBY"}
	 */
	static std::vector<QString> getAvailableTextFormats();

private:
	cv::DMatch match;
	cv::KeyPoint keyPoint1;
	cv::KeyPoint keyPoint2;
};

QList<RawviewTableRow> createRawviewTableRows(const std::vector<cv::KeyPoint>& keyPoints1,
										const std::vector<cv::KeyPoint>& keyPoints2,
										const std::vector<cv::DMatch>& matches);

}}

#endif

