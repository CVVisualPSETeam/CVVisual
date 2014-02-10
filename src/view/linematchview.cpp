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
			qtutil::CVVKeyPoint *key = new qtutil::CVVKeyPoint{keypoint};
			leftKeys.push_back(key);
			matchscene->addLeftKeypoint(key);
		}
		for(auto& keypoint:rightKeyPoints)
		{
			qtutil::CVVKeyPoint *key = new qtutil::CVVKeyPoint{keypoint};
			rightKeys.push_back(key);
			matchscene->addRightKeyPoint(key);
		}
		DEBUG(0,"left size %s",leftKeys.size());
		DEBUG(0,"right size %s",rightKeys.size());
		for(auto& match:matches)
		{
			DEBUG(0,"1111111111111111111111111111111111111111111111111111111111111111111111");
			qtutil::CVVMatch *cvmatch = new qtutil::CVVMatch(leftKeys.at(match.queryIdx),rightKeys.at(match.trainIdx),match.distance);
			DEBUG(0,"2222222222222222222222222222222222222222222222");
			matchscene->addMatch(cvmatch);
			DEBUG(0,"3333333333333333333333333333333333333333333333333333");
		}
		setLayout(layout);
	}
}}
