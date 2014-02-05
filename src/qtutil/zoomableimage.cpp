#include "zoomableimage.hpp"

#include <algorithm>

#include <QHBoxLayout>
#include <QScrollBar>

#include "util.hpp"
#include "types.hpp"

#include <iostream>

template<int depth>
std::string printPixel(const cv::Mat& mat, int spalte, int zeile)
{
	std::stringstream ss{};
	auto p=mat.ptr<cvv::qtutil::DepthType<depth>>(zeile)
				+mat.channels()*spalte*sizeof(cvv::qtutil::DepthType<depth>);
	ss<<(p[0]);
	for(int c=1;c<mat.channels();c++)
	{
		ss<<"\n"<<(p[c]);
	}
	return ss.str();
}
template<>
std::string printPixel<CV_8U>(const cv::Mat& mat, int spalte, int zeile)
{
	std::stringstream ss{};
	auto p=mat.ptr<cvv::qtutil::DepthType<CV_8U>>(zeile)
				+mat.channels()*spalte*sizeof(cvv::qtutil::DepthType<CV_8U>);
	ss<<static_cast<cvv::qtutil::DepthType<CV_16U>>(p[0]);
	for(int c=1;c<mat.channels();c++)
	{
		ss<<"\n"<<static_cast<cvv::qtutil::DepthType<CV_16U>>(p[c]);
	}
	return ss.str();
}
template<>
std::string printPixel<CV_8S>(const cv::Mat& mat, int spalte, int zeile)
{
	std::stringstream ss{};
	auto p=mat.ptr<cvv::qtutil::DepthType<CV_8S>>(zeile)
				+mat.channels()*spalte*sizeof(cvv::qtutil::DepthType<CV_8S>);
	ss<<static_cast<cvv::qtutil::DepthType<CV_16S>>(p[0]);
	for(int c=1;c<mat.channels();c++)
	{
		ss<<"\n"<<static_cast<cvv::qtutil::DepthType<CV_16S>>(p[c]);
	}
	return ss.str();
}

std::string printPixel(const cv::Mat& mat, int i, int j)
{
	if(i>=0&&j>=0)
	{
		if(i<mat.cols&&j<mat.rows)
		{
			switch(mat.depth())
			{
				case CV_8U:return printPixel<CV_8U>(mat,i,j); break;
				case CV_8S:return printPixel<CV_8S>(mat,i,j); break;
				case CV_16U:return printPixel<CV_16U>(mat,i,j); break;
				case CV_16S:return printPixel<CV_16S>(mat,i,j); break;
				case CV_32S:return printPixel<CV_32S>(mat,i,j); break;
				case CV_32F:return printPixel<CV_32F>(mat,i,j); break;
				case CV_64F:return printPixel<CV_64F>(mat,i,j); break;
			}
		}
	}
	return "";
}

namespace cvv{ namespace qtutil{
ZoomableImage::ZoomableImage(const cv::Mat& mat,QWidget* parent):
	QWidget{parent},
	mat_{mat},
	view_{new QGraphicsView{}},
	scene_{new QGraphicsScene{this}},
	zoom_{1},
	threshold_{60},
	autoShowValues_{true},
	valuesVisible_{false},
	values_{}
{
	// qt5 doc : "The view does not take ownership of scene."
	view_->setScene(scene_);
	QObject::connect((view_->horizontalScrollBar()),&QScrollBar::valueChanged,this,
							&ZoomableImage::viewScrolled);
	QObject::connect((view_->verticalScrollBar()),&QScrollBar::valueChanged,this,
							&ZoomableImage::viewScrolled);
	QObject::connect(this,SIGNAL(updateArea(QRectF,qreal)),
			 this,SLOT(drawValues()));

	QHBoxLayout *layout=new QHBoxLayout{};
	layout->addWidget(view_);
	setLayout(layout) ;
	updateMat(mat_);
}

void ZoomableImage::updateMat(cv::Mat mat)
{
	mat_ = mat;
	auto result = convertMatToQPixmap(mat_);
	emit updateConversionResult(result.first);
	scene_->clear();
	pixmap_ = scene_->addPixmap(result.second);

	drawValues();
}

void ZoomableImage::updateZoom(qreal factor)
{
	if(factor <= 0) {return;}
	qreal newscale=factor/zoom_;
	zoom_=factor;
	view_->scale(newscale,newscale);
	// will be called in resize event
	// emit updateArea(visibleArea(),zoom_);
}

void ZoomableImage::drawValues()
{
	//delete old values
	for(auto& elem:values_)
	{
		scene_->removeItem(elem);
		delete elem;
	}
	values_.clear();

	//draw new values?
	if(!(autoShowValues_&&(zoom_>=threshold_))){return;}
	auto r=visibleArea();
	for(int i=std::max(0,static_cast<int>(r.left())-2);
		i<std::min(mat_.cols,static_cast<int>(r.right())+2);i++)
	{
		for(int j=std::max(0,static_cast<int>(r.top())-2);
			j<std::min(mat_.rows,static_cast<int>(r.bottom())+2); j++)
		{

			QString s(printPixel(mat_,i,j).c_str());

			s.replace('\n',"<br>");
			QGraphicsTextItem* txt=scene_->addText("");
			txt->setHtml(
				QString("<div style='background-color:rgba(255, 255, 255, 0.5);'>")+
						s + "</div>"
			);
			txt->setPos(i,j);
			txt->setScale(0.008);
			values_.push_back(txt);
		}
	}
}


void ZoomableImage::showFullImage()
{
	updateZoom(
		std::min(
		static_cast<qreal>(view_->viewport()->width())/static_cast<qreal>(imageWidth()),
		static_cast<qreal>(view_->viewport()->height())/static_cast<qreal>(imageHeight())
		)
	);
}

QRectF ZoomableImage::visibleArea() const
{
	QRectF result{};
	result.setTopLeft(view_->mapToScene(QPoint{0,0}));
	result.setBottomRight(view_->mapToScene(QPoint{view_->viewport()->width(),
							view_->viewport()->height()}));
	return result;
}

QPointF ZoomableImage::mapImagePointToParent(QPointF point)
{
	return mapToParent(view_->mapToParent(view_->mapFromScene(pixmap_->mapToScene(point))));
}
}}
