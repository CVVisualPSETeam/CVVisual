
#include <QHBoxLayout>
#include <QPoint>

#include "matchscene.hpp"

namespace cvv{ namespace qtutil{
	MatchScene::MatchScene(cv::Mat imageLeft,cv::Mat imageRight,const std::vector<cv::KeyPoint>& keypoints_left,
		const std::vector<cv::KeyPoint>& keypoints_right,const std::vector<cv::DMatch>& matches, QWidget* parent):
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

			for(auto key:keypoints_left)
			{
				CVVKeyPoint keypoint{key,leftImage_,leftImWidget_};
				keypoints_left_.push_back(keypoint);
			};
			
			for(auto key:keypoints_right)
			{
				CVVKeyPoint keypoint{key,rightImage_,rightImWidget_};
				keypoints_right_.push_back(keypoint);
			};
			
			for(auto match:matches)
			{
				matches_.emplace_back(keypoints_left.at(match.queryIdx),
					keypoints_right.at(match.trainIdx),match.distance);
			};
						
			QResizeEvent event{size(),size()};
			resizeEvent(&event);

			TRACEPOINT;
		}
	//unfinished
	/*void MatchScene::draw(const std::vector<cv::KeyPoint>& keypoints_left,
		const std::vector<cv::KeyPoint>& keypoints_right,const std::vector<cv::DMatch>& matches)
	{
		for(auto key:keypoints_left_)
		{
			cv::Point2f pt=key.keyPoint().pt;
			QPointF point{pt.x,pt.y};
			QPointF mid = leftImWidget_->mapToScene(leftImage_->mapImagePointToParent(point));
			QPointF dif{0.5,0.5};
			QPen pen{Qt::red};
			graphicScene_->addEllipse(QRectF{mid-dif,mid+dif},pen);
		}

		for(auto key:keypoints_right_)
		{
			cv::Point2f pt=key.keyPoint().pt;
			QPointF point{pt.x,pt.y};
			QPointF mid = rightImWidget_->mapToScene(rightImage_->mapImagePointToParent(point));
			QPointF dif{0.5,0.5};
			graphicScene_->addEllipse(QRectF{mid-dif,mid+dif});
		}
	}*/

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
