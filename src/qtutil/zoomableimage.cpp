
#include <QHBoxLayout>
#include <QScrollBar>

#include "zoomableimage.hpp"
#include "util.hpp"

namespace cvv{ namespace qtutil{
ZoomableImage::ZoomableImage(const cv::Mat& mat,QWidget* parent):
	QWidget{parent},
	mat_{mat},
	view_{new QGraphicsView{}},
	scene_{cvv::util::make_unique<QGraphicsScene>()},
	zoom_{1}
{
	// qt5 doc : "The view does not take ownership of scene."
	view_->setScene(scene_.get());
	QObject::connect((view_->horizontalScrollBar()),&QScrollBar::valueChanged,this,&ZoomableImage::viewScrolled);
	QObject::connect((view_->verticalScrollBar()),  SIGNAL(valueChanged(int)),this,SLOT(viewScrolled()));

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
	std::unique_ptr<QGraphicsScene> tmp=cvv::util::make_unique<QGraphicsScene>();
	view_->setScene(tmp.get());
	scene_.reset(tmp.release());
	scene_->addPixmap(result.second);
}

void ZoomableImage::updateZoom(qreal factor)
{
	if(factor > 0)
	{
		view_->scale(factor/zoom_,factor/zoom_);
		zoom_=factor;
		emit updateArea(visibleArea());
	}
}


QRectF ZoomableImage::visibleArea() const
{
	QRectF result{};
	result.setTopLeft(view_->mapToScene(QPoint{0,0}));
	result.setTopRight(view_->mapToScene(QPoint{view_->width(),view_->height()}));
	return result;
}

}}
