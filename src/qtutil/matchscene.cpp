
#include <QHBoxLayout>
#include <QPoint>

#include "matchscene.hpp"
#include "../dbg/dbg.hpp"

namespace cvv{ namespace qtutil{

MatchScene::MatchScene(cv::Mat imageLeft,cv::Mat imageRight/*,const std::vector<cv::KeyPoint>& keypoints_left,
	const std::vector<cv::KeyPoint>& keypoints_right,const std::vector<cv::DMatch>& matches*/, QWidget* parent):
		QWidget{parent}//,leftImage_{imageLeft},rightImage_{imageRight}
{
	TRACEPOINT;	
	//begin Widget
	QHBoxLayout *basicLayout = new QHBoxLayout{};
	graphicScene_		 = new QGraphicsScene{this};
	graphicView_		 = new QGraphicsView{graphicScene_};			
	
	basicLayout->addWidget(graphicView_);
	basicLayout->setSizeConstraint(QLayout::SetNoConstraint);
	setLayout(basicLayout);

	leftImage_		 = new ZoomableImage{imageLeft};
	rightImage_		 = new ZoomableImage{imageRight};

	leftImWidget_ 	= graphicScene_->addWidget(leftImage_);
	rightImWidget_ 	= graphicScene_->addWidget(rightImage_);

	QResizeEvent event{size(),size()};
	resizeEvent(&event);
/*
	for(auto& key:keypoints_left)
	{
		CVVKeyPoint *keypoint=new CVVKeyPoint{key,leftImage_,leftImWidget_};
		keypoints_left_.push_back(keypoint);
		graphicScene_->addItem(keypoint);
	};
	
	for(auto& key:keypoints_right)
	{
		CVVKeyPoint *keypoint=new CVVKeyPoint{key,rightImage_,rightImWidget_};
		keypoints_right_.push_back(keypoint);
		graphicScene_->addItem(keypoint);
	};
	
	for(auto& match:matches)
	{
		CVVMatch *cvmatch=new CVVMatch{keypoints_left_.at(match.queryIdx),
			keypoints_right_.at(match.trainIdx),match.distance};
		matches_.push_back(cvmatch);
		graphicScene_->addItem(cvmatch);
	};
*/
	TRACEPOINT;
}

void MatchScene::addLeftKeypoint(CVVKeyPoint *keypoint)
{
	//keypoints_left_.push_back(keypoint);
	keypoint->setProxyWidget(leftImWidget_);
	keypoint->setZoomableImage(leftImage_);
	graphicScene_->addItem(keypoint);
	connect(leftImage_,SIGNAL(updateArea(QRectF,qreal)),keypoint,SLOT(updateImageSet(QRectF,qreal)));
}
void MatchScene::addRightKeyPoint(CVVKeyPoint *keypoint)
{
	//keypoints_right_.push_back(keypoint);
	keypoint->setProxyWidget(rightImWidget_);
	keypoint->setZoomableImage(rightImage_);
	graphicScene_->addItem(keypoint);
	connect(rightImage_,SIGNAL(updateArea(QRectF,qreal)),keypoint,SLOT(updateImageSet(QRectF,qreal)));
}
void MatchScene::addMatch(CVVMatch *cvmatch)
{
	//matches_.push_back(cvmatch);
	graphicScene_->addItem(cvmatch);
	connect(this,SIGNAL(updatePen(const MatchPen&)),cvmatch,SLOT(updatePen(const MatchPen&)));
}

void MatchScene::resizeEvent(QResizeEvent * event)
{
		TRACEPOINT;
		(void) event;
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
