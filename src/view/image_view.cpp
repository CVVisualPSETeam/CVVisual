
#include <memory>

#include <QString>
#include <QWidget>
#include <QHBoxLayout>

#include <opencv2/core/core.hpp>

#include "../qtutil/accordion.hpp"
#include "../qtutil/zoomableimageoptpanel.hpp"
#include "../qtutil/zoomableimage.hpp"
#include "../util/util.hpp"
#include "image_view.hpp"

namespace cvv
{
namespace view
{

ImageView::ImageView(const cv::Mat &image, QWidget *parent)
	    : QWidget{ parent }, image{nullptr}
{
	auto layout = util::make_unique<QHBoxLayout>(this);
	auto accor = util::make_unique<qtutil::Accordion>(this);
	
	accor->setMinimumWidth(300);
	accor->setMaximumWidth(300);

	auto zoomim = util::make_unique<qtutil::ZoomableImage>();
	accor->insert(QString("ImageInformation:"),
		std::move(util::make_unique<qtutil::ZoomableOptPanel>(*zoomim)));
	zoomim->setMat(image);

	layout->addWidget(accor.release());
	this->image = (*zoomim);
	layout->addWidget(zoomim.release());

	setLayout(layout.release());
}

void ImageView::showFullImage()
{
	image->showFullImage();
}


}} //namespaces
