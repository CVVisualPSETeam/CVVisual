#include <QString>
#include <QPushButton>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

#include "image_call_tab.hpp"
#include "../view/image_view.hpp"
#include "../controller/view_controller.hpp"
#include "../impl/single_image_call.hpp"
#include "../qtutil/util.hpp"

#include "../dbg/dbg.hpp"

namespace cvv {
namespace gui {

ImageCallTab::ImageCallTab(const cvv::impl::SingleImageCall& call):
	imageCall_{call}
{
	TRACEPOINT;
	setName(imageCall_->description());

	createGui();
	TRACEPOINT;
}

ImageCallTab::ImageCallTab(const QString& tabName, const cvv::impl::SingleImageCall& call):
	imageCall_{call}
{
	TRACEPOINT;
	setName(tabName);

	createGui();
	TRACEPOINT;
}

void ImageCallTab::helpButtonClicked() const
{
	TRACEPOINT;
	cvv::qtutil::openHelpBrowser("SingleImageView");
	TRACEPOINT;
}

size_t ImageCallTab::getId() const
{
	TRACEPOINT;
	return imageCall_->getId();
}

void ImageCallTab::createGui()
{
	TRACEPOINT;
	hlayout_ = new QHBoxLayout{this};
	hlayout_->setAlignment(Qt::AlignTop);
	hlayout_->addWidget(new QLabel{"Single Image View"});
	helpButton_ = new QPushButton{"Help", this};
	hlayout_->addWidget(helpButton_);
	connect(helpButton_, SIGNAL(clicked()), this, SLOT(helpButtonClicked()));

	upperBar_ = new QWidget{this};
	upperBar_->setLayout(hlayout_);

	vlayout_ = new QVBoxLayout{this};

	vlayout_->addWidget(upperBar_);
	setView();

	setLayout(vlayout_);
	TRACEPOINT;
}

void ImageCallTab::setView()
{
	TRACEPOINT;
	imageView_ = new cvv::view::ImageView{imageCall_->mat(), this};
	vlayout_->addWidget(imageView_);
	TRACEPOINT;
}

}}//namespaces
