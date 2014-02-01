#ifndef CVVISUAL_MATCH_CALL_HPP
#define CVVISUAL_MATCH_CALL_HPP

#include <vector>
#include <utility>
#include <type_traits>

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"

#include "call.hpp"

namespace cvv { namespace impl {

/**
 * Contains all the calldata (= location, images and their keypoints).
 */
class MatchCall: public Call {
public:
	MatchCall(cv::InputArray img1, std::vector<cv::KeyPoint> keypoints1, cv::InputArray img2,
			std::vector<cv::KeyPoint> keypoints2, std::vector<cv::DMatch> matches,
			impl::CallMetaData data, QString type, QString description,
			QString requestedView);
	
	size_t matrixCount() const override { return 2; }
	const cv::Mat& matrixAt(size_t index) const override;
	
	const cv::Mat& img1() const {return img1_;}
	const cv::Mat& img2() const {return img2_;}
	const std::vector<cv::KeyPoint>& keyPoints1() const {return keypoints1_;}
	const std::vector<cv::KeyPoint>& keyPoints2() const {return keypoints2_;}
	const std::vector<cv::DMatch>& matches() const {return matches_;}
	
private:
	cv::Mat img1_;
	std::vector<cv::KeyPoint> keypoints1_;
	cv::Mat img2_;
	std::vector<cv::KeyPoint> keypoints2_;
	std::vector<cv::DMatch> matches_;
};


void debugMatchCall(
		cv::InputArray img1, std::vector<cv::KeyPoint> keypoints1,
		cv::InputArray img2, std::vector<cv::KeyPoint> keypoints2,
		std::vector<cv::DMatch> matches, const CallMetaData& data,
		const char* match, const char* description, const char* view);

}} //namespaces cvv::impl


#endif
