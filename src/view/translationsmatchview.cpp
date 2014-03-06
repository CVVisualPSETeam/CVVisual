
#include <QHBoxLayout>

#include "../qtutil/accordion.hpp"
#include "../qtutil/matchview/matchscene.hpp"
#include "../qtutil/matchview/cvvkeypoint.hpp"
#include "../qtutil/matchview/cvvmatch.hpp"
#include "../qtutil/matchview/matchmanagement.hpp"
#include "../qtutil/matchview/singlecolorkeypointpen.hpp"
#include "../util/util.hpp"

#include "translationsmatchview.hpp"

namespace cvv
{
namespace view
{

TranslationMatchView::TranslationMatchView(
    std::vector<cv::KeyPoint> leftKeyPoints,
    std::vector<cv::KeyPoint> rightKeyPoints, std::vector<cv::DMatch> matches,
    cv::Mat leftIm, cv::Mat rightIm, bool usetrainIdx, QWidget *parent)
    : MatchView{ parent }
{
	TRACEPOINT;
	auto layout = util::make_unique<QHBoxLayout>();
	auto accor = util::make_unique<qtutil::Accordion>();
	auto matchscene =
	    util::make_unique<qtutil::MatchScene>(leftIm, rightIm);
	auto matchmnt = util::make_unique<qtutil::MatchManagement>(matches);
	auto keypen = util::make_unique<qtutil::SingleColorKeyPen>();

	qtutil::MatchScene *matchscene_ptr = matchscene.get();
	qtutil::MatchManagement *matchmnt_ptr = matchmnt.get();
	qtutil::SingleColorKeyPen *keypen_ptr = keypen.get();

	accor->setMinimumWidth(350);
	accor->setMaximumWidth(350);

	accor->insert("Match Settings", std::move(matchmnt));
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

	std::vector<qtutil::CVVKeyPoint *> leftKeys;
	std::vector<qtutil::CVVKeyPoint *> leftinvisibleKeys;
	std::vector<qtutil::CVVKeyPoint *> rightKeys;
	std::vector<qtutil::CVVKeyPoint *> rightinvisibleKeys;

	for (auto &keypoint : leftKeyPoints)
	{
		// Key visible
		auto key = util::make_unique<qtutil::CVVKeyPoint>(keypoint);
		connect(keypen_ptr, SIGNAL(settingsChanged(KeyPointSettings &)),
			key.get(), SLOT(updateSettings(KeyPointSettings &)));

		leftKeys.push_back(key.get());
		matchscene_ptr->addLeftKeypoint(std::move(key));


		// Keyinvisible
		auto keyinvisible =
		    util::make_unique<qtutil::CVVKeyPoint>(keypoint);

		keyinvisible->setShow(false);
		rightinvisibleKeys.push_back(keyinvisible.get());
		matchscene_ptr->addRightKeyPoint(std::move(keyinvisible));

	}

	for (auto &keypoint : rightKeyPoints)
	{
		// Key Visible
		auto key = util::make_unique<qtutil::CVVKeyPoint>(keypoint);
		connect(keypen_ptr, SIGNAL(settingsChanged(KeyPointSettings &)),
			key.get(), SLOT(updateSettings(KeyPointSettings &)));

		rightKeys.push_back(key.get());
		matchscene_ptr->addRightKeyPoint(std::move(key));

		// KeyInvisible
		auto keyinvisible =
		    util::make_unique<qtutil::CVVKeyPoint>(keypoint);

		keyinvisible->setShow(false);
		leftinvisibleKeys.push_back(keyinvisible.get());
		matchscene_ptr->addLeftKeypoint(std::move(keyinvisible));

	}

	for (auto &match : matches)
	{
		// Match left
		auto cvmatchleft = util::make_unique<qtutil::CVVMatch>(
		    leftKeys.at(match.queryIdx),
		    leftinvisibleKeys.at(
			(usetrainIdx ? match.trainIdx : match.imgIdx)),
		    match);
		connect(matchmnt_ptr, SIGNAL(settingsChanged(MatchSettings &)),
			cvmatchleft.get(),
			SLOT(updateSettings(MatchSettings &)));
		matchscene_ptr->addMatch(std::move(cvmatchleft));

		// Match right
		auto cvmatchright = util::make_unique<qtutil::CVVMatch>(
		    rightinvisibleKeys.at(match.queryIdx),
		    rightKeys.at((usetrainIdx ? match.trainIdx : match.imgIdx)),
		    match);

		connect(matchmnt_ptr, SIGNAL(settingsChanged(MatchSettings &)),
			cvmatchright.get(),
			SLOT(updateSettings(MatchSettings &)));
		matchscene_ptr->addMatch(std::move(cvmatchright));
	}
	matchmnt_ptr->updateAll();
	TRACEPOINT;
}
}
}
