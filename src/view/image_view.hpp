#ifndef CVVISUAL_IMAGE_VIEW_HPP
#define CVVISUAL_IMAGE_VIEW_HPP

#include <memory>

#include <QString>
#include <QWidget>
#include <QHBoxLayout>

#include <opencv2/core/core.hpp>

#include "../qtutil/accordion.hpp"
#include "../qtutil/zoomableimageoptpanel.hpp"
#include "../qtutil/zoomableimage.hpp"
#include "../util/util.hpp"

namespace cvv
{
namespace view
{

/**
 * @brief Shows one image.
 */
class ImageView : public QWidget
{
	Q_OBJECT

signals:

	/**
	 * @brief update left Footer.
	 * Signal to update the left side of the footer with newText.
	 * @param newText to update the footer with.
	 */
	void updateLeftFooter(const QString &newText);

	/**
	 * @brief update right Footer.
	 * Signal to update the right side of the footer with newText.
	 * @param newText to update the footer with.
	 */
	void updateRightFoooter(const QString &newText);

      public:
	/**
	 * @brief Constructor.
	 * @param image to show.
	 * @param parent of this QWidget.
	 **/
	ImageView(const cv::Mat &image, QWidget *parent = nullptr)
	    : QWidget{ parent }
	{
		layout_ = util::make_unique<QHBoxLayout>(this);
		accor_ = util::make_unique<qtutil::Accordion>(this);
		
		accor_->setMinimumWidth(250);
		accor_->setMaximumWidth(250);

		layout_->addWidget(accor_.get());

		zoomim_ = util::make_unique<qtutil::ZoomableImage>();
		
		accor_->insert(QString("ImageInformation:"),
			std::move(util::make_unique<qtutil::ZoomableOptPanel>(*zoomim_)));
		zoomim_->setMat(image);
		layout_->addWidget(zoomim_.get());

		setLayout(layout_.get());
	}
	
private:
	std::unique_ptr<QHBoxLayout> layout_;
	std::unique_ptr<qtutil::Accordion> accor_;
	std::unique_ptr<qtutil::ZoomableImage> zoomim_;
};
}
} // namespaces

#endif

