#include <QHBoxLayout>

#include "../qtutil/accordion.hpp"
#include "../qtutil/matchscene.hpp"
#include "../qtutil/singlecolorpen.hpp"
#include "../qtutil/cvvkeypoint.hpp"
#include "../qtutil/cvvmatch.hpp"

#include "linematchview.hpp"

namespace cvv{ namespace view{

	LineMatchView::LineMatchView(std::vector<cv::KeyPoint> leftKeyPoints,std::vector<cv::KeyPoint> rightKeyPoints,std::vector<cv::DMatch> matches,cv::Mat leftIm,cv::Mat rightIm,QWidget *parent):
		MatchView{parent}
	{
		QHBoxLayout *layout		= new QHBoxLayout{};
		qtutil::Accordion *accor	= new qtutil::Accordion{};
		qtutil::MatchScene *matchscene	= new qtutil::MatchScene{leftIm,rightIm};

		layout->addWidget(accor);
		layout->addWidget(matchscene);

		accor->setMinimumWidth(250);
		accor->setMaximumWidth(250);
		
		std::vector<qtutil::CVVKeyPoint*> leftKeys;
		std::vector<qtutil::CVVKeyPoint*> rightKeys;

		for(auto& keypoint:leftKeyPoints)
		{
			leftKeys.push_back(new qtutil::CVVKeyPoint(keypoint));
			matchscene->addLeftKeypoint(leftKeys.back());
		}
		for(auto& keypoint:rightKeyPoints)
		{
			leftKeys.push_back(new qtutil::CVVKeyPoint(keypoint));
			matchscene->addRightKeyPoint(rightKeys.back());
		}
		for(auto& match:matches)
		{
			qtutil::CVVMatch *cvmatch = new qtutil::CVVMatch(leftKeys.at(match.queryIdx),rightKeys.at(match.trainIdx),match.distance);
			matchscene->addMatch(cvmatch);
		}
	}
}}
