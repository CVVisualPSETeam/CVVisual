#ifndef CVVISUAL_MATCH_SCENE
#define CVVISUAL_MATCH_SCENE

#include <vector>

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "cvvmatch.hpp"
#include "cvvkeypoint.hpp"
#include "zoomableimage.hpp"
#include "../dbg/dbg.hpp"

namespace cvv{ namespace qtutil{

class MatchPen;

class MatchScene:public QWidget{
Q_OBJECT
public:	
	MatchScene(cv::Mat imageLeft,cv::Mat imageRight,const std::vector<cv::KeyPoint>& keypoints_left,
		const std::vector<cv::KeyPoint>& keypoints_right,const std::vector<cv::DMatch>& matches, QWidget* parent = nullptr);

signals:

	void updatePen(const MatchPen&);

protected:
	 void resizeEvent(QResizeEvent * event);

private:

	std::vector<cvv::qtutil::CVVKeyPoint*> keypoints_left_;	
	std::vector<cvv::qtutil::CVVKeyPoint*> keypoints_right_;
	std::vector<cvv::qtutil::CVVMatch*> matches_;
	
	QGraphicsView 		*graphicView_;
	QGraphicsScene 		*graphicScene_;	

	qtutil::ZoomableImage 	*leftImage_;
	qtutil::ZoomableImage 	*rightImage_;

	QGraphicsProxyWidget	*leftImWidget_;
	QGraphicsProxyWidget	*rightImWidget_;

};
}}
#endif
