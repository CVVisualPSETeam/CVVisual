
#include <QHBoxLayout>

#include "../qtutil/accordion.hpp"
#include "../qtutil/matchview/matchscene.hpp"
#include "../qtutil/matchview/cvvkeypoint.hpp"
#include "../qtutil/matchview/cvvmatch.hpp"
#include "../qtutil/matchview/singlecolormatchpen.hpp"
#include "../qtutil/matchview/singlecolorkeypointpen.hpp"
#include "../util/util.hpp"

#include "translationsmatchview.hpp"

namespace cvv{ namespace view{

TranslationMatchView::TranslationMatchView(std::vector<cv::KeyPoint> leftKeyPoints,
		std::vector<cv::KeyPoint> rightKeyPoints,
		std::vector<cv::DMatch> matches,
		cv::Mat leftIm,
		cv::Mat rightIm,
		bool usetrainIdx,
		QWidget *parent):
	MatchView{parent}
{
	TRACEPOINT;
	auto layout	= util::make_unique<QHBoxLayout>();
	auto accor	= util::make_unique< qtutil::Accordion>();
	auto matchscene	= util::make_unique<qtutil::MatchScene>(leftIm,rightIm);
	auto matchpen	= util::make_unique<qtutil::SingleColorMatchPen>();
	auto keypen	= util::make_unique<qtutil::SingleColorKeyPen>();

	qtutil::MatchScene *matchscene_ptr	= matchscene.get();
	qtutil::SingleColorMatchPen *matchpen_ptr	= matchpen.get();
	qtutil::SingleColorKeyPen *keypen_ptr	= keypen.get();

	accor->setMinimumWidth(250);
	accor->setMaximumWidth(250);


	accor->insert("Match Color",std::move(matchpen));
	accor->insert("KeyPoint Color",std::move(keypen));
	accor->insert("Left Image ",std::move(matchscene_ptr->getLeftMatInfoWidget()));
	accor->insert("Right Image ",std::move(matchscene_ptr->getRightMatInfoWidget()));


	layout->addWidget(accor.release());
	layout->addWidget(matchscene.release());

	setLayout(layout.release());

	std::vector<qtutil::CVVKeyPoint*> leftKeys;
	std::vector<qtutil::CVVKeyPoint*> leftinvisibleKeys;
	std::vector<qtutil::CVVKeyPoint*> rightKeys;
	std::vector<qtutil::CVVKeyPoint*> rightinvisibleKeys;

	for(auto& keypoint:leftKeyPoints)
	{
		//Key visible
		auto key = util::make_unique<qtutil::CVVKeyPoint>(keypoint);
		connect(keypen_ptr,SIGNAL(settingsChanged(KeyPointSettings&)),
			key.get(),SLOT(updateSettings(KeyPointSettings&)));

		matchscene_ptr->addLeftKeypoint(key.get());
		leftKeys.push_back(key.release());


		//Keyinvisible
		auto keyinvisible = util::make_unique<qtutil::CVVKeyPoint>(keypoint);
		keyinvisible->setShow(false);
		matchscene_ptr->addRightKeyPoint(keyinvisible.get());
		rightinvisibleKeys.push_back(keyinvisible.release());
	}

	for(auto& keypoint:rightKeyPoints)
	{
		//Key Visible
		auto key = util::make_unique<qtutil::CVVKeyPoint>(keypoint);
		connect(keypen_ptr,SIGNAL(settingsChanged(KeyPointSettings&)),
			key.get(),SLOT(updateSettings(KeyPointSettings&)));

		matchscene_ptr->addRightKeyPoint(key.get());
		rightKeys.push_back(key.release());


		//KeyInvisible
		auto keyinvisible = util::make_unique<qtutil::CVVKeyPoint>(keypoint);
		keyinvisible->setShow(false);
		matchscene_ptr->addLeftKeypoint(keyinvisible.get());
		leftinvisibleKeys.push_back(keyinvisible.release());
	}

	for(auto& match:matches)
	{
		//Match left
		auto cvmatchleft = util::make_unique<qtutil::CVVMatch>(
					leftKeys.at(match.queryIdx),
					leftinvisibleKeys.at((usetrainIdx?match.trainIdx:match.imgIdx)),match.distance);
		connect(matchpen_ptr,SIGNAL(settingsChanged(MatchSettings&)),cvmatchleft.get(),
					SLOT(updateSettings(MatchSettings&)));
		matchscene_ptr->addMatch(cvmatchleft.release());


		//Match right
		auto cvmatchright = util::make_unique<qtutil::CVVMatch>(
					rightinvisibleKeys.at(match.queryIdx),
					rightKeys.at((usetrainIdx?match.trainIdx:match.imgIdx)),match.distance);

		connect(matchpen_ptr,SIGNAL(settingsChanged(MatchSettings&))
			,cvmatchright.get(),SLOT(updateSettings(MatchSettings&)));
		matchscene_ptr->addMatch(cvmatchright.release());
	}
	TRACEPOINT;
}

}}
