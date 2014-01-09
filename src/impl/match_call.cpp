#include "match_call.hpp"

namespace cvv { namespace impl {

MatchCall::MatchCall(cv::InputArray img1, std::vector<cv::KeyPoint> keypoints1, cv::InputArray img2,
		std::vector<cv::KeyPoint> keypoints2, std::vector<cv::DMatch> matches,
		impl::CallMetaData data, QString type ) :
	Call(data, type), img1_{img1}, keypoints1_{std::move(keypoints1)}, img2_{img2},
		keypoints2_{std::move(keypoints2)}, matches_{std::move(matches)} {}


}} //namespaces cvv::impl
