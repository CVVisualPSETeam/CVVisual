#include <QHBoxLayout>

#include "../qtutil/accordion.hpp"
#include "../qtutil/matchview/matchscene.hpp"
#include "../qtutil/matchview/singlecolormatchpen.hpp"
#include "../qtutil/matchview/cvvkeypoint.hpp"
#include "../qtutil/matchview/cvvmatch.hpp"
#include "../qtutil/matchview/singlecolormatchpen.hpp"
#include "../qtutil/matchview/singlecolorkeypointpen.hpp"
#include "../util/util.hpp"

#include "linematchview.hpp"

namespace cvv
{
namespace view
{

LineMatchView::LineMatchView(std::vector<cv::KeyPoint> leftKeyPoints,
			     std::vector<cv::KeyPoint> rightKeyPoints,
			     std::vector<cv::DMatch> matches, cv::Mat leftIm,
			     cv::Mat rightIm, bool usetrainIdx, QWidget *parent)
    : MatchView{ parent }
{
	auto layout = util::make_unique<QHBoxLayout>();
	auto accor = util::make_unique<qtutil::Accordion>();
	auto matchscene =
	    util::make_unique<qtutil::MatchScene>(leftIm, rightIm);
	auto matchpen = util::make_unique<qtutil::SingleColorMatchPen>(matches);
	auto keypen = util::make_unique<qtutil::SingleColorKeyPen>();
	//auto matchSelector = util::make_unique<qtutil::MatchSelector>();

	qtutil::MatchScene *matchscene_ptr = matchscene.get();
	qtutil::SingleColorMatchPen *matchpen_ptr = matchpen.get();
	qtutil::SingleColorKeyPen *keypen_ptr = keypen.get();
	//qtutil::MatchSelector *matchSelector_ptr = matchSelector.get();

	accor->setMinimumWidth(250);
	accor->setMaximumWidth(250);

	std::vector<qtutil::CVVKeyPoint *> leftKeys;
	std::vector<qtutil::CVVKeyPoint *> rightKeys;

	accor->insert("Match Color", std::move(matchpen));
	accor->insert("KeyPoint Color", std::move(keypen));
	accor->insert("Left Image ",
		      std::move(matchscene_ptr->getLeftMatInfoWidget()));
	accor->insert("Right Image ",
		      std::move(matchscene_ptr->getRightMatInfoWidget()));
	accor->insert("Sync Zoom ",
		      std::move(matchscene_ptr->getSyncZoomWidget()));

	layout->addWidget(accor.release());
	layout->addWidget(matchscene.release());

	setLayout(layout.release());

	for (auto &keypoint : leftKeyPoints)
	{
		auto key = util::make_unique<qtutil::CVVKeyPoint>(keypoint);
		connect(keypen_ptr, SIGNAL(settingsChanged(KeyPointSettings &)),
			key.get(), SLOT(updateSettings(KeyPointSettings &)));

		leftKeys.push_back(key.get());
		matchscene_ptr->addLeftKeypoint(std::move(key));
	}

	for (auto &keypoint : rightKeyPoints)
	{
		auto key = util::make_unique<qtutil::CVVKeyPoint>(keypoint);
		connect(keypen_ptr, SIGNAL(settingsChanged(KeyPointSettings &)),
			key.get(), SLOT(updateSettings(KeyPointSettings &)));

		rightKeys.push_back(key.get());
		matchscene_ptr->addRightKeyPoint(std::move(key));
	}

	for (auto &match : matches)
	{
		auto cvmatch = util::make_unique<qtutil::CVVMatch>(
		    leftKeys.at(match.queryIdx),
		    rightKeys.at((usetrainIdx ? match.trainIdx : match.imgIdx)),
		    match);

		connect(matchpen_ptr, SIGNAL(settingsChanged(MatchSettings &)),
			cvmatch.get(), SLOT(updateSettings(MatchSettings &)));

	//	connect(matchSelector_ptr,
	//		SIGNAL(settingsChanged(MatchSettings &)), cvmatch.get(),
	//		SLOT(updateSettings(MatchSettings &)));
		matchscene_ptr->addMatch(std::move(cvmatch));
	}
}
}
}
