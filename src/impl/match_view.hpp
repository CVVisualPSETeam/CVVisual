#ifndef CVVISUAL_MATCH_VIEW_HPP
#define CVVISUAL_MATCH_VIEW_HPP

#include <QString>
#include <QWidget>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>

#include "match_call.hpp"

namespace cvv {
namespace impl {

class MatchView: public QWidget
{
Q_OBJECT
signals:
	void updateLeftFooter(QString newText);
	void updateRightFoooter(QString newText);
	void outputInternalSelection(const std::vector<cvv::impl::MatchCall>& matchCalls);
public:
    //virtual MatchView createMatchView(const std::vector<cv::InputArray>& img1, const std::vector<cv::KeyPoint>& keypoints1, const std::vector<cv::InputArray>& img2, const std::vector<cv::KeyPoint>& keypoints2, const std::vector<std::vector<cv::DMatch>>& matches);
};

}} //namespaces

#endif
