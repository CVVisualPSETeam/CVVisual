#ifndef CVVISUAL_MATCH_VIEW_HPP
#define CVVISUAL_MATCH_VIEW_HPP

#include <QString>
#include <QWidget>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>

#include "../impl/match_call.hpp"

namespace cvv {
namespace view {

class MatchView: public QWidget
{
Q_OBJECT

signals:

	/**
	 * @brief update left footer
	 * Signal to update the left side of the footer with newText.
	 * @param newText
	 */
	void updateLeftFooter(QString newText);

	/**
	 * @brief update right footer
	 * Signal to update the right side of the footer with newText.
	 * @param newText
	 */
	void updateRightFoooter(QString newText);

	/**
	 * @todo comment
	 * @brief output internal selection
	 * @param matchCalls
	 */
	void outputInternalSelection(const std::vector<cvv::impl::MatchCall>& matchCalls);

public:

	/**
	 * @brief factory method to create a MatchView
	 * Factory method to create MatchViews.
	 * @param img1
	 * @param keypoints1
	 * @param img2
	 * @param keypoints2
	 * @param matches
	 * @return
	 */
	//virtual MatchView createMatchView(const std::vector<cv::InputArray>& img1, const std::vector<cv::KeyPoint>& keypoints1, const std::vector<cv::InputArray>& img2, const std::vector<cv::KeyPoint>& keypoints2, const std::vector<std::vector<cv::DMatch>>& matches);
};

}} //namespaces

#endif
