#ifndef CVVISUAL_MATCH_CALL_HPP
#define CVVISUAL_MATCH_CALL_HPP

#include <vector>
#include <utility>

#include "call.hpp"

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"

namespace cvv { namespace impl {

/**
 * Contains all the calldata (= location, images and their keypoints).
 */
class MatchCall: public Call {
public:
	MatchCall(cv::InputArray img1, std::vector<cv::KeyPoint> keypoints1, cv::InputArray img2,
			std::vector<cv::KeyPoint> keypoints2, std::vector<cv::DMatch> matches,
			impl::CallMetaData data, QString type );
	
	cv::InputArray img1() const {return img1_;}
	cv::InputArray img2() const {return img2_;}
	const std::vector<cv::KeyPoint>& keyPoints1() const {return keypoints1_;}
	const std::vector<cv::KeyPoint>& keyPoints2() const {return keypoints2_;}
	const std::vector<cv::DMatch>& matches() const {return matches_;}
private:
	cv::InputArray img1_;
	std::vector<cv::KeyPoint> keypoints1_;
	cv::InputArray img2_;
	std::vector<cv::KeyPoint> keypoints2_;
	std::vector<cv::DMatch> matches_;
};


}} //namespaces cvv::impl


#endif
