#include "zoomableimage.hpp"

#include <algorithm>

#include <QHBoxLayout>
#include <QScrollBar>
#include <QAction>
#include <QMenu>
#include <QFileDialog>
#include <QPixmap>

#include "util.hpp"
#include "types.hpp"

#include <iostream>

template<int depth>
void putInStream(std::stringstream& ss,const cvv::qtutil::DepthType<depth>& val){ss<<val;}

template<> void putInStream<CV_8U>(std::stringstream& ss,const cvv::qtutil::DepthType<CV_8U>& val)
	{ss<<static_cast<cvv::qtutil::DepthType<CV_16S>>(val);}

template<> void putInStream<CV_8S>(std::stringstream& ss,const cvv::qtutil::DepthType<CV_8S>& val)
	{ss<<static_cast<cvv::qtutil::DepthType<CV_16S>>(val);}


template<int depth,int channels>
std::string printPixel(const cv::Mat& mat, int spalte, int zeile)
{
	std::stringstream ss{};
	auto p=mat.at<cv::Vec<cvv::qtutil::DepthType<depth>,channels>>(zeile,spalte);

	putInStream<depth>(ss,p[0]);
	for(int c=1;c<mat.channels();c++)
	{
		ss<<"\n";
		putInStream<depth>(ss,p[c]);
	}
	return ss.str();
}

template<int depth>
std::string printPixel(const cv::Mat& mat, int i, int j)
{
	if(mat.channels()<1){return "<1 channel";}
	switch(mat.channels())
	{
	case 1: return printPixel<depth,1>(mat,i,j); break;
	case 2: return printPixel<depth,2>(mat,i,j); break;
	case 3: return printPixel<depth,3>(mat,i,j); break;
	case 4: return printPixel<depth,4>(mat,i,j); break;
	case 5: return printPixel<depth,5>(mat,i,j); break;
	case 6: return printPixel<depth,6>(mat,i,j); break;
	case 7: return printPixel<depth,7>(mat,i,j); break;
	case 8: return printPixel<depth,8>(mat,i,j); break;
	case 9: return printPixel<depth,9>(mat,i,j); break;
	case 10: return printPixel<depth,10>(mat,i,j); break;
	default: return ">10 channels";
	}
}

std::string printPixel(const cv::Mat& mat, int i, int j)
{
	if(i>=0&&j>=0)
	{
		if(i<mat.cols&&j<mat.rows)
		{
			switch(mat.depth())
			{
				case CV_8U: return printPixel<CV_8U >(mat,i,j); break;
				case CV_8S: return printPixel<CV_8S >(mat,i,j); break;
				case CV_16U:return printPixel<CV_16U>(mat,i,j); break;
				case CV_16S:return printPixel<CV_16S>(mat,i,j); break;
				case CV_32S:return printPixel<CV_32S>(mat,i,j); break;
				case CV_32F:return printPixel<CV_32F>(mat,i,j); break;
				case CV_64F:return printPixel<CV_64F>(mat,i,j); break;
			}
			return "unknown depth";
		}
	}
	return "";
}

namespace cvv{ namespace qtutil{
ZoomableImage::ZoomableImage(const cv::Mat& mat,QWidget* parent):
	QWidget{parent},
	mat_{mat},
	view_{new structures::GraphicsView{}},
	scene_{new QGraphicsScene{this}},
	zoom_{1},
	threshold_{60},
	autoShowValues_{true},
	values_{},
	scrollFactorCTRL_{1.025},
	scrollFactorCTRLShift_{1.01}
{
	TRACEPOINT;
	// qt5 doc : "The view does not take ownership of scene."
	view_->setScene(scene_);
	QObject::connect((view_->horizontalScrollBar()),&QScrollBar::valueChanged,this,
							&ZoomableImage::viewScrolled);
	QObject::connect((view_->verticalScrollBar()),&QScrollBar::valueChanged,this,
							&ZoomableImage::viewScrolled);
	QObject::connect(this,SIGNAL(updateArea(QRectF,qreal)),
			 this,SLOT(drawValues()));
	//scrollbars should have strong focus
	view_->horizontalScrollBar()->setFocusPolicy(Qt::FocusPolicy::NoFocus);
	view_->verticalScrollBar()->setFocusPolicy(Qt::NoFocus);
	view_->setFocusPolicy(Qt::NoFocus);
	QHBoxLayout *layout=new QHBoxLayout{};
	layout->addWidget(view_);
	layout->setMargin(0);
	setLayout(layout) ;
	setMat(mat_);
	//rightklick
	setContextMenuPolicy(Qt::CustomContextMenu);
	QObject::connect(this,SIGNAL(customContextMenuRequested(const QPoint&)),
			 this,SLOT(rightClick(QPoint)));
	TRACEPOINT;
}

void ZoomableImage::setMat(cv::Mat mat)
{
	TRACEPOINT;
	mat_ = mat;
	auto result = convertMatToQPixmap(mat_);
	emit updateConversionResult(result.first);
	scene_->clear();
	pixmap_ = scene_->addPixmap(result.second);

	drawValues();
	TRACEPOINT;
}

void ZoomableImage::setZoom(qreal factor)
{
	TRACEPOINT;
	if(factor <= 0)
	{
		TRACEPOINT;
		return;
	}
	qreal newscale=factor/zoom_;
	zoom_=factor;
	view_->scale(newscale,newscale);
	// will be called in resize event
	// emit updateArea(visibleArea(),zoom_);
	TRACEPOINT;
}

void ZoomableImage::drawValues()
{
	TRACEPOINT;
	//delete old values
	for(auto& elem:values_)
	{
		scene_->removeItem(elem);
		delete elem;
	}
	values_.clear();
	//draw new values?
	if(!(autoShowValues_&&(zoom_>=threshold_)))
	{
		TRACEPOINT;
		return;
	}
	TRACEPOINT;
	auto r=visibleArea();
	for(int i=std::max(0,static_cast<int>(r.left())-1);
		i<std::min(mat_.cols,static_cast<int>(r.right())+1);i++)
	{
		for(int j=std::max(0,static_cast<int>(r.top())-1);
			j<std::min(mat_.rows,static_cast<int>(r.bottom())+1); j++)
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
	TRACEPOINT;
}

void ZoomableImage::wheelEvent(QWheelEvent * event)
{
	TRACEPOINT;

	if(QApplication::keyboardModifiers() & Qt::ControlModifier)
	{
		qreal f=scrollFactorCTRL_;;
		if(QApplication::keyboardModifiers() & Qt::ShiftModifier)
		{
			f=scrollFactorCTRLShift_;;
		}

		qreal scroll=((event->angleDelta().x())+(event->angleDelta().y()))*f;
		if(scroll<0.0)
		{
			scroll=-1.0/scroll;
			f=1/f;
		}
		setZoom(f*zoom_);
	}else{
		QWidget::wheelEvent(event);
	}
	TRACEPOINT;
}

void ZoomableImage::setArea(QRectF rect,qreal zoom)
{
	setZoom(zoom);
	view_->centerOn(rect.topLeft()+(rect.bottomRight()-rect.topLeft())/2);
}

void ZoomableImage::showFullImage()
{
	TRACEPOINT;
	setZoom(
		std::min(
		static_cast<qreal>(view_->viewport()->width())/static_cast<qreal>(imageWidth()),
		static_cast<qreal>(view_->viewport()->height())/static_cast<qreal>(imageHeight())
		)
	);
	TRACEPOINT;
}

QRectF ZoomableImage::visibleArea() const
{
	TRACEPOINT;
	QRectF result{};
	result.setTopLeft(view_->mapToScene(QPoint{0,0}));
	result.setBottomRight(view_->mapToScene(QPoint{view_->viewport()->width(),
							view_->viewport()->height()}));
	TRACEPOINT;
	return result;
}


void ZoomableImage::rightClick(const QPoint & pos)
{
	TRACEPOINT;
	QPoint p=mapToGlobal(pos);
	QMenu menu;

	menu.addAction("Save orginal image");
	menu.addAction("Save visible image");

	QAction* item = menu.exec(p);
	if (item)
	{
		TRACEPOINT;
		QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
			"/home/jana/untitled.png",
			tr("BMP (*.bmp);;GIF (*.gif);;JPG (*.jpg);;PNG (*.png);;"
				"PBM (*.pbm);;PGM (*.pgm);;PPM (*.ppm);;XBM (*.xbm);;"
				"XPM (*.xpm)"));
		if(fileName=="")
		{
			TRACEPOINT;
			return;
		}
		QPixmap pmap;
		if((item->text())=="Save orginal image")
		{
			pmap=convertMatToQPixmap(mat_).second;
		}else{
			pmap=QPixmap::grabWidget (view_->viewport());
		}

		pmap.save(fileName,0,100);
	}
	TRACEPOINT;
}

QPointF ZoomableImage::mapImagePointToParent(QPointF point) const
{
	TRACEPOINT;
	return mapToParent(view_->mapToParent(view_->mapFromScene(pixmap_->mapToScene(point))));
}

namespace structures {

void GraphicsView::wheelEvent(QWheelEvent * event){
	TRACEPOINT;
	if(QApplication::keyboardModifiers() & Qt::ControlModifier)
	{
		event->ignore();
	}else{
		QGraphicsView::wheelEvent(event);
	}
	TRACEPOINT;
}

}

}}
