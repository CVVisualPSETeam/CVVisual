
#include <QHBoxLayout>
#include <QPoint>
#include <QScrollBar>
#include <QPainterPath>
#include <QTransform>

#include "matchscene.hpp"
#include "../../dbg/dbg.hpp"

namespace cvv
{
namespace qtutil
{

MatchScene::MatchScene(const cv::Mat &imageLeft, const cv::Mat &imageRight, QWidget *parent)
    : QGraphicsView{ parent }
{
	auto basicLayout = util::make_unique<QHBoxLayout>();

	auto graphicScene = util::make_unique<QGraphicsScene>(this);
	graphicScene_ = graphicScene.get();
	auto graphicView =
	    util::make_unique<structures::MatchSceneGraphicsView>(
		graphicScene.release());
	graphicView_ = graphicView.get();

	auto leftImage = util::make_unique<ZoomableImage>(imageLeft);
	auto rightImage = util::make_unique<ZoomableImage>(imageRight);
	leftImage_ = leftImage.get();
	rightImage_ = rightImage.get();

	auto leftImWidget = util::make_unique<ZoomableProxyObject>(leftImage.release());
	auto rightImWidget = util::make_unique<ZoomableProxyObject>( rightImage.release() );

	leftImWidget_=leftImWidget.get();
	rightImWidget_=rightImWidget.get();

	graphicScene_->addItem(leftImWidget.release());
	graphicScene_->addItem(rightImWidget.release());

	leftImWidget_->setFlag(QGraphicsItem::ItemIsFocusable);
	rightImWidget_->setFlag(QGraphicsItem::ItemIsFocusable);

	basicLayout->addWidget(graphicView.release());
	setLayout(basicLayout.release());

	connect(graphicView_, SIGNAL(signalResized()), this,
		SLOT(viewReized()));
}

std::unique_ptr<SyncZoomWidget> MatchScene::getSyncZoomWidget()
{
	std::vector<ZoomableImage *> images;
	images.push_back(leftImage_);
	images.push_back(rightImage_);
	return util::make_unique<SyncZoomWidget>(images);
}

void MatchScene::addLeftKeypoint(std::unique_ptr<CVVKeyPoint> keypoint)
{
	keypoint->setZoomableImage(leftImage_);
	graphicScene_->addItem(keypoint.release());
}

void MatchScene::addRightKeyPoint(std::unique_ptr<CVVKeyPoint> keypoint)
{
	keypoint->setZoomableImage(rightImage_);
	graphicScene_->addItem(keypoint.release());
}

void MatchScene::addMatch(std::unique_ptr<CVVMatch> cvmatch)
{
	graphicScene_->addItem(cvmatch.release());
}

void MatchScene::selectAllVisible()
{
	QPainterPath selectionPath{};
	selectionPath.addRect(graphicScene_->itemsBoundingRect());
	graphicScene_->setSelectionArea(selectionPath, QTransform{});
}

void MatchScene::viewReized()
{
	int width = graphicView_->viewport()->width();
	int heigth = graphicView_->viewport()->height();

	// left
	leftImWidget_->setPos(0, 0);
	leftImWidget_->setMinimumSize((width / 2), heigth);
	leftImWidget_->setMaximumSize(width / 2, heigth);

	// right
	rightImWidget_->setPos(width / 2, 0);
	rightImWidget_->setMinimumSize(width / 2, heigth);
	rightImWidget_->setMaximumSize(width / 2, heigth);
	rightImWidget_->update();
	leftImWidget_->update();
	graphicView_->setSceneRect(0, 0, width, heigth);
}
}
}
