#include "match_call.hpp"

#include <stdexcept>

namespace cvv { namespace impl {

MatchCall::MatchCall(cv::InputArray img1, std::vector<cv::KeyPoint> keypoints1, cv::InputArray img2,
		std::vector<cv::KeyPoint> keypoints2, std::vector<cv::DMatch> matches,
		impl::CallMetaData data, QString type, QString description, QString requestedView):
	Call{data, std::move(type), std::move(description), std::move(requestedView)},
		img1_{img1.getMat()}, keypoints1_{std::move(keypoints1)}, img2_{img2.getMat()},
		keypoints2_{std::move(keypoints2)}, matches_{std::move(matches)} {}


const cv::Mat& MatchCall::matrixAt(size_t index) const {
	switch(index) {
		case 0:
			return img1();
		case 1:
			return img2();
		default:
			throw std::out_of_range{""};
	}
}

}} //namespaces cvv::impl
