
#include <QHBoxLayout>
#include <QPoint>

#include "matchscene.hpp"

namespace cvv{ namespace qtutil{
	MatchScene::MatchScene(cv::Mat imageLeft,cv::Mat imageRight,const std::vector<cv::KeyPoint>& keypoints_left,
		const std::vector<cv::KeyPoint>& keypoints_right,const std::vector<cv::DMatch>& matches, QWidget* parent):
			QWidget{parent}//,leftImage_{imageLeft},rightImage_{imageRight}
		{	
		//begin Widget
			QHBoxLayout *basicLayout = new QHBoxLayout{};
			graphicScene_		 = new QGraphicsScene{this};
			graphicView_		 = new QGraphicsView{graphicScene_};
			leftImage_		 = new ZoomableImage{imageLeft};
			rightImage_		 = new ZoomableImage{imageRight};

			basicLayout->addWidget(graphicView_);
			basicLayout->setSizeConstraint(QLayout::SetNoConstraint);
			setLayout(basicLayout);

	
			leftImWidget_ 	= graphicScene_->addWidget(leftImage_);
			rightImWidget_ 	= graphicScene_->addWidget(rightImage_);
			
						
			QResizeEvent event{size(),size()};
			resizeEvent(&event);

			draw(keypoints_left,keypoints_right,matches);
		}
	//unfinished
	void MatchScene::draw(const std::vector<cv::KeyPoint>& keypoints_left,
		const std::vector<cv::KeyPoint>& keypoints_right,const std::vector<cv::DMatch>& matches)
	{
		for(auto key:keypoints_left)
		{
			cv::Point2f pt=key.pt;
			QPointF point{pt.x,pt.y};
			QPointF mid = leftImWidget_->mapToScene(leftImage_->mapImagePointToParent(point));
			QPointF dif{0.5,0.5};
			graphicScene_->addEllipse(QRectF{mid-dif,mid+dif});
		}

		for(auto key:keypoints_right)
		{
			cv::Point2f pt=key.pt;
			QPointF point{pt.x,pt.y};
			QPointF mid = leftImWidget_->mapToScene(rightImage_->mapImagePointToParent(point));
			QPointF dif{0.5,0.5};
			graphicScene_->addEllipse(QRectF{mid-dif,mid+dif});
		}
		(void)matches;
	}

	void MatchScene::resizeEvent(QResizeEvent * event)
	{
			(void) event;
			int width = graphicView_->width()-2;
			int heigth = graphicView_->height()-2;
			//left
			leftImWidget_->setPos(0,0);
			leftImWidget_->setMinimumSize((width/2),heigth);
			leftImWidget_->setMaximumSize(width/2,heigth);

			//right
			rightImWidget_->setPos(width/2,0);
			rightImWidget_->setMinimumSize(width/2,heigth);
			rightImWidget_->setMaximumSize(width/2,heigth);
			graphicView_->setSceneRect(1,1,width-1,heigth-1);
	}
}}
