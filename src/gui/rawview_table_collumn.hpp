#ifndef CVVISUAL_RAWVIEWTABLECOLLUMN_HPP
#define	CVVISUAL_RAWVIEWTABLECOLLUMN_HPP

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
class RawviewTableCollumn
{
public:
	
    RawviewTableCollumn(cv::DMatch match, cv::KeyPoint keyPoint1, cv::KeyPoint keyPoint2);

    void addToTable(QTableWidget *table, size_t row);
	
	
	float matchDistance() { return match.distance; }
	
	int matchImgIdx() { return match.imgIdx; }
	
	int matchQueryIdx() { return match.queryIdx; }
	
	int matchTrainIdx() { return match.trainIdx; }
	

	float keyPoint1XCoord() { return keyPoint1.pt.x; }

	float keyPoint1YCoord() { return keyPoint1.pt.y; }
	
	cv::Point2f keyPoint1Coords() { return keyPoint1.pt; }
	
	float keyPoint1Size() { return keyPoint1.size; }
	
	float keyPoint1Angle() { return keyPoint1.angle; }
	
	float keyPoint1Response() { return keyPoint1.response; }
	
	int keyPoint1Octave() { return keyPoint1.octave; }
	
	int keyPoint1ClassId() { return keyPoint1.class_id; }
	
	
	float keyPoint2XCoord() { return keyPoint2.pt.x; }

	float keyPoint2YCoord() { return keyPoint2.pt.y; }

	cv::Point2f keyPoint2Coords() { return keyPoint2.pt; }
	
	float keyPoint2Size() { return keyPoint2.size; }
	
	float keyPoint2Angle() { return keyPoint2.angle; }
	
	float keyPoint2Response() { return keyPoint2.response; }
	
	int keyPoint2Octave() { return keyPoint2.octave; }
	
	int keyPoint2ClassId() { return keyPoint2.class_id; }
	
private:
	cv::DMatch match;
	cv::KeyPoint keyPoint1;
	cv::KeyPoint keyPoint2;
};

QList<RawviewTableCollumn> createRawviewTableCollumns(const std::vector<cv::KeyPoint>& keyPoints1,
										const std::vector<cv::KeyPoint>& keyPoints2,
										const std::vector<std::vector<cv::DMatch>>& matches);

}}

#endif

