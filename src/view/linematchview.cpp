#include <QHBoxLayout>

#include "../qtutil/accordion.hpp"
#include "../qtutil/matchscene.hpp"
#include "../qtutil/singlecolorpen.hpp"
#include "../qtutil/cvvkeypoint.hpp"
#include "../qtutil/cvvmatch.hpp"
#include "../qtutil/singlecolorpen.hpp"
#include "../qtutil/singlecolorkeypointpen.hpp"
#include "../util/util.hpp"

#include "linematchview.hpp"

namespace cvv{ namespace view{

	LineMatchView::LineMatchView(std::vector<cv::KeyPoint> leftKeyPoints,std::vector<cv::KeyPoint> rightKeyPoints,std::vector<cv::DMatch> matches,cv::Mat leftIm,cv::Mat rightIm,QWidget *parent):
		MatchView{parent}
	{
		QHBoxLayout *layout		= new QHBoxLayout{this};
		qtutil::Accordion *accor	= new qtutil::Accordion{this};
		qtutil::MatchScene *matchscene	= new qtutil::MatchScene{leftIm,rightIm,this};
		auto matchpen			= util::make_unique<qtutil::SingleColorPen>(this);
		auto keypen			= util::make_unique<qtutil::SingleColorKeyPen>(this);

	
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
			connect(keypen.get(),SIGNAL(settingsChanged(const KeyPointPen&)),key,SLOT(updatePen(const KeyPointPen&)));
			matchscene->addLeftKeypoint(key);
		}
		for(auto& keypoint:rightKeyPoints)
		{
			qtutil::CVVKeyPoint *key = new qtutil::CVVKeyPoint{keypoint};
			connect(keypen.get(),SIGNAL(settingsChanged(const KeyPointPen&)),key,SLOT(updatePen(const KeyPointPen&)));
			rightKeys.push_back(key);
			matchscene->addRightKeyPoint(key);
		}
		for(auto& match:matches)
		{
			qtutil::CVVMatch *cvmatch = new qtutil::CVVMatch(leftKeys.at(match.queryIdx),rightKeys.at(match.trainIdx),match.distance);
			connect(matchpen.get(),SIGNAL(settingsChanged(const MatchPen&)),cvmatch,SLOT(updatePen(const MatchPen&)));
			matchscene->addMatch(cvmatch);
		}

		accor->insert("Match Color",std::move(matchpen));
		accor->insert("KeyPoint Color",std::move(keypen));
		setLayout(layout);
	}
}}
