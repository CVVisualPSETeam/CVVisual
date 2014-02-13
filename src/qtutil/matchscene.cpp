
#include <QHBoxLayout>
#include <QPoint>
#include <QScrollBar>

#include "matchscene.hpp"
#include "../dbg/dbg.hpp"

namespace cvv{ namespace qtutil{

MatchScene::MatchScene(cv::Mat imageLeft,cv::Mat imageRight, QWidget* parent):
		QWidget{parent}
{
	TRACEPOINT;	

	QHBoxLayout *basicLayout = new QHBoxLayout{};
	graphicScene_		 = new QGraphicsScene{this};
	graphicView_		 = new QGraphicsView{graphicScene_};			
	
	basicLayout->addWidget(graphicView_);
	basicLayout->setSizeConstraint(QLayout::SetNoConstraint);
	setLayout(basicLayout);

	graphicView_->horizontalScrollBar()->setFocusPolicy(Qt::FocusPolicy::NoFocus);
	graphicView_->verticalScrollBar()->setFocusPolicy(Qt::NoFocus);
	graphicView_->setFocusPolicy(Qt::NoFocus);

	leftImage_		= new ZoomableImage{imageLeft};
	rightImage_		= new ZoomableImage{imageRight};

	leftImWidget_ 	= graphicScene_->addWidget(leftImage_);
	rightImWidget_ 	= graphicScene_->addWidget(rightImage_);

	QResizeEvent event{size(),size()};
	resizeEvent(&event);

	TRACEPOINT;
}

void MatchScene::addLeftKeypoint(CVVKeyPoint *keypoint)
{
	keypoint->setZoomableImage(leftImage_);
	graphicScene_->addItem(keypoint);
}
void MatchScene::addRightKeyPoint(CVVKeyPoint *keypoint)
{
	keypoint->setZoomableImage(rightImage_);
	graphicScene_->addItem(keypoint);
}
void MatchScene::addMatch(CVVMatch *cvmatch)
{
	graphicScene_->addItem(cvmatch);
}

void MatchScene::resizeEvent(QResizeEvent*)
{
		TRACEPOINT;
		int width = graphicView_->viewport()->width();
		int heigth = graphicView_->viewport()->height();

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
