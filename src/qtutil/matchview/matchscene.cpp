
#include <QHBoxLayout>
#include <QPoint>
#include <QScrollBar>


#include "matchscene.hpp"
#include "../../dbg/dbg.hpp"

namespace cvv{ namespace qtutil{

MatchScene::MatchScene(cv::Mat imageLeft,cv::Mat imageRight, QWidget* parent):
		QWidget{parent}
{
	TRACEPOINT;

	auto basicLayout	= util::make_unique<QHBoxLayout>();
	auto graphicScene	= util::make_unique<QGraphicsScene>();
	graphicScene_		= graphicScene.get();
	auto graphicView	= util::make_unique<QGraphicsView>(graphicScene.release());
	graphicView_		=graphicView.get();

	auto leftImage		= util::make_unique<ZoomableImage>(imageLeft);
	auto rightImage		= util::make_unique<ZoomableImage>(imageRight);
	leftImage_		= leftImage.get();
	rightImage_		= rightImage.get();

	TRACEPOINT;
	//leftImWidget_		= graphicScene_->addWidget(leftImage.release());
	//rightImWidget_		= graphicScene_->addWidget(rightImage.release());
	leftImWidget_ = new ZoomableProxyObject{leftImage.release()};
	TRACEPOINT;

	rightImWidget_= new ZoomableProxyObject{rightImage.release()};
	TRACEPOINT;

	graphicScene_->addItem(leftImWidget_);
	TRACEPOINT;
	graphicScene_->addItem(rightImWidget_);

	TRACEPOINT;
	leftImWidget_->setFlag(QGraphicsItem::ItemIsFocusable);
	rightImWidget_->setFlag(QGraphicsItem::ItemIsFocusable);
/*
	graphicView_->horizontalScrollBar()->setFocusPolicy(Qt::FocusPolicy::NoFocus);
	graphicView_->verticalScrollBar()->setFocusPolicy(Qt::NoFocus);
	graphicView_->setFocusPolicy(Qt::NoFocus);
*/
	basicLayout->addWidget(graphicView.release());
	//basicLayout->setSizeConstraint(QLayout::SetNoConstraint);
	setLayout(basicLayout.release());

	adjustImages();

	TRACEPOINT;
}

void MatchScene::addLeftKeypoint(CVVKeyPoint *keypoint)
{
	TRACEPOINT;
	keypoint->setZoomableImage(leftImage_);
	graphicScene_->addItem(keypoint);
	TRACEPOINT;
}
void MatchScene::addRightKeyPoint(CVVKeyPoint *keypoint)
{
	TRACEPOINT;
	keypoint->setZoomableImage(rightImage_);
	graphicScene_->addItem(keypoint);
	TRACEPOINT;
}
void MatchScene::addMatch(CVVMatch *cvmatch)
{
	TRACEPOINT;
	graphicScene_->addItem(cvmatch);
	TRACEPOINT;
}

void MatchScene::adjustImages()
{
	TRACEPOINT;
	QResizeEvent event{size(),size()};
	resizeEvent(&event);
	TRACEPOINT;
}

void MatchScene::resizeEvent(QResizeEvent*)
{
	TRACEPOINT;
	int width	= graphicView_->viewport()->width();
	int heigth	= graphicView_->viewport()->height();

	//left
	leftImWidget_->setPos(0,0);
	leftImWidget_->setMinimumSize((width/2),heigth);
	leftImWidget_->setMaximumSize(width/2,heigth);

	//right
	rightImWidget_->setPos(width/2,0);
	rightImWidget_->setMinimumSize(width/2,heigth);
	rightImWidget_->setMaximumSize(width/2,heigth);
	graphicView_->setSceneRect(0,0,width,heigth);

	TRACEPOINT;
}
}}
