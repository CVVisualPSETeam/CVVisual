#ifndef CVVISUAL_DEPTH_MATCH_VIEW
#define CVVISUAL_DEPTH_MATCH_VIEW

#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"

#include "match_view.hpp"
namespace cvv{ namespace view{

/*
 * @brief this class interprets the translation of matches as depth value.
 * Unfinished
 */
class DepthMatchView: public MatchView{

Q_OBJECT
public:
	/*
	 * @brief the constructor
	 * @param lefKeyPoints (queryindx) the keypoint from the left image
	 * @param rightKeyPoint (trainIdx/imIdx) the keypoints from the right Image
	 * @param matches the matches between the images
	 * @param usetrainIdx if true the trainIdx will be taken for rightKeyPoint if false
	 *	the imIdx will be taken
	 * @param parent the parent widget
	 */
	DepthMatchView(std::vector<cv::KeyPoint> leftKeyPoints,std::vector<cv::KeyPoint> rightKeyPoints,
		std::vector<cv::DMatch> matches,cv::Mat leftIm,cv::Mat rightIm,QWidget *parent=nullptr);
};
}}
#endif