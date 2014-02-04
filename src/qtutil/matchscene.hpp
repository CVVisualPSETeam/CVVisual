#ifndef CVVISUAL_MATCH_SCENE
#define CVVISUAL_MATCH_SCENE

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "zoomableimage.hpp"

namespace cvv{ namespace qtutil{

class MatchScene:public QWidget{
public:	
	MatchScene(cv::Mat imageLeft,cv::Mat imageRight,const std::vector<cv::KeyPoint>& keypoints_left,
		const std::vector<cv::KeyPoint>& keypoints_right,const std::vector<cv::DMatch>& matches, QWidget* parent = nullptr);
public slots:
	
	void draw(const std::vector<cv::KeyPoint>& keypoints_left,
		const std::vector<cv::KeyPoint>& keypoints_right,const std::vector<cv::DMatch>& matches);
signals:
	//TODO
	void changeSelection();

protected:
	 void resizeEvent(QResizeEvent * event);

private:
	QGraphicsView 		*graphicView_;
	QGraphicsScene 		*graphicScene_;	

	qtutil::ZoomableImage 	*leftImage_;
	qtutil::ZoomableImage 	*rightImage_;

	QGraphicsProxyWidget	*leftImWidget_;
	QGraphicsProxyWidget	*rightImWidget_;

};
}}
#endif
