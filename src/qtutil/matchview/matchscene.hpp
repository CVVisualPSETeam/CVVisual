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
#include "../zoomableimage.hpp"
#include "../zoomableimageoptpanel.hpp"
#include "../../dbg/dbg.hpp"

namespace cvv{ namespace qtutil{

class MatchPen;

class MatchScene:public QWidget{
Q_OBJECT
public:	
	MatchScene(cv::Mat imageLeft,cv::Mat imageRight, QWidget* parent = nullptr);
	
	std::unique_ptr<ZoomableOptPanel> getLeftMatInfoWidget()
		{TRACEPOINT;return util::make_unique<ZoomableOptPanel>(*leftImage_);}

	std::unique_ptr<ZoomableOptPanel> getRightMatInfoWidget()
		{TRACEPOINT;return util::make_unique<ZoomableOptPanel>(*rightImage_);}
	void adjustImages();

public slots:
	void addLeftKeypoint(CVVKeyPoint*);
	void addRightKeyPoint(CVVKeyPoint*);
	void addMatch(CVVMatch*);

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
